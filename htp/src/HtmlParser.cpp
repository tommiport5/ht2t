/*
 * HtmlParser.cpp
 *
 *  Created on: 21.03.2018
 *      Author: Heckmann
 */

#include "HtmlParser.h"
#include <limits>
#include <cctype>
#include <iostream>
#include <algorithm>

#include "StateMachine/State.h"
#include "StateMachine/IdleState.h"

using namespace std;

inline string lowercase(string &&any)
{
	string tmp = move(any);
	transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
	return tmp;
}

inline std::string trim(const std::string &s)
{
   auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
   auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
   return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}

inline bool emptyOrBlank(const std::string &s)
{
   auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){
	   return std::isspace(c);
   });
   return wsfront == s.end();
}


HtmlParser::HtmlParser(const std::string &path)
: ifs(path)
, Buf()
, debug(true)
,TagRegex("<\\s*(\\w*)[^>]*>")
,EndRegex("<\\s*/\\s*(\\w*)[^>]*>")
{
}

void HtmlParser::quickParse()
{
	smatch m;
	string s;
	int next;
	int Start;

	while (!ifs.eof()) {
		Buf += ifs.get();		// there are a lot of smarter solutions on the net, but I doubt, that they are faster
	}
	s = Buf;
	next = 0;
	while (regex_search(s, m, TagRegex)) {
		if (m[1].length() > 0) {
			Start =  next + m.position();
			Parsed.push_back(Node(
					lowercase(Buf.substr(next + m.position(1), m[1].length())),
					Start, m.str(),false)
					);
		}
		next += m.position() + m[0].length();
		s = m.suffix().str();
	}
	s = Buf;
	next = 0;
	while (regex_search(s, m, EndRegex)) {
		if (m[1].length() > 0) {
			Start =  next + m.position();
			End.push_back(Node(
					lowercase(Buf.substr(next + m.position(1), m[1].length())),
					Start, m.str(),true)
					);
		}
		next += m.position() + m[0].length();
		s = m.suffix().str();
	}
	Parsed.merge(End);
}

const unsigned int TabLength = 8;

pair<unsigned long, unsigned long> HtmlParser::getRowCol(unsigned long pos)
{
	unsigned long row = 1, col = 1;

	for (unsigned int i = 0; i < pos; i++) {
		if (i >= Buf.length()) break;
		if (Buf[i] == '\n') {
			col = 1;
			++row;
		} else if (Buf[i] == '\t'){
			col += 8;
		} else {
			++col;
		}
	}
	return pair<unsigned long, unsigned long>(row,col);
}

/**
 * structurize
 * Structurizes the 'Parsed' list, i.e. combines paragraphs with embedded elements and creates new nodes for untagged text in the body.
 *
 * Returns false, if no body can be found. Combining the paragraphs has been done by then.
 */
bool HtmlParser::structurize()
{
	Context ctx(Idle);

	for(auto n = Parsed.begin(); n != Parsed.end(); ++n) {
		IState *cs = ctx.getCurrentState()->handleState(ctx,n);
		ctx.changeState(cs);
	}
	// erase all the duplicates after iteration, because they could have been used in NodeStack
	ctx.doErase(Parsed);

	auto p = find_if(Parsed.begin(),Parsed.end(), [&](Node &n) -> bool {
		return n.getTyp() == NodeType::body;
	});
	if (p == Parsed.end()) return false;

	p->forAllChildrenThat(Node::hasChildren, [&](Node &n, int unused) {
		unsigned long content = n.start() + n.raw().length();
		for (auto pp=n.nested().begin(); pp!=n.nested().end(); ++pp ) {
			if (pp->shallBePrinted()) {
				string tmp = Buf.substr(content, pp->start()-content);
				if (!emptyOrBlank(tmp)) {
/*					auto pos = getRowCol(content);
					auto epos = getRowCol(pp->start());
					cout << "line " << pos.first << ", char " << pos.second << " to " << epos.first << ", " << epos.second << ": " << tmp << endl;*/
					Node nn(NodeType::text, content, move(tmp), false);
					Texts.push_back(move(nn));
				}
			}
			if (pp->printsANewline()) {
					Texts.push_back(*pp);
			}
			content = pp->getOverallEnd();
		}
	});
	Texts.sort([](const Node &f,  const Node &s) -> bool {
		return f.start() < s.start();
	});
	return true;
}

void HtmlParser::print()
{
	std::list<Node>::iterator rec;

	for_each(Parsed.begin(),Parsed.end(), [&] (Node &p) {
		cout << (const string&) p;
		if (debug) cout << " " << p.raw();
		cout << endl;
		if (debug) {
			p.forAllChildrenThat(Node::always,[&](Node &t, int level) {
				for (int i=0; i<level; i++) cout << "-> ";
				auto pos = getRowCol(t.start());
				auto epos = getRowCol(t.getLastPosition());
				cout << (const string&) t << "(" << pos.first << ", " << pos.second
						<< " - " << epos.first << ", " << epos.second << ")" << endl;
			});
		}
	});
}

void HtmlParser::printET()
{
	for_each(Texts.begin(),Texts.end(), [&] (Node &p) {
		auto pos = getRowCol(p.start());
		auto epos = getRowCol(p.getLastPosition());
		cout << (const string&) p << "(" << pos.first << ", " << pos.second
				<< " - " << epos.first << ", " << epos.second << ")" << endl;
	});
}

bool HtmlParser::eof()
{
	return ifs.eof();
}

bool HtmlParser::getExtractedText(string &result)
{
	if (Current == Texts.end())return false;
	result.clear();
	if (Current->getTyp() == NodeType::text) {
		Fil.preprocess(Current->raw().begin(), Current->raw().end(), &result);
	} else {
		result = "\n";
	}
	++Current;
	return true;
}

HtmlParser::~HtmlParser()
{
	ifs.close();
}


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
#include <iconv.h>

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


HtmlParser::HtmlParser(std::istream &In, std::ostream &Out, bool Verb)
: debug(Verb)
,TagRegex("<\\s*(\\w*)[^>]*>")
,EndRegex("<\\s*/\\s*(\\w*)[^>]*>")
,MetaRegex("<\\s*(meta)\\s*[^>]*>", regex_constants::ECMAScript | regex_constants::icase)
,CommentRegex("<\\s*(!--)[^>]*>", regex_constants::ECMAScript | regex_constants::icase)
,InputCharset("")
,out(Out)
{
//	not possible with istream, maybe do something with read
//	ifstream ifs(path);
//	stringstream buffer;
//	buffer << ifs.rdbuf();
//	Buf = buffer.str();
	while (!In.eof()) {
		Buf += In.get();
	}
}

/**
 * findExpressions
 * Parses the buffer for the given expression and creates nodes from them.
 * The nodes are inserted in the list res.
 * The caller has to tell us here, if the expression finds an end tag.
 */
void HtmlParser::findExpressions(std::regex &pat, std::list<Node> &res, bool findsEnd)
{
	smatch m;
	string s;
	int next;
	int Start;

	s = Buf;
	next = 0;
	while (regex_search(s, m, pat)) {
		if (m[1].length() > 0) {
			Start =  next + m.position();
			res.push_back(Node(
					lowercase(Buf.substr(next + m.position(1), m[1].length())),
					Start, m.str(),findsEnd)
					);
		}
		next += m.position() + m[0].length();
		s = m.suffix().str();
	}
}

std::string &HtmlParser::readCharset()
{
	if (InputCharset.empty()) {
		regex cs("charset=\"?([^\">]*)", regex_constants::ECMAScript | regex_constants::icase);

		findMetas();
		find_if(Temp.begin(), Temp.end(),[&](Node n){
			smatch m;
			if (regex_search(n.raw(), m, cs)) {
				InputCharset = m[1];
				return true;
			}
			return false;
		});
	}
	return InputCharset;
}

bool HtmlParser::convert2UTF_8()
{
	iconv_t idesc;
	size_t InLen = Buf.length();
	size_t OutLen = 2*InLen;	// should be enough
	const char *fromBuf = Buf.c_str();
	char **pFrom = const_cast<char **>(&fromBuf);

	readCharset();

	if ((InputCharset == "utf-8") || InputCharset.empty()) return true;

	idesc = iconv_open("utf-8", InputCharset.c_str());
	if (idesc == (iconv_t)-1) return false;
	char *toBuf = new char[OutLen];
	char **pTo = &toBuf;
	char *saveTo = toBuf;	//
	size_t res = iconv(idesc, pFrom, &InLen, pTo, &OutLen);
	if (res != (size_t)-1) {
		string tmp(saveTo);
		Buf.swap(tmp);
	}
	delete []saveTo;
	iconv_close(idesc);
	return res != (size_t) -1;
}


void HtmlParser::quickParse()
{
	findExpressions(TagRegex,Parsed, false);
	Temp.clear();
	findExpressions(EndRegex,Temp,true);
	Parsed.merge(Temp);
	//Temp.clear(); already removed by merge
	findExpressions(CommentRegex,Temp,true);
	Parsed.merge(Temp);
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

	Texts.clear();
	p->forAllChildrenThat(Node::hasChildren, [&](Node &n, int unused) {
		if (n.shallBePrinted()) {
			unsigned long content = n.start() + n.raw().length();
			for (auto pp=n.nested().begin(); pp!=n.nested().end(); ++pp ) {
					string tmp = Buf.substr(content, pp->start()-content);
					if (!emptyOrBlank(tmp)) {
						Node nn(NodeType::text, content, move(tmp), false);
						Texts.push_back(move(nn));
					}
				if (pp->printsANewline() || pp->printsABlank()) {
						Texts.push_back(*pp);
				}
				content = pp->getOverallEnd();
			}
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
		out << (const string&) p;
		if (debug) out << " " << p.raw();
		out << endl;
		if (debug) {
			p.forAllChildrenThat(Node::always,[&](Node &t, int level) {
				for (int i=0; i<level; i++) out << "-> ";
				auto pos = getRowCol(t.start());
				auto epos = getRowCol(t.getLastPosition());
				out << (const string&) t << "(" << pos.first << ", " << pos.second
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
		out << (const string&) p << "(" << pos.first << ", " << pos.second
				<< " - " << epos.first << ", " << epos.second << ")" << endl;
	});
}

bool HtmlParser::getExtractedText(string &result)
{
	if (Current == Texts.end())return false;
	result.clear();
	if (Current->getTyp() == NodeType::text) {
		Fil.preprocess(Current->raw().begin(), Current->raw().end(), &result);
	} else if (Current->printsANewline()) {
		result = "\n";
	} else if (Current->printsABlank()) {
		result = " ";
	}
	++Current;
	return true;
}

HtmlParser::~HtmlParser()
{
}


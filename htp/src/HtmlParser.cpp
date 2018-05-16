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

using namespace std;

string lowercase(string &&any)
{
	string tmp = move(any);
	transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
	return tmp;
}

HtmlParser::HtmlParser(const std::string &path)
: ifs(path)
, Buf()
, debug(true)
,TagRegex("<\\s*(\\w*)[^>]*>")
,EndRegex("<\\s*/\\s*(\\w*)[^>]*>")
{
	currentParagraph = Parsed.end();
}

void HtmlParser::quickParse()
{
	smatch m;
	string s;
	int next;
	int Start;

	while (!ifs.eof()) {
		Buf += ifs.get();		// brute force
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
	currentParagraph = Parsed.begin();
}

void HtmlParser::print()
{
	for_each(Parsed.begin(),Parsed.end(), [&] (Node &p) {
		cout << (const string&) p;
		if (debug) cout << " " << p.raw();
		cout << endl;
	});
}

bool HtmlParser::eof()
{
	return ifs.eof();
}

bool HtmlParser::getHeaderOrParagraph(string &result)
{
	result = "";
	while (currentParagraph != Parsed.end()) {
		if (currentParagraph->getTyp() == NodeType::p || currentParagraph->getTyp() == NodeType::h) {
			result = currentParagraph->completeNode(Buf, currentParagraph, Parsed.end());
			return true;
		} else {
			++currentParagraph;
		}
	}
	return false;
}

HtmlParser::~HtmlParser()
{
	ifs.close();
}


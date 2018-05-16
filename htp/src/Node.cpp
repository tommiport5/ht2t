/*
 * Node.cpp
 *
 *  Created on: 02.05.2018
 *      Author: Heckmann
 */

#include "Node.h"
#include <algorithm>
#include <cctype>

using namespace std;


const map<const string, const NodeType::T> NodeType::NTab  {
		{"a", a},
		{"b", b},
		{"body", body},
		{"br", br},
		{"button", button},
		{"col", col},
		{"div", div},
		{"form", form},
		{"head", head},
		{"html", html},
		{"i", i},
		{"img", img},
		{"li", li},
		{"meta", meta},
		{"option", option},
		{"p", p},
		{"script", script},
		{"span", span},
		{"table", table},
		{"td", td},
		{"th", th},
		{"tr", tr},
		{"title", title},
		{"u", u},
		{"ul", ul}
};

const string NodeType::UnknownString = "unknown";
const string NodeType::HString = "h";



NodeType::NodeType(const std::string &s)
{
	auto it = NTab.find(s.c_str());
	if (s.empty()) {
		_Typ = unknown;
	} else 	if (it != NTab.end()) {
		_Typ = it->second;
	} else if (tolower(s[0]) == 'h')  {
		if (s.length() == 2 && s[1] > '0' && s[1] <= '6') _Typ = h;
		else {
			_Typ = unknown;
		}
	} else {
		_Typ = unknown;
	}
}

const bool NodeType::operator==(const NodeType &rhs) const
{
	if (_Typ == unknown && rhs._Typ == unknown) return false;
	return _Typ == rhs._Typ;
}

/**
 * const char * operator
 * inefficient debugging aid
 */
NodeType::operator const string & () const
{
	if (_Typ == h) return HString;
	if (_Typ == unknown) return UnknownString;
	map<const string, const NodeType::T> ::const_iterator it = find_if(NTab.begin(),NTab.end(),[&] (const pair<const string, const T> &nt ) -> bool {
		if (nt.second == _Typ) return true;
		else return false;
	});
	if (it == NTab.end()) return UnknownString;
	return it->first;
}

bool Node::operator<(const Node &rhs) const
{
	return start < rhs.start;
}

Node::operator const string & () const
{
	static string res;

	if (_isEnd) res = "/";
	else res = "";

	res += _nt;

	return res;
}


string &&Node::completeNode(const std::string &text, list<Node>::const_iterator &n, list<Node>::const_iterator end) const
{
	static string res;

	if (n!= end) {
		list<Node>::const_iterator p = n;
		list<Node>::const_iterator last = p;

		++p;
		while (p != end) {
			if (p->getTyp() == n->getTyp()) {
				// Node ends here, or new node of same type starts, in which case we end the old one automatically (and silently),
				// because this is a frequent programming error for web authors
				int StartOfString = last->start + last->_raw.length();
				res += text.substr(StartOfString, p->start - StartOfString);
				//++p;
				break;
			} else {
				// in all other cases, we just merge the text and continue
				int StartOfString = last->start + last->_raw.length();
				res += text.substr(StartOfString, p->start - StartOfString);
				last = p;
				++p;
			}
		}
		n = p;
	}
	return move(res);
}

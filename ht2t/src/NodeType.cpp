/*
 * NodeType.cpp
 *
 *  Created on: 24.05.2018
 *      Author: dad
 */

#include <algorithm>
#include "NodeType.h"

using namespace std;

const map<const string, const NodeType::T> NodeType::NTab  {
		{"a", a},
		{"b", b},
		{"body", body},
		{"br", br},
		{"button", button},
		{"col", col},
		{"!--", comment},
		{"div", div},
		{"form", form},
		{"head", head},
		{"html", html},
		{"i", i},
		{"img", img},
		{"li", li},
		{"meta", meta},
		{"noscript", noscript},
		{"option", option},
		{"p", p},
		{"script", script},
		{"style", style},
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
const string NodeType::TextString = "text";

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
	if (_Typ == text) return TextString;
	map<const string, const NodeType::T> ::const_iterator it = find_if(NTab.begin(),NTab.end(),[&] (const pair<const string, const T> &nt ) -> bool {
		if (nt.second == _Typ) return true;
		else return false;
	});
	if (it == NTab.end()) return UnknownString;
	return it->first;
}

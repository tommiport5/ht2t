/*
 * Filter.cpp
 *
 *  Created on: 02.06.2018
 *      Author: dad
 */

#include "Filter.h"

#include <algorithm>
#include <functional>
#include <cctype>
#include <map>

using namespace std;


const map<string, string> EntityTable = {
		{"&quot;", u8"\""},
		{"&amp;", u8"&"},
		{"&auml;", u8"ä"},
		{"&ouml;", u8"ö"},
		{"&uuml;", u8"ü"},
		{"&Auml;", u8"Ä"},
		{"&Ouml;", u8"Ö"},
		{"&Uuml;", u8"Ü"},
};

Filter::~Filter() {
}

/**
 * prp
 * preprocess one input character
 */
void Filter::prp(char c)
{
	switch (ppstate) {
	case neutral:
		if (isblank(c)) {
			*_o += ' ';
			ppstate = blank;
		} else if (isspace(c)) {
			// space, but not blank -> newline etc
			*_o += ' ';
			ppstate = lf;
		} else if (c == '&') {
			ent = c;
			ppstate = entity;
		} else {
			*_o += c;
		}
		break;
	case entity:
		ent += c;
		if (c ==';') {
			*_o += lookupEntity(ent);
			ppstate = neutral;
		}
		break;
	case blank:
	case lf:
		if (isspace(c)) {
			// nothing, swallow it
		} else if (c == '&') {
			ent = c;
			ppstate = entity;
		} else {
			*_o += c;
			ppstate = neutral;
		}
		break;
	}
}

void Filter::reset()
{
	ppstate = neutral;
}

const std::string& Filter::lookupEntity(const std::string& ent)
{
	try {
		return EntityTable.at(ent);
	} catch (const out_of_range) {
		return ent;
	}
}

/**
 * preprocess
 * processes the string between s and e and writes the result to o.
 * The interface is modeled similar to transform(), but it allows to change the size of the sequence,
 * i.e. drop characters or insert ones
 */
void Filter::preprocess(string::const_iterator s, string::const_iterator e, string *o)
{
	_o = o;
	for_each(s, e, [&](char c) {prp(c);});
}


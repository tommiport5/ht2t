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
#include <cstdlib>
#include <cstring>
#include <map>

using namespace std;


const map<string, string> EntityTable = {
		{"&quot;", u8"\""},
		{"&amp;", u8"&"},
		{"&lt;", u8"<"},
		{"&gt;", u8">"},
		{"&apos;", u8"'"},
		{"&auml;", u8"ä"},
		{"&ouml;", u8"ö"},
		{"&uuml;", u8"ü"},
		{"&Auml;", u8"Ä"},
		{"&Ouml;", u8"Ö"},
		{"&Uuml;", u8"Ü"},
		{"&nbsp;", u8" "},
		{"&deg;", u8"°"},
		{"&hellip;", u8"..."},
		{"&copy;", u8"©"}
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
		if (isspace(c)) {
			*_o += ent;
			ppstate = isblank(c) ? blank : lf;
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

const std::string& Filter::lookupEntity(const std::string& enty)
{
	union {long l; char c[sizeof(long)];} cres;
	if (enty[1] == '#') {
		if (tolower(enty[2] == 'x')) {
			cres.l = strtol(enty.c_str() + 2, NULL, 16);
		} else {
			cres.l = strtol(enty.c_str() + 2, NULL, 10);
		}
		ent.clear();
		bool found = false;
		for (char *p = &cres.c[sizeof(long)-1]; p >= &cres.c[0]; --p) {
			if (found || *p != '\0') {
				ent += *p;
				found = true;
			}
		}
		return ent;
	}
	try {
		return EntityTable.at(enty);
	} catch (const out_of_range) {
		return enty;
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


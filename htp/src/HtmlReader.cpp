/*
 * HtmlReader.cpp
 *
 *  Created on: 20.06.2018
 *      Author: dad
 */

#include "HtmlReader.h"
#include <functional>
#include <cctype>

using namespace std;

HtmlReader::~HtmlReader()
{
	// nothing to do yet
}

HtmlReader::HtmlReader(istream& id)
: idata(id)
{
	sh = &outside;
}

string&& HtmlReader::getBuffer()
{
	Buffer.clear();

	while (!idata.eof()) {
		int c = idata.get();
		Buffer += c;
		if (sh(this, c))
			break;
	}

	return move(Buffer);
}

bool HtmlReader::moreFollows()
{
	return !idata.eof();
}

bool HtmlReader::outside(HtmlReader* pThis, int c)
{
	if (c == '<') pThis->sh = &lt;
	return false;
}

bool HtmlReader::lt(HtmlReader* pThis, int c)
{
	if (c == '/') pThis->sh = &sl;
	else if (!isblank(c)) pThis->sh = &outside;
	return false;
}

bool HtmlReader::sl(HtmlReader* pThis, int c)
{
	if (tolower(c) == 'h') pThis->sh = &h;
	else if (!isblank(c)) pThis->sh = &outside;
	return false;
}

bool HtmlReader::h(HtmlReader* pThis, int c)
{
	if (tolower(c) == 't') pThis->sh = &t;
	else pThis->sh = &outside;
	return false;
}

bool HtmlReader::t(HtmlReader* pThis, int c)
{
	if (tolower(c) == 'm') pThis->sh = &m;
	else pThis->sh = &outside;
	return false;
}

bool HtmlReader::m(HtmlReader* pThis, int c)
{
	if (tolower(c) == 'l') pThis->sh = &l;
	else pThis->sh = &outside;
	return false;
}

bool HtmlReader::l(HtmlReader* pThis, int c)
{
	if (c == '>') {
		pThis->sh = &outside;
		return true;
	}
	else if (!isblank(c)) pThis->sh = &outside;
	return false;
}

/*
 * HtmlReader.h
 *
 *  Created on: 20.06.2018
 *      Author: dad
 */

#ifndef HTMLREADER_H_
#define HTMLREADER_H_

#include <istream>
#include <string>

class HtmlReader {
	std::istream &idata;
	std::string Buffer;

	typedef bool (*StateHandler)(HtmlReader *, int);
	StateHandler sh;

	static bool outside(HtmlReader *pThis, int c);
	static bool lt(HtmlReader *pThis, int c);
	static bool sl(HtmlReader *pThis, int c);
	static bool h(HtmlReader *pThis, int c);
	static bool t(HtmlReader *pThis, int c);
	static bool m(HtmlReader *pThis, int c);
	static bool l(HtmlReader *pThis, int c);

public:
	virtual ~HtmlReader();
	HtmlReader(std::istream &id);
	HtmlReader() = delete;

	std::string &&getBuffer();
	bool moreFollows();
};

#endif /* HTMLREADER_H_ */

/*
 * HtmlParser.h
 *
 *  Created on: 21.03.2018
 *      Author: Heckmann
 */

#ifndef HTMLPARSER_H_
#define HTMLPARSER_H_

#include <fstream>
#include <list>
#include <regex>

#include "Node.h"

class HtmlParser {
	std::ifstream ifs;

	std::string Buf;

	// "machine readable" version of the HTML string
	// created by a "quick parse"
	std::list<Node> End;
	std::list<Node> Parsed;
	std::list<Node>::const_iterator currentParagraph;

	bool findHeaderOrParagraph();

	bool debug;

	std::regex TagRegex;
	std::regex EndRegex;

public:
	HtmlParser(const std::string &path);
	virtual ~HtmlParser();

	bool eof();
	void quickParse();
	void print();
	bool getHeaderOrParagraph(std::string &result);
};

#endif /* HTMLPARSER_H_ */

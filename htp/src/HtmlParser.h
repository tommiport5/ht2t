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
	//std::list<Node>::const_iterator currentParagraph;

	std::pair<unsigned long, unsigned long> getRowCol(unsigned long pos);

	std::list<Node *> NodeStack;
	std::list<std::list<Node>::iterator> ToErase;

	bool debug;

	std::regex TagRegex;
	std::regex EndRegex;

public:
	HtmlParser(const std::string &path);
	virtual ~HtmlParser();

	bool eof();
	void quickParse();
	bool structurize();
	bool getExtractedText(std::string &result);
	void print();
};

#endif /* HTMLPARSER_H_ */

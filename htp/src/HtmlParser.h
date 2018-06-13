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
#include "Filter.h"

class HtmlParser {

	std::string Buf;

	void findExpressions(std::regex &pat, std::list<Node> &res, bool findsEnd);
	void findMetas() {findExpressions(MetaRegex, Metas, false);};

	// "machine readable" version of the HTML string
	// created by a "quick parse"
	std::list<Node> Ends;
	std::list<Node> Parsed;
	std::list<Node> Texts;
	std::list<Node> Metas;
	std::list<Node>::iterator Current;

	std::pair<unsigned long, unsigned long> getRowCol(unsigned long pos);

	std::list<Node *> NodeStack;
	std::list<std::list<Node>::iterator> ToErase;

	bool debug;

	std::regex TagRegex;
	std::regex EndRegex;
	std::regex MetaRegex;

	Filter Fil;

	std::string InputCharset;

public:
	HtmlParser(const std::string &path);
	virtual ~HtmlParser();

	std::string &readCharset();
	bool convert2UTF_8();
	void quickParse();
	bool structurize();
	void print();
	void printET();
	void startExtracting() {Current = Texts.begin();Fil.reset();};
	bool getExtractedText(std::string &result);
};

#endif /* HTMLPARSER_H_ */

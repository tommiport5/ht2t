/*
 * Filter.h
 *
 *  Created on: 02.06.2018
 *      Author: dad
 */

#ifndef FILTER_H_
#define FILTER_H_

#include <string>

class Filter {
	std::string *_o;
	std::string ent;

	enum PPState {neutral, entity, lf, blank};
	PPState ppstate;


	void prp(char c);
	const std::string &lookupEntity(const std::string &ent);

public:
	virtual ~Filter();


	void reset();
	void preprocess(std::string::const_iterator s, std::string::const_iterator e, std::string *o);
};

#endif /* FILTER_H_ */

/*
 * NodeType.h
 *
 *  Created on: 24.05.2018
 *      Author: dad
 */

#ifndef NODETYPE_H_
#define NODETYPE_H_

#include <string>
#include <map>

class NodeType {
public:
	enum T {a, b, body, br, button, col, comment, div, form, h, head, html, i, img, li, meta, noscript,
		option, p, script, span, style, table, td, th, tr, title, u, ul, unknown, text};
	enum H {stop, embed, discard, skip};
	NodeType(const std::string &s);
	NodeType(const NodeType &rhs): _Typ(rhs._Typ) {};
	NodeType(const NodeType::T t): _Typ(t) {};
	const bool operator==(const NodeType &rhs) const;
	operator const std::string & () const;
	const T Typ() const {return _Typ;};
	const H Hnd() {return _Hnd[_Typ];};
private:
	T _Typ;
	static const H _Hnd[];
	static const std::string UnknownString;
	static const std::string HString;
	static const std::string TextString;
	static const std::map<const std::string, const T> NTab ;
};

#endif /* NODETYPE_H_ */

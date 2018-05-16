/*
 * Node.h
 *
 *  Created on: 02.05.2018
 *      Author: Heckmann
 */

#ifndef NODE_H_
#define NODE_H_

#include <list>
#include <string>
#include <utility>
#include <map>

class NodeType {

	static const std::string UnknownString;
	static const std::string HString;
public:
	enum T {a, b, body, br, button, col, div, form, h, head, html, i, img, li, meta, option, p, script, span, table, td, th, tr, title, u, ul, unknown};
	NodeType(const std::string &s);
	NodeType(const NodeType &rhs): _Typ(rhs._Typ) {};
	const bool operator==(const NodeType &rhs) const;
	operator const std::string & () const;
	const T Typ() const {return _Typ;};
private:
	T _Typ;
	static const std::map<const std::string, const T> NTab ;
};

class Node {
	NodeType _nt;
	unsigned long start;
	std::string _raw;
	bool _isEnd;
	std::list<Node> nested;
	// std::map<std::string> attributes (maybe later)
public:
	//virtual ~Node();
	Node(NodeType Nt, unsigned long Start, std::string &&Raw, bool End) : _nt(Nt), start(Start), _raw(Raw) , _isEnd(End) {};
	bool operator<(const Node &rhs) const;
	operator const std::string & () const;
	const bool isEnd() const {return _isEnd;};
	const std::string &raw() const {return _raw;};
	const NodeType::T getTyp()const {return _nt.Typ();};
	std::string &&completeNode(const std::string &text, std::list<Node>::const_iterator &n, std::list<Node>::const_iterator end) const;
};

#endif /* NODE_H_ */

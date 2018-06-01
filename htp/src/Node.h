/*
 * Node.h
 *
 *  Created on: 02.05.2018
 *      Author: Heckmann
 */

#ifndef NODE_H_
#define NODE_H_

#include <list>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <functional>
#include "NodeType.h"

class Node {
	NodeType _nt;
	unsigned long _start, _lastPosition;
	std::string _raw;
	bool _isEnd;
	std::list<Node> _nested;
	// std::map<std::string> attributes (maybe later)
public:
	virtual ~Node();
	Node(const Node &rhs);
	Node(NodeType Nt, unsigned long Start, std::string &&Raw, bool End);
	bool operator<(const Node &rhs) const;
	operator const std::string () const;

	unsigned long start() const {return _start;};
	const bool isEnd() const {return _isEnd;};
	const std::string &raw() const {return _raw;};
	const NodeType::T getTyp()const {return _nt.Typ();};
	std::list<Node> &nested() {return _nested;};
	unsigned long getLastPosition() {return _lastPosition;};
	void setLastPosition(unsigned long pos) {_lastPosition = pos;};
	void setRaw(const std::string &r) {_raw = r;};
	void makeEnd(bool is) {_isEnd = is;};

	bool shallBePrinted();

	static bool hasChildren(Node &n) {return !n._nested.empty();};
	static bool always(Node &) {return true;};
	static bool isPrintable(Node &);
	//static void insertBackground(Node &);

	unsigned long getOverallEnd();
	Node* embed(std::list<Node>::iterator p);
	Node* embed(Node *p);
	void extractText(std::string &to, std::string &from);

	void forAllChildrenThat(std::function<bool(Node&)> condition, std::function<void(Node&, int)> operation, int level = 1);
};

#endif /* NODE_H_ */

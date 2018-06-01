/*
 * Node.cpp
 *
 *  Created on: 02.05.2018
 *      Author: Heckmann
 */

#include "Node.h"
#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;



Node::Node(NodeType Nt, unsigned long Start, std::string &&Raw, bool End) :
_nt(Nt)
, _start(Start)
,_lastPosition(0)
, _raw(Raw)
, _isEnd(End)
{
}

Node::Node(const Node &rhs):
_nt(rhs._nt)
,_start(rhs._start)
,_lastPosition(rhs._lastPosition)
,_raw(rhs._raw)
,_isEnd(rhs._isEnd)
{
}

Node::~Node()
{
}

bool Node::operator<(const Node &rhs) const
{
	return _start < rhs._start;
}

Node::operator const string () const
{
	string res;

	if (_isEnd) res = "/";
	else res = "";

	res += _nt;

	return res;
}

/**
 * shallBePrinted
 * used to filter out unwanted nodes
 * this is a method that can be applied to a node as opposed to isPrintable below, which is a predicate for use in forAllChildrenThat
 */
bool Node::shallBePrinted()
{
	return getTyp() != NodeType::script;
}

bool Node::isPrintable(Node &n)
{
	return n.getTyp() == NodeType::text;
}

/**
 * embed
 * Add a copy of the Node pointed to by p into the nested list of this node.
 * Returns a pointer to this copy.
 * Comes in two flavors, because you cannot make an iterator from a pointer.
 */
Node* Node::embed(std::list<Node>::iterator p)
{
	//cout << "Pushing " << (const string)*p << "(Pos.: " << p->_start <<  ", 0x" << hex << (unsigned long)&(*p) <<  ") into " << (const string)*this;
	//cout << dec << "(Pos.: " << _start << ", 0x" << hex << (unsigned long)this << dec << ")" << endl;
	_nested.push_back(*p);
	return &_nested.back();
}

Node* Node::embed(Node* p)
{
	_nested.push_back(*p);
	return &_nested.back();
}

unsigned long Node::getOverallEnd()
{
	if (_nested.empty()) {
		return getLastPosition();
	} else {
		return _nested.back().getLastPosition();
	}
}


void Node::extractText(std::string &to, std::string &from)
{
	//int StartOfString = _start + _raw.length();
	//to += from.substr(StartOfString, _lastPosition - StartOfString);
	to += _raw;
}


/**
 * forAllChildren
 * traverses the tree below the current node, depth first.
 * Executes the operation on each node where the condition is met.
 */
void Node::forAllChildrenThat(std::function<bool(Node&)> condition, std::function<void(Node&, int)> operation, int level)
{
	if (condition(*this)) operation(*this, level);
	for_each(_nested.begin(), _nested.end(), [&condition, &operation, level](Node &n) {
		n.forAllChildrenThat(condition, operation, level+1);
	});

}


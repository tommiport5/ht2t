/*
 * State.cpp
 *
 *  Created on: 29.05.2018
 *      Author: dad
 */

#include <algorithm>
#include "State.h"

#include "IdleState.h"
#include "HeadState.h"
#include "BodyState.h"
#include "DivState.h"
#include "AState.h"
#include "HState.h"
#include "PState.h"

Node UnknownNode = {NodeType(NodeType::unknown),0,"",false};


IState* State::pushAndChange(Context& ctx, std::list<Node>::iterator event,
		IState* newState)
{
	Node *emb = ctx.topOfStack()->embed(event);
	ctx.push(emb);
	ctx.eraseDelayed(event);
	return newState;
}

IState* State::popAndChange(Context& ctx, std::list<Node>::iterator event)
{
	ctx.topOfStack()->embed(event);
	ctx.pop();
	ctx.eraseDelayed(event);
	return ctx.getStateFromNode(ctx.topOfStack());
}


IState* State::embedAndContinue(Context& ctx, std::list<Node>::iterator event)
{
	ctx.topOfStack()->embed(event);
	ctx.eraseDelayed(event);
	return this;
}

void Context::push(Node *node)
{
	NodeStack.push_front(node);
}

Node* Context::topOfStack()
{
	return NodeStack.front();
}

Node* Context::pop()
{
	Node *ret;

	if (NodeStack.empty()) {
		ret = &UnknownNode;
	} else {
		ret = NodeStack.front();
		NodeStack.pop_front();
	}
	return ret;
}
void Context::eraseDelayed(std::list<Node>::iterator& n)
{
	ToErase.push_back(n);
}



IState* Context::getStateFromNode(Node* cur)
{
	switch(cur->getTyp()) {
	case NodeType::head:
		return &Head;
	case NodeType::body:
		return &Body;
	case NodeType::div:
		return &Div;
	case NodeType::h:
		return &H;
	case NodeType::p:
		return &P;
	case NodeType::a:
		return &A;
	default:
		return &Idle;
	}
}

void Context::doErase(std::list<Node> prs)
{
	for_each(ToErase.begin(), ToErase.end(), [&prs] (std::list<Node>::iterator p) {
		prs.erase(p);
	});
	ToErase.clear();
}
/*
int StackSize = NodeStack.size();
if (StackSize) {
	for_each(NodeStack.begin(), NodeStack.end(), [](auto p) {
		cout << (const string) *p << " ";
	});
	cout << endl;
}
*/

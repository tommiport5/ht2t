/*
 * State.h
 *
 *  Created on: 29.05.2018
 *      Author: dad
 *
 *      Interface for the states and context for the state machine
 */

#ifndef ISSTATE_H_
#define ISSTATE_H_

#include "../Node.h"

extern Node UnknownNode;

class Context;

class IState {
public:
	virtual IState *handleState(Context &ctx, std::list<Node>::iterator event) = 0;
};

class Context {
	std::list<std::list<Node>::iterator> ToErase;
	std::list<Node *> NodeStack;
	IState *pCur;
public:
	Context(IState &InitialState) : pCur(&InitialState) {};
	virtual ~Context() {};

	void changeState(IState *newState) {pCur = newState;};
	void eraseDelayed(std::list<Node>::iterator &n);
	void push(Node *node);
	Node *topOfStack();
	Node *pop();
	IState *getStateFromNode(Node *cur);

	void doErase(std::list<Node> prs);
	IState *getCurrentState() {return pCur;};
};

#endif /* ISSTATE_H_ */

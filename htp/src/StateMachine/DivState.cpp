/*
 * DivState.cpp
 *
 *  Created on: 30.05.2018
 *      Author: dad
 */

#include "DivState.h"

#include "HState.h"
#include "PState.h"
#include "AState.h"

DivState Div;

IState *DivState::handleState(Context& ctx, std::list<Node>::iterator event)
{
	IState *state = &Div;

	Node *emb = ctx.topOfStack()->embed(event);
	if (event->getTyp() == NodeType::h) {
		ctx.push(emb);
		state = &H;
	}
	if (event->getTyp() == NodeType::p) {
		ctx.push(emb);
		state = &P;
	}
	if (event->getTyp() == NodeType::a) {
		ctx.push(emb);
		state = &A;
	}
	if (event->getTyp() == NodeType::div) {
		if (event->isEnd()) {
			ctx.pop();
			state = ctx.getStateFromNode(ctx.topOfStack());
		} else {
			// nested div
			ctx.push(emb);
		}
	}
	ctx.eraseDelayed(event);
	return state;
}

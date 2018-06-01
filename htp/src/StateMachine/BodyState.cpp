/*
 * BodyState.cpp
 *
 *  Created on: 30.05.2018
 *      Author: dad
 */

#include "BodyState.h"

#include "IdleState.h"
#include "DivState.h"
#include "PState.h"
#include "ScriptState.h"

BodyState Body;

//TODO event h ergänzen

IState *BodyState::handleState(Context& ctx, std::list<Node>::iterator event)
{
	IState *state = &Body;

	if (event->getTyp() == NodeType::html) {
		// premature
		ctx.pop();
		return &Idle;
	}
	Node *emb = ctx.topOfStack()->embed(event);
	if (event->getTyp() == NodeType::div) {
		ctx.push(emb);
		state = &Div;
	}
	if (event->getTyp() == NodeType::p) {
		ctx.push(emb);
		state = &P;
	}
	if (event->getTyp() == NodeType::script) {
		ctx.push(emb);
		state = &Script;
	}
	if (event->getTyp() == NodeType::body && event->isEnd()) {
		ctx.pop();
		state = &Idle;
	}
	ctx.eraseDelayed(event);
	return state;
}

/*
 * IdleState.cpp
 *
 *  Created on: 29.05.2018
 *      Author: dad
 */

#include "IdleState.h"

#include "HeadState.h"
#include "BodyState.h"
#include "ScriptState.h"

IdleState Idle;

IState *IdleState::handleState(Context &ctx, std::list<Node>::iterator event)
{
	if (event->getTyp() == NodeType::head) {
		ctx.push(&*event);
		return &Head;
	}
	if (event->getTyp() == NodeType::body) {
		ctx.push(&*event);
		return &Body;
	}
	if (event->getTyp() == NodeType::script) {
		ctx.push(&UnknownNode);	// placeholder, just to be popped
		ctx.eraseDelayed(event);
		return &Script;
	}
	return &Idle;
}

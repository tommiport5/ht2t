/*
 * IdleState.cpp
 *
 *  Created on: 29.05.2018
 *      Author: dad
 */

#include "IdleState.h"

#include "HeadState.h"
#include "BodyState.h"
#include "IgnoredNTStates.h"

IdleState Idle;

IState *IdleState::handleState(Context &ctx, std::list<Node>::iterator event)
{
	switch (event->getTyp()) {
	case NodeType::head:
		ctx.push(&*event);
		return &Head;
	case NodeType::body:
		ctx.push(&*event);
		return &Body;
	case NodeType::script:
		ctx.push(&*event);
		ctx.eraseDelayed(event);
		return &Script;
	default:
		return &Idle;
	}

/*
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
*/
}

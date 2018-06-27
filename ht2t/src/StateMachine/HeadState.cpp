/*
 * HeadState.cpp
 *
 *  Created on: 29.05.2018
 *      Author: dad
 */

#include "HeadState.h"

#include "IdleState.h"
#include "BodyState.h"
#include "IgnoredNTStates.h"

HeadState Head;

IState *HeadState::handleState(Context& ctx, std::list<Node>::iterator event)
{
	switch (event->getTyp()) {
	case NodeType::script:
		return pushAndChange(ctx, event, &Script);
	case NodeType::style:
		return pushAndChange(ctx, event, &Style);
	case NodeType::body:
		// error </head> missing
		ctx.pop();
		ctx.push(&*event);
		return &Body;
	case NodeType::head:
		if (event->isEnd()) {
			ctx.topOfStack()->embed(event);
			ctx.pop();
			ctx.eraseDelayed(event);
			return &Idle;
		} else {
			// error, second <head> tag
			return embedAndContinue(ctx, event);
		}
	default:
		return embedAndContinue(ctx, event);
	}

/*
	if (event->getTyp() == NodeType::script) {
		ctx.push(&UnknownNode);	// placeholder, just to be popped
		//ToErase.push_back(event++);
		return &Script;
	}
	if (event->getTyp() == NodeType::body) {
		// premature
		ctx.pop();
		ctx.push(&*event);
		return &Body;
	}
	ctx.topOfStack()->embed(event);
	if (event->getTyp() == NodeType::head && event->isEnd()) {
		ctx.pop();
		ctx.eraseDelayed(event);
		return &Idle;
	}
	ctx.eraseDelayed(event);
	return &Head;
*/
}

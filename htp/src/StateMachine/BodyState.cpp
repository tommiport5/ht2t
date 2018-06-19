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
#include "IgnoredNTStates.h"

BodyState Body;

//TODO event h ergänzen

IState *BodyState::handleState(Context& ctx, std::list<Node>::iterator event)
{
	switch (event->getTyp()) {
	case NodeType::html:
		// </body> missing
		ctx.pop();
		return &Idle;
	case NodeType::div:
		return pushAndChange(ctx, event, &Div);
	case NodeType::p:
		return pushAndChange(ctx, event, &P);
	case NodeType::script:
		return pushAndChange(ctx, event, &Script);
	case NodeType::style:
		return pushAndChange(ctx, event, &Style);
	case NodeType::table:
		return pushAndChange(ctx, event, &Table);
	case NodeType::body:
		if (event->isEnd()) {
			ctx.topOfStack()->embed(event);
			ctx.eraseDelayed(event);
			ctx.pop();
			return &Idle;
		} else {
			// illegal: nested body
			return embedAndContinue(ctx, event);
		}
	default:
		return embedAndContinue(ctx, event);
	}
}

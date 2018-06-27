/*
 * AState.cpp
 *
 *  Created on: 30.05.2018
 *      Author: dad
 */

#include "AState.h"

AState A;

IState *AState::handleState(Context& ctx, std::list<Node>::iterator event)
{
	switch (event->getTyp()) {
	case NodeType::a:
		if (event->isEnd())
			return popAndChange(ctx, event);
		else {
			// </a> missing
			Node *emb = ctx.topOfStack()->embed(event);
			ctx.eraseDelayed(event);
			ctx.pop();
			ctx.push(emb);
			// state remains the same
			return this;
		}
	default:
		return embedAndContinue(ctx, event);
	}
}

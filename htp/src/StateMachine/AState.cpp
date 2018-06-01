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
	IState *state = &A;
	Node *emb = ctx.topOfStack()->embed(event);
	if (event->getTyp() == NodeType::a) {
		if (event->isEnd()) {
			ctx.pop();
			state = ctx.getStateFromNode(ctx.topOfStack());
		} else {
			// </p> missing
			ctx.pop();
			ctx.push(emb);
			// state remains the same
		}
	}
	ctx.eraseDelayed(event);
	return state;
}

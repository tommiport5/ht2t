/*
 * HState.cpp
 *
 *  Created on: 30.05.2018
 *      Author: dad
 */

#include "HState.h"

HState H;

IState *HState::handleState(Context& ctx, std::list<Node>::iterator event)
{
	IState *state = &H;

	Node *emb = ctx.topOfStack()->embed(event);
	if (event->getTyp() == NodeType::h) {
		if (event->isEnd()) {
			ctx.pop();
			state = ctx.getStateFromNode(ctx.topOfStack());
		} else {
			// </h> missing
			ctx.pop();
			ctx.push(emb);
			// state remains the same
		}
	}
	ctx.eraseDelayed(event);
	return state;
}

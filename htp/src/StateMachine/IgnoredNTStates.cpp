/*
 * ScriptState.cpp
 *
 *  Created on: 30.05.2018
 *      Author: dad
 */

#include "IgnoredNTStates.h"

ScriptState Script;

IState *IgnoredNTState::handleState(Context& ctx, std::list<Node>::iterator event)
{
	IState *state = this;

	ctx.topOfStack()->embed(event);
	if (event->getTyp() == nt.Typ()){
		if (event->isEnd()) {
			ctx.pop();
			state = ctx.getStateFromNode(ctx.topOfStack());
		}
	}
	ctx.eraseDelayed(event);
	return state;
}

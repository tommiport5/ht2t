/*
 * ScriptState.cpp
 *
 *  Created on: 30.05.2018
 *      Author: dad
 */

#include "ScriptState.h"

ScriptState Script;

IState *ScriptState::handleState(Context& ctx, std::list<Node>::iterator event)
{
	IState *state = &Script;

	ctx.topOfStack()->embed(event);
	if (event->getTyp() == NodeType::script){
		if (event->isEnd()) {
			ctx.pop();
			state = ctx.getStateFromNode(ctx.topOfStack());
		}
	}
	ctx.eraseDelayed(event);
	return state;
}

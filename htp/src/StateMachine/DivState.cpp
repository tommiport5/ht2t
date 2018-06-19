/*
 * DivState.cpp
 *
 *  Created on: 30.05.2018
 *      Author: dad
 */

#include "DivState.h"

#include "HState.h"
#include "PState.h"
#include "AState.h"
#include "IgnoredNTStates.h"

DivState Div;

IState *DivState::handleState(Context& ctx, std::list<Node>::iterator event)
{
	switch (event->getTyp()) {
	case NodeType::h:
		return pushAndChange(ctx, event, &H);
	case NodeType::p:
		return pushAndChange(ctx, event, &P);
	case NodeType::a:
		return pushAndChange(ctx, event, &A);
	case NodeType::script:
		return pushAndChange(ctx, event, &Script);
	case NodeType::style:
		return pushAndChange(ctx, event, &Style);
	case NodeType::table:
		return pushAndChange(ctx, event, &Table);
	case NodeType::div:
		if (event->isEnd())
			return popAndChange(ctx, event);
		else
			return pushAndChange(ctx, event, &Div);
	default:
		return embedAndContinue(ctx, event);
	}
}

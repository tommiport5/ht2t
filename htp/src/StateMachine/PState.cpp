/*
 * PState.cpp
 *
 *  Created on: 30.05.2018
 *      Author: dad
 */

#include "PState.h"

#include "HState.h"
#include "AState.h"


PState P;

IState *PState::handleState(Context& ctx, std::list<Node>::iterator event)
{
	switch (event->getTyp()) {
	case NodeType::p:
		if (event->isEnd()) {
			return popAndChange(ctx, event);
		} else {
			// </p> missing, we must embed an artificial </p> to mark the end of the previous <p>
			Node arti(*event);	// copy start position etc
			arti.setLastPosition(arti.start());
			// also set end marker and "" text
			arti.makeEnd(true);
			arti.setRaw("");
			ctx.topOfStack()->embed(&arti);
			ctx.pop();
			Node *emb = ctx.topOfStack()->embed(event);
			ctx.push(emb);
			// state remains the same
			return this;
		}
	case NodeType::h:
		return pushAndChange(ctx, event,&H);
	case NodeType::a:
		return pushAndChange(ctx, event,&A);
	default:
		return embedAndContinue(ctx, event);
	}

/*
	IState *state = &P;
	Node *emb;

	if (event->getTyp() == NodeType::p) {
		if (event->isEnd()) {
			emb = ctx.topOfStack()->embed(event);
			ctx.pop();
			state = ctx.getStateFromNode(ctx.topOfStack());
		} else {
			// </p> missing, we must embed an artificial </p> to mark the end of the previous <p>
			Node arti(*event);	// copy start position etc
			arti.setLastPosition(arti.start());
			// also set end marker and "" text
			arti.makeEnd(true);
			arti.setRaw("");
			ctx.topOfStack()->embed(&arti);
			ctx.pop();
			emb = ctx.topOfStack()->embed(event);
			ctx.push(emb);
			// state remains the same
		}
	} else {
		emb = ctx.topOfStack()->embed(event);
	}
	if (event->getTyp() == NodeType::h) {
		ctx.push(emb);
		state = &H;
	}
	if (event->getTyp() == NodeType::a) {
		ctx.push(emb);
		state = &A;
	}
	ctx.eraseDelayed(event);
	return state;
*/
}

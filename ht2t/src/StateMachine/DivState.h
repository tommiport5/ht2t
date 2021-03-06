/*
 * DivState.h
 *
 *  Created on: 30.05.2018
 *      Author: dad
 */

#ifndef DIVSTATE_H_
#define DIVSTATE_H_

#include "State.h"

class DivState : public State
{
	public:
	virtual IState *handleState(Context &ctx, std::list<Node>::iterator event);
	virtual ~DivState() {};
};

extern DivState Div;

#endif /* DIVSTATE_H_ */

/*
 * IdleState.h
 *
 *  Created on: 29.05.2018
 *      Author: dad
 */

#ifndef IDLESTATE_H_
#define IDLESTATE_H_

#include "State.h"

class IdleState: public State {
public:
	virtual IState *handleState(Context &ctx, std::list<Node>::iterator event);
};

extern IdleState Idle;

#endif /* IDLESTATE_H_ */

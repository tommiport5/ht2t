/*
 * PState.h
 *
 *  Created on: 30.05.2018
 *      Author: dad
 */

#ifndef PSTATE_H_
#define PSTATE_H_

#include "State.h"

class PState: public State {
	public:
	virtual IState *handleState(Context &ctx, std::list<Node>::iterator event);
	virtual ~PState() {};
};

extern PState P;

#endif /* PSTATE_H_ */

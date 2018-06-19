/*
 * HState.h
 *
 *  Created on: 30.05.2018
 *      Author: dad
 */

#ifndef HSTATE_H_
#define HSTATE_H_

#include "State.h"

class HState : public State {
	virtual IState *handleState(Context &ctx, std::list<Node>::iterator event);
};

extern HState H;

#endif /* HSTATE_H_ */

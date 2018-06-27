/*
 * AState.h
 *
 *  Created on: 30.05.2018
 *      Author: dad
 */

#ifndef ASTATE_H_
#define ASTATE_H_

#include "State.h"

class AState: public State {
public:
	virtual IState *handleState(Context &ctx, std::list<Node>::iterator event);
	virtual ~AState() {};
};

extern AState A;

#endif /* ASTATE_H_ */

/*
 * BodyState.h
 *
 *  Created on: 30.05.2018
 *      Author: dad
 */

#ifndef BODYSTATE_H_
#define BODYSTATE_H_

#include "State.h"

class BodyState: public State {
	virtual IState *handleState(Context &ctx, std::list<Node>::iterator event);
};

extern BodyState Body;

#endif /* BODYSTATE_H_ */

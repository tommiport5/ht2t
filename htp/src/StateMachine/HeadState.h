/*
 * HeadState.h
 *
 *  Created on: 29.05.2018
 *      Author: dad
 */

#ifndef HEADSTATE_H_
#define HEADSTATE_H_

#include "State.h"

class HeadState: public IState {
public:
	virtual IState *handleState(Context &ctx, std::list<Node>::iterator event);
};

extern HeadState Head;

#endif /* HEADSTATE_H_ */

/*
 * ScriptState.h
 *
 *  Created on: 30.05.2018
 *      Author: dad
 */

#ifndef SCRIPTSTATE_H_
#define SCRIPTSTATE_H_

#include "State.h"

class ScriptState: public IState {
	virtual IState *handleState(Context &ctx, std::list<Node>::iterator event);
};

extern ScriptState Script;

#endif /* SCRIPTSTATE_H_ */

/*
 * ScriptState.h
 *
 *  Created on: 30.05.2018
 *      Author: dad
 */

#ifndef SCRIPTSTATE_H_
#define SCRIPTSTATE_H_

#include "State.h"
#include "../NodeType.h"

class IgnoredNTState: public State {
public:
	virtual IState *handleState(Context &ctx, std::list<Node>::iterator event);
protected:
	NodeType nt;
	IgnoredNTState(NodeType Nt) :nt(Nt) {};
	//prohibit construction without NodeType:
	IgnoredNTState() = delete;
};

class ScriptState : public IgnoredNTState {
public:
	ScriptState() : IgnoredNTState(NodeType::script) {};
};

extern ScriptState Script;

class StyleState : public IgnoredNTState {
public:
	StyleState() : IgnoredNTState(NodeType::style) {};
};

extern StyleState Style;

class TableState : public IgnoredNTState {
public:
	TableState() : IgnoredNTState(NodeType::table) {};
};

extern TableState Table;

#endif /* SCRIPTSTATE_H_ */

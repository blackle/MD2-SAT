#pragma once
#include "Variable.h"

class Solver;

class OneHotConstraint {
public:
	static void at_least_one(Solver* solver, const VariableList& vars);
	static void at_most_one(Solver* solver, const VariableList& vars);
	static void naive(Solver* solver, const VariableList& vars);
	static void commander(Solver* solver, const VariableList& vars, size_t group_size);
	static Variable commander_variable(Solver* solver, const VariableList& vars);

private:
	OneHotConstraint() = delete;
};

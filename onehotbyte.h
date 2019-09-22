#pragma once
#include <cryptominisat5/cryptominisat.h>
#include "variable.h"
#include "solver.h"

class OneHotByte
{
public:
	OneHotByte(Solver* solver);

	Variable at(size_t i) const;
	const VariableList& all() const;
	inline int length() const { return 256; }

	int to_int() const;

private:
	Solver* m_solver;
	VariableList m_vars;
};

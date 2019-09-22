#pragma once
#include <cryptominisat5/cryptominisat.h>
#include <vector>
#include "Variable.h"

class Solver;

class OneHotByte
{
public:
	explicit OneHotByte(Solver* solver);
	explicit OneHotByte(Solver* solver, const VariableList& vars);

	Variable at(size_t i) const;
	const VariableList& all() const;
	constexpr int length() const { return 256; }

	int to_int() const;

private:
	Solver* m_solver;
	VariableList m_vars;
};

typedef std::vector<OneHotByte> OneHotByteList;

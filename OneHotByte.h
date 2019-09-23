#pragma once
#include <cryptominisat5/cryptominisat.h>
#include <vector>
#include "Variable.h"
#include <iostream>

class Solver;

class OneHotByte
{
public:
	explicit OneHotByte(Solver* s);
	explicit OneHotByte(Solver* s, const VariableList& vars);

	Variable at(size_t i) const;
	const VariableList& all() const;
	constexpr size_t length() const { return 256; }

	uint8_t to_int() const;
	void assume(uint8_t val) const;

	Solver* solver() const;

	friend std::ostream& operator<<(std::ostream& os, const OneHotByte& byte);

private:
	Solver* m_solver;
	VariableList m_vars;
};

typedef std::vector<OneHotByte> OneHotByteList;

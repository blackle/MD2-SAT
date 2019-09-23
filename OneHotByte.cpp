#include "OneHotByte.h"
#include "OneHotConstraint.h"
#include "Solver.h"
#include <assert.h>
#include <iomanip>

OneHotByte::OneHotByte(Solver* s)
	: m_solver(s)
	, m_vars(s->new_vars(length()))
{
	OneHotConstraint::commander(m_solver, all(), 4);
}

OneHotByte::OneHotByte(Solver* s, const VariableList& vars)
	: m_solver(s)
	, m_vars(vars)
{
	assert(m_vars.size() == length());
}

Variable OneHotByte::at(size_t i) const
{
	return m_vars.at(i);
}

const VariableList& OneHotByte::all() const
{
	return m_vars;
}

uint8_t OneHotByte::to_int() const
{
	auto model = m_solver->get_model();
	for (size_t i = 0; i < length(); i++) {
		if (model[m_vars[i].var()] == CMSat::l_True) {
			return i;
		}
	}
	return -1;
}

void OneHotByte::assume(uint8_t val) const
{
	for (size_t i = 0; i < length(); i++) {
		m_solver->assume(at(i) ^ (i != val));
	}
}

Solver* OneHotByte::solver() const
{
	return m_solver;
}

std::ostream& operator<<(std::ostream& os, const OneHotByte& byte)
{
	os << std::setw(3) << (int)byte.to_int();
	return os;
}

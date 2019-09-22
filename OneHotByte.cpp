#include "OneHotByte.h"
#include "OneHotConstraint.h"
#include "Solver.h"

OneHotByte::OneHotByte(Solver* solver)
	: m_solver(solver)
	, m_vars(solver->new_vars(length()))
{
	OneHotConstraint::commander(solver, all(), 4);
}

OneHotByte::OneHotByte(Solver* solver, const VariableList& vars)
	: m_solver(solver)
	, m_vars(vars)
{}

Variable OneHotByte::at(size_t i) const
{
	return m_vars.at(i);
}

const VariableList& OneHotByte::all() const
{
	return m_vars;
}

int OneHotByte::to_int() const
{
	auto model = m_solver->get_model();
	for (int i = 0; i < length(); i++) {
		if (model[m_vars[i].var()] == CMSat::l_True) {
			return i;
		}
	}
	return -1;
}

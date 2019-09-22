#include "solver.h"

Solver::Solver()
	: m_clause_count(0)
{}

Variable Solver::new_var()
{
	m_sat_solver.new_var();
	return Variable(m_sat_solver.nVars()-1, false);
}

VariableList Solver::new_vars(const size_t n)
{
	size_t initial = m_sat_solver.nVars();
	m_sat_solver.new_vars(n);

	VariableList vars;
	for (size_t i = initial; i < initial+n; i++) {
		vars.push_back(Variable(i, false));
	}
	return vars;
}

unsigned Solver::var_count() const
{
	return m_sat_solver.nVars();
}

void Solver::add_clause(const VariableList& vars)
{
	m_clause_count++;
	m_sat_solver.add_clause(vars);
}

unsigned Solver::clause_count() const
{
	return m_clause_count;
}

CMSat::lbool Solver::solve()
{
	return m_sat_solver.solve();
}

const std::vector<CMSat::lbool>& Solver::get_model() const
{
	return m_sat_solver.get_model();
}

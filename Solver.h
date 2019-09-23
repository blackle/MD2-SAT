#pragma once
#include <cryptominisat5/cryptominisat.h>
#include "Variable.h"

class Solver {
public:
	Solver();

	Variable new_var();
	VariableList new_vars(const size_t n);
	unsigned var_count() const;

	void add_clause(const VariableList& vars);
	unsigned clause_count() const;

	void assume(const Variable& var);

	CMSat::lbool solve();
	const std::vector<CMSat::lbool>& get_model() const;
private:
	CMSat::SATSolver m_sat_solver;
	VariableList m_assumptions;
	unsigned m_clause_count;
};

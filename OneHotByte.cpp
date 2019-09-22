#include "OneHotByte.h"

static void at_least_one(Solver* solver, const VariableList& lits)
{
	solver->add_clause(lits);
}

static void at_most_one(Solver* solver, const VariableList& lits)
{
	for (size_t i = 0; i < lits.size(); i++) {
		for (size_t j = i+1; j < lits.size(); j++) {
			solver->add_clause({~lits[i], ~lits[j]});
		}
	}
}

static Variable commander_variable(Solver* solver, const VariableList& lits)
{
	auto var = solver->new_var();

	//(a or b or c ...) => var
	for (size_t i = 0; i < lits.size(); i++) {
		solver->add_clause({~lits[i], var});
	}

	//var => (a or b or c ...)
	VariableList clause(lits);
	clause.push_back(~var);
	solver->add_clause(clause);

	return var;
}

static void commander_recursive(Solver* solver, const VariableList& lits, size_t groupSize)
{
	at_least_one(solver, lits);

	if (lits.size() == 1) {
		return;
	} else if (lits.size() < groupSize) {
		at_most_one(solver, lits);
		return;
	}

	VariableList treevars;
	for (size_t i = 0; i < lits.size()/groupSize; i++) {
		int idx = groupSize*i;
		VariableList group(&lits[idx], &lits[idx]+groupSize);
		at_most_one(solver, group);

		auto treevar = commander_variable(solver, group);
		treevars.push_back(treevar);
	}

	commander_recursive(solver, treevars, groupSize);
}

OneHotByte::OneHotByte(Solver* solver)
	: m_solver(solver)
	, m_vars(solver->new_vars(length()))
{
	commander_recursive(solver, all(), 4);
}

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

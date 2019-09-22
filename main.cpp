#include <cryptominisat5/cryptominisat.h>
#include <assert.h>
#include <vector>
using namespace CMSat;

typedef CMSat::Lit Variable;
typedef std::vector<Variable> VariableList;

class Solver {
public:
	Solver();

	Variable new_var();
	VariableList new_vars(const size_t n);
	unsigned var_count() const;

	void add_clause(const VariableList& vars);
	unsigned clause_count() const;

	lbool solve();
	const std::vector<lbool>& get_model() const;
private:
	CMSat::SATSolver m_sat_solver;
	unsigned m_clause_count;
};

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

lbool Solver::solve()
{
	return m_sat_solver.solve();
}

const std::vector<lbool>& Solver::get_model() const
{
	return m_sat_solver.get_model();
}

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
		if (model[m_vars[i].var()] == l_True) {
			return i;
		}
	}
	return -1;
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	Solver solver;
	OneHotByte byte(&solver);

	// std::vector<CMSat::Lit> clause;

	// //Let's use 4 threads
	// solver.set_num_threads(4);

	// //We need 3 variables
	// solver.new_vars(3);

	// //add "1 0"
	// clause.push_back(CMSat::Lit(0, true));
	// solver.add_clause(byte.all());

	// //add "-2 0"
	// clause.clear();
	// clause.push_back(CMSat::Lit(1, true));
	// solver.add_clause(clause);

	// //add "-1 2 3 0"
	// clause.clear();
	// clause.push_back(CMSat::Lit(0, true));
	// clause.push_back(CMSat::Lit(1, false));
	// clause.push_back(CMSat::Lit(2, false));
	// solver.add_clause({byte.at(0)});
	solver.add_clause({byte.at(143)});

	lbool ret = solver.solve();
	// solver.print_stats();
	assert(ret == l_True);
	std::cout << ret << " " << byte.to_int() << std::endl;
	// std::cout
	// << "Solution is: "
	// << solver.get_model()[0]
	// << ", " << solver.get_model()[1]
	// << ", " << solver.get_model()[2]
	// << std::endl;

	// //assumes 3 = FALSE, no solutions left
	// std::vector<CMSat::Lit> assumptions;
	// assumptions.push_back(CMSat::Lit(2, true));
	// ret = solver.solve(&assumptions);
	// assert(ret == l_False);

	// //without assumptions we still have a solution
	// ret = solver.solve();
	// assert(ret == l_True);

	// //add "-3 0"
	// //No solutions left, UNSATISFIABLE returned
	// clause.clear();
	// clause.push_back(CMSat::Lit(2, true));
	// solver.add_clause(clause);
	// ret = solver.solve();
	// assert(ret == l_False);

	return 0;
}

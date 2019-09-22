#include <cryptominisat5/cryptominisat.h>
#include <assert.h>
#include <vector>
using namespace CMSat;

class OneHotByte
{
public:
	OneHotByte(SATSolver* solver);

	CMSat::Lit at(int i) const;
	std::vector<CMSat::Lit> all() const;
	inline int length() const { return 256; }

	int to_int() const;

private:
	SATSolver* m_solver;
	int m_start;
};

static CMSat::Lit make_var(SATSolver* solver)
{
	solver->new_var();
	return CMSat::Lit(solver->nVars()-1, false);
}

static void at_least_one(SATSolver* solver, const std::vector<CMSat::Lit>& lits)
{
	solver->add_clause(lits);
}

static void at_most_one(SATSolver* solver, const std::vector<CMSat::Lit>& lits)
{
	for (size_t i = 0; i < lits.size(); i++) {
		for (size_t j = i+1; j < lits.size(); j++) {
			solver->add_clause({~lits[i], ~lits[j]});
		}
	}
}

static CMSat::Lit commander_variable(SATSolver* solver, const std::vector<CMSat::Lit>& lits)
{
	auto var = make_var(solver);

	//(a or b or c ...) => var
	for (size_t i = 0; i < lits.size(); i++) {
		solver->add_clause({~lits[i], var});
	}

	//var => (a or b or c ...)
	std::vector<CMSat::Lit> clause(lits);
	clause.push_back(~var);
	solver->add_clause(clause);

	return var;
}

static void commander_recursive(SATSolver* solver, const std::vector<CMSat::Lit>& lits, size_t groupSize)
{
	at_least_one(solver, lits);

	if (lits.size() == 1) {
		return;
	} else if (lits.size() < groupSize) {
		at_most_one(solver, lits);
		return;
	}

	std::vector<CMSat::Lit> treevars;
	for (size_t i = 0; i < lits.size()/groupSize; i++) {
		int idx = groupSize*i;
		std::vector<CMSat::Lit> group(&lits[idx], &lits[idx]+groupSize);
		at_most_one(solver, group);

		auto treevar = commander_variable(solver, group);
		treevars.push_back(treevar);
	}

	commander_recursive(solver, treevars, groupSize);
}

OneHotByte::OneHotByte(SATSolver* solver)
	: m_solver(solver)
	, m_start(solver->nVars())
{
	solver->new_vars(length());

	//commander variable one-hot encoding
	commander_recursive(solver, all(), 4);
}

CMSat::Lit OneHotByte::at(int i) const
{
	assert((i >= m_start) && (i < m_start+length()));
	return CMSat::Lit(m_start + i, false);
}

std::vector<CMSat::Lit> OneHotByte::all() const
{
	std::vector<CMSat::Lit> lits;
	lits.reserve(length());
	for (int i = 0; i < length(); i++) {
		lits.push_back(at(i));
	}
	return lits;
}

int OneHotByte::to_int() const
{
	auto model = m_solver->get_model();
	for (int i = 0; i < length(); i++) {
		if (model[m_start+i] == l_True) {
			return i;
		}
	}
	return -1;
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	CMSat::SATSolver solver;
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
	// solver.add_clause({byte.at(143)});

	lbool ret = solver.solve();
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

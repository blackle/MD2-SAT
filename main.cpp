#include <cryptominisat5/cryptominisat.h>
#include <assert.h>
#include "Variable.h"
#include "Solver.h"
#include "OneHotByte.h"

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	Solver solver;
	OneHotByte byte(&solver);
	std::vector<OneHotByte> bytes;
	for (int i = 0; i < 835; i++) {
		bytes.push_back(OneHotByte(&solver));
	}

	solver.add_clause({byte.at(143)});

	std::cerr << "variables:" << solver.var_count() << std::endl;
	std::cerr << "clauses:" << solver.clause_count() << std::endl;
	// CMSat::lbool ret = solver.solve();
	// solver.print_stats();
	// assert(ret == CMSat::l_True);
	// std::cout << ret << " " << byte.to_int() << std::endl;

	return 0;
}

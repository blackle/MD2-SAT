#include <assert.h>
#include "Solver.h"
#include "CompressionMatrix.h"

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	Solver solver;
	CompressionMatrix compression_matrix(&solver);

	std::cerr << "variables:" << solver.var_count() << std::endl;
	std::cerr << "clauses:" << solver.clause_count() << std::endl;
	// CMSat::lbool ret = solver.solve();
	// solver.print_stats();
	// assert(ret == CMSat::l_True);
	// std::cout << ret << " " << byte.to_int() << std::endl;

	return 0;
}

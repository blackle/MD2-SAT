#include <cryptominisat5/cryptominisat.h>
#include <assert.h>
#include "variable.h"
#include "solver.h"
#include "onehotbyte.h"

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	Solver solver;
	OneHotByte byte(&solver);

	solver.add_clause({byte.at(143)});

	CMSat::lbool ret = solver.solve();
	// solver.print_stats();
	assert(ret == CMSat::l_True);
	std::cout << ret << " " << byte.to_int() << std::endl;

	return 0;
}

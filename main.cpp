#include <assert.h>
#include "Solver.h"
#include "CompressionMatrix.h"

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	Solver solver;
	CompressionMatrix compression_matrix(&solver);
	compression_matrix.chaining_output()[0].assume(175);
	compression_matrix.chaining_output()[1].assume(149);
	compression_matrix.chaining_output()[2].assume(206);
	// compression_matrix.chaining_output()[3].assume(12);

	compression_matrix.message_block()[0].assume(0);
	compression_matrix.message_block()[1].assume(0);
	compression_matrix.message_block()[2].assume(0);
	compression_matrix.message_block()[3].assume(0);

	std::cerr << "variables: " << solver.var_count() << std::endl;
	std::cerr << "clauses: " << solver.clause_count() << std::endl;
	CMSat::lbool ret = solver.solve();
	assert(ret == CMSat::l_True);
	std::cout << compression_matrix << std::endl;

	return 0;
}

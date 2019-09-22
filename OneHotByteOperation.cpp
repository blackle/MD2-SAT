#include "OneHotByteOperation.h"
#include "OneHotByte.h"
#include "Solver.h"

static void exclusive_or_implication(Solver* solver, const OneHotByte& a, const OneHotByte& b, const OneHotByte& c)
{
	for (int i = 0; i < a.length(); i++) {
		for (int j = 0; j < b.length(); j++) {
			solver->add_clause({~a.at(i), ~b.at(j), c.at(i^j)});
		}
	}
}

//TODO: test this
void OneHotByteOperation::exclusive_or(Solver* solver, const OneHotByte& a, const OneHotByte& b, const OneHotByte& c)
{
	exclusive_or_implication(solver, b, c, a);
	exclusive_or_implication(solver, a, c, b);
	exclusive_or_implication(solver, a, b, c);
}

OneHotByte OneHotByteOperation::sbox(const OneHotByte& a)
{
	return a; //TODO
}

OneHotByte OneHotByteOperation::add(const OneHotByte& a, unsigned b)
{
	(void)b;
	return a; //TODO
}

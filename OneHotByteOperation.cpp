#include "OneHotByteOperation.h"
#include "OneHotConstraint.h"
#include "OneHotByte.h"
#include "Solver.h"
#include <algorithm>

static void exclusive_or_implication(Solver* solver, const OneHotByte& a, const OneHotByte& b, const OneHotByte& c)
{
	for (size_t i = 0; i < a.length(); i++) {
		for (size_t j = 0; j < b.length(); j++) {
			solver->add_clause({~a.at(i), ~b.at(j), c.at(i^j)});
		}
	}
}

//TODO: test this
void OneHotByteOperation::exclusive_or_naive(Solver* solver, const OneHotByte& a, const OneHotByte& b, const OneHotByte& c)
{
	exclusive_or_implication(solver, b, c, a);
	exclusive_or_implication(solver, a, c, b);
	exclusive_or_implication(solver, a, b, c);
}

static Variable exclusive_or_commander_bit(Solver* solver, const OneHotByte& a, size_t bit)
{
	VariableList vars;
	for (size_t i = 0; i < a.length(); i++) {
		if ((i >> bit) % 2 == 1) {
			vars.push_back(a.at(i));
		}
	}
	return OneHotConstraint::commander_variable(solver, vars);
}

void OneHotByteOperation::exclusive_or_commander(Solver* solver, const OneHotByte& a, const OneHotByte& b, const OneHotByte& c)
{
	for (size_t bit = 0; bit < 8; bit++) {
		auto abit = exclusive_or_commander_bit(solver, a, bit);
		auto bbit = exclusive_or_commander_bit(solver, b, bit);
		auto cbit = exclusive_or_commander_bit(solver, c, bit);

		solver->add_clause({~abit, ~bbit, ~cbit});
		solver->add_clause({~abit,  bbit,  cbit});
		solver->add_clause({ abit, ~bbit,  cbit});
		solver->add_clause({ abit,  bbit, ~cbit});
	}
}

static const std::vector<int> sbox_permutation = {
	41, 46, 67, 201, 162, 216, 124, 1, 61, 54, 84, 161, 236, 240, 6,
	19, 98, 167, 5, 243, 192, 199, 115, 140, 152, 147, 43, 217, 188,
	76, 130, 202, 30, 155, 87, 60, 253, 212, 224, 22, 103, 66, 111, 24,
	138, 23, 229, 18, 190, 78, 196, 214, 218, 158, 222, 73, 160, 251,
	245, 142, 187, 47, 238, 122, 169, 104, 121, 145, 21, 178, 7, 63,
	148, 194, 16, 137, 11, 34, 95, 33, 128, 127, 93, 154, 90, 144, 50,
	39, 53, 62, 204, 231, 191, 247, 151, 3, 255, 25, 48, 179, 72, 165,
	181, 209, 215, 94, 146, 42, 172, 86, 170, 198, 79, 184, 56, 210,
	150, 164, 125, 182, 118, 252, 107, 226, 156, 116, 4, 241, 69, 157,
	112, 89, 100, 113, 135, 32, 134, 91, 207, 101, 230, 45, 168, 2, 27,
	96, 37, 173, 174, 176, 185, 246, 28, 70, 97, 105, 52, 64, 126, 15,
	85, 71, 163, 35, 221, 81, 175, 58, 195, 92, 249, 206, 186, 197,
	234, 38, 44, 83, 13, 110, 133, 40, 132, 9, 211, 223, 205, 244, 65,
	129, 77, 82, 106, 220, 55, 200, 108, 193, 171, 250, 36, 225, 123,
	8, 12, 189, 177, 74, 120, 136, 149, 139, 227, 99, 232, 109, 233,
	203, 213, 254, 59, 0, 29, 57, 242, 239, 183, 14, 102, 88, 208, 228,
	166, 119, 114, 248, 235, 117, 75, 10, 49, 68, 80, 180, 143, 237,
	31, 26, 219, 153, 141, 51, 159, 17, 131, 20
};

static const std::vector<int> sbox_permutation_inverse = {
	221, 7, 143, 95, 126, 18, 14, 70, 203, 183, 239, 76, 204, 178, 227,
	159, 74, 253, 47, 15, 255, 68, 39, 45, 43, 97, 247, 144, 152, 222,
	32, 246, 135, 79, 77, 163, 200, 146, 175, 87, 181, 0, 107, 26, 176,
	141, 1, 61, 98, 240, 86, 251, 156, 88, 9, 194, 114, 223, 167, 220,
	35, 8, 89, 71, 157, 188, 41, 2, 241, 128, 153, 161, 100, 55, 207,
	238, 29, 190, 49, 112, 242, 165, 191, 177, 10, 160, 109, 34, 229,
	131, 84, 137, 169, 82, 105, 78, 145, 154, 16, 213, 132, 139, 228,
	40, 65, 155, 192, 122, 196, 215, 179, 42, 130, 133, 234, 22, 125,
	237, 120, 233, 208, 66, 63, 202, 6, 118, 158, 81, 80, 189, 30, 254,
	182, 180, 136, 134, 209, 75, 44, 211, 23, 250, 59, 244, 85, 67, 106,
	25, 72, 210, 116, 94, 24, 249, 83, 33, 124, 129, 53, 252, 56, 11, 4,
	162, 117, 101, 232, 17, 142, 64, 110, 198, 108, 147, 148, 166, 149,
	206, 69, 99, 243, 102, 119, 226, 113, 150, 172, 60, 28, 205, 48, 92,
	20, 197, 73, 168, 50, 173, 111, 21, 195, 3, 31, 217, 90, 186, 171,
	138, 230, 103, 115, 184, 37, 218, 51, 104, 5, 27, 52, 248, 193, 164,
	54, 185, 38, 201, 123, 212, 231, 46, 140, 91, 214, 216, 174, 236, 12,
	245, 62, 225, 13, 127, 224, 19, 187, 58, 151, 93, 235, 170, 199, 57,
	121, 36, 219, 96
};

OneHotByte OneHotByteOperation::sbox(const OneHotByte& a)
{
	VariableList vars;
	for (size_t i = 0; i < a.length(); i++) {
		vars.push_back(a.at(sbox_permutation_inverse[i]));
	}
	return OneHotByte(a.solver(), vars);
}

OneHotByte OneHotByteOperation::add(const OneHotByte& a, int b)
{
	VariableList vars;
	for (size_t i = 0; i < a.length(); i++) {
		int offset = (a.length() + (i - b)) % a.length();
		vars.push_back(a.at(offset));
	}
	return OneHotByte(a.solver(), vars);
}

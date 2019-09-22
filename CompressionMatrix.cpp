#include "CompressionMatrix.h"
#include "Solver.h"

constexpr int NUM_ROUNDS = 18;
constexpr int SUB_MATRIX_LENGTH = 16;
constexpr int STRIDE_LENGTH = SUB_MATRIX_LENGTH*3;
constexpr int MATRIX_LENGTH = SUB_MATRIX_LENGTH*3*NUM_ROUNDS - SUB_MATRIX_LENGTH*2;

CompressionMatrix::CompressionMatrix(Solver* solver)
	: m_solver(solver)
{
	for (int i = 0; i < MATRIX_LENGTH; i++) {
		m_bytes.push_back(OneHotByte(m_solver));
	}
}

OneHotByteList CompressionMatrix::message_block() const
{
	auto start = &m_bytes[0];
	return OneHotByteList(start, start+SUB_MATRIX_LENGTH);
}

OneHotByteList CompressionMatrix::chaining_input() const
{
	auto start = &m_bytes[SUB_MATRIX_LENGTH];
	return OneHotByteList(start, start+SUB_MATRIX_LENGTH);
}

OneHotByteList CompressionMatrix::chaining_output() const
{
	auto start = &m_bytes[STRIDE_LENGTH*(NUM_ROUNDS-1)];
	return OneHotByteList(start, start+SUB_MATRIX_LENGTH);
}

std::optional<OneHotByte> CompressionMatrix::left_neighbour(unsigned index) const
{
	if (index == 0) return {};

	return m_bytes.at(index - 1); //TODO: add T offset
}

std::optional<OneHotByte> CompressionMatrix::top_neighbour(unsigned index) const
{
	if (index < STRIDE_LENGTH) return {};

	return m_bytes.at(index - STRIDE_LENGTH);
}

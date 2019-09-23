#include "CompressionMatrix.h"
#include "OneHotByteOperation.h"
#include "Solver.h"

constexpr int SUB_MATRIX_LENGTH = 4;
constexpr int NUM_ROUNDS = SUB_MATRIX_LENGTH+3;
constexpr int STRIDE_LENGTH = SUB_MATRIX_LENGTH*3;
constexpr int MATRIX_LENGTH = SUB_MATRIX_LENGTH*3*NUM_ROUNDS - SUB_MATRIX_LENGTH*2;

CompressionMatrix::CompressionMatrix(Solver* s)
	: m_solver(s)
{
	for (unsigned i = 0; i < MATRIX_LENGTH; i++) {
		OneHotByte byte(m_solver);
		m_bytes.push_back(byte);

		auto left = left_neighbour(i);
		auto top = top_neighbour(i);

		if (i == STRIDE_LENGTH && top) {
			OneHotByte zero(m_solver);
			zero.assume(0);
			OneHotByteOperation::exclusive_or_commander(m_solver, OneHotByteOperation::sbox(zero), *top, byte);
			continue;
		}

		if (left && top) {
			OneHotByteOperation::exclusive_or_commander(m_solver, OneHotByteOperation::sbox(*left), *top, byte);
		}
	}

	for (unsigned i = 0; i < SUB_MATRIX_LENGTH; i++) {
		OneHotByteOperation::exclusive_or_commander(m_solver, m_bytes[i], m_bytes[i + SUB_MATRIX_LENGTH], m_bytes[i + SUB_MATRIX_LENGTH*2]);
	}
}

OneHotByteList CompressionMatrix::message_block() const
{
	auto start = &m_bytes[SUB_MATRIX_LENGTH];
	return OneHotByteList(start, start+SUB_MATRIX_LENGTH);
}

OneHotByteList CompressionMatrix::chaining_input() const
{
	auto start = &m_bytes[0];
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

	auto neighbour = m_bytes.at(index - 1);
	if (index % STRIDE_LENGTH == 0) {
		return OneHotByteOperation::add(neighbour, index / STRIDE_LENGTH - 1); //TODO: might be wrong
	}
	return neighbour;
}

std::optional<OneHotByte> CompressionMatrix::top_neighbour(unsigned index) const
{
	if (index < STRIDE_LENGTH) return {};

	return m_bytes.at(index - STRIDE_LENGTH);
}

Solver* CompressionMatrix::solver() const
{
	return m_solver;
}

std::ostream& operator<<(std::ostream& os, const CompressionMatrix& cm)
{
	for (int i = 0; i < MATRIX_LENGTH; i++) {
		if (i % STRIDE_LENGTH == 0) {
			os << "|";
		}

		os << " " << cm.m_bytes.at(i);

		if (i % SUB_MATRIX_LENGTH == SUB_MATRIX_LENGTH-1) {
			os << " |";
		} 
		if (i % STRIDE_LENGTH == STRIDE_LENGTH-1) {
			os << std::endl;
		}
	}
	return os;
}

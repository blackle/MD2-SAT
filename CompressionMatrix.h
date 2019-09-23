#pragma once
#include "OneHotByte.h"
#include <optional>
#include <iostream>

class Solver;

class CompressionMatrix {
public:
	explicit CompressionMatrix(Solver* s);

	OneHotByteList message_block() const;
	OneHotByteList chaining_input() const;
	OneHotByteList chaining_output() const;

	Solver* solver() const;

	friend std::ostream& operator<<(std::ostream& os, const CompressionMatrix& cm);

private:
	Solver* m_solver;
	OneHotByteList m_bytes;

	std::optional<OneHotByte> left_neighbour(unsigned index) const;
	std::optional<OneHotByte> top_neighbour(unsigned index) const;
};

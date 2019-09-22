#pragma once
#include "OneHotByte.h"

class Solver;

class CompressionMatrix {
public:
	explicit CompressionMatrix(Solver* solver);

	OneHotByteList message_block() const;
	OneHotByteList chaining_input() const;
	OneHotByteList chaining_output() const;
private:
	Solver* m_solver;
	OneHotByteList m_bytes;
};

#pragma once
#include "Variable.h"
#include "Solver.h"
#include "OneHotByte.h"

class CompressionMatrix {
public:
	CompressionMatrix(Solver* solver);

	OneHotByteList message_block() const;
	OneHotByteList chaining_input() const;
	OneHotByteList chaining_output() const;
private:
	Solver* m_solver;
	OneHotByteList m_bytes;
};

#pragma once

class Solver;
class OneHotByte;

class OneHotByteOperation {
public:
	static void exclusive_or(Solver* solver, const OneHotByte& a, const OneHotByte& b, const OneHotByte& c);
	static OneHotByte sbox(const OneHotByte& a);
	static OneHotByte add(const OneHotByte& a, int b);

private:
	OneHotByteOperation() = delete;
};
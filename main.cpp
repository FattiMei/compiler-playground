#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <stack>


enum class Opcode {
	Increment,
	Decrement,
	Left,
	Right,
	Get,
	Put,
	OpenBrace,
	ClosedBrace
};


union Operand {
	int count;
	int jump_offset;
};


struct Instruction {
	int position;
	Opcode opcode;
	Operand operand;
};


std::ostream& operator<<(std::ostream &os, Opcode op) {
	switch (op) {
		case Opcode::Increment  : os << "Increment"  ; break;
		case Opcode::Decrement  : os << "Decrement"  ; break;
		case Opcode::Left       : os << "Left"       ; break;
		case Opcode::Right      : os << "Right"      ; break;
		case Opcode::Get        : os << "Get"        ; break;
		case Opcode::Put        : os << "Put"        ; break;
		case Opcode::OpenBrace  : os << "OpenBrace"  ; break;
		case Opcode::ClosedBrace: os << "ClosedBrace"; break;
	}

	return os;
}


std::ostream& operator<<(std::ostream &os, Instruction I) {
	os << "(" << I.position << " , " << I.opcode << ")" << std::endl;

	return os;
}


std::vector<Instruction> load_program_source(std::istream &in) {
	std::vector<Instruction> program;
	int i = 0;

	while (!in.eof()) {
		const char symbol = in.get();
		Opcode opcode;

		switch (symbol) {
			case '+': program.push_back({i, Opcode::Increment})  ; break;
			case '-': program.push_back({i, Opcode::Decrement})  ; break;
			case '<': program.push_back({i, Opcode::Left})       ; break;
			case '>': program.push_back({i, Opcode::Right})      ; break;
			case ',': program.push_back({i, Opcode::Get})        ; break;
			case '.': program.push_back({i, Opcode::Put})        ; break;
			case '[': program.push_back({i, Opcode::OpenBrace})  ; break;
			case ']': program.push_back({i, Opcode::ClosedBrace}); break;
		}

		++i;
	}

	return program;
}


void build_jump_table(std::vector<Instruction> &program) {
	std::stack<int> call_stack;

	for (int i = 0; i < program.size(); ++i) {
		if (program[i].opcode == Opcode::OpenBrace) {
			call_stack.push(i);
		}
		else if (program[i].opcode == Opcode::ClosedBrace) {
			if (call_stack.empty()) {
				std::cerr << "Unexpected closed parenthesis at " << program[i].position << std::endl;
				break;
			}

			program[i].operand.jump_offset = call_stack.top();
			program[call_stack.top()].operand.jump_offset = i;
			call_stack.pop();
		}
	}
}


void run(size_t memory_size, std::istream &in, std::ostream &out, const std::vector<Instruction> &program) {
	std::vector<char> memory(memory_size);
	std::stack<int> call_stack;

	int pc   = 0;
	int head = 0;

	while (pc < program.size()) {
		const Instruction I = program[pc];

		switch (I.opcode) {
			case Opcode::Increment  : memory[head] += 1;					break;
			case Opcode::Decrement  : memory[head] -= 1;					break;
			case Opcode::Left       : --head;						break;
			case Opcode::Right      : ++head;						break;
			case Opcode::Get        : memory[head] = in.get();				break;
			case Opcode::Put        : out << memory[head];					break;
			case Opcode::OpenBrace  : pc = memory[head] == 0 ? I.operand.jump_offset : pc;	break;
			case Opcode::ClosedBrace: pc = memory[head] == 0 ? pc : I.operand.jump_offset;	break;
		}

		++pc;
	}
}


int main(int argc, char *argv[]) {
	if (argc < 3) {
		std::cerr << "Usage        bf program.b \"input stream\"" << std::endl;
		std::cerr << "Alternative  bf program.b -" << std::endl;
		return 1;
	}


	std::ifstream in(argv[1]);
	std::vector<Instruction> program = load_program_source(in);
	build_jump_table(program);


	run(1000, std::cin, std::cout, program);


	return 0;
}

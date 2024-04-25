#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <stack>
#include <cstring>


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

		switch (symbol) {
			case '+': program.push_back({i, Opcode::Increment,   1}); break;
			case '-': program.push_back({i, Opcode::Decrement,   1}); break;
			case '<': program.push_back({i, Opcode::Left,        1}); break;
			case '>': program.push_back({i, Opcode::Right,       1}); break;
			case ',': program.push_back({i, Opcode::Get,         1}); break;
			case '.': program.push_back({i, Opcode::Put,         1}); break;
			case '[': program.push_back({i, Opcode::OpenBrace,   1}); break;
			case ']': program.push_back({i, Opcode::ClosedBrace, 1}); break;
		}

		++i;
	}

	return program;
}


void build_jump_table(std::vector<Instruction> &program) {
	std::stack<size_t> call_stack;

	for (size_t i = 0; i < program.size(); ++i) {
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

	size_t pc   = 0;
	size_t head = 0;

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


void transpile_to_c(size_t memory_size, const std::vector<Instruction> &program, std::ostream &out) {
	out
		<< "#include <stdio.h>\n\n"
		<< "char memory[" << memory_size << "];\n\n"
		<< "int main() {\n"
		<< "int head = 0;\n";

	for (Instruction I : program) {
		switch (I.opcode) {
			case Opcode::Increment  : out << "memory[head] += 1;"		; break;
			case Opcode::Decrement  : out << "memory[head] -= 1;"		; break;
			case Opcode::Left       : out << "head -= 1;"			; break;
			case Opcode::Right      : out << "head += 1;"			; break;
			case Opcode::Get        : out << "memory[head] = getchar();"	; break;
			case Opcode::Put        : out << "putchar(memory[head]);"	; break;
			case Opcode::OpenBrace  : out << "while (memory[head] != 0) {"	; break;
			case Opcode::ClosedBrace: out << "}"				; break;
		}

		out << std::endl;
	}

	out << "}\n";
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


	if (argc > 3 and strcmp(argv[3], "--transpile") == 0) {
		transpile_to_c(1000, program, std::cout);
	}
	else {
		run(1000, std::cin, std::cout, program);
	}


	return 0;
}

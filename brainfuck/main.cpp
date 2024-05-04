#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <stack>
#include <cstring>
#include <cassert>


struct Instruction {
	int  position;
	char opcode;
	int  operand;
};


std::ostream& operator<<(std::ostream &os, Instruction I) {
	os << "(" << I.position << " , " << I.opcode << ", " << I.operand << ")" << std::endl;

	return os;
}


std::vector<Instruction> load_program_source(std::istream &in) {
	std::vector<Instruction> program;
	int i = 0;

	while (!in.eof()) {
		const char symbol = in.get();

		if (strchr("+-<>,.[]", symbol) != NULL) {
			if (program.size() > 0 and strchr("+-<>", symbol) != NULL and program[program.size() - 1].opcode == symbol) {
				program[program.size() - 1].operand += 1;
			}
			else {
				program.push_back({i, symbol, 1});
			}
		}

		++i;
	}

	return program;
}


void build_jump_table(std::vector<Instruction> &program) {
	std::stack<size_t> call_stack;

	for (size_t i = 0; i < program.size(); ++i) {
		if (program[i].opcode == '[') {
			call_stack.push(i);
		}
		else if (program[i].opcode == ']') {
			if (call_stack.empty()) {
				std::cerr << "Unexpected closed parenthesis at " << program[i].position << std::endl;
				break;
			}

			program[i].operand = call_stack.top();
			program[call_stack.top()].operand = i;
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
			case '+': memory[head] += I.operand;			break;
			case '-': memory[head] -= I.operand;			break;
			case '<': head -= I.operand;				break;
			case '>': head += I.operand;				break;
			case ',': memory[head] = in.eof() ? 0 : in.get();	break;
			case '.': out << memory[head];				break;
			case '[': pc = memory[head] == 0 ? I.operand : pc;	break;
			case ']': pc = memory[head] == 0 ? pc : I.operand;	break;
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
			case '+': out << "memory[head] += "	<< I.operand << ";"	; break;
			case '-': out << "memory[head] -= "	<< I.operand << ";"	; break;
			case '<': out << "head -= "		<< I.operand << ";"	; break;
			case '>': out << "head += "		<< I.operand << ";"	; break;
			case ',': out << "memory[head] = getchar();"			; break;
			case '.': out << "putchar(memory[head]);"			; break;
			case '[': out << "while (memory[head] != 0) {"			; break;
			case ']': out << "}"						; break;
			default: assert(0);
		}

		out << std::endl;
	}

	out << "}\n";
}


void compile_to_x86_asm(size_t memory_size, const std::vector<Instruction> &program, std::ostream &out) {
	// @DESIGN: we might investigate std::format
	const std::string head_reg{"%rax"};
	const std::string  val_reg{"%rbx"};

	out
		<< "\t.data\n"
		<< "memory: .zero " << memory_size << '\n'
		<< "\t.globl run\n"
		<< "\t.text\n"
		<< "run:\n"
		<< "lea memory, " << head_reg << '\n';

	for (size_t i = 0; i < program.size(); ++i) {
		const Instruction I = program[i];

		switch (I.opcode) {
			case '+':
				out << "mov\t(" << head_reg << ")," << val_reg << '\n';
				out << "add\t$" << I.operand << ','  << val_reg << '\n';
				out << "mov\t" << val_reg << ",("   << head_reg << ")\n";
				break;

			case '-':
				out << "mov\t(" << head_reg << ")," << val_reg << '\n';
				out << "sub\t$" << I.operand << ','  << val_reg << '\n';
				out << "mov\t" << val_reg << ",("   << head_reg << ")\n";
				break;

			case '<':
				out << "sub\t$" << I.operand << ',' << head_reg << '\n';
				break;

			case '>':
				out << "add\t$" << I.operand << ',' << head_reg << '\n';
				break;

			case ',':
				assert(0 && "Not implemented");
				break;

			case '.':
				out << "push\t" << head_reg << '\n';
				out << "mov\t(" << head_reg << ")," << "%rdi" << '\n';
				out << "call\tputchar\n";
				out << "pop \t" << head_reg << '\n';
				break;

			case '[':
				// exploit the fact that the label pointers are exactly the indices in the program array
				out << 'L' << i << ":\n";
				out << "mov\t(" << head_reg << ")," << val_reg << '\n';

				// branching logic, uses only the lowest bits of rbx
				out << "cmp\t" << "$0, " << "%bl" << '\n';
				out << "jz\t" << 'L' << I.operand << '\n';
				break;

			case ']':
				out << "jmp\tL" << I.operand << '\n';
				out << 'L' << i << ":\n";
				break;
		}
	}

	out << "ret" << std::endl;
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


	if (argc > 3) {
		if (strcmp(argv[3], "--transpile") == 0) {
			transpile_to_c(1000, program, std::cout);
		}
		else if (strcmp(argv[3], "--compile") == 0) {
			compile_to_x86_asm(1000, program, std::cout);
		}
	}

	else {
		run(1000, std::cin, std::cout, program);
	}


	return 0;
}

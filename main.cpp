#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>


enum class Opcode{
	Increment,
	Decrement,
	Left,
	Right,
	Get,
	Put,
	OpenBrace,
	ClosedBrace
};


using Instruction = std::pair<int, Opcode>;


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
	os << "(" << I.first << " , " << I.second << ")" << std::endl;

	return os;
}


std::vector<Instruction> load_source(const std::string &filename) {
	std::vector<Instruction> program;
	std::ifstream source(filename);


	int i = 0;
	while (!source.eof() && source.good()) {
		const char symbol = source.get();

		switch (symbol) {
			case '+': program.push_back({i, Opcode::Increment});   break;
			case '-': program.push_back({i, Opcode::Decrement});   break;
			case '<': program.push_back({i, Opcode::Left});        break;
			case '>': program.push_back({i, Opcode::Right});       break;
			case ',': program.push_back({i, Opcode::Get});         break;
			case '.': program.push_back({i, Opcode::Put});         break;
			case '[': program.push_back({i, Opcode::OpenBrace});   break;
			case ']': program.push_back({i, Opcode::ClosedBrace}); break;
		}

		++i;
	}


	return program;
}


bool check_parenthesis(const std::vector<Instruction> &program) {
	int depth = 0;

	for (const Instruction I : program) {
		if (I.second == Opcode::OpenBrace) {
			++depth;
		}
		else if (I.second == Opcode::OpenBrace) {
			if (depth == 0) {
				return false;
			}

			--depth;
		}
	}

	return true;
}


int main(int argc, char *argv[]) {
	if (argc < 3) {
		std::cerr << "Usage: bf program.b \"input stream\"" << std::endl;
		return 1;
	}


	const std::vector<Instruction> program = load_source(argv[1]);


	// it would be nice to automatically derive the printing of the vector
	for (const Instruction I : program) {
		std::cout << "(" << I.first << ", " << I.second << ")" << std::endl;
	}


	if (!check_parenthesis(program)) {
		std::cerr << "Problem with the parenthesis, quitting" << std::endl;
	}


	// actual interpretation
	std::vector<char> memory(1000);
	std::stack<int> call_stack;
	int ptr = 0;
	int pc = 0;


	while (pc < program.size()) {
		const Instruction I = program[pc];

		switch (I.second) {
			case Opcode::Increment:
				memory[ptr] += 1;
				break;
			case Opcode::Decrement:
				memory[ptr] -= 1;
				break;
			case Opcode::Left:
				--ptr;
				break;
			case Opcode::Right:
				++ptr;
				break;
			case Opcode::Get:
				memory[ptr] = std::cin.get();
				break;
			case Opcode::Put:
				std::cout << memory[ptr];
				break;
			case Opcode::OpenBrace:
				if (memory[ptr] == 0) {
					int depth = 1;

					do {
						++pc;
						if (program[pc].second == Opcode::OpenBrace) {
							++depth;
						}
						else if (program[pc].second == Opcode::ClosedBrace) {
							--depth;
						}
					} while (depth > 0 && pc < program.size());
				}
				else {
					call_stack.push(pc);
				}

				break;
			case Opcode::ClosedBrace:
				if (memory[ptr] == 0) {
					call_stack.pop();
				}
				else {
					pc = call_stack.top();
				}

				break;
		}

		++pc;
	}


	return 0;
}

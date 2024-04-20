#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>


enum class OpCode{
	Increment,
	Decrement,
	Left,
	Right,
	Get,
	Put,
	OpenBrace,
	ClosedBrace
};


// it would be nice to automatically derive such code
std::ostream& operator<<(std::ostream &os, OpCode op) {
	switch (op) {
		case OpCode::Increment  : os << "Increment"  ; break;
		case OpCode::Decrement  : os << "Decrement"  ; break;
		case OpCode::Left       : os << "Left"       ; break;
		case OpCode::Right      : os << "Right"      ; break;
		case OpCode::Get        : os << "Get"        ; break;
		case OpCode::Put        : os << "Put"        ; break;
		case OpCode::OpenBrace  : os << "OpenBrace"  ; break;
		case OpCode::ClosedBrace: os << "ClosedBrace"; break;
	}

	return os;
}


using Instruction = std::pair<int, OpCode>;


// @TODO: still can't find in the cpp documentation a way to slurp a file
std::vector<Instruction> load_source(const std::string &filename) {
	std::vector<Instruction> program;
	std::ifstream source(filename);


	int i = 0;
	while (!source.eof() && source.good()) {
		const char symbol = source.get();

		// bug con lo switch case, non mettevo i break
		// ma se non metti i break non dovrebbe beccare tutti i casi ???
		// @TODO: dei warning possono beccare questa cosa??
		switch (symbol) {
			case '+': program.push_back({i, OpCode::Increment});   break;
			case '-': program.push_back({i, OpCode::Decrement});   break;
			case '<': program.push_back({i, OpCode::Left});        break;
			case '>': program.push_back({i, OpCode::Right});       break;
			case ',': program.push_back({i, OpCode::Get});         break;
			case '.': program.push_back({i, OpCode::Put});         break;
			case '[': program.push_back({i, OpCode::OpenBrace});   break;
			case ']': program.push_back({i, OpCode::ClosedBrace}); break;
		}

		++i;
	}


	return program;
}


bool check_parenthesis(const std::vector<Instruction> &program) {
	int depth = 0;

	for (const Instruction I : program) {
		if (I.second == OpCode::OpenBrace) {
			++depth;
		}
		else if (I.second == OpCode::OpenBrace) {
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
		std::cerr << "Usage: bf program.bf \"input stream\"" << std::endl;
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
			case OpCode::Increment:
				memory[ptr] += 1;
				break;
			case OpCode::Decrement:
				memory[ptr] -= 1;
				break;
			case OpCode::Left:
				--ptr;
				break;
			case OpCode::Right:
				++ptr;
				break;
			case OpCode::Get:
				memory[ptr] = std::cin.get();
				break;
			case OpCode::Put:
				std::cout << memory[ptr];
				break;
			case OpCode::OpenBrace:
				if (memory[ptr] == 0) {
					int depth = 1;

					do {
						++pc;
						if (program[pc].second == OpCode::OpenBrace) {
							++depth;
						}
						else if (program[pc].second == OpCode::ClosedBrace) {
							--depth;
						}
					} while (depth > 0 && pc < program.size());
				}
				else {
					call_stack.push(pc);
				}

				break;
			case OpCode::ClosedBrace:
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

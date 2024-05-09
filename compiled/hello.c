#include <stdio.h>

char memory[1000];

int main() {
int head = 0;
memory[head] += 10;
while (memory[head] != 0) {
head += 1;
memory[head] += 7;
head += 1;
memory[head] += 10;
head += 1;
memory[head] += 3;
head += 1;
memory[head] += 1;
head -= 4;
memory[head] -= 1;
}
head += 1;
memory[head] += 2;
putchar(memory[head]);
head += 1;
memory[head] += 1;
putchar(memory[head]);
memory[head] += 7;
putchar(memory[head]);
putchar(memory[head]);
memory[head] += 3;
putchar(memory[head]);
head += 1;
memory[head] += 2;
putchar(memory[head]);
head -= 2;
memory[head] += 15;
putchar(memory[head]);
head += 1;
putchar(memory[head]);
memory[head] += 3;
putchar(memory[head]);
memory[head] -= 6;
putchar(memory[head]);
memory[head] -= 8;
putchar(memory[head]);
head += 1;
memory[head] += 1;
putchar(memory[head]);
head += 1;
putchar(memory[head]);
}

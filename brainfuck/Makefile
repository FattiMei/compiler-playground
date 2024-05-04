CXX      = g++-13
STD      = -std=c++2a
WARNINGS = -Wimplicit-fallthrough -Wall -Wextra -Wpedantic


target = bf


all: $(target) hello


$(target): main.cpp
	$(CXX) $(STD) $(WARNINGS) -O2 -o $@ main.cpp


hello: hello.s runtime.c
	gcc -g -no-pie -o $@ $^


hello.s: $(target)
	./$^ samples/hello.b - --compile_to_x86 > $@


run: $(target)
	./$^ samples/hello.b -


compile:


.PHONY: clean


clean:
	rm $(target) hello.s

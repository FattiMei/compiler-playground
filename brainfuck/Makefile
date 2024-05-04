CXX = g++
WARNINGS = -Wimplicit-fallthrough -Wall -Wextra -Wpedantic


target = bf


all: $(target) hello


$(target): main.cpp
	$(CXX) $(WARNINGS) -O2 -o $@ main.cpp


hello: hello.s runtime.c
	gcc -g -no-pie -o $@ $^


hello.s: $(target)
	./$^ samples/hello.b - --compile > $@


run: $(target)
	./$^ samples/hello.b -


compile:


.PHONY: clean


clean:
	rm $(target) hello.s

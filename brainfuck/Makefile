CXX         = g++
OPT         = -O0
WARNINGS    = -Wimplicit-fallthrough -Wall -Wextra -Wpedantic
TARGET_ARCH = --compile_to_x86


bf     = bf
source = samples/hello.b


all: $(bf) compile


$(bf): main.cpp
	$(CXX) $(WARNINGS) $(OPT) -o $@ $^


run: $(bf) $(source)
	./$^ $(source) -


compile: $(bf) $(source) runtime.c
	./$< $(source) - $(TARGET_ARCH) > out.s
	gcc -g -o out out.s runtime.c


transpile: $(bf) $(source)
	./$< $(source) - --transpile > out.c


.PHONY: clean


clean:
	rm $(bf)

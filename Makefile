CXX = g++
WARNINGS = -Wimplicit-fallthrough


target = bf


all: $(target)


$(target): main.cpp
	$(CXX) $(WARNINGS) -O2 -o $@ main.cpp


run: $(target)
	./$^ samples/hello.b -


.PHONY: clean


clean:
	rm $(target)

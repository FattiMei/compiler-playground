CXX = g++
WARNINGS = -Wimplicit-fallthrough


all:
	$(CXX) $(WARNINGS) main.cpp

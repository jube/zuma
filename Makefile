CXX = clang++
CXXFLAGS = -Wall -g -O2 -std=c++11
LDFLAGS = -lsfml-system -lsfml-window -lsfml-graphics

TARGETS = zuma zuma2

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

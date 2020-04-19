.PHONY: all clean

OPT				?= 1
OPTFLAGS		?= -DDEBUG -march=native -Og -g
CXXFLAGS		+= -fopenmp -std=c++17


all: ex1 ex2 ex3 ex4 gol

clean:
	$(RM) ex1 ex2 ex3 ex4 gol



ex1: src/ex1.cpp
	$(CXX) $(OPTFLAGS) $(CXXFLAGS) -o $@ $<

ex2: src/ex2.cpp
	$(CXX) $(OPTFLAGS) $(CXXFLAGS) -o $@ $<

ex3: src/ex3.cpp
	$(CXX) $(OPTFLAGS) $(CXXFLAGS) -o $@ $<

ex4: src/ex4.cpp
	$(CXX) $(OPTFLAGS) $(CXXFLAGS) -o $@ $<

gol: src/gol.cpp
	$(CXX) $(OPTFLAGS) $(CXXFLAGS) -o $@ $< $(shell pkg-config --libs allegro-5 allegro_font-5 allegro_image-5 allegro_ttf-5 allegro_video-5 allegro_primitives-5)


brief: clean
	./bench.sh $(OPT) > BRIEF.md

bench: clean
	./bench.sh $(OPT) --no-docs > docs/bench/new_bench.md


all: clean m_solver

m_solver:
	g++ math_solver.cpp main.cpp -o solver -std=c++17

clean:
	rm -f solver

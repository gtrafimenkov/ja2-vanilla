CCd	= gcc
CXX	= g++
CFLAGd	= -fPIC
COMPILE_FLAGS	= -c -Wall -Werror -DUNITTESTABLE -I./ja2/Build
COVERAGE_FLAGS	= -fprofile-arcs -ftest-coverage

test:
	$(CXX) $(CFLAG) $(COMPILE_FLAGS) tester.cc
	$(CC)  $(CFLAG) $(COMPILE_FLAGS) $(COVERAGE_FLAGS) ja2/Build/Strategic/TownMilitia.c
	$(CXX) $(CFLAG) $(COVERAGE_FLAGS) -o tester tester.o TownMilitia.o -lgtest -lpthread
	./tester
	gcov ja2/Build/Strategic/TownMilitia.c
	mkdir -p lcov-report
	lcov --capture --directory . --output-file lcov-report/coverage.info
	genhtml lcov-report/coverage.info --output-directory lcov-report
	mkdir -p gcovr-report
	gcovr --root . --html --html-details --output gcovr-report/coverage.html

# dependencies:
#   sudo apt install lcov
#   sudo apt install gcovr
#   sudo apt install libgtest-dev

# usage:
#   make test
#   open lcov-report/Strategic/TownMilitia.c.gcov.html
#   open gcovr-report/coverage.html

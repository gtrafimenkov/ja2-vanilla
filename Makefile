CC	= gcc
CFLAG = -fPIC -fprofile-arcs -ftest-coverage
COMPILE_FLAGS = $(CFLAG) -c -Wall -Werror -DUNITTESTABLE -I./ja2/Build

test:
	$(CC) $(COMPILE_FLAGS) ja2/Build/Strategic/TownMilitia.c
	$(CC) $(COMPILE_FLAGS) tester.c
	$(CC) $(CFLAG) -o tester tester.o TownMilitia.o
	./tester
	gcov tester.c ja2/Build/Strategic/TownMilitia.c
	mkdir -p lcov-report
	lcov --capture --directory . --output-file lcov-report/coverage.info
	genhtml lcov-report/coverage.info --output-directory lcov-report
	mkdir -p gcovr-report
	gcovr --root . --html --html-details --output gcovr-report/coverage.html

CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11

SRC_FILES=back/*.c
HEADER_FILES=back/*.h

CPP_FILES=$(wildcard *.cpp)

TEST_SRC=tests/*.c

ifeq ($(shell uname), Linux)
	LIB_CHECK=-lcheck -lm -lpthread -lrt -lsubunit
else
# LIB_CHECK=-lcheck -lm
	LIB_CHECK=-lm $(shell pkg-config --cflags --libs check)
endif

all: install

install: clean
	mkdir -p viewer
	cd viewer && qmake ../3dviewer_v1/3dviewer_v1.pro && make
	open viewer/3dviewer_v1.app

uninstall:
	rm -rf viewer

run:
	open viewer/3dviewer_v1.app

dvi:
	@open documentation.md
	
clean:
	rm -rf viewer
	rm -rf test
	rm -rf 3dviewer_v1/*.gcda 3dviewer_v1/*.gcno
	rm -rf gcov_report
	rm -rf report
	rm -rf *.gcno *.gcda

test: clean
	$(CC) --coverage back/*.c $(TEST_SRC) -o test $(LIB_CHECK)
	./test
	rm -rf *.gcda *.gcno

gcov_report:
	$(CC) $(CFLAGS) --coverage $(SRC_FILES) $(TEST_SRC) $(LIB_CHECK) -o gcov_report
	./gcov_report
	lcov -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
	open ./report/index.html
	rm -rf *.gcno *.gcda *.info gcov_report

dist: install
	tar --totals -cvf 3dviewer_v1.tar 3dviewer_v1
	rm -rf viewer

style_test:
	# clang-format -n -style=google $(SRC_FILES) $(HEADER_FILES) $(CPP_FILES) 
	clang-format -i -style=google $(SRC_FILES) $(HEADER_FILES) $(CPP_FILES) 

CC := g++
CFLAGS = `pkg-config --cflags Qt5Widgets` -Wall -Wextra -Werror -std=c++17
GCOVFLAGS = -fprofile-arcs -ftest-coverage
LDFLAGS := `pkg-config --libs Qt5Widgets` -fPIC -no-pie

TARGET = s21brick_game.a
TEST_TARGET := test

TARGETDIR := .
BUILDDIR := ./build
TESTDIR := ./tests
GCOVDIR := ./gcov
SOURCEDIR := ./brick_game/
INSTALLDIR := ./game
GCOVDIR := ./gcov
LIBS := -lncurses 
TARGETS := install uninstall all lib $(TARGET) clean test gcov_report rebuild dist valg

install: install_1 lib $(TARGET)


install_1: 
	mkdir -p $(INSTALLDIR)
	bash -c "touch $(INSTALLDIR)/record.txt"
	echo "0" > $(INSTALLDIR)/record.txt

uninstall:
	rm -rf tetris_game

all: test
	./run.sh
lib:
	mkdir -p build
	$(CC) $(CFLAGS) -c $(SOURCEDIR)/tetris/s21_TetrisController.cpp -o $(BUILDDIR)/s21_TetrisController.o  $(LIBS) $(LDFLAGS)
	$(CC) $(CFLAGS) -c $(SOURCEDIR)/tetris/s21_TetrisModel.cpp -o $(BUILDDIR)/s21_TetrisModel.o  $(LIBS) $(LDFLAGS)

	$(CC) $(CFLAGS) -c $(SOURCEDIR)/snake/s21_SnakeController.cpp -o $(BUILDDIR)/s21_SnakeController.o  $(LIBS) $(LDFLAGS)
	$(CC) $(CFLAGS) -c $(SOURCEDIR)/snake/s21_SnakeModel.cpp -o $(BUILDDIR)/s21_SnakeModel.o  $(LIBS) $(LDFLAGS)
	ar rcs $(TARGET) $(BUILDDIR)/s21_TetrisController.o $(BUILDDIR)/s21_TetrisModel.o $(BUILDDIR)/s21_SnakeController.o $(BUILDDIR)/s21_SnakeModel.o

$(TARGET): CFLAGS := `pkg-config --cflags Qt5Widgets`
$(TARGET):
	moc gui/desktop/s21_front_desktop.h -o gui/desktop/moc_s21_front_desktop.cpp
	g++ $(CFLAGS) -c gui/desktop/moc_s21_front_desktop.cpp `pkg-config --cflags Qt5Widgets` -o gui/desktop/moc_s21_front_desktop.o $(LIBS) $(LDFLAGS)

	$(CC) $(CFLAGS) gui/cli/s21_front_cli.cpp gui/cli/s21_cli.cpp gui/desktop/s21_front_desktop.cpp gui/desktop/moc_s21_front_desktop.cpp gui/main.cpp -o $(INSTALLDIR)/game -L. -l:$(TARGET) $(LIBS) $(LDFLAGS)

clean:
	rm -rf $(BUILDDIR)/*.o *.a tetris $(BUILDDIR)/*.gcno $(BUILDDIR)/*.gcda tetris_game *.html *.css *.gcno *.gcda test *.txt $(GCOVDIR) tetris_game.tar.gz latex html game gui/desktop/moc_s21_front_desktop.o


test: LIBS := -lgtest -lgtest_main -pthread
test: lib install_1
	$(CC) -c --coverage tests/main.cpp -o $(BUILDDIR)/test.o $(LIBS) --coverage
	$(CC) -o $(INSTALLDIR)/test $(BUILDDIR)/test.o -L. -l:$(TARGET) $(LIBS) --coverage
	chmod 777 test.sh
	./test.sh

valg: test
	chmod 777 valgrind.sh
	./valgrind.sh

gcov_report: CFLAGS += --coverage
gcov_report: LDFLAGS += --coverage
gcov_report: test
	mkdir -p $(GCOVDIR)
	gcovr . -r . --html --html-details -o $(GCOVDIR)/coverage.html 

dist: install
	cp brick_game gui makefile $(INSTALLDIR) -r
	tar -czf $(INSTALLDIR).tar.gz $(INSTALLDIR)
	rm -rf $(BUILDDIR)/*.o *.a tetris $(BUILDDIR)/*.gcno $(BUILDDIR)/*.gcda tetris_game *.html *.css *.gcno *.gcda test *.txt $(GCOVDIR) tetris_game.tar.gz latex html game gui/desktop/moc_s21_front_desktop.o
dvi:
	doxygen
#open html/index.html
clang-i:
	clang-format --style=google -i tests/*.cpp gui/*.cpp gui/cli/*.h gui/cli/*.cpp gui/desktop/*.h gui/desktop/*.cpp brick_game/*.h brick_game/snake/*.cpp brick_game/snake/*.h brick_game/tetris/*.cpp brick_game/tetris/*.h

.PHONY: $(TARGETS)
##
## Makefile
## File description:
## bootloader
##

SRC	=	sources/main.cpp \
		sources/Reception.cpp \
		sources/Stock.cpp \
		sources/Cook.cpp \
		sources/Pizza.cpp \
		sources/MsgQueue.cpp \
		sources/ThreadPool.cpp \
		sources/Kitchen.cpp

NAME	=	plazza

OBJ	=	$(SRC:.cpp=.o)

CPPFLAGS	=	-I./includes

LDLIBS	=	-lrt -pthread

CXXFLAGS = -Wall -Wextra

TESTS =

TEST_OBJ = $(TESTS:.cpp=.o)

LIB_TEST = libutest.a

TEST_NAME = unit_tests

TEST_INCLUDE = -lcriterion --coverage

all: $(NAME)

$(NAME):	$(OBJ)
		g++ -o $(NAME) $(OBJ) $(CPPFLAGS) $(CXXFLAGS) $(LDLIBS)

debug:	CXXFLAGS += -g3
debug:	$(NAME)

unit_tests: fclean
		$(CC) $(TESTS) $(TEST_INCLUDE) -o $(TEST_NAME)

tests_run: unit_tests
			./$(TEST_NAME)
			gcovr

clean:
	rm -f sources/*.o
	rm -f sources/*~
	rm -f includes/*~
	rm -f tests/*.o

fclean:	clean
	rm -f $(NAME)
	rm -f $(TEST_NAME)

re:	fclean all

help:
	@printf "LIST OF OPTIONS:\n\n"
	@printf "\thelp: displays all the make options and they're utilities\n\n"
	@printf "\tbuild: build the project\n\n"
	@printf "\tdebug: build the project with -g3 flag for valgrind\n\n"
	@printf "\tunit_tests: build units tests for the project\n\n"
	@printf "\ttests_run: build and execute unit tests for the project\n\n"
	@printf "\tclean: remove temporary files used for build\n\n"
	@printf "\tfclean: clean and remove binaries\n\n"
	@printf "\tre: fclean then build\n\n"

.PHONY:	all build debug clean fclean re

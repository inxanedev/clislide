CC=g++
FLAGS=-I. -std=c++17 -lstdc++fs
NAME=prog
LINK=-lncurses
build: main.cpp
	$(CC) main.cpp -o $(NAME) $(FLAGS) $(LINK)
run: main.cpp
	@make build --no-print-directory
	@./$(NAME)
test: main.cpp
	@clear
	@make run --no-print-directory

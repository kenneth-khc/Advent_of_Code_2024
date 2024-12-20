NAME := day6_2

CXX := g++
CXXFLAGS := -Wall -Werror -Wextra -g3 -std=c++2a -O3

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

.PHONY: fsan
fsan: CXXFLAGS += -fsanitize=address,undefined
fsan: all

.PHONY: clean
clean:
	$(RM) $(OBJS)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re: fclean all

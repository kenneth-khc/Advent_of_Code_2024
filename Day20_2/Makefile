NAME := day20_2

CXX := g++
CXXFLAGS := -Wall -Werror -Wextra -g3 -std=c++2a

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

.PHONY: clean
clean:
	$(RM) $(OBJS)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re: fclean all

.PHONY: fsan
fsan: CXXFLAGS += -fsanitize=address,undefined
fsan: all

.PHONY: optimized
optimized: CXXFLAGS += -O3
optimized: all
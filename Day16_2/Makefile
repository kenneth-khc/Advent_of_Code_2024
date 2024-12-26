NAME := day16_2

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

.PHONY:
fsan: CXXFLAGS += -fsanitize=address,undefined
fsan: all
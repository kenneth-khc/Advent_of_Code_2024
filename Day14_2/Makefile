NAME := day14_2

CXX := c++
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

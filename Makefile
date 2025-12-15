NAME    = libftpp.a
CXX     = c++
CXXFLAGS= -Wall -Wextra -Werror -Iinclude
AR      = ar rcs

SRCS    = src/vector.cpp \
          src/string.cpp \
          src/algo.cpp

OBJS    = $(SRCS:.cpp=.o)

# Default target
all: $(NAME)

# Create the static library
$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

# Compile .cpp -> .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleanup
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

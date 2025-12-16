CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17 -g -Iinclude

LIB_NAME := libpool.a
LIB_DIR  := lib
BUILD    := build

TESTS := test_basic test_move test_edge

TEST_SRCS := $(addsuffix .cpp,$(addprefix tests/,$(TESTS)))
TEST_BINS := $(addprefix $(BUILD)/,$(TESTS))

all: $(LIB_DIR)/$(LIB_NAME) $(TEST_BINS)

# ---------- Library (header-only for now) ----------
$(LIB_DIR)/$(LIB_NAME):
	mkdir -p $(LIB_DIR)
	ar rcs $@

# ---------- Tests ----------
$(BUILD)/%: tests/%.cpp $(LIB_DIR)/$(LIB_NAME)
	mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $< -L$(LIB_DIR) -lpool -o $@

clean:
	rm -rf $(BUILD)

fclean: clean
	rm -rf $(LIB_DIR)

re: fclean all

.PHONY: all clean fclean re

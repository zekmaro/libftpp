CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17 -g -Iinclude

LIB_NAME := libftpp.a
LIB_DIR  := lib
BUILD    := build

TESTS := test_basic test_move test_edge

TEST_SRCS := $(addsuffix .cpp,$(addprefix tests/pool/,$(TESTS)))
TEST_BINS := $(addprefix $(BUILD)/pool/,$(TESTS))

all: $(TEST_BINS)

# ---------- Tests ----------
$(BUILD)/pool/%: tests/pool/%.cpp
	mkdir -p $(BUILD)/pool
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BUILD)

fclean: clean
	rm -rf $(LIB_DIR)

re: fclean all

.PHONY: all clean fclean re

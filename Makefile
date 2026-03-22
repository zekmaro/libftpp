CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17 -g -Iinclude

LIB_NAME := libftpp.a
LIB_DIR  := lib
BUILD    := build

POOL_TESTS := test_basic test_move test_edge
POOL_BINS  := $(addprefix $(BUILD)/pool/,$(POOL_TESTS))

DB_BINS    := $(BUILD)/data_buffer/test_data_buffer

all: $(POOL_BINS) $(DB_BINS)

# ---------- Pool tests ----------
$(BUILD)/pool/%: tests/pool/%.cpp
	mkdir -p $(BUILD)/pool
	$(CXX) $(CXXFLAGS) $< -o $@

# ---------- DataBuffer tests ----------
$(BUILD)/data_buffer/%: tests/data_buffer/%.cpp
	mkdir -p $(BUILD)/data_buffer
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BUILD)

fclean: clean
	rm -rf $(LIB_DIR)

re: fclean all

.PHONY: all clean fclean re

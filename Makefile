CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17 -g -Iinclude

LIB_NAME := libftpp.a
LIB_DIR  := lib
BUILD    := build

POOL_TESTS := test_basic test_move test_edge
POOL_BINS  := $(addprefix $(BUILD)/pool/,$(POOL_TESTS))

DB_BINS      := $(BUILD)/data_buffer/test_data_buffer
MEMENTO_BINS  := $(BUILD)/memento/test_memento
OBSERVER_BINS   := $(BUILD)/observer/test_observer
SINGLETON_BINS    := $(BUILD)/singleton/test_singleton
STATEMACHINE_BINS := $(BUILD)/state_machine/test_state_machine

all: $(POOL_BINS) $(DB_BINS) $(MEMENTO_BINS) $(OBSERVER_BINS) $(SINGLETON_BINS) $(STATEMACHINE_BINS)

# ---------- Pool tests ----------
$(BUILD)/pool/%: tests/pool/%.cpp
	mkdir -p $(BUILD)/pool
	$(CXX) $(CXXFLAGS) $< -o $@

# ---------- DataBuffer tests ----------
$(BUILD)/data_buffer/%: tests/data_buffer/%.cpp
	mkdir -p $(BUILD)/data_buffer
	$(CXX) $(CXXFLAGS) $< -o $@

# ---------- Memento tests ----------
$(BUILD)/memento/%: tests/memento/%.cpp
	mkdir -p $(BUILD)/memento
	$(CXX) $(CXXFLAGS) $< -o $@

# ---------- Observer tests ----------
$(BUILD)/observer/%: tests/observer/%.cpp
	mkdir -p $(BUILD)/observer
	$(CXX) $(CXXFLAGS) $< -o $@

# ---------- Singleton tests ----------
$(BUILD)/singleton/%: tests/singleton/%.cpp
	mkdir -p $(BUILD)/singleton
	$(CXX) $(CXXFLAGS) $< -o $@

# ---------- StateMachine tests ----------
$(BUILD)/state_machine/%: tests/state_machine/%.cpp
	mkdir -p $(BUILD)/state_machine
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BUILD)

fclean: clean
	rm -rf $(LIB_DIR)

re: fclean all

.PHONY: all clean fclean re

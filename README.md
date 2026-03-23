# libftpp — Further Exploration into Advanced C++

A comprehensive C++ toolbox built as part of a 42 school project. The goal is to implement fundamental systems from scratch — data structures, design patterns, threading, networking, and mathematics — using modern C++ (C++11 and later).

## Build

```bash
make        # build the library and tests
make re     # clean rebuild
make clean  # remove build artifacts
make fclean # remove build artifacts and library
```

## Run tests

```bash
# Pool
./build/pool/test_basic
./build/pool/test_move
./build/pool/test_edge

# DataBuffer
./build/data_buffer/test_data_buffer

# Design Patterns
./build/memento/test_memento
./build/observer/test_observer
./build/singleton/test_singleton
./build/state_machine/test_state_machine
```

## Structure

```
include/                    # all headers
  libftpp.hpp               # single include for the whole library
  data_stuctures/
    pool.hpp                # Pool<T> and Pool::Object
    data_buffer.hpp         # DataBuffer
  design_patterns/
    memento.hpp
    observer.hpp
    singleton.hpp
    state_machine.hpp
src/                        # implementation files
tests/
  pool/                     # Pool tests
  data_buffer/              # DataBuffer tests
  memento/                  # Memento tests
  observer/                 # Observer tests
```

## Implemented

### Pool\<T\> — `include/data_stuctures/pool.hpp`

Fixed-size object pool with manual lifetime management. Pre-allocates raw aligned memory and constructs objects in-place using placement new. No `malloc`/`free` per object.

```cpp
Pool<MyType> pool;           // default capacity 256
pool.resize(64);             // resize (only when no live objects)

auto obj = pool.acquire(args...);  // construct in-place, returns RAII handle
obj->method();               // access via ->
*obj;                        // access via *
// obj goes out of scope → destructor called, slot released automatically
```

**Key design decisions:**
- `Pool::Object` is move-only — no copying
- Free slot tracking via O(1) stack (not O(n) scan)
- `liveCount_` for O(1) safety checks in `resize()`
- Asserts catch misuse in debug builds, compiled out in release (`-DNDEBUG`)
- Pool must outlive all its `Pool::Object` handles

### DataBuffer — `include/data_stuctures/data_buffer.hpp`

Polymorphic byte container for serialization. Write any type with `<<`, read back in the same order with `>>`. Used as the snapshot type in `Memento`.

```cpp
DataBuffer buf;
buf << 42 << 3.14f;   // serialize

int a; float b;
buf >> a >> b;         // deserialize — must match write order
```

### Memento — `include/design_patterns/memento.hpp`

Base class mixin for save/restore state. Inherit from it and implement `_saveToSnapshot` and `_loadFromSnapshot` to define what gets saved.

```cpp
class Player : public Memento {
    void _saveToSnapshot(Snapshot& s) const override { s << health_ << x_; }
    void _loadFromSnapshot(Snapshot& s) override     { s >> health_ >> x_; }
};

auto snap = player.save();   // capture state
player.load(snap);           // restore state
```

### Observer\<TEvent\> — `include/design_patterns/observer.hpp`

Event notification system. Subscribe lambdas to events, fire all of them with `notify`.

```cpp
Observer<std::string> events;
events.subscribe("player_died", []() { showGameOver(); });
events.subscribe("player_died", []() { saveScore(); });
events.notify("player_died"); // both lambdas fire in order
```

### Singleton\<TType\> — `include/design_patterns/singleton.hpp`

Mixin base class enforcing a single instance. Inherit from it, declare it as `friend`, and keep your constructor private.

```cpp
class AudioManager : public Singleton<AudioManager> {
    friend class Singleton<AudioManager>;
    AudioManager() {}
};

AudioManager::instantiate();       // create the instance
AudioManager::instance()->play();  // access it anywhere
// calling instantiate() twice throws
// calling instance() before instantiate() throws
```

**Key design decisions:**
- Instance stored as `unique_ptr<TType>` — no memory leak, cleaned up at program exit
- Copy and move deleted — only one instance can exist
- `instantiate()` uses perfect forwarding to support any constructor arguments

### StateMachine\<TState\> — `include/design_patterns/state_machine.hpp`

Manages states and transitions. Register states, define transition lambdas (run when switching), and action lambdas (run on `update()`). Throws on any missing registration.

```cpp
StateMachine<State> sm(State::Idle);
sm.addState(State::Running);
sm.addTransition(State::Idle, State::Running, []() { std::cout << "starting\n"; });
sm.addAction(State::Running, []() { std::cout << "running\n"; });

sm.transitionTo(State::Running);  // fires transition lambda
sm.update();                      // fires action for current state
```

## TODO

- [ ] IOStream — `ThreadSafeIOStream` with prefix and thread-local support
- [ ] Thread — `Thread`, `ThreadSafeQueue`, `WorkerPool`, `PersistentWorker`
- [ ] Network — `Message`, `Client`, `Server`
- [ ] Mathematics — `IVector2`, `IVector3`, `Random2DCoordinateGenerator`, `PerlinNoise2D`

## Rules

- C++11 or later, compiled with `-Wall -Wextra -Werror`
- No external libraries (no Boost etc.)
- Forbidden: `*printf()`, `*alloc()`, `free()`
- No memory leaks
- Each header must be self-contained with include guards

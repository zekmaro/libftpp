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
./build/pool/test_basic
./build/pool/test_move
./build/pool/test_edge
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
    test_basic.cpp
    test_move.cpp
    test_edge.cpp
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

Polymorphic byte container with stream operator overloads (`<<`, `>>`). Used for serialization.

### Design Patterns — `include/design_patterns/`

| Class | Description |
|---|---|
| `Memento` | Save/restore object state via snapshots |
| `Observer<TEvent>` | Subscribe lambdas to events, notify on trigger |
| `Singleton<T>` | Enforce single instance with `instantiate()` / `instance()` |
| `StateMachine<TState>` | State transitions with registered actions |

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

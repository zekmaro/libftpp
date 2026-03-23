#include <iostream>
#include <cassert>
#include "design_patterns/memento.hpp"

class Player : public Memento {
public:
    int     health_;
    float   x_;
    float   y_;

    Player(int health, float x, float y)
        : health_(health), x_(x), y_(y) {}

private:
    void _saveToSnapshot(Snapshot& snapshot) const override {
        snapshot << health_ << x_ << y_;
    }

    void _loadFromSnapshot(Snapshot& snapshot) override {
        snapshot >> health_ >> x_ >> y_;
    }
};

// Test: save and restore basic values
void test_save_and_load() {
    Player p(100, 1.5f, 2.5f);

    Memento::Snapshot snap = p.save();

    p.health_ = 0;
    p.x_      = 99.0f;
    p.y_      = 99.0f;

    p.load(snap);

    assert(p.health_ == 100);
    assert(p.x_      == 1.5f);
    assert(p.y_      == 2.5f);

    std::cout << "PASS test_save_and_load\n";
}

// Test: snapshot is independent — modifying object doesn't affect saved snapshot
void test_snapshot_independence() {
    Player p(50, 0.0f, 0.0f);

    Memento::Snapshot snap = p.save();

    p.health_ = 999;
    p.load(snap);

    assert(p.health_ == 50);

    std::cout << "PASS test_snapshot_independence\n";
}

// Test: load can be called multiple times from the same snapshot
void test_multiple_loads() {
    Player p(75, 3.0f, 4.0f);

    Memento::Snapshot snap = p.save();

    p.health_ = 0;
    p.load(snap);
    assert(p.health_ == 75);

    p.health_ = 0;
    p.load(snap);
    assert(p.health_ == 75);

    std::cout << "PASS test_multiple_loads\n";
}

// Test: multiple snapshots are independent
void test_multiple_snapshots() {
    Player p(100, 0.0f, 0.0f);

    Memento::Snapshot snap1 = p.save();

    p.health_ = 50;
    Memento::Snapshot snap2 = p.save();

    p.load(snap1);
    assert(p.health_ == 100);

    p.load(snap2);
    assert(p.health_ == 50);

    std::cout << "PASS test_multiple_snapshots\n";
}

int main() {
    test_save_and_load();
    test_snapshot_independence();
    test_multiple_loads();
    test_multiple_snapshots();

    std::cout << "All Memento tests passed.\n";
    return 0;
}

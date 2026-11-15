#pragma once

class Visitor;

class Headphones {
private:
    Visitor* visitor;

public:
    Headphones() : visitor(nullptr) {}
    bool is_assigned() const { return visitor != nullptr; }
    void set_visitor(Visitor* v) { visitor = v; }
    Visitor* get_visitor() const { return visitor; }
};

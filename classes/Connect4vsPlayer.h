#pragma once
#include "Connect4.h"

// Simple subclass of Connect4 that disables AI entirely, forcing two human players.
class Connect4vsPlayer : public Connect4
{
public:
    Connect4vsPlayer() : Connect4() {}
    ~Connect4vsPlayer() override {}

    // override AI-related behaviour
    bool gameHasAI() override { return false; }
    void updateAI() override {}
};

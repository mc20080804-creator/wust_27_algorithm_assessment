#pragma once
#include "simulator.hpp"

class MyScheduler {
public:
    Action decide(const View& v, int e);
};
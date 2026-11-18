#include"simulator.hpp"
#include"my_scheduler.hpp"
#include<cstdlib>

Action MyScheduler::decide(const View& v, int e) {
    const Car& car = v.cars[e];
    int currentFloor = car.floor;

    // 当前楼层有人要下，就先开门。
    if (car.car_call[currentFloor] != -1) {
        if (currentFloor == kFloors) {
            return Action::OpenDown;
        }
        return Action::OpenUp;
    }

    // 找一个亮着的轿厢按钮，朝那个楼层移动一层。
    for (int floor = 1; floor <= kFloors; ++floor) {
        if (car.car_call[floor] != -1) {
            if (floor > currentFloor) {
                return Action::Up;
            }
            return Action::Down;
        }
    }
    int targetFloor = 0;        // 0 表示还没找到目标楼层
    int bestDistance = kFloors;

    for (int floor = 1; floor <= kFloors; ++floor) {
        if (v.hall_up[floor] == -1 && v.hall_down[floor] == -1) {
            continue;
        }

        int distance = std::abs(floor - currentFloor);
        if (distance < bestDistance) {
            bestDistance = distance;
            targetFloor = floor;
        }
    }

    if (targetFloor == 0) {
        return Action::Idle;
    }
    if (targetFloor > currentFloor) {
        return Action::Up;
    }
    if (targetFloor < currentFloor) {
        return Action::Down;
    }

    if (v.hall_up[currentFloor] != -1) {
        return Action::OpenUp;
    }
    return Action::OpenDown;
}
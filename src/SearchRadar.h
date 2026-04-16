#ifndef SEARCHRADAR_HPP
#define SEARCHRADAR_HPP

#include "Radar.h"


namespace godot {

class Vehicle;
class SearchRadar : public Radar {
    GDCLASS(SearchRadar, Radar)

protected:
    static void _bind_methods();

public:
    Vehicle* select_best_target(TypedArray<Vehicle> possibleTargets);
};

}

#endif
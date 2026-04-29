#pragma once

#include "Radar.h"
using namespace godot;

class semiActiveRadar : public Radar {
    GDCLASS(semiActiveRadar, Radar)

protected:
    static void _bind_methods();

public:

    void select_best_target(TypedArray<Vehicle>, Vehicle*) override;
    semiActiveRadar();
    ~semiActiveRadar();

    Radar* clone() override;
};

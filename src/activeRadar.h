#pragma once

#include "semiActiveRadar.h"

class activeRadar : public semiActiveRadar
{
    GDCLASS(activeRadar, semiActiveRadar)
protected:
    static void _bind_methods();

public:
    void select_best_target(TypedArray<Vehicle>, Vehicle*) override;
    activeRadar();
    ~activeRadar();
    Radar* clone() override;
};

#pragma once

#include "semiActiveRadar.h"

class activeRadar : public semiActiveRadar
{
public:
    void select_best_target(TypedArray<Vehicle>, Vehicle*) override;
    activeRadar();
    ~activeRadar();
};

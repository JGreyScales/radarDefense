#ifndef Driveable_HPP
#define Driveable_HPP

#include "Vehicle.h"
using namespace godot;

class Driveable : public Vehicle
{
    GDCLASS(Driveable, Vehicle)

protected:
    static void _bind_methods();

private:

public:
    Driveable();
    ~Driveable();

    void tick(float deltaTime);
    void move(MapIcon* target, float deltaTime);
    void avoid_incoming();
    void engage(Vehicle* target);
    Vehicle* clone();
};


#endif
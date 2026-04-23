#ifndef HARDPOINT_H
#define HARDPOINT_H

#include "Weapon.h"

using namespace godot;
class HardPoint
{
private:
    Weapon* equippedWapon;
    Vector<Weapon*> options;

public:
    HardPoint();
    ~HardPoint();

    Vector<String> getWeaponOptionNames();
    Weapon* getEquippedWeapon();
    void setEquippedWeapon(Weapon*);
    
};
#endif
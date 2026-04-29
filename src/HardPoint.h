#ifndef HARDPOINT_H
#define HARDPOINT_H

#include "Weapon.h"

using namespace godot;
class HardPoint
{
private:
    Weapon* equippedWeapon;
    Vector<Weapon*> options;

public:
    HardPoint();
    ~HardPoint();

    Vector<String> getWeaponOptionNames();
    void addWeaponOption(Weapon*);
    Weapon* getEquippedWeapon();
    void setEquippedWeapon(Weapon*);
    
};
#endif
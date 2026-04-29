#ifndef LOADOUT_H
#pragma once
#define LOADOUT_H

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "HardPoint.h"

namespace godot {
class Vehicle;

class Loadout: public Object {
    GDCLASS(Loadout, Object);

protected:
    static void _bind_methods();
    
private:
	/* data */
    Vector<HardPoint*> hardPoints;
    int16_t totalLoadoutWeight;
    String id;
public:
	Loadout(/* args */);
    Loadout(const Loadout &p_other);
	~Loadout();

    void copy_from(const Loadout &p_other);
    void calculateLoadoutWeight();
    int16_t getLoadoutWeight();
    Weapon* getWeaponAtIndex(int index);
    void setWeaponArraySize(int index);
    void setWeaponAtIndex(int index, Weapon* weapon);
    void setHardpointAtIndex(int index, HardPoint* hardpoint);
    HardPoint* calculateBestWeaponToEngageWith(Vehicle* target);

};
} //namespace godot
#endif
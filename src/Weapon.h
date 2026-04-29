#ifndef WEAPON_H
#define WEAPON_H

#include "Flyable.h"

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class Weapon
{
private:
    uint8_t capacity;
    String name;
    String id;
    uint8_t dropweight;
    int maximumRange;
    int effectiveRange;

public:
    Weapon();
    Weapon(String id);
    ~Weapon();

    Flyable* deploy(Vehicle* parent, Vehicle* target);
    String getName();
    String getID();
    uint8_t getDropWeight();
    uint8_t getCapacity();
    
    void setName(String name);
    void setID(String id);
    void setDropWeight(uint8_t dropWeight);
    void setCapacity(uint8_t value);


    int getMaximumRange(Flyable* self);
    int getEffectiveRange(Flyable* self);

    Weapon* clone();
};


#endif
#pragma once

#include "Flyable.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>



namespace godot {
class Vehicle;

class Weapon : public Object {
	GDCLASS(Weapon, Object);

protected:
    static void _bind_methods();

private:
	uint8_t capacity;
	String name;
	String id;
	uint8_t dropweight;
	int maximumRange;
	int effectiveRange;

// protected:
// 	static void _bind_methods();

public:
	Weapon();
	Weapon(String id);
	~Weapon();

	Flyable*deploy(Vehicle *parent, Vehicle *target);
	String getName();
	String getID();
	uint8_t getDropWeight();
	uint8_t getCapacity();

	void setName(String name);
	void setID(String id);
	void setDropWeight(uint8_t dropWeight);
	void setCapacity(uint8_t value);

	int getMaximumRange(Flyable *self);
	int getEffectiveRange(Flyable *self);

	Weapon *clone();
};

} //namespace godot
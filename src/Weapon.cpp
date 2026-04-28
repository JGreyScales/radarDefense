#include "Weapon.h"
#include "Vehicle.h"
#include "Globals.h"

Weapon::Weapon(String id) {
    this->id = id;
}

Weapon::~Weapon() {
}

Flyable *Weapon::deploy(Vehicle *parent, Vehicle*target) {
	Flyable* spawnedObject = static_cast<Flyable*>(GlobalManager::get_vehicle_from_id(this->id));

	spawnedObject->set_move_waypoint(target);
	spawnedObject->get_radar()->set_data_link_child(parent);

	return spawnedObject;
}

String Weapon::getName() {
	return this->name;
}

String Weapon::getID() {
	return this->id;
}

uint8_t Weapon::getDropWeight() {
	return this->dropweight;
}

uint8_t Weapon::getCapcity() {
	return this->capacity;
}

void Weapon::setName(String name) {
    this->name = name;
}

void Weapon::setID(String id) {
    this->id = id;
}

void Weapon::setDropWeight(uint8_t dropWeight) {
    this->dropweight = dropweight;
}

int Weapon::getMaximumRange(Flyable *self) {
	return self->calculateMaximumRange();
}

int Weapon::getEffectiveRange(Flyable *self) {
	return self->calculateMaximumEffectiveRange();
}

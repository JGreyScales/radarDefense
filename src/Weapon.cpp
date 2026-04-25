#include "Weapon.h"
#include "Vehicle.h"

Weapon::Weapon(String id) {
    this->id = id;
}

Weapon::~Weapon() {
}

Flyable *Weapon::deploy(void *parent, void *target) {
	return nullptr;
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

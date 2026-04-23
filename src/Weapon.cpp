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

int Weapon::calculateMaximumRange(int verticalZOffset) {
	return 0;
}

int Weapon::calculateEffectiveRange(int verticalZOffset) {
	return 0;
}

int Weapon::getMaximumRange(int verticalZOffset) {
    this->calculateMaximumRange(verticalZOffset);
	return this->maximumRange;
}

int Weapon::getEffectiveRange(int verticalZOffset) {
    this->calculateEffectiveRange(verticalZOffset);
	return this->effectiveRange;
}

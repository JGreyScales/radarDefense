#include "Weapon.h"
#include "Vehicle.h"
#include "Globals.h"

Weapon::Weapon() {
}

Weapon::Weapon(String id) {
	this->id = id;
}

Weapon::~Weapon() {
}

Flyable *Weapon::deploy(Vehicle *parent, Vehicle*target) {

	if (this->capacity-- > 0){
		Flyable* spawnedObject = static_cast<Flyable*>(GlobalManager::get_vehicle_from_id(this->id));

		spawnedObject->set_speed(parent->get_speed());
		spawnedObject->set_direction(parent->get_direction());
		spawnedObject->set_move_waypoint(target);
		spawnedObject->get_radar()->set_host_radar(parent->get_radar());
		spawnedObject->get_radar()->set_launch_target(target);
		parent->set_weight(parent->get_weight() - this->dropweight);

		return spawnedObject;
	}
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

uint8_t Weapon::getCapacity() {
	return this->capacity;
}

uint8_t Weapon::getCapacity() {
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

void Weapon::setCapacity(uint8_t value) {
	this->capacity = value;
}

// need to clone an instance of the missile at location for this to correctly math

int Weapon::getMaximumRange(Flyable *self) {
	this->maximumRange = self->calculateMaximumRange();
	return this->maximumRange;
}

int Weapon::getEffectiveRange(Flyable *self) {
	this->effectiveRange = self->calculateMaximumEffectiveRange();
	return this->effectiveRange;
}

Weapon *Weapon::clone() {
	Weapon* returnValue = memnew(Weapon);

	returnValue->setCapacity(this->getCapacity());
	returnValue->setDropWeight(this->getDropWeight());
	returnValue->setID(this->getID());
	returnValue->setName(this->getName());

	return returnValue;
}

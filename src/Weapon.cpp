#include "Weapon.h"
#include "Vehicle.h"
#include "Globals.h"
#include "Flyable.h"

namespace godot {
    
void Weapon::_bind_methods() {
    // Getters
    ClassDB::bind_method(D_METHOD("get_name"), &Weapon::getName);
    ClassDB::bind_method(D_METHOD("get_id"), &Weapon::getID);
    ClassDB::bind_method(D_METHOD("get_drop_weight"), &Weapon::getDropWeight);
    ClassDB::bind_method(D_METHOD("get_capacity"), &Weapon::getCapacity);
    ClassDB::bind_method(D_METHOD("get_maximum_range", "flyable_instance"), &Weapon::getMaximumRange);
    ClassDB::bind_method(D_METHOD("get_effective_range", "flyable_instance"), &Weapon::getEffectiveRange);

    // Setters
    ClassDB::bind_method(D_METHOD("set_name", "name"), &Weapon::setName);
    ClassDB::bind_method(D_METHOD("set_id", "id"), &Weapon::setID);
    ClassDB::bind_method(D_METHOD("set_drop_weight", "weight"), &Weapon::setDropWeight);
    ClassDB::bind_method(D_METHOD("set_capacity", "value"), &Weapon::setCapacity);

    // Logic
    ClassDB::bind_method(D_METHOD("deploy", "parent", "target"), &Weapon::deploy);
    ClassDB::bind_method(D_METHOD("clone"), &Weapon::clone);

    // Properties (Optional: makes these visible in the Godot Inspector)
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "id"), "set_id", "get_id");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "capacity"), "set_capacity", "get_capacity");
}

Weapon::Weapon() {
    capacity = 0;
    dropweight = 0;
    maximumRange = 0;
    effectiveRange = 0;
}

Weapon::Weapon(String id) {
    this->id = id;
    capacity = 0;
    dropweight = 0;
    maximumRange = 0;
    effectiveRange = 0;
}

Weapon::~Weapon() {
}

Flyable *Weapon::deploy(Vehicle *parent, Vehicle *target) {
    if (this->capacity-- > 0) {
        Flyable* spawnedObject = static_cast<Flyable*>(GlobalManager::get_vehicle_from_id(this->id));

        spawnedObject->set_speed(parent->get_speed());
        spawnedObject->set_direction(parent->get_direction());
        spawnedObject->set_move_waypoint(target);
        spawnedObject->get_radar()->set_host_radar(parent->get_radar());
        spawnedObject->get_radar()->set_launch_target(target);
        parent->set_weight(parent->get_weight() - this->dropweight);

        return spawnedObject;
    }
    
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
    this->dropweight = dropWeight; 
}

void Weapon::setCapacity(uint8_t value) {
    this->capacity = value;
}

int Weapon::getMaximumRange(Flyable *self) {
    this->maximumRange = self->calculateMaximumRange();
    return this->maximumRange;
}

int Weapon::getEffectiveRange(Flyable *self) {
    this->effectiveRange = self->calculateMaximumEffectiveRange();
    return this->effectiveRange;
}

Weapon *Weapon::clone() {
    UtilityFunctions::print("cloning weapon");
    Weapon* returnValue = memnew(Weapon);

    returnValue->setCapacity(this->getCapacity());
    returnValue->setDropWeight(this->getDropWeight());
    returnValue->setID(this->getID());
    returnValue->setName(this->getName());

    return returnValue;
}
}
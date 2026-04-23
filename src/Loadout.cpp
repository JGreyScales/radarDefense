#include "Loadout.h"

godot::Loadout::Loadout() {
}

godot::Loadout::~Loadout() {
}

void godot::Loadout::calculateLoadoutWeight() {
	int totalWeight = 0;
    for (size_t i = 0; i < this->hardPoints.size(); i++)
    {
        HardPoint* selectedHardpoint = this->hardPoints[i];
        if (selectedHardpoint && selectedHardpoint->getEquippedWeapon()){
            totalWeight += selectedHardpoint->getEquippedWeapon()->getDropWeight();
        }
    }
    this->totalLoadoutWeight = totalWeight;
}

int16_t godot::Loadout::getLoadoutWeight() {
	return this->totalLoadoutWeight;
}

Weapon *godot::Loadout::getWeaponAtIndex(int index) {
    if (this->hardPoints.size() < index){
        return nullptr;
    } else {
        return this->hardPoints[index]->getEquippedWeapon();
    }
}

HardPoint *godot::Loadout::calculateBestWeaponToEngageWith(Vehicle *target) {
	return nullptr;
}

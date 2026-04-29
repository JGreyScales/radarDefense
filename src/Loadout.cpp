#include "Loadout.h"

godot::Loadout::Loadout() {
	this->hardPoints = Vector<HardPoint *>();
}

godot::Loadout::~Loadout() {
}

void godot::Loadout::calculateLoadoutWeight() {
	int totalWeight = 0;
	for (size_t i = 0; i < this->hardPoints.size(); i++) {
		HardPoint *selectedHardpoint = this->hardPoints[i];
		if (selectedHardpoint && selectedHardpoint->getEquippedWeapon()) {
			totalWeight += selectedHardpoint->getEquippedWeapon()->getDropWeight();
		}
	}
	this->totalLoadoutWeight = totalWeight;
}

int16_t godot::Loadout::getLoadoutWeight() {
	return this->totalLoadoutWeight;
}

Weapon *godot::Loadout::getWeaponAtIndex(int index) {
	if (this->hardPoints.size() < index) {
		return nullptr;
	} else {
		return this->hardPoints[index]->getEquippedWeapon();
	}
}

void godot::Loadout::setWeaponArraySize(int size) {
	this->hardPoints.resize(size);

	for (int i = 0; i < size; i++) {
		this->hardPoints.write[i] = nullptr;
	}
}

void godot::Loadout::setWeaponAtIndex(int index, Weapon *weapon) {
	this->hardPoints.get(index)->setEquippedWeapon(weapon);
}

void godot::Loadout::setHardpointAtIndex(int index, HardPoint *hardpoint) {
	if (index < 0 || index >= this->hardPoints.size()) {
		return;
	}

	HardPoint *old_hp = this->hardPoints.get(index);

	if (old_hp != nullptr) {
		if (old_hp != hardpoint) {
			delete old_hp;
		}
	}

	this->hardPoints.set(index, hardpoint);
}

HardPoint *godot::Loadout::calculateBestWeaponToEngageWith(Vehicle *target) {
	return nullptr;
}

#include "Loadout.h"
#include "Weapon.h"

void Loadout::_bind_methods() {
    // Binding the weight calculation and getter
    ClassDB::bind_method(D_METHOD("calculate_loadout_weight"), &Loadout::calculateLoadoutWeight);
    ClassDB::bind_method(D_METHOD("get_loadout_weight"), &Loadout::getLoadoutWeight);

    // Binding Weapon/Hardpoint interaction
    // NOTE: For these to work in GDScript, 'Weapon' and 'HardPoint' must also be GDCLASS-enabled
    ClassDB::bind_method(D_METHOD("get_weapon_at_index", "index"), &Loadout::getWeaponAtIndex);
    ClassDB::bind_method(D_METHOD("set_weapon_array_size", "size"), &Loadout::setWeaponArraySize);
    ClassDB::bind_method(D_METHOD("set_weapon_at_index", "index", "weapon"), &Loadout::setWeaponAtIndex);
    ClassDB::bind_method(D_METHOD("set_hardpoint_at_index", "index", "hardpoint"), &Loadout::setHardpointAtIndex);
    
    // Binding the logic method
    ClassDB::bind_method(D_METHOD("calculate_best_weapon_to_engage_with", "target"), &Loadout::calculateBestWeaponToEngageWith);

    // If you want totalLoadoutWeight to appear as a property in Godot:
    ClassDB::add_property("Loadout", PropertyInfo(Variant::INT, "total_loadout_weight"), "set_weapon_array_size", "get_loadout_weight");
}
godot::Loadout::Loadout() {
	this->hardPoints = Vector<HardPoint *>();
}


godot::Loadout::~Loadout() {
}


Loadout* Loadout::clone() {
    Loadout* newLoadout = memnew(Loadout);
    newLoadout->id = this->id;
    newLoadout->setWeaponArraySize(this->hardPoints.size());

    for (int i = 0; i < this->hardPoints.size(); i++) {
        if (this->hardPoints[i] != nullptr) {
            newLoadout->setHardpointAtIndex(i, this->hardPoints[i]->clone());
        }
    }

    newLoadout->calculateLoadoutWeight();
    return newLoadout;
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
			memdelete(old_hp);
		}
	}

	this->hardPoints.set(index, hardpoint);
}

HardPoint *godot::Loadout::calculateBestWeaponToEngageWith(Vehicle *target) {
	return nullptr;
}

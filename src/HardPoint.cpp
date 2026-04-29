#include "HardPoint.h"
#include "Weapon.h"

namespace godot {
void HardPoint::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_weapon_option_names"), &HardPoint::getWeaponOptionNames);
	ClassDB::bind_method(D_METHOD("add_weapon_option", "weapon"), &HardPoint::addWeaponOption);
	ClassDB::bind_method(D_METHOD("get_equipped_weapon"), &HardPoint::getEquippedWeapon);
	ClassDB::bind_method(D_METHOD("set_equipped_weapon", "weapon"), &HardPoint::setEquippedWeapon);

	// Adding a property for the editor/GDScript
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "equipped_weapon", PROPERTY_HINT_RESOURCE_TYPE, "Weapon"), "set_equipped_weapon", "get_equipped_weapon");
}

HardPoint::HardPoint() {
	this->equippedWeapon = nullptr;
	this->options = Vector<Weapon *>();
}

HardPoint::~HardPoint() {
	if (equippedWeapon)
		memdelete(equippedWeapon);

	for (int i = 0; i < options.size(); i++) {
		if (options[i])
			memdelete(options[i]);
	}
	options.clear();
}

PackedStringArray HardPoint::getWeaponOptionNames() {
	PackedStringArray optionNames;

	for (size_t i = 0; i < this->options.size(); i++) {
		Weapon *selectedOption = this->options[i];
		if (selectedOption) {
			optionNames.append(selectedOption->getName() + "x" + selectedOption->getCapacity());
		}
	}

	return optionNames;
}

void HardPoint::addWeaponOption(Weapon *option) {
	this->options.append(option);
}

Weapon *HardPoint::getEquippedWeapon() {
	return this->equippedWeapon;
}

void HardPoint::setEquippedWeapon(Weapon *selectedWeapon) {
	if (this->equippedWeapon != nullptr) {
		memdelete(this->equippedWeapon);
		this->equippedWeapon = nullptr;
	}

	if (selectedWeapon != nullptr) {
		this->equippedWeapon = selectedWeapon->clone();
	} else {
		this->equippedWeapon = nullptr;
	}
}

HardPoint *HardPoint::clone() {
	HardPoint *newHp = memnew(HardPoint);

	if (this->equippedWeapon != nullptr) {
		newHp->setEquippedWeapon(this->equippedWeapon->clone());
	}

	for (int i = 0; i < this->options.size(); i++) {
		Weapon *currentOption = this->options[i];
		if (currentOption != nullptr) {
			newHp->addWeaponOption(currentOption->clone());
		}
	}

	return newHp;
}
} //namespace godot
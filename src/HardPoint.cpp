#include "HardPoint.h"

HardPoint::HardPoint() {
}

HardPoint::~HardPoint() {
}

Vector<String> HardPoint::getWeaponOptionNames() {
	Vector<String> optionNames = Vector<String>();

	for (size_t i = 0; i < this->options.size(); i++)
	{
		Weapon* selectedOption = this->options[i];
		if (selectedOption){
			optionNames.append(selectedOption->getName());
		}
	}
	
	return optionNames;
}

Weapon *HardPoint::getEquippedWeapon() {
	return this->equippedWapon;
}

void HardPoint::setEquippedWeapon(Weapon * selectedWeapon) {
	this->equippedWapon = selectedWeapon;
}

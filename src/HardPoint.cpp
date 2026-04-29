#include "HardPoint.h"

HardPoint::HardPoint() {
	this->equippedWeapon = nullptr;
	this->options = Vector<Weapon*>();
}

HardPoint::~HardPoint() {
    if (equippedWeapon) memdelete(equippedWeapon);
    
    for (int i = 0; i < options.size(); i++) {
        if (options[i]) memdelete(options[i]);
    }
	options.clear();
}

Vector<String> HardPoint::getWeaponOptionNames() {
	Vector<String> optionNames = Vector<String>();

	for (size_t i = 0; i < this->options.size(); i++)
	{
		Weapon* selectedOption = this->options[i];
		if (selectedOption){
			optionNames.append(selectedOption->getName() + "x" + selectedOption->getCapacity());
		}
	}
	
	return optionNames;
}

void HardPoint::addWeaponOption(Weapon* option) {
	this->options.append(option);
}

Weapon *HardPoint::getEquippedWeapon() {
	return this->equippedWeapon;
}

void HardPoint::setEquippedWeapon(Weapon * selectedWeapon) {
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
#pragma once
#include "Weapon.h"
#include "activeRadar.h"
#include "Loadout.h"

#define MISSILE_MAX_ALT 30


class objectCreationList{
public:
    static void registry_setup();

    // missiles
    static void AIM_54_A();

    // Planes
    static void F14B();

};
#include "objectCreationList.h"
#include "Globals.h"
#include "Flyable.h"


void objectCreationList::registry_setup() {
    objectCreationList::AIM_54_A();

    // planes
    objectCreationList::F14B();
}

void objectCreationList::AIM_54_A() {
	String id = "AIM_54_A";
    String name = "AIM-54A";

    Radar* AIM_54_A_Radar = memnew(activeRadar);
    Flyable* AIM_54_A_Missile = memnew(Flyable);
    Weapon* AIM_54_A_Weapon = memnew(Weapon);


    AIM_54_A_Radar->set_scan_chunk_size(60);
    AIM_54_A_Radar->set_search_area(60);
    AIM_54_A_Radar->set_search_elevation(60);
    AIM_54_A_Radar->set_maximum_range(25);
    AIM_54_A_Radar->set_precision_factor(30);
    AIM_54_A_Radar->set_offensive(true);
    AIM_54_A_Radar->set_name(name + " Radar");
    AIM_54_A_Radar->set_id(id + "_RADAR");
    GlobalManager::register_radar_into_registry(AIM_54_A_Radar);

    AIM_54_A_Missile->set_weight(443);
    AIM_54_A_Missile->set_fuel_time(30);
    AIM_54_A_Missile->set_fuel_weight_per_L(5.66);
    AIM_54_A_Missile->set_fuel_flow_rate_L(1);
    AIM_54_A_Missile->set_engine_thrust_output(14350);
    AIM_54_A_Missile->set_PID_prop_term(0.0061);
    AIM_54_A_Missile->set_PID_int_term(0.0445);
    AIM_54_A_Missile->set_PID_int_term_limit(1);
    AIM_54_A_Missile->set_PID_der_term(0.00025);
    AIM_54_A_Missile->set_target_elevation(2.25);
    AIM_54_A_Missile->set_turn_rate(20);
    AIM_54_A_Missile->set_optimal_turn_speed(800);
    AIM_54_A_Missile->set_lift_coefficent(20);
    AIM_54_A_Missile->set_max_speed(1800);
    AIM_54_A_Missile->set_radar_cross_section(8);
    AIM_54_A_Missile->set_ir_signature(45);
    AIM_54_A_Missile->set_name(name);
    AIM_54_A_Missile->set_id(id);
    AIM_54_A_Missile->set_radar(AIM_54_A_Radar);
    AIM_54_A_Missile->set_proximity_fuze_radius(20);
    AIM_54_A_Missile->set_maximum_altitude(MISSILE_MAX_ALT);
    GlobalManager::register_vehicle_into_registry(AIM_54_A_Missile);

    AIM_54_A_Weapon->setName(name);
    AIM_54_A_Weapon->setID(id);
    AIM_54_A_Weapon->setDropWeight(443);
    GlobalManager::register_weapon_into_registry(AIM_54_A_Weapon);
}

void objectCreationList::F14B() {
    String id = "F_14_B";
    String name = "F-14B";

    Radar* F14B_Radar = memnew(SearchRadar);
    Flyable* F14B_Plane = memnew(Flyable);
    Loadout* F14B_Loadout = memnew(Loadout);

    

    Weapon* aim54A_1 = GlobalManager::get_weapon_from_id("AIM_54_A");
    aim54A_1->setCapacity(1);
    Weapon* aim54A_2 = GlobalManager::get_weapon_from_id("AIM_54_A");
    aim54A_2->setCapacity(2);
    // hard point 2
    HardPoint* hardpoint2 = memnew(HardPoint);
    hardpoint2->addWeaponOption(aim54A_1);
    // hard point 4
    HardPoint* hardpoint4 = memnew(HardPoint);
    hardpoint4->addWeaponOption(aim54A_2);
    // hard point 6
    HardPoint* hardpoint6 = memnew(HardPoint);
    hardpoint6->addWeaponOption(aim54A_2);
    // hard point 8
    HardPoint* hardpoint8 = memnew(HardPoint);
    hardpoint8->addWeaponOption(aim54A_1);

    F14B_Loadout->setWeaponArraySize(9);
    F14B_Loadout->setHardpointAtIndex(2, hardpoint2);
    F14B_Loadout->setHardpointAtIndex(4, hardpoint4);
    F14B_Loadout->setHardpointAtIndex(6, hardpoint6);
    F14B_Loadout->setHardpointAtIndex(8, hardpoint8);


    F14B_Radar->set_scan_chunk_size(10);
    F14B_Radar->set_search_area(65);
    F14B_Radar->set_search_elevation(55);
    F14B_Radar->set_maximum_range(185);
    F14B_Radar->set_precision_factor(40);
    F14B_Radar->set_offensive(false);
    F14B_Radar->set_name("AN/AWG-9");
    F14B_Radar->set_id("AN_AWG_9");
    GlobalManager::register_radar_into_registry(F14B_Radar);

    
    F14B_Plane->set_despawn_if_cant_hit(false);
    F14B_Plane->set_weight(43210);
    F14B_Plane->set_fuel_time(3180);
    F14B_Plane->set_fuel_weight_per_L(5.09);
    F14B_Plane->set_fuel_flow_rate_L(1);
    F14B_Plane->set_engine_thrust_output(12400);
    F14B_Plane->set_PID_prop_term(0.008);
    F14B_Plane->set_PID_int_term(0.01);
    F14B_Plane->set_PID_int_term_limit(1);
    F14B_Plane->set_PID_der_term(0.001);
    F14B_Plane->set_target_elevation(0);
    F14B_Plane->set_turn_rate(15);
    F14B_Plane->set_optimal_turn_speed(781);
    F14B_Plane->set_lift_coefficent(1.8);
    F14B_Plane->set_max_speed(2215);
    F14B_Plane->set_radar_cross_section(65);
    F14B_Plane->set_ir_signature(70);
    F14B_Plane->set_name(name);
    F14B_Plane->set_id(id);
    F14B_Plane->set_radar(F14B_Radar);
    F14B_Plane->set_loadout(F14B_Loadout);
    F14B_Plane->set_proximity_fuze_radius(0);
    F14B_Plane->set_maximum_altitude(17);
    GlobalManager::register_vehicle_into_registry(F14B_Plane);
}

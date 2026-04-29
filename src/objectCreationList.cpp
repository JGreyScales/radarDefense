#include "objectCreationList.h"
#include "Globals.h"


void objectCreationList::AIM_54_A() {
    String id = "AIM_54_A";
    String name = "AIM-54A";

    Radar* AIM_54_A_Radar = memnew(activeRadar);
    Flyable* AIM_54_A_Missile = memnew(Flyable);
    Weapon* AIM_54_A_Weapon = memnew(Weapon);


    AIM_54_A_Radar->set_scan_chunk_size(60);
    AIM_54_A_Radar->set_search_area(60);
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
    GlobalManager::register_vehicle_into_registry(AIM_54_A_Missile);

    AIM_54_A_Weapon->setName(name);
    AIM_54_A_Weapon->setID(id);
    AIM_54_A_Weapon->setDropWeight(443);
    GlobalManager::register_weapon_into_registry(AIM_54_A_Weapon);
}
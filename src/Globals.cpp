#include "Globals.h"

#include <godot_cpp/variant/utility_functions.hpp>


using namespace godot;
GlobalManager* GlobalManager::singleton = nullptr;


void godot::GlobalManager::register_vehicle_into_registry(Vehicle *vehicle) {
    String ID = vehicle->get_id();

    UtilityFunctions::print("Adding: " + ID + " to the registry");
    if (vehicleRegistry.has(ID)){
        UtilityFunctions::print("aborting... already in the registry");
    } else {
        GlobalManager::vehicleRegistry[ID] = vehicle;
    }
}

void GlobalManager::_bind_methods() {
    ClassDB::bind_static_method("GlobalManager", D_METHOD("get_object_from_id", "id"), &GlobalManager::get_object_from_id);

    ClassDB::bind_static_method("GlobalManager", D_METHOD("get_singleton"), &GlobalManager::get_singleton);
}

GlobalManager *godot::GlobalManager::get_singleton() {
	return GlobalManager::singleton;
}

godot::GlobalManager::GlobalManager() {
    GlobalManager::singleton = this;

    // this is where vehicles will be created
}

godot::GlobalManager::~GlobalManager() {
    for (const KeyValue<String, Vehicle*> &E : vehicleRegistry) {
        Vehicle* v = E.value;
        if (v) {
            memdelete(v);
        }
    }
    vehicleRegistry.clear();

    // we are the singleton, we are being deleted as soon as this line finishes
    GlobalManager::singleton = nullptr;
}

Vehicle *godot::GlobalManager::get_object_from_id(String id) {
	return GlobalManager::vehicleRegistry[id]->clone();
}
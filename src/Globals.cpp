#include "Globals.h"

#include <godot_cpp/variant/utility_functions.hpp>
Vector<Vehicle*> GlobalManager::activeVehicles;

Radar* createDemoRadar(){
    SearchRadar* demoRadar = memnew(SearchRadar);

    demoRadar->set_name("demo radar");
    demoRadar->set_id("demo1_radar");
    demoRadar->set_offensive(false);
    demoRadar->set_has_at_least_one_viable_target(false);
    demoRadar->set_precision_factor(50);
    demoRadar->set_maximum_range(500);
    demoRadar->set_search_area(180);
    demoRadar->set_search_elevation(90);
    demoRadar->set_scan_chunk_size(5);

    return demoRadar;
}

Radar* createDemoFrontRadar(){
    SearchRadar* demoRadar = memnew(SearchRadar);

    demoRadar->set_name("demo front radar");
    demoRadar->set_id("demo1_front_radar");
    demoRadar->set_offensive(false);
    demoRadar->set_has_at_least_one_viable_target(false);
    demoRadar->set_precision_factor(80);
    demoRadar->set_maximum_range(500);
    demoRadar->set_search_area(60);
    demoRadar->set_search_elevation(90);
    demoRadar->set_scan_chunk_size(5);

    return demoRadar;
}

Driveable* createDemoRadarTruck(){
    Driveable* demoTruck = memnew(Driveable);

    // driveable
    demoTruck->set_name("Demo Truck");
    demoTruck->set_path("");
    demoTruck->set_id("demo_radar_truck");
    demoTruck->set_radar(GlobalManager::get_radar_from_id("demo1_radar"));
    demoTruck->set_max_speed(40);
    demoTruck->set_radar_cross_section(90);
    demoTruck->set_ir_signature(90);
    demoTruck->set_line_of_sight_detection(20);
    demoTruck->set_rwr_detection(0);

    return demoTruck;
}

Driveable* createDemoFrontRadarTruck(){
    Driveable* demoTruck = memnew(Driveable);

    // driveable
    demoTruck->set_name("Demo Front Truck");
    demoTruck->set_path("");
    demoTruck->set_id("demo_front_radar_truck");
    demoTruck->set_radar(GlobalManager::get_radar_from_id("demo1_front_radar"));
    demoTruck->set_max_speed(40);
    demoTruck->set_radar_cross_section(60);
    demoTruck->set_ir_signature(90);
    demoTruck->set_line_of_sight_detection(20);
    demoTruck->set_rwr_detection(0);

    return demoTruck;
}


using namespace godot;

void GlobalManager::_physics_process(double delta) {
    const Vector<Vehicle*>& vehicles = GlobalManager::get_active_vehicles();

    for (Vehicle* v : vehicles) {
        if (!v) continue;

        v->tick(delta);
    }
}

void GlobalManager::_notification(int p_what) {
    if (p_what == Node::NOTIFICATION_READY) {
        setup_initial_scene();
    }
}

void GlobalManager::setup_initial_scene() {
    UtilityFunctions::print("Creating registries....");

    // this is where vehicles will be created

    // radars
    this->register_radar_into_registry(createDemoRadar());
    this->register_radar_into_registry(createDemoFrontRadar());

    // driveables
    this->register_vehicle_into_registry(createDemoRadarTruck());
    this->register_vehicle_into_registry(createDemoFrontRadarTruck());

    UtilityFunctions::print("GlobalManager: Scene is ready. Spawning units...");

    Vehicle* demoVehicle = GlobalManager::get_vehicle_from_id("demo_radar_truck");
    demoVehicle->set_x(50);
    demoVehicle->set_y(50);
    this->add_child(demoVehicle);

    Vehicle* demoVehiclex = GlobalManager::get_vehicle_from_id("demo_front_radar_truck");
    demoVehiclex->set_x(200);
    demoVehiclex->set_y(200);
    this->add_child(demoVehiclex);
    UtilityFunctions::print("Vehicle spawned and added to the scene!");
}

GlobalManager* GlobalManager::singleton = nullptr;


void godot::GlobalManager::register_vehicle_into_registry(Vehicle *vehicle) {
    String ID = vehicle->get_id();

    UtilityFunctions::print("Adding: " + ID + " to the registry");
    if (GlobalManager::vehicleRegistry.has(ID)){
        UtilityFunctions::print("aborting... already in the registry");
    } else {
        GlobalManager::vehicleRegistry[ID] = vehicle;
    }
}

void godot::GlobalManager::register_radar_into_registry(Radar *radar) {
    String ID = radar->get_id();

    UtilityFunctions::print("Adding: " + ID + " to the registry");
    if (GlobalManager::radarRegistry.has(ID)){
        UtilityFunctions::print("aborting... already in the registry");
    } else {
        GlobalManager::radarRegistry[ID] = radar;
    }
}

void GlobalManager::_bind_methods() {
    ClassDB::bind_static_method("GlobalManager", D_METHOD("get_vehicle_from_id", "id"), &GlobalManager::get_vehicle_from_id);
    ClassDB::bind_static_method("GlobalManager", D_METHOD("get_radar_from_id", "id"), &GlobalManager::get_radar_from_id);

    ClassDB::bind_static_method("GlobalManager", D_METHOD("get_singleton"), &GlobalManager::get_singleton);
}

GlobalManager *godot::GlobalManager::get_singleton() {
	return GlobalManager::singleton;
}

godot::GlobalManager::GlobalManager() {
    GlobalManager::singleton = this;
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

Vector<Vehicle *> godot::GlobalManager::get_active_vehicles() {
	return GlobalManager::activeVehicles;
}

Vehicle *godot::GlobalManager::get_vehicle_from_id(String id) {
	if (!vehicleRegistry.has(id)) {
        UtilityFunctions::print("Error: Vehicle ID not found: ", id);
        return nullptr;
    }
    Vehicle* original = vehicleRegistry[id];
    return original ? original->clone() : nullptr;
}

Radar *godot::GlobalManager::get_radar_from_id(String id) {
    if (!radarRegistry.has(id)){
        UtilityFunctions::print("Error: Radar ID not found: ", id);
        return nullptr;
    }

	Radar* original = radarRegistry[id];
    return original ? original->clone() : nullptr;
}

void godot::GlobalManager::register_active_vehicle(Vehicle *p_vehicle) {
    GlobalManager::activeVehicles.push_back(p_vehicle);
}

void godot::GlobalManager::unregister_active_vehicle(Vehicle *p_vehicle) {
    GlobalManager::activeVehicles.erase(p_vehicle);
}


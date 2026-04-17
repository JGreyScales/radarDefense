#ifndef GLOBAL_MANAGER_HPP
#define GLOBAL_MANAGER_HPP

#include "Driveable.h"
#include "SearchRadar.h"

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/templates/hash_map.hpp>

namespace godot {

class GlobalManager : public Node2D {
    GDCLASS(GlobalManager, Node2D)

private:
    static GlobalManager *singleton;
    static HashMap<String, Vehicle*> vehicleRegistry;
    static HashMap<String, Radar*> radarRegistry;

    static Vector<Vehicle*> activeVehicles;

    void register_vehicle_into_registry(Vehicle* vehicle);
    void register_radar_into_registry(Radar* vehicle);
protected:
    static void _bind_methods();

public:
    void GlobalManager::_physics_process(double delta);
    void GlobalManager::_notification(int p_what);
    void GlobalManager::setup_initial_scene();
    static GlobalManager *get_singleton();

    GlobalManager();
    ~GlobalManager();

    static Vector<Vehicle*> get_active_vehicles();
    static Vehicle* get_vehicle_from_id(String id);
    static Radar* get_radar_from_id(String id);

    static void register_active_vehicle(Vehicle* p_vehicle);
    static void unregister_active_vehicle(Vehicle* p_vehicle);
};

}

#endif
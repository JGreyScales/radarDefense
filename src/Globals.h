#ifndef GLOBAL_MANAGER_HPP
#define GLOBAL_MANAGER_HPP

#include "Vehicle.h"

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/templates/hash_map.hpp>

namespace godot {

class GlobalManager : public Object {
    GDCLASS(GlobalManager, Object)

private:
    static GlobalManager *singleton;
    static HashMap<String, Vehicle*> vehicleRegistry;

    void register_vehicle_into_registry(Vehicle* vehicle);
protected:
    static void _bind_methods();

public:
    static GlobalManager *get_singleton();

    GlobalManager();
    ~GlobalManager();

    static Vehicle* get_object_from_id(String id);
};

}

#endif
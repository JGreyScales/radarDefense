#ifndef GLOBAL_MANAGER_HPP
#define GLOBAL_MANAGER_HPP

#include "Driveable.h"
#include "SearchRadar.h"

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/templates/vector.hpp>

namespace godot {

class GlobalManager : public Node2D {
    GDCLASS(GlobalManager, Node2D)

private:
    // UI systems
    static Control* popupUI;
    static Label* nameTitle;
    static Label* nameValue;
    static Label* xyzTitle;
    static Label* xyzValue;

    static GlobalManager *singleton;
    static HashMap<String, Vehicle*> vehicleRegistry;
    static HashMap<String, Radar*> radarRegistry;
    static Vector<Vehicle*> activeVehicles;
    static Vehicle* selected_vehicle;

    void register_vehicle_into_registry(Vehicle* vehicle);
    void register_radar_into_registry(Radar* radar);
    static bool should_ui_element_be_visible(String text);

protected:
    static void _bind_methods();

public:
    GlobalManager();
    ~GlobalManager();

    void _physics_process(double delta) override;
    void _process(double delta) override;
    void _input(const Ref<InputEvent> &event) override;
    void _notification(int p_what);

    void setup_initial_scene();
    static GlobalManager *get_singleton();

    static Vector<Vehicle*> get_active_vehicles();
    static Vehicle* get_vehicle_from_id(String id);
    static Radar* get_radar_from_id(String id);

    static void register_active_vehicle(Vehicle* p_vehicle);
    static void unregister_active_vehicle(Vehicle* p_vehicle);

    static void set_selected_vehicle(Vehicle* p_vehicle);
    static Vehicle* get_selected_vehicle() { return selected_vehicle; }

    static void set_ui_name_value(String name);
    static void set_ui_xyz_value(String xyz);
    static void resize_ui();
};

}

#endif
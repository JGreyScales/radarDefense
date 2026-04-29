#pragma once
#include "Driveable.h"
#include "SearchRadar.h"
#include "targetListButton.h"
#include "objectCreationList.h"

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/classes/grid_container.hpp>
#include <godot_cpp/classes/button_group.hpp>


#define METERS_TO_GODOT_UNIT 1.0f
#define KM_TO_GODOT_UNIT     1000.0f

#define GODOT_UNIT_TO_KM     0.001f

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
    static GridContainer* targetList;

    static Ref<ButtonGroup> targetListGroup;

    static GlobalManager *singleton;
    static HashMap<String, Vehicle*> vehicleRegistry;
    static HashMap<String, Radar*> radarRegistry;
    static HashMap<String, Weapon*> weaponRegistry;
    static Vector<Vehicle*> activeVehicles;
    static Vehicle* selected_vehicle;
    static Vehicle* selectedTargetVehicle;


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
    void _on_vehicle_selected(Vehicle* p_vehicle_obj);

    void setup_initial_scene();
    static GlobalManager *get_singleton();

    static Vector<Vehicle*> get_active_vehicles();
    static Vehicle* get_vehicle_from_id(String id);
    static Radar* get_radar_from_id(String id);
    static Weapon* get_weapon_from_id(String id);
    
    static void register_vehicle_into_registry(Vehicle* vehicle);
    static void register_radar_into_registry(Radar* radar);
    static void register_weapon_into_registry(Weapon* weapon);

    static void register_active_vehicle(Vehicle* p_vehicle);
    static void unregister_active_vehicle(Vehicle* p_vehicle);

    static void set_selected_vehicle(Vehicle* p_vehicle);
    static Vehicle* get_selected_vehicle() { return selected_vehicle; }
    static Vehicle* get_selected_target_vehicle() { return selectedTargetVehicle; }

    static void set_ui_name_value(String name);
    static void set_ui_xyz_value(String xyz);
    static void add_target_to_list(Vehicle* target);
    static void clear_target_list();
    static void remove_target_from_list(Vehicle *target);
    static void resize_ui();
};

}
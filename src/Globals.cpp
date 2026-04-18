#include "Globals.h"

#include <godot_cpp/variant/utility_functions.hpp>
Vector<Vehicle *> GlobalManager::activeVehicles;
godot::Vehicle *godot::GlobalManager::selected_vehicle = nullptr;

godot::Control *godot::GlobalManager::popupUI = nullptr;
godot::Label *godot::GlobalManager::nameTitle = nullptr;
godot::Label *godot::GlobalManager::nameValue = nullptr;
godot::Label *godot::GlobalManager::xyzTitle = nullptr;
godot::Label *godot::GlobalManager::xyzValue = nullptr;

Radar *createDemoRadar() {
	SearchRadar *demoRadar = memnew(SearchRadar);

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

Radar *createDemoFrontRadar() {
	SearchRadar *demoRadar = memnew(SearchRadar);

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

Driveable *createDemoRadarTruck() {
	Driveable *demoTruck = memnew(Driveable);

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

Driveable *createDemoFrontRadarTruck() {
	Driveable *demoTruck = memnew(Driveable);

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
	const Vector<Vehicle *> &vehicles = GlobalManager::get_active_vehicles();

	for (Vehicle *v : vehicles) {
		if (!v)
			continue;

		v->tick(delta);
	}
}

void godot::GlobalManager::_process(double delta) {
	if (this->selected_vehicle) {
		this->selected_vehicle->UItick(delta);
	}
}

void godot::GlobalManager::_input(const Ref<InputEvent> &event) {
    Ref<InputEventMouseButton> mouse_event = event;
    Vector2 local_click_pos = get_local_mouse_position();

    if (mouse_event.is_valid() && mouse_event->is_pressed() && 
        mouse_event->get_button_index() == MOUSE_BUTTON_RIGHT) {
        if (GlobalManager::selected_vehicle != nullptr) {
            MapIcon *wayPoint = memnew(MapIcon);
            
			wayPoint->set_x(local_click_pos.x);
			wayPoint->set_y(local_click_pos.y);

			this->selected_vehicle->set_move_waypoint(wayPoint);
			this->add_child(wayPoint);

            UtilityFunctions::print("Created MapIcon at: ", local_click_pos, " for vehicle: ", selected_vehicle->get_name());
        }
    }
}

void GlobalManager::_notification(int p_what) {
	if (p_what == Node::NOTIFICATION_READY) {
		ResourceLoader *rl = ResourceLoader::get_singleton();
		Ref<PackedScene> scene_res = rl->load("res://ui/VehicleInfoPopup.tscn");

		if (scene_res.is_valid()) {
			Node *instance = scene_res->instantiate();
			this->popupUI = Object::cast_to<Control>(instance);

			if (this->popupUI) {
				CanvasLayer *layer = memnew(CanvasLayer);
				layer->set_layer(100);
				add_child(layer);
				layer->add_child(this->popupUI);
				this->popupUI->set_visible(false);
				this->popupUI->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);

				this->popupUI->set_anchors_preset(Control::PRESET_TOP_RIGHT);

				this->popupUI->set_offset(Side::SIDE_LEFT, -170);
				this->popupUI->set_offset(Side::SIDE_RIGHT, -10);
				this->popupUI->set_offset(Side::SIDE_TOP, -2);

				this->nameTitle = Object::cast_to<Label>(this->popupUI->get_node_or_null("PanelContainer/MarginContainer/GridContainer/nameTitle"));
				this->nameValue = Object::cast_to<Label>(this->popupUI->get_node_or_null("PanelContainer/MarginContainer/GridContainer/nameValue"));
				this->xyzTitle  = Object::cast_to<Label>(this->popupUI->get_node_or_null("PanelContainer/MarginContainer/GridContainer/xyzTitle"));
				this->xyzValue  = Object::cast_to<Label>(this->popupUI->get_node_or_null("PanelContainer/MarginContainer/GridContainer/xyzValue"));

				// Debug check to ensure your paths are right
				if (!this->nameTitle) {
					UtilityFunctions::print("ERROR: Could not find UI Labels! Check your NodePaths.");
				}
			}
		}
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

	Vehicle *demoVehicle = GlobalManager::get_vehicle_from_id("demo_radar_truck");
	demoVehicle->set_x(50);
	demoVehicle->set_y(50);
	this->add_child(demoVehicle);

	Vehicle *demoVehiclex = GlobalManager::get_vehicle_from_id("demo_front_radar_truck");
	demoVehiclex->set_x(200);
	demoVehiclex->set_y(200);
	this->add_child(demoVehiclex);
	UtilityFunctions::print("Vehicle spawned and added to the scene!");
}

GlobalManager *GlobalManager::singleton = nullptr;

void godot::GlobalManager::register_vehicle_into_registry(Vehicle *vehicle) {
	String ID = vehicle->get_id();

	UtilityFunctions::print("Adding: " + ID + " to the registry");
	if (GlobalManager::vehicleRegistry.has(ID)) {
		UtilityFunctions::print("aborting... already in the registry");
	} else {
		GlobalManager::vehicleRegistry[ID] = vehicle;
	}
}

void godot::GlobalManager::register_radar_into_registry(Radar *radar) {
	String ID = radar->get_id();

	UtilityFunctions::print("Adding: " + ID + " to the registry");
	if (GlobalManager::radarRegistry.has(ID)) {
		UtilityFunctions::print("aborting... already in the registry");
	} else {
		GlobalManager::radarRegistry[ID] = radar;
	}
}

bool godot::GlobalManager::should_ui_element_be_visible(String text) {
	return text.is_empty();
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
	GlobalManager::selected_vehicle = nullptr;
}

godot::GlobalManager::~GlobalManager() {
	for (const KeyValue<String, Vehicle *> &E : vehicleRegistry) {
		Vehicle *v = E.value;
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
	Vehicle *original = vehicleRegistry[id];
	return original ? original->clone() : nullptr;
}

Radar *godot::GlobalManager::get_radar_from_id(String id) {
	if (!radarRegistry.has(id)) {
		UtilityFunctions::print("Error: Radar ID not found: ", id);
		return nullptr;
	}

	Radar *original = radarRegistry[id];
	return original ? original->clone() : nullptr;
}

void godot::GlobalManager::register_active_vehicle(Vehicle *p_vehicle) {
	GlobalManager::activeVehicles.push_back(p_vehicle);
}

void godot::GlobalManager::unregister_active_vehicle(Vehicle *p_vehicle) {
	GlobalManager::activeVehicles.erase(p_vehicle);
}

void godot::GlobalManager::set_selected_vehicle(Vehicle *p_vehicle) {
	if (selected_vehicle == p_vehicle) {
		selected_vehicle->deselect();
		selected_vehicle = nullptr;
		return;
	}

	if (selected_vehicle != nullptr) {
		selected_vehicle->deselect();
	}

	selected_vehicle = p_vehicle;

	if (selected_vehicle != nullptr) {
		selected_vehicle->select();
		if (GlobalManager::popupUI) {
			UtilityFunctions::print(p_vehicle->get_name());

			GlobalManager::popupUI->show();
			selected_vehicle->UItick(0);
		}
	} else {
		if (GlobalManager::popupUI) {
			GlobalManager::popupUI->hide();
		}
	}
}

void godot::GlobalManager::set_ui_name_value(String name) {
    if (!GlobalManager::nameTitle || !GlobalManager::nameValue) return;

    if (name.is_empty()) {
        GlobalManager::nameTitle->set_visible(false);
        GlobalManager::nameValue->set_visible(false);
    } else {
        GlobalManager::nameTitle->set_visible(true);
        GlobalManager::nameValue->set_visible(true);
        GlobalManager::nameValue->set_text(name);
    }

	GlobalManager::resize_ui();
}

void godot::GlobalManager::set_ui_xyz_value(String xyz) {
    if (!GlobalManager::xyzTitle || !GlobalManager::xyzValue) return;

    if (xyz.is_empty()) {
        GlobalManager::xyzTitle->set_visible(false);
        GlobalManager::xyzValue->set_visible(false);
    } else {
        GlobalManager::xyzTitle->set_visible(true);
        GlobalManager::xyzValue->set_visible(true);
        GlobalManager::xyzValue->set_text(xyz);
    }

	GlobalManager::resize_ui();
}

void godot::GlobalManager::resize_ui() {
    if (!GlobalManager::popupUI) return;
    Control *panel = Object::cast_to<Control>(GlobalManager::popupUI->get_node_or_null("PanelContainer"));
    
    if (panel) {
        panel->reset_size();
        Vector2 min_size = panel->get_combined_minimum_size();
        
        if (min_size.x > 0) {
            GlobalManager::popupUI->set_offset(Side::SIDE_LEFT, -(min_size.x) - 1);
            GlobalManager::popupUI->set_size(min_size);
        }
    }
}

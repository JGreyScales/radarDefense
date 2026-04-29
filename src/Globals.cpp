#include "Globals.h"
#include "Weapon.h"

#include <godot_cpp/variant/utility_functions.hpp>
Vector<Vehicle *> GlobalManager::activeVehicles;
godot::Vehicle *godot::GlobalManager::selected_vehicle = nullptr;
Vehicle* GlobalManager::selectedTargetVehicle = nullptr;
Ref<ButtonGroup> GlobalManager::targetListGroup;

godot::Control *godot::GlobalManager::popupUI = nullptr;
godot::Label *godot::GlobalManager::nameTitle = nullptr;
godot::Label *godot::GlobalManager::nameValue = nullptr;
godot::Label *godot::GlobalManager::xyzTitle = nullptr;
godot::Label *godot::GlobalManager::xyzValue = nullptr;
godot::GridContainer* godot::GlobalManager::targetList = nullptr;


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
		this->targetListGroup.instantiate();
		ResourceLoader *rl = ResourceLoader::get_singleton();
		Ref<PackedScene> scene_res = rl->load("res://UI/VehicleInfoPopup.tscn");

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
				
				
				this->targetList  = Object::cast_to<GridContainer>(this->popupUI->get_node_or_null("PanelContainer/MarginContainer/GridContainer/targetList"));

				if (!this->nameTitle) {
					UtilityFunctions::print("ERROR: Could not find UI Labels! Check your NodePaths.");
				}

				if (!this->targetList) {
					UtilityFunctions::print("ERROR: Could not find gridcontainers! Check your NodePaths.");
				}
			}
		}
		setup_initial_scene();
	}
}

void godot::GlobalManager::_on_vehicle_selected(Vehicle* p_vehicle_obj) {
	Vehicle* v = Object::cast_to<Vehicle>(p_vehicle_obj);
    if (v) {
        UtilityFunctions::print("Signal received! Vehicle selected: ", v->get_name());
		Vehicle* prev = GlobalManager::selectedTargetVehicle;
		GlobalManager::selectedTargetVehicle = p_vehicle_obj;
		p_vehicle_obj->force_redraw();
		if (prev){
			prev->force_redraw();
		}
    }
}

void GlobalManager::setup_initial_scene() {
	UtilityFunctions::print("Creating registries....");

	UtilityFunctions::print("GlobalManager: Scene is ready. Spawning units...");
	objectCreationList::registry_setup();


	// Vehicle *demoVehicle = GlobalManager::get_vehicle_from_id("demo_radar_truck");
	// demoVehicle->set_x(50);
	// demoVehicle->set_y(50);
	// this->add_child(demoVehicle);

	// Vehicle *demoVehiclex = GlobalManager::get_vehicle_from_id("demo_front_radar_truck");
	// demoVehiclex->set_x(200);
	// demoVehiclex->set_y(200);
	// this->add_child(demoVehiclex);

	// Vehicle *demoVehicley = GlobalManager::get_vehicle_from_id("demo_radar_truck");
	// demoVehicley->set_x(50);
	// demoVehicley->set_y(300);
	// this->add_child(demoVehicley);
	// UtilityFunctions::print("Vehicle spawned and added to the scene!");
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

void godot::GlobalManager::register_weapon_into_registry(Weapon *weapon) {
	String ID = weapon->getID();

	UtilityFunctions::print("Adding: " + ID + " to the registry");
	if (GlobalManager::weaponRegistry.has(ID)) {
		UtilityFunctions::print("aborting... already in the registry");
	} else {
		GlobalManager::weaponRegistry[ID] = weapon;
	}
}

bool godot::GlobalManager::should_ui_element_be_visible(String text) {
	return text.is_empty();
}

void GlobalManager::_bind_methods() {
	ClassDB::bind_static_method("GlobalManager", D_METHOD("get_vehicle_from_id", "id"), &GlobalManager::get_vehicle_from_id);
	ClassDB::bind_static_method("GlobalManager", D_METHOD("get_radar_from_id", "id"), &GlobalManager::get_radar_from_id);
	ClassDB::bind_method(D_METHOD("_on_vehicle_selected", "p_vehicle_obj"), &GlobalManager::_on_vehicle_selected);

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

Weapon *godot::GlobalManager::get_weapon_from_id(String id) {
	if (!weaponRegistry.has(id)){
		UtilityFunctions::print("Error: Weapon ID not found: ", id);
		return nullptr;
	}

	return weaponRegistry[id]->clone();
}


void godot::GlobalManager::register_active_vehicle(Vehicle *p_vehicle) {
	GlobalManager::activeVehicles.push_back(p_vehicle);
}

void godot::GlobalManager::unregister_active_vehicle(Vehicle *p_vehicle) {
	GlobalManager::activeVehicles.erase(p_vehicle);
}

void godot::GlobalManager::set_selected_vehicle(Vehicle *p_vehicle) {

	Vehicle* previousTarget = GlobalManager::selectedTargetVehicle;

    if (selected_vehicle == p_vehicle) {
        selected_vehicle->deselect();
        selected_vehicle = nullptr;
        GlobalManager::selectedTargetVehicle = nullptr;
        return;
    }


    if (selected_vehicle != nullptr) {
        selected_vehicle->deselect();
        GlobalManager::clear_target_list();
    }

    selected_vehicle = p_vehicle;

    if (selected_vehicle != nullptr) {
        std::unordered_set<Vehicle*>* targetListObjects = selected_vehicle->get_radar()->get_presence_map();
        
        for (Vehicle* target : *targetListObjects) {
            if (target != nullptr) {
                add_target_to_list(target);
                
                if (target == previousTarget) {
                    GlobalManager::selectedTargetVehicle = target;
                }
            }
        }
        
        selected_vehicle->select();
        if (GlobalManager::popupUI) {
            GlobalManager::popupUI->show();
            selected_vehicle->UItick(0);
        }
    } else {
        if (GlobalManager::popupUI) {
            GlobalManager::popupUI->hide();
        }
    }

	if (previousTarget){
		previousTarget->force_redraw();
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

void godot::GlobalManager::add_target_to_list(Vehicle *target) {

	if (!GlobalManager::targetList) {
        UtilityFunctions::print("ERROR: Cannot add target, targetList is null!");
        return; 
    }

	TargetListButton* btn = memnew(TargetListButton);
	int count = GlobalManager::targetList->get_child_count() + 1;
	String num_str = String::num_int64(count).pad_zeros(3);
	String size_str = "Unknown";

	int rcs = target->get_radar_cross_section();

	if (rcs < 20) {
		size_str = "SMALL-";
	} else if (rcs < 50) {
		size_str = "MEDIUM-";
	} else {
		size_str = "LARGE-";
	}
	
	btn->setup("TRACK-" + size_str + num_str, target, GlobalManager::targetListGroup);
	btn->connect("vehicle_selected", Callable(GlobalManager::get_singleton(), "_on_vehicle_selected"));

	GlobalManager::targetList->add_child(btn);
}

void godot::GlobalManager::clear_target_list() {
    if (GlobalManager::targetList == nullptr) {
        return;
    }

    TypedArray<Node> children = GlobalManager::targetList->get_children();
    
    for (int i = 0; i < children.size(); i++) {
        Node* child = Object::cast_to<Node>(children[i]);
        TargetListButton* btn = Object::cast_to<TargetListButton>(child);

        if (btn) {
            btn->set_button_group(Ref<ButtonGroup>());
        }

        GlobalManager::targetList->remove_child(child);
        child->queue_free();
    }

	Vehicle* previousTarget = GlobalManager::selectedTargetVehicle;
	GlobalManager::selectedTargetVehicle = nullptr;

	if (previousTarget){
		previousTarget->force_redraw();
	}
}

void godot::GlobalManager::remove_target_from_list(Vehicle *target) {
    if (GlobalManager::targetList == nullptr || target == nullptr) {
        return;
    }

    TypedArray<Node> children = GlobalManager::targetList->get_children();

    for (int i = 0; i < children.size(); i++) {
        TargetListButton* btn = Object::cast_to<TargetListButton>(children[i]);
        
        if (btn && btn->get_target_object() == target) {
            
            if (GlobalManager::selectedTargetVehicle == target) {

				Vehicle* previousTarget = GlobalManager::selectedTargetVehicle;

                GlobalManager::selectedTargetVehicle = nullptr;
				previousTarget->force_redraw();
            }

            btn->set_button_group(Ref<ButtonGroup>());

            GlobalManager::targetList->remove_child(btn);
            btn->queue_free();

            break;
        }
    }
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

#include "Globals.h"
#include "Weapon.h"
#include "MyCamera.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/string.hpp>

Vector<Vehicle *> GlobalManager::activeVehicles;
godot::Vehicle *godot::GlobalManager::selected_vehicle = nullptr;
Vehicle* GlobalManager::selectedTargetVehicle = nullptr;
Ref<ButtonGroup> GlobalManager::targetListGroup;

godot::Control *godot::GlobalManager::popupUI = nullptr;
godot::Label *godot::GlobalManager::nameTitle = nullptr;
godot::Label *godot::GlobalManager::nameValue = nullptr;
godot::Label *godot::GlobalManager::xyzTitle = nullptr;
godot::Label *godot::GlobalManager::xyzValue = nullptr;
godot::Label *godot::GlobalManager::weightTitle = nullptr;
godot::Label *godot::GlobalManager::weightValue = nullptr;
godot::Label *godot::GlobalManager::thrustDeltaTitle = nullptr;
godot::Label *godot::GlobalManager::thrustDeltaValue = nullptr;
godot::Label *godot::GlobalManager::fuelTimeTitle = nullptr;
godot::Label *godot::GlobalManager::fuelTimeValue = nullptr;

godot::PanelContainer *godot::GlobalManager::zUnitPopupUI = nullptr;
godot::Label *godot::GlobalManager::altTitle = nullptr;
godot::Label *godot::GlobalManager::AltValue = nullptr;
godot::HSlider *godot::GlobalManager::altSlider = nullptr;





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
		MyCamera* mainCamera = memnew(MyCamera);
		mainCamera->set_name("MainCamera");
		add_child(mainCamera);
		mainCamera->make_current();

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
				this->weightTitle  = Object::cast_to<Label>(this->popupUI->get_node_or_null("PanelContainer/MarginContainer/GridContainer/WeightTitle"));
				this->weightValue  = Object::cast_to<Label>(this->popupUI->get_node_or_null("PanelContainer/MarginContainer/GridContainer/WeightValue"));
				this->thrustDeltaTitle  = Object::cast_to<Label>(this->popupUI->get_node_or_null("PanelContainer/MarginContainer/GridContainer/ThrustDeltaTitle"));
				this->thrustDeltaValue  = Object::cast_to<Label>(this->popupUI->get_node_or_null("PanelContainer/MarginContainer/GridContainer/ThrustDeltaValue"));
				this->fuelTimeTitle  = Object::cast_to<Label>(this->popupUI->get_node_or_null("PanelContainer/MarginContainer/GridContainer/FuelTimeTitle"));
				this->fuelTimeValue = Object::cast_to<Label>(this->popupUI->get_node_or_null("PanelContainer/MarginContainer/GridContainer/FuelTimeValue"));
				
				
				this->targetList  = Object::cast_to<GridContainer>(this->popupUI->get_node_or_null("PanelContainer/MarginContainer/GridContainer/targetList"));

				this->zUnitPopupUI = Object::cast_to<PanelContainer>(this->popupUI->get_node_or_null("z_panel_container"));
				if (this->zUnitPopupUI){
					this->altTitle = Object::cast_to<Label>(this->popupUI->get_node_or_null("z_panel_container/MarginContainer/GridContainer/altitudeTitle"));
					this->AltValue = Object::cast_to<Label>(this->popupUI->get_node_or_null("z_panel_container/MarginContainer/GridContainer/altitudeValue"));
					this->altSlider = Object::cast_to<HSlider>(this->popupUI->get_node_or_null("z_panel_container/MarginContainer/GridContainer/HSlider"));
					this->altSlider->set_value(0.0);

				} else {
					UtilityFunctions::print("ERROR: Could not find zUnit Object");
				}




				if (!this->fuelTimeValue) {
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

	UtilityFunctions::print("Spawning vehicles");

	Vehicle* f14B_1 = GlobalManager::get_vehicle_from_id("F_14_B");
	UtilityFunctions::print("f14b radar is " + f14B_1->get_radar()->get_name());
	f14B_1->set_x(50);
	f14B_1->set_y(50);
	f14B_1->set_z(17);
	f14B_1->set_speed(781);
	this->add_child(f14B_1);
	UtilityFunctions::print("Vehicle spawned and added to the scene!");

}

GlobalManager *GlobalManager::singleton = nullptr;

void godot::GlobalManager::register_vehicle_into_registry(Vehicle *vehicle) {
	String ID = vehicle->get_id();

	UtilityFunctions::print("Adding: " + ID + " to the Vehicle registry");
	if (GlobalManager::vehicleRegistry.has(ID)) {
		UtilityFunctions::print("aborting... already in the registry");
	} else {
		GlobalManager::vehicleRegistry[ID] = vehicle;
	}
	UtilityFunctions::print("DONE");
}

void godot::GlobalManager::register_radar_into_registry(Radar *radar) {
	String ID = radar->get_id();

	UtilityFunctions::print("Adding: " + ID + " to the  Radar registry");
	if (GlobalManager::radarRegistry.has(ID)) {
		UtilityFunctions::print("aborting... already in the registry");
	} else {
		GlobalManager::radarRegistry[ID] = radar;
	}
	UtilityFunctions::print("DONE");
}

void godot::GlobalManager::register_weapon_into_registry(Weapon *weapon) {
	String ID = weapon->getID();

	UtilityFunctions::print("Adding: " + ID + " to the  Weapon registry");
	if (GlobalManager::weaponRegistry.has(ID)) {
		UtilityFunctions::print("aborting... already in the registry");
	} else {
		GlobalManager::weaponRegistry[ID] = weapon;
	}
	UtilityFunctions::print("DONE");
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

	for (const KeyValue<String, Radar *> &R : radarRegistry) {
		Radar *v = R.value;
		if (v) {
			memdelete(v);
		}
	}

	for (const KeyValue<String, Weapon *> &W : weaponRegistry) {
		Weapon *v = W.value;
		if (v) {
			memdelete(v);
		}
	}

	vehicleRegistry.clear();
	weaponRegistry.clear();
	radarRegistry.clear();

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
        
		if (MyCamera* cam = MyCamera::getSingleton()) {
            cam->set_follow_target(selected_vehicle);
        }

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
		if (MyCamera* cam = MyCamera::getSingleton()) {
            cam->set_follow_target(nullptr);
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

void godot::GlobalManager::set_ui_weight_value(String value) {
	if (!GlobalManager::weightTitle || !GlobalManager::weightValue) return;

    if (value.is_empty()) {
        GlobalManager::weightTitle->set_visible(false);
        GlobalManager::weightValue->set_visible(false);
    } else {
        GlobalManager::weightTitle->set_visible(true);
        GlobalManager::weightValue->set_visible(true);
		float val = value.to_float() * 0.0004535924f;
		GlobalManager::weightValue->set_text(String::num(val, 2) + "t");
    }

	GlobalManager::resize_ui();
}

void godot::GlobalManager::set_ui_thrust_delta_value(String value) {
	if (!GlobalManager::thrustDeltaTitle || !GlobalManager::thrustDeltaValue) return;

    if (value.is_empty()) {
        GlobalManager::thrustDeltaTitle->set_visible(false);
        GlobalManager::thrustDeltaValue->set_visible(false);
    } else {
        GlobalManager::thrustDeltaTitle->set_visible(true);
        GlobalManager::thrustDeltaValue->set_visible(true);
		GlobalManager::thrustDeltaValue->set_text(value + "m/s^2");
    }

	GlobalManager::resize_ui();
}

void godot::GlobalManager::set_ui_fuel_time_value(String value) {
	if (!GlobalManager::fuelTimeTitle || !GlobalManager::fuelTimeValue) return;

    if (value.is_empty()) {
        GlobalManager::fuelTimeTitle->set_visible(false);
        GlobalManager::fuelTimeValue->set_visible(false);
    } else {
        GlobalManager::fuelTimeValue->set_visible(true);
        GlobalManager::thrustDeltaValue->set_visible(true);
		GlobalManager::fuelTimeValue->set_text(value + "s");
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

	if (GlobalManager::zUnitPopupUI) {
		UtilityFunctions::print("xxx");
		GlobalManager::zUnitPopupUI->set_anchors_and_offsets_preset(Control::LayoutPreset::PRESET_BOTTOM_LEFT);

		Vector2 z_min_size = GlobalManager::zUnitPopupUI->get_combined_minimum_size();
		
		GlobalManager::zUnitPopupUI->set_begin(Vector2(0, -z_min_size.y));
		GlobalManager::zUnitPopupUI->set_end(Vector2(z_min_size.x, 0));
	}
}

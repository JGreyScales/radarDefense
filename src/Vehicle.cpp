#include "Vehicle.h"
#include "Globals.h"
#include "Radar.h"
#include "Loadout.h"

using namespace godot;

void Vehicle::_notification(int p_what) {
	MapIcon::_notification(p_what);
	GlobalManager *gm = GlobalManager::get_singleton();

	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
			if (gm)
				gm->register_active_vehicle(this);
			break;

		case NOTIFICATION_EXIT_TREE:
			if (gm)
				gm->unregister_active_vehicle(this);
			break;

		case NOTIFICATION_READY:
			if (hitbox == nullptr) {
				hitbox = memnew(Area2D);
				hitbox->set_monitoring(false);
				hitbox->set_monitorable(true);
				hitbox->set_collision_layer(2);
				hitbox->set_pickable(true);
				hitbox->connect("input_event", Callable(this, "_on_hitbox_input_event"));

				add_child(hitbox);

				hitbox_shape = memnew(CollisionShape2D);
				hitbox->add_child(hitbox_shape);

				Ref<RectangleShape2D> rect;
				rect.instantiate();
				rect->set_size(Vector2(20, 20));
				hitbox_shape->set_shape(rect);
			}

			// if (this->currentScaleFactor != 1.0f) {
            //     Vector2 inv_scale = Vector2(1.0f / this->currentScaleFactor, 1.0f / this->currentScaleFactor);
            //     TypedArray<Node> children = get_children();
            //     for (int i = 0; i < children.size(); i++) {
            //         Node2D *child_node = Object::cast_to<Node2D>(children[i]);
            //         if (child_node) {
            //             child_node->set_scale(inv_scale);
            //         }
            //     }
            // }
			break;
	}
}

void godot::Vehicle::_bind_methods() {
	// Basic Identification
	ClassDB::bind_method(D_METHOD("get_id"), &Vehicle::get_id);

	// Navigation & Physics
	ClassDB::bind_method(D_METHOD("get_max_speed"), &Vehicle::get_max_speed);
	ClassDB::bind_method(D_METHOD("set_max_speed", "p_max_speed"), &Vehicle::set_max_speed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_speed"), "set_max_speed", "get_max_speed");

	ClassDB::bind_method(D_METHOD("get_speed"), &Vehicle::get_speed);
	ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &Vehicle::set_speed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "speed"), "set_speed", "get_speed");

	ClassDB::bind_method(D_METHOD("get_direction"), &Vehicle::get_direction);
	ClassDB::bind_method(D_METHOD("set_direction", "p_direction"), &Vehicle::set_direction);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "direction"), "set_direction", "get_direction");

	// Signatures & Electronic Warfare
	ClassDB::bind_method(D_METHOD("get_radar_cross_section"), &Vehicle::get_radar_cross_section);
	ClassDB::bind_method(D_METHOD("set_radar_cross_section", "p_rcs"), &Vehicle::set_radar_cross_section);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "radar_cross_section"), "set_radar_cross_section", "get_radar_cross_section");

	ClassDB::bind_method(D_METHOD("get_ir_signature"), &Vehicle::get_ir_signature);
	ClassDB::bind_method(D_METHOD("set_ir_signature", "p_ir"), &Vehicle::set_ir_signature);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "ir_signature"), "set_ir_signature", "get_ir_signature");

	// Detection Stats
	ClassDB::bind_method(D_METHOD("get_line_of_sight_detection"), &Vehicle::get_line_of_sight_detection);
	ClassDB::bind_method(D_METHOD("set_line_of_sight_detection", "p_los"), &Vehicle::set_line_of_sight_detection);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "line_of_sight_detection"), "set_line_of_sight_detection", "get_line_of_sight_detection");

	// Radar System (Child Node)
	ClassDB::bind_method(D_METHOD("get_radar"), &Vehicle::get_radar);
	ClassDB::bind_method(D_METHOD("set_radar", "radar"), &Vehicle::set_radar);
	// We don't necessarily need an ADD_PROPERTY for the Radar pointer itself
	// unless you want to assign it via the Inspector, but set_radar handles add_child.

	// UI & Tracking Logic
	ClassDB::bind_method(D_METHOD("get_display_content"), &Vehicle::get_display_content);
	ClassDB::bind_method(D_METHOD("add_being_tracked_by", "tracked_by"), &Vehicle::add_being_tracked_by);
	ClassDB::bind_method(D_METHOD("remove_being_tracked_by", "tracked_by"), &Vehicle::remove_being_tracked_by);

	ClassDB::bind_method(D_METHOD("get_move_waypoint"), &Vehicle::get_move_waypoint);
	ClassDB::bind_method(D_METHOD("set_move_waypoint", "target"), &Vehicle::set_move_waypoint);

	ClassDB::bind_method(D_METHOD("get_rwr_detection"), &Vehicle::get_rwr_detection);
	ClassDB::bind_method(D_METHOD("set_rwr_detection", "p_rwr"), &Vehicle::set_rwr_detection);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "rwr_detection"), "set_rwr_detection", "get_rwr_detection");

	// Factory Method
	ClassDB::bind_static_method("Vehicle", D_METHOD("create_prototype", "id", "pilotSkill"), &Vehicle::create_prototype);
	ClassDB::bind_method(D_METHOD("_on_hitbox_input_event", "viewport", "event", "shape_idx"), &Vehicle::_on_hitbox_input_event);
}

Vehicle::Vehicle() {
	this->id = "";

	this->radar = nullptr;
	this->hitbox = nullptr;
	this->hitbox_shape = nullptr;
	this->moveWaypoint = nullptr;
	this->loadout = nullptr;

	this->maxSpeed = 0;
	this->radarCrossSection = 0;
	this->irSignature = 0;
	this->lineOfSightDetection = 0;
	this->rwrDetection = 0;
}

Vehicle::~Vehicle() {

}

void godot::Vehicle::_on_hitbox_input_event(godot::Node *viewport, godot::Ref<godot::InputEvent> event, int32_t shape_idx) {
    Ref<InputEventMouseButton> mouse_event = event;

    if (mouse_event.is_valid() && mouse_event->is_pressed() && mouse_event->get_button_index() == MOUSE_BUTTON_LEFT) {
        
        get_viewport()->set_input_as_handled();

        GlobalManager::get_singleton()->set_selected_vehicle(this);
    }
}

void godot::Vehicle::select() {
	this->isSelected = true;
}

void godot::Vehicle::deselect() {
	this->isSelected = false;
}

Vehicle *Vehicle::create_prototype(String id, uint8_t pilotSkill) {
	return GlobalManager::get_vehicle_from_id(id);
}

String Vehicle::get_display_content() {
	String mapIconContent = MapIcon::get_display_content();
	mapIconContent += "\nAltitude: " + String::num(this->get_z());
	mapIconContent += "\nVelocity: " + String::num(this->get_speed());

	return mapIconContent;
}

bool godot::Vehicle::get_is_selected() {
	return this->isSelected;
}

String godot::Vehicle::get_id() {
	return this->id;
}

Radar *godot::Vehicle::get_radar() {
	return this->radar;
}

uint16_t godot::Vehicle::get_max_speed() {
	return this->maxSpeed;
}




uint8_t godot::Vehicle::get_radar_cross_section() {
	return this->radarCrossSection;
}

uint8_t godot::Vehicle::get_ir_signature() {
	return this->irSignature;
}

uint8_t godot::Vehicle::get_line_of_sight_detection() {
	return this->lineOfSightDetection;
}

uint8_t godot::Vehicle::get_rwr_detection() {
	return this->rwrDetection;
}

Vector<Vehicle *> godot::Vehicle::get_being_tracked_by() {
	return this->beingTrackedBy;
}

MapIcon *godot::Vehicle::get_move_waypoint() {
	return this->moveWaypoint;
}

uint16_t godot::Vehicle::get_weight() {
	return this->weight;
}

Loadout *godot::Vehicle::get_loadout() {
	return this->loadout;
}

void godot::Vehicle::set_id(String id) {
	this->id = id;
}

void godot::Vehicle::set_radar(Radar *radar) {
	this->radar = radar;
	add_child(radar);
}

void godot::Vehicle::set_max_speed(const uint16_t p_max_speed) {
	this->maxSpeed = p_max_speed;
}


void godot::Vehicle::set_radar_cross_section(const uint8_t p_rcs) {
	this->radarCrossSection = p_rcs;
}

void godot::Vehicle::set_ir_signature(const uint8_t p_ir) {
	this->irSignature = p_ir;
}

void godot::Vehicle::set_line_of_sight_detection(const uint8_t p_los) {
	this->lineOfSightDetection = p_los;
}

void godot::Vehicle::set_rwr_detection(const uint8_t p_rwr) {
	this->rwrDetection = p_rwr;
}

void godot::Vehicle::set_being_tracked_by(const Vector<Vehicle *> p_tracked_by) {
	this->beingTrackedBy = p_tracked_by;
}

void godot::Vehicle::add_being_tracked_by(Vehicle *tracked_by) {
	int64_t index = this->beingTrackedBy.find(tracked_by);
	if (index == -1) {
		this->beingTrackedBy.append(tracked_by);
	}
}

void godot::Vehicle::remove_being_tracked_by(Vehicle *tracked_by) {
	// godot has a custom erase
	// it will find the object and erase it
	this->beingTrackedBy.erase(tracked_by);
}

void godot::Vehicle::set_move_waypoint(MapIcon *waypoint) {
	if (this->moveWaypoint != nullptr && this->moveWaypoint != waypoint){
		this->moveWaypoint->queue_free();
	}
	this->moveWaypoint = waypoint;
}

void godot::Vehicle::set_weight(uint16_t value) {
    this->weight = value;
}

void Vehicle::set_loadout(Loadout* p_loadout) {
    if (this->loadout != nullptr) {
        memdelete(this->loadout);
        this->loadout = nullptr;
    }

    if (p_loadout != nullptr) {
        this->loadout = p_loadout->clone();
    }
}
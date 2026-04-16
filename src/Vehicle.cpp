#include "Vehicle.h"

using namespace godot;

void godot::Vehicle::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_id"), &Vehicle::get_id);

    ClassDB::bind_method(D_METHOD("get_max_speed"), &Vehicle::get_max_speed);
    ClassDB::bind_method(D_METHOD("set_max_speed", "p_max_speed"), &Vehicle::set_max_speed);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_speed"), "set_max_speed", "get_max_speed");

    ClassDB::bind_method(D_METHOD("get_speed"), &Vehicle::get_speed);
    ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &Vehicle::set_speed);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "speed"), "set_speed", "get_speed");

    ClassDB::bind_method(D_METHOD("get_direction"), &Vehicle::get_direction);
    ClassDB::bind_method(D_METHOD("set_direction", "p_direction"), &Vehicle::set_direction);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "direction"), "set_direction", "get_direction");

    ClassDB::bind_method(D_METHOD("get_radar_cross_section"), &Vehicle::get_radar_cross_section);
    ClassDB::bind_method(D_METHOD("set_radar_cross_section", "p_rcs"), &Vehicle::set_radar_cross_section);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "radar_cross_section"), "set_radar_cross_section", "get_radar_cross_section");

    ClassDB::bind_method(D_METHOD("get_ir_signature"), &Vehicle::get_ir_signature);
    ClassDB::bind_method(D_METHOD("set_ir_signature", "p_ir"), &Vehicle::set_ir_signature);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "ir_signature"), "set_ir_signature", "get_ir_signature");

    ClassDB::bind_method(D_METHOD("get_line_of_sight_detection"), &Vehicle::get_line_of_sight_detection);
    ClassDB::bind_method(D_METHOD("set_line_of_sight_detection", "p_los"), &Vehicle::set_line_of_sight_detection);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "line_of_sight_detection"), "set_line_of_sight_detection", "get_line_of_sight_detection");

    ClassDB::bind_method(D_METHOD("get_rwr_detection"), &Vehicle::get_rwr_detection);
    ClassDB::bind_method(D_METHOD("set_rwr_detection", "p_rwr"), &Vehicle::set_rwr_detection);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "rwr_detection"), "set_rwr_detection", "get_rwr_detection");

    ClassDB::bind_method(D_METHOD("get_display_content"), &Vehicle::get_display_content);

    ClassDB::bind_method(D_METHOD("add_being_tracked_by", "tracked_by"), &Vehicle::add_being_tracked_by);
    ClassDB::bind_method(D_METHOD("remove_being_tracked_by", "tracked_by"), &Vehicle::remove_being_tracked_by);
    
    ClassDB::bind_static_method("Vehicle", D_METHOD("create_prototype", "id"), &Vehicle::create_prototype);
}

Vehicle::Vehicle() {
	this->maxSpeed = 0;
    this->speed = 0;
    this->direction = 0.0f;
    this->radarCrossSection = 0;
    this->irSignature = 0;
    this->lineOfSightDetection = 0;
    this->rwrDetection = 0;
}

Vehicle::~Vehicle() {
    
}

Vehicle* Vehicle::create_prototype(String id){
    return nullptr;
}


String Vehicle::get_display_content() {
	String mapIconContent = MapIcon::get_display_content();
    mapIconContent + "\nAltitude: " + this->get_z();
    mapIconContent + "\nVelocity: " + this->get_speed();
    return mapIconContent;
}

String godot::Vehicle::get_id() {
	return this->id;
}

uint16_t godot::Vehicle::get_max_speed() {
	return this->maxSpeed;
}

uint16_t godot::Vehicle::get_speed() {
	return this->speed;
}

float godot::Vehicle::get_direction() {
	return this->direction;
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

void godot::Vehicle::set_max_speed(const uint16_t p_max_speed) {
    this->maxSpeed = p_max_speed;
}

void godot::Vehicle::set_speed(const uint16_t p_speed) {
    this->speed = p_speed;
    queue_redraw();
}

void godot::Vehicle::set_direction(const float p_direction) {
    set_rotation_degrees(p_direction);
    this->direction = p_direction;
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

#include "Driveable.h"
#include "Globals.h"
#include "Radar.h"

void Driveable::_bind_methods() {
	ClassDB::bind_method(D_METHOD("tick", "delta_time"), &Driveable::tick);

	ClassDB::bind_method(D_METHOD("move", "target", "delta_time"), &Driveable::move);

	ClassDB::bind_method(D_METHOD("avoid_incoming"), &Driveable::avoid_incoming);
	ClassDB::bind_method(D_METHOD("engage", "target"), &Driveable::engage);

	ClassDB::bind_method(D_METHOD("clone"), &Driveable::clone);
}

Driveable::Driveable() {
}

Driveable::~Driveable() {
}

void Driveable::tick(float deltaTime) {
	Radar *radar = this->get_radar();
	MapIcon *moveWaypoint = this->get_move_waypoint();

	if (radar) {
		radar->scan_chunk(this, deltaTime);
	}

	if (moveWaypoint) {
		this->move(moveWaypoint, deltaTime);
	}
}

void Driveable::UItick(float deltaTime) {
	GlobalManager::set_ui_name_value(this->get_name());
	GlobalManager::set_ui_xyz_value("X:" + godot::String::num((int)this->get_x()) + " Y:" + godot::String::num((int)this->get_y()) + " Z:" + godot::String::num((int)this->get_z()));
}

void Driveable::move(MapIcon *target, float deltaTime) {
    if (!target) {
        this->set_speed(0);
        return;
    }

    Vector2 current_pos(this->get_x(), this->get_y());
    Vector2 target_pos(target->get_x(), target->get_y());
    
    float distance = current_pos.distance_to(target_pos);

    if (distance < 2.0f) {
        this->set_speed(0);
        target->queue_free(); 
        this->set_move_waypoint(nullptr);
        return;
    }

    Vector2 direction_vec = (target_pos - current_pos).normalized();
    float targetAngle = Math::rad_to_deg(direction_vec.angle());

    float rotationStep = 120.0f * deltaTime;
    float currentAngle = this->get_direction();
    
    float angleDiff = Math::fmod(targetAngle - currentAngle + 180.0f, 360.0f);
    if (angleDiff < 0) angleDiff += 360.0f;
    angleDiff -= 180.0f;

    if (Math::abs(angleDiff) <= rotationStep) {
        this->set_direction(targetAngle);
    } else {
        this->set_direction(currentAngle + (angleDiff > 0 ? 1 : -1) * rotationStep);
    }

    float rad = Math::deg_to_rad(this->get_direction());
    Vector2 velocity = Vector2(Math::cos(rad), Math::sin(rad)) * this->get_max_speed();

    this->set_x(current_pos.x + velocity.x * deltaTime);
    this->set_y(current_pos.y + velocity.y * deltaTime);
    this->set_speed(this->get_max_speed());
}

void Driveable::avoid_incoming() {
	return;
	// ideally in the future they will try to drive away from the missile or bomb
}

void Driveable::engage(Vehicle *target) {
	return;
}

Vehicle *Driveable::clone() {
	Driveable *returnValue = memnew(Driveable);

	// map icon
	returnValue->set_name(this->get_name());
	returnValue->set_path(this->get_path());

	// vehicle
	returnValue->set_id(this->get_id());
	returnValue->set_radar(Radar::create_prototype(this->get_radar()->get_id()));
	returnValue->set_max_speed(this->get_max_speed());
	returnValue->set_radar_cross_section(this->get_radar_cross_section());
	returnValue->set_ir_signature(this->get_ir_signature());
	returnValue->set_line_of_sight_detection(this->get_line_of_sight_detection());
	returnValue->set_rwr_detection(this->get_rwr_detection());

	// driveable
	// none to really clone

	return returnValue;
}

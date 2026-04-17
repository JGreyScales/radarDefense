#include "Driveable.h"
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
    Radar* radar = this->get_radar();
    MapIcon* moveWaypoint = this->get_move_waypoint();

    if (radar){
        radar->scan_chunk(this, deltaTime);
    }

    if(moveWaypoint){
        this->move(moveWaypoint, deltaTime);
    }
}

void Driveable::move(MapIcon *target, float deltaTime) {
    if (!target) {
        this->set_speed(0);
        return;
    }

    float dx = target->get_x() - this->get_x();
    float dy = target->get_y() - this->get_y();

    if (Math::abs(dx) < 2 && Math::abs(dy) < 2){
        this->set_speed(0);
        return;
    }

    float targetAngle = atan2(dy, dx) * (180.0f / 3.14f);

    float rotationStep = 60.0f * deltaTime;
    
    float angleDiff = targetAngle - this->get_direction();
    while (angleDiff < -180) angleDiff += 360;
    while (angleDiff > 180) angleDiff -= 360;

    if (abs(angleDiff) <= rotationStep) {
        this->set_direction(targetAngle);
    } else {
        float newDirection = this->get_direction() + (angleDiff > 0 ? 1 : -1) * rotationStep;
        this->set_direction(newDirection);
    }
    float rad = this->get_direction() * (3.14f / 180.0f);
    

    float newX = this->get_x() + this->get_max_speed() * cos(rad) * deltaTime;
    float newY = this->get_y() + this->get_max_speed() * sin(rad) * deltaTime;

    this->set_x(newX);
    this->set_y(newY);
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
    Driveable* returnValue = memnew(Driveable);

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

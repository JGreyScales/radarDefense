#ifndef VEHICLE_H
#define VEHICLE_H

#include "MapIcon.h"

#include <godot_cpp/classes/area2d.hpp> 
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/circle_shape2d.hpp>

namespace godot {

    class Radar;
    class GlobalManager;
    // note this is a partially virtual class
    // therefore it will not appear in godot menus
class Vehicle : public MapIcon {
    GDCLASS(Vehicle, MapIcon)


protected:
    void Vehicle::_notification(int p_what);
    static void _bind_methods();

private:
    Area2D* hitbox;
    CollisionShape2D* hitbox_shape;

    String id;

    // Pilot
    Radar* radar;
    // Loadout

    uint16_t maxSpeed;
    uint16_t speed;
    float direction;
    uint8_t radarCrossSection;
    uint8_t irSignature;
    uint8_t lineOfSightDetection;
    uint8_t rwrDetection;

    Vector<Vehicle*> beingTrackedBy;
    MapIcon* moveWaypoint;

public:
    Vehicle();
    ~Vehicle();

    static Vehicle* create_prototype(String id, uint8_t pilotSkill);

    String get_id();
    Radar* get_radar();
    uint16_t get_max_speed();
    String get_display_content() override;
    uint16_t get_speed();
    float get_direction();
    uint8_t get_radar_cross_section();
    uint8_t get_ir_signature();
    uint8_t get_line_of_sight_detection();
    uint8_t get_rwr_detection();
    Vector<Vehicle*> get_being_tracked_by();
    MapIcon* get_move_waypoint();

    void set_id(String id);
    void set_radar(Radar* radar);
    void set_max_speed(const uint16_t p_max_speed);
    void set_speed(const uint16_t p_speed);
    void set_direction(const float p_direction);
    void set_radar_cross_section(const uint8_t p_rcs);
    void set_ir_signature(const uint8_t p_ir);
    void set_line_of_sight_detection(const uint8_t p_los);
    void set_rwr_detection(const uint8_t p_rwr);
    void set_being_tracked_by(const Vector<Vehicle*> p_tracked_by);
    void add_being_tracked_by(Vehicle* tracked_by);
    void remove_being_tracked_by(Vehicle* tracked_by);
    void set_move_waypoint(MapIcon* waypoint);

    
    virtual void tick(float deltaTime) = 0;
    virtual void move(MapIcon* target, float deltaTime) = 0;
    virtual void avoid_incoming() = 0;
    virtual void engage(Vehicle* target) = 0;
    virtual Vehicle* clone() = 0;
};

}

#endif
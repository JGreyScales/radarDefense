#ifndef RADAR_HPP
#define RADAR_HPP

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/theme_db.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/world2d.hpp>
#include <godot_cpp/classes/physics_direct_space_state2d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters2d.hpp>
#include <godot_cpp/classes/circle_shape2d.hpp>
#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/collision_polygon2d.hpp>

#include "Vehicle.h"

namespace godot {

class Radar : public Area2D {
    GDCLASS(Radar, Area2D)

private:
    String name;
    String id;
    bool offensive;
    bool hasAtLeastOneViableTarget;
    uint8_t precisionFactor;
    uint8_t maximumRange;
    uint8_t searchArea;
    uint8_t minSearchAzimuth;
    uint8_t curX;
    uint8_t curY;
    uint8_t scanChankSize;
    uint8_t dataLinkRange;
    // flyable dataLinkChild

    // godot stuff
    PackedVector2Array radarConePoints;
    CollisionPolygon2D radarCollisionObject;
public:
    Radar();
    ~Radar();

    virtual void _draw() override;


    Vector<Vehicle*> scan_chunk(Vehicle* parent);
    float calculate_detection_score(Vehicle* parent, Vehicle* target);
    // Vector<Vehicle> hitable_targets(Weapon selectedWeapon)

    uint8_t get_precision_factor();
    uint8_t get_search_area();
    uint8_t get_scan_chunk_size();
    uint8_t get_maximum_range();
    CollisionPolygon2D* get_radar_collision_object();
    void set_radar_collision_polygon();

    void clear_radar_points();
    PackedVector2Array get_radar_points();
    void add_radar_point(Vector2);

    

    virtual Vehicle select_best_target(Vector<Vehicle>) = 0;


};

}

#endif
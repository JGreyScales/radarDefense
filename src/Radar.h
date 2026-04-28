#ifndef RADAR_HPP
#define RADAR_HPP

#include <deque>
#include <unordered_set>

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
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/time.hpp>

namespace godot {
    
class Vehicle;
class GlobalManager;

struct TrackedFrame {
    double timestamp;
    Vehicle* target;
};

class Radar : public Area2D {
    GDCLASS(Radar, Area2D)

protected:
    static void _bind_methods();    
    void Radar::_notification(int p_what);
private:

    String name;
    String id;
    bool offensive;
    bool hasAtLeastOneViableTarget;
    uint8_t precisionFactor;
    uint8_t maximumRange;
    uint8_t searchArea;
    uint8_t searchelevation;
    double curX;
    uint8_t scanChankSize;
    Vehicle* dataLinkChild;

    // linger target stuff
    float lingerTime;
    std::deque<TrackedFrame> targetHistory;
    std::unordered_set<Vehicle*> presenceMap;

    // godot stuff
    PackedVector2Array radarConePoints;
    CollisionPolygon2D* radarCollisionObject;
public:
    Radar();
    ~Radar();

    virtual void _draw() override;

    static Radar* create_prototype(String id);


    void scan_chunk(Vehicle* parent, double delta);
    void add_target_entry(Vehicle* target);
    void remove_older_than(double expireryTime, Vehicle* parent);
    float calculate_detection_score(Vehicle* parent, Vehicle* target);

    // Vector<Vehicle> hitable_targets(Weapon selectedWeapon)


    String get_name();
    String get_id();
    bool get_offensive();
    bool get_has_at_least_one_viable_target();
    uint8_t get_precision_factor();
    uint8_t get_maximum_range();
    uint8_t get_search_area();
    uint8_t get_search_elevation();
    double get_cur_x();
    uint8_t get_scan_chunk_size();
    float get_linger_time();
    std::deque<TrackedFrame>* get_target_history();
    std::unordered_set<Vehicle*>* get_presence_map();
    PackedVector2Array get_radar_points();
    CollisionPolygon2D* get_radar_collision_object();
    Vehicle* get_data_link_child();



    void set_id(String id);
    void set_precision_factor(uint8_t newPF);
    void set_maximum_range(uint8_t newMR);
    void set_search_elevation(uint8_t newSE);
    void set_radar_collision_polygon();
    void set_name(String name);
    void set_offensive(bool newState);
    void set_has_at_least_one_viable_target(bool newState);
    void set_cur_x(double newX);
    void set_scan_chunk_size(uint8_t newSize);
    void set_search_area(uint8_t newSearchArea);
    void set_data_link_child(Vehicle* child);

    void clear_radar_points();
    void add_radar_point(Vector2);

    

    virtual Vehicle* select_best_target(TypedArray<Vehicle>) = 0;
    virtual Radar* clone() = 0;

};

}

#endif
#include "Radar.h"


#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
using namespace godot;

godot::Radar::~Radar() {

    // delete the last radar draw
    this->clear_radar_points();
    queue_redraw();
}

void godot::Radar::_draw() {
	PackedVector2Array conePoints = this->get_radar_points();
    if (conePoints.size() > 2){
        Color color = Color(0, 255, 65, 0.4);
        draw_colored_polygon(conePoints, color);
    }
}

Vector<Vehicle*> godot::Radar::scan_chunk(Vehicle *parent) {
	
    // draw the radar polygon
    this->clear_radar_points();
    Vector2 origin = Vector2(parent->get_x(), parent->get_y());
    float angleSrc = parent->get_direction() + curX;

    // if we have hit a scan boundry
    // reset to the left side
    if (angleSrc > this->get_search_area()){
        // negative to move left or origin in rotation
        angleSrc = -this->get_search_area();
    }

    float angle_to = angleSrc + this->get_scan_chunk_size();

    // add the origin point
    this->add_radar_point(Vector2(0,0));

    int resolution = 8;
    for (int i = 0; i <= resolution; i++){
        float angle = Math::deg_to_rad(angleSrc + (angle_to - angleSrc) * i / resolution);
        this->add_radar_point(Vector2(0,0) + Vector2(Math::cos(angle), Math::sin(angle)) * this->get_maximum_range());
    }
    

    queue_redraw();
    // uses the points we just calculated
    this->set_radar_collision_polygon();

    // godot will use the child object of radarCollisionObject
    TypedArray<Node2D> overlapping_bodies = get_overlapping_bodies();
    // begin looking for targets
    Vector<Vehicle*> detectedVehicles;

    for (int i = 0; i < overlapping_bodies.size(); i++) {
        // cast_to checks if the body is actually a Vehicle
        Vehicle *v = Object::cast_to<Vehicle>(overlapping_bodies[i]);
        if (v) {
            Vector3 artificalOrigin = Vector3(parent->get_x(), parent->get_y(), parent->get_z());
            float dist = artificalOrigin.distance_to(Vector3(v->get_x(), v->get_y(), v->get_z()));
            
            // 2. Normalized Cross Section (0.0 to 1.0)
            float cross_section = (float)v->get_radar_cross_section() / 100.0f;

            float dist_factor = Math::pow(1.0f - (dist / this->get_maximum_range()), 2.0f);

            float detection_score = (cross_section * this->get_precision_factor()) * dist_factor;

            // 5. Threshold check (e.g., 0.1 for a "faint" signal)
            if (detection_score > 0.01f) { 
                detectedVehicles.push_back(v);
            }
        }
    }

	return detectedVehicles;
}

uint8_t godot::Radar::get_precision_factor() {
	return this->precisionFactor;
}

uint8_t godot::Radar::get_search_area() {
	return this->searchArea;
}

uint8_t godot::Radar::get_scan_chunk_size() {
	return this->scanChankSize;
}

uint8_t godot::Radar::get_maximum_range() {
	return this->maximumRange;
}

CollisionPolygon2D* godot::Radar::get_radar_collision_object() {
	return &(this->radarCollisionObject);
}

void godot::Radar::set_radar_collision_polygon() {
    this->get_radar_collision_object()->set_polygon(this->get_radar_points());
}

void godot::Radar::clear_radar_points() {
    this->radarConePoints.clear();
}

PackedVector2Array godot::Radar::get_radar_points() {
	return this->radarConePoints;
}

void godot::Radar::add_radar_point(Vector2 point) {
    this->radarConePoints.append(point);
}

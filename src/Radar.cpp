#include "Radar.h"
#include "Vehicle.h"


using namespace godot;

void Radar::_bind_methods() {
    // Methods: Getters
    ClassDB::bind_method(D_METHOD("get_radar_name"), &Radar::get_name);
    ClassDB::bind_method(D_METHOD("get_id"), &Radar::get_id);
    ClassDB::bind_method(D_METHOD("get_offensive"), &Radar::get_offensive);
    ClassDB::bind_method(D_METHOD("get_has_at_least_one_viable_target"), &Radar::get_has_at_least_one_viable_target);
    ClassDB::bind_method(D_METHOD("get_precision_factor"), &Radar::get_precision_factor);
    ClassDB::bind_method(D_METHOD("get_maximum_range"), &Radar::get_maximum_range);
    ClassDB::bind_method(D_METHOD("get_search_area"), &Radar::get_search_area);
    ClassDB::bind_method(D_METHOD("get_search_elevation"), &Radar::get_search_elevation);
    ClassDB::bind_method(D_METHOD("get_cur_x"), &Radar::get_cur_x);
    ClassDB::bind_method(D_METHOD("get_scan_chunk_size"), &Radar::get_scan_chunk_size);
    ClassDB::bind_method(D_METHOD("get_linger_time"), &Radar::get_linger_time);
    ClassDB::bind_method(D_METHOD("get_radar_points"), &Radar::get_radar_points);

    // Methods: Setters
    ClassDB::bind_method(D_METHOD("set_radar_name", "name"), &Radar::set_name);
    ClassDB::bind_method(D_METHOD("set_offensive", "new_state"), &Radar::set_offensive);
    ClassDB::bind_method(D_METHOD("set_has_at_least_one_viable_target", "new_state"), &Radar::set_has_at_least_one_viable_target);
    ClassDB::bind_method(D_METHOD("set_cur_x", "new_x"), &Radar::set_cur_x);
    ClassDB::bind_method(D_METHOD("set_maximum_range", "newMR"), &Radar::set_maximum_range);
    ClassDB::bind_method(D_METHOD("set_precision_factor", "newPF"), &Radar::set_precision_factor);
    ClassDB::bind_method(D_METHOD("set_search_elevation", "newSE"), &Radar::set_search_area);
    ClassDB::bind_method(D_METHOD("set_scan_chunk_size", "new_size"), &Radar::set_scan_chunk_size);
    ClassDB::bind_method(D_METHOD("set_search_area", "new_search_area"), &Radar::set_search_area); // Matches your header typo 'set_search_are'

    // Logic Methods
    ClassDB::bind_method(D_METHOD("calculate_detection_score", "parent", "target"), &Radar::calculate_detection_score);
    ClassDB::bind_method(D_METHOD("scan_chunk", "parent"), &Radar::scan_chunk);
    ClassDB::bind_method(D_METHOD("add_target_entry", "target"), &Radar::add_target_entry);
    ClassDB::bind_method(D_METHOD("remove_older_than", "expiry_frame", "parent"), &Radar::remove_older_than);

    // Properties (Exposes them to the Inspector)
    ADD_GROUP("Radar Settings", "");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "radar_name"), "set_radar_name", "get_radar_name");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "offensive"), "set_offensive", "get_offensive");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "precision_factor"), "set_precision_factor", "get_precision_factor"); // Note: No setter provided in header for precision
    ADD_PROPERTY(PropertyInfo(Variant::INT, "maximum_range"), "set_maximum_range", "get_maximum_range");     // Note: No setter provided in header for range
    ADD_PROPERTY(PropertyInfo(Variant::INT, "search_area"), "set_search_area", "get_search_area");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "search_elevation"), "set_search_elevation", "get_search_elevation");
    
    ADD_GROUP("Scan State", "");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "scan_cur_x"), "set_cur_x", "get_cur_x");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "scan_chunk_size"), "set_scan_chunk_size", "get_scan_chunk_size");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_targets"), "set_has_at_least_one_viable_target", "get_has_at_least_one_viable_target");
}

godot::Radar::Radar() {

	this->name = "";
	this->id = "";
	this->offensive = false;
	this->hasAtLeastOneViableTarget = false;
	this->precisionFactor = 0;
	this->maximumRange = 0;
	this->searchArea = 0;
	this->searchelevation = 0;
	this->curX = 0;
	this->scanChankSize = 0;
	this->lingerTime = 0;

	radarCollisionObject = memnew(CollisionPolygon2D);
	add_child(radarCollisionObject);
}

godot::Radar::~Radar() {
	// delete the last radar draw
	this->clear_radar_points();
}

void godot::Radar::_draw() {
	PackedVector2Array conePoints = this->get_radar_points();
	if (conePoints.size() > 2) {
		Color color = Color(0.0, 1.0, 0.25, 0.4);
		draw_colored_polygon(conePoints, color);
	}
}

void Radar::scan_chunk(Vehicle *parent) {
	if (!parent)
		return;

	// draw the radar polygon
	this->clear_radar_points();
	Vector2 origin = Vector2(parent->get_x(), parent->get_y());
	float angleSrc = parent->get_direction() + this->get_cur_x();

	float angle_to = angleSrc + this->get_scan_chunk_size();

	// add the origin point
	this->add_radar_point(Vector2(0, 0));

	int resolution = 8;
	for (int i = 0; i <= resolution; i++) {
		float angle = Math::deg_to_rad(angleSrc + (angle_to - angleSrc) * i / resolution);
		this->add_radar_point(Vector2(0, 0) + Vector2(Math::cos(angle), Math::sin(angle)) * this->get_maximum_range());
	}

	queue_redraw();
	// uses the points we just calculated
	this->set_radar_collision_polygon();

	// godot will use the child object of radarCollisionObject
	TypedArray<Node2D> overlapping_bodies = get_overlapping_bodies();

	for (int i = 0; i < overlapping_bodies.size(); i++) {
		// cast_to checks if the body is actually a Vehicle
		Vehicle *v = Object::cast_to<Vehicle>(overlapping_bodies[i]);
		if (v) {
			// detection score 0.01 is faint signal
			if (this->calculate_detection_score(parent, v) > 0.2f) {
				UtilityFunctions::print("adding " + v->get_name() + " to tracked targets of " + parent->get_name());
				this->add_target_entry(v);
				v->add_being_tracked_by(parent);
			}
		}
	}

	// move to next sector for next pass
	this->set_cur_x(angle_to);
	if (curX > (float)searchArea) {
		curX = -(float)searchArea;
	}

	// remove old targets that arent in range anymore
	this->remove_older_than(Engine::get_singleton()->get_physics_frames() - this->get_linger_time(), parent);
}

void Radar::add_target_entry(Vehicle *target) {
	uint64_t current_frame = Engine::get_singleton()->get_physics_frames();
	auto &p_map = *get_presence_map();
	auto &history = *get_target_history();

	if (p_map.find(target) != p_map.end()) {
		// Find the existing entry in the history to update it
		for (auto it = history.begin(); it != history.end(); ++it) {
			if (it->target == target) {
				it->frame = current_frame;

				TrackedFrame updatedEntry = *it;
				history.erase(it);
				history.push_back(updatedEntry);
				break;
			}
		}
		return;
	}

	// If new, add normally
	history.push_back({ current_frame, target });
	p_map.insert(target);
}

void Radar::remove_older_than(uint64_t expiry_frame, Vehicle *parent) {
	std::deque<TrackedFrame> *history = this->get_target_history();
	std::unordered_set<Vehicle *> *p_map = this->get_presence_map();

	while (!history->empty() && history->front().frame < expiry_frame) {
		// Must erase from map before popping from deque
		history->front().target->remove_being_tracked_by(parent);
		p_map->erase(history->front().target);
		history->pop_front();
	}
}

float Radar::calculate_detection_score(Vehicle *parent, Vehicle *target) {
	Vector3 artificalOrigin = Vector3(parent->get_x(), parent->get_y(), parent->get_z());
	Vector3 targetXYZ = Vector3(target->get_x(), target->get_y(), target->get_z());
	float dist = artificalOrigin.distance_to(targetXYZ);

	float horizontal_dist = Vector2(parent->get_x(), parent->get_y()).distance_to(Vector2(target->get_x(), target->get_y()));
	float z_diff = target->get_z() - parent->get_z();
	float elevationToTarget = Math::abs(Math::rad_to_deg(Math::atan2(z_diff, horizontal_dist)));

	if (elevationToTarget > this->get_search_elevation() || dist > this->get_maximum_range()) {
		return 0;
	}

	float cross_section = (float)target->get_radar_cross_section() / 100.0f;
	float precision = (float)this->get_precision_factor();

	float dist_ratio = Math::clamp(dist / (float)this->get_maximum_range(), 0.0f, 1.0f);
	float falloff = Math::pow(1.0f - dist_ratio, 4.0f);

	// f(x)=(c\cdot100)\cdot(1-\frac{x}{R})^{4}+(c\cdot p)
	// ^ can be placed into desmos
	// c = cross section
	// p = precision
	// r = max range
	// x = distance

	// y=0.4 to show the limit to what is detection and not
	float detection_score = (cross_section * 100.0f * falloff) + (cross_section * precision);

	return detection_score;
}

String godot::Radar::get_name() {
	return this->name;
}

String godot::Radar::get_id() {
	return this->id;
}

bool godot::Radar::get_offensive() {
	return this->offensive;
}

bool godot::Radar::get_has_at_least_one_viable_target() {
	return this->hasAtLeastOneViableTarget;
}

uint8_t godot::Radar::get_precision_factor() {
	return this->precisionFactor;
}

uint8_t godot::Radar::get_search_area() {
	return this->searchArea;
}

uint8_t godot::Radar::get_search_elevation() {
	return this->searchelevation;
}

uint8_t godot::Radar::get_cur_x() {
	return this->curX;
}

uint8_t godot::Radar::get_scan_chunk_size() {
	return this->scanChankSize;
}

uint8_t godot::Radar::get_linger_time() {
	return this->lingerTime;
}

std::deque<TrackedFrame> *godot::Radar::get_target_history() {
	return &(this->targetHistory);
}

std::unordered_set<Vehicle *> *godot::Radar::get_presence_map() {
	return &(this->presenceMap);
}

uint8_t godot::Radar::get_maximum_range() {
	return this->maximumRange;
}

CollisionPolygon2D *godot::Radar::get_radar_collision_object() {
	return this->radarCollisionObject;
}

void godot::Radar::set_precision_factor(uint8_t newPF) {
	this->precisionFactor = newPF;
}

void godot::Radar::set_maximum_range(uint8_t newMR) {
	this->maximumRange = newMR;
}

void godot::Radar::set_search_elevation(uint8_t newSE) {
	this->searchelevation = newSE;
}

void godot::Radar::set_radar_collision_polygon() {
	if (this->get_radar_collision_object()) {
		this->get_radar_collision_object()->set_polygon(this->get_radar_points());
	}
}

void godot::Radar::set_name(String name) {
	this->name = name;
}

void godot::Radar::set_offensive(bool newState) {
	this->offensive = newState;
}

void godot::Radar::set_has_at_least_one_viable_target(bool newState) {
	this->hasAtLeastOneViableTarget = newState;
}

void godot::Radar::set_cur_x(uint8_t newX) {
	this->curX = newX;
}

void godot::Radar::set_scan_chunk_size(uint8_t newSize) {
	this->scanChankSize = newSize;
	if (this->scanChankSize > 0 && this->searchArea > 0) {
		// rounds down
		this->lingerTime = (uint64_t)((this->searchArea * 2) / this->scanChankSize) + 1;
	} else {
		this->lingerTime = 1;
	}
}

void godot::Radar::set_search_area(uint8_t newSearchArea) {
	this->searchArea = newSearchArea;
	if (this->scanChankSize > 0 && this->searchArea > 0) {
		// rounds down
		this->lingerTime = (uint64_t)((this->searchArea * 2) / this->scanChankSize) + 1;
	} else {
		this->lingerTime = 1;
	}
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

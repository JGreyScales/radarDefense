#include "Radar.h"
#include "Globals.h"
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
	ClassDB::bind_method(D_METHOD("set_search_elevation", "newSE"), &Radar::set_search_elevation);
	ClassDB::bind_method(D_METHOD("set_scan_chunk_size", "new_size"), &Radar::set_scan_chunk_size);
	ClassDB::bind_method(D_METHOD("set_search_area", "new_search_area"), &Radar::set_search_area); // Matches your header typo 'set_search_are'

	// Logic Methods
	ClassDB::bind_method(D_METHOD("calculate_detection_score", "parent", "target"), &Radar::calculate_detection_score);
	ClassDB::bind_method(D_METHOD("scan_chunk", "parent", "deltatime"), &Radar::scan_chunk);
	ClassDB::bind_method(D_METHOD("add_target_entry", "target"), &Radar::add_target_entry);
	ClassDB::bind_method(D_METHOD("remove_older_than", "expiry_frame", "parent"), &Radar::remove_older_than);

	// Properties (Exposes them to the Inspector)
	ADD_GROUP("Radar Settings", "");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "radar_name"), "set_radar_name", "get_radar_name");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "offensive"), "set_offensive", "get_offensive");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "precision_factor"), "set_precision_factor", "get_precision_factor"); // Note: No setter provided in header for precision
	ADD_PROPERTY(PropertyInfo(Variant::INT, "maximum_range"), "set_maximum_range", "get_maximum_range"); // Note: No setter provided in header for range
	ADD_PROPERTY(PropertyInfo(Variant::INT, "search_area"), "set_search_area", "get_search_area");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "search_elevation"), "set_search_elevation", "get_search_elevation");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "linger_time"), "", "get_linger_time"); // no setter

	ADD_GROUP("Scan State", "");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "scan_cur_x"), "set_cur_x", "get_cur_x");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "scan_chunk_size"), "set_scan_chunk_size", "get_scan_chunk_size");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_targets"), "set_has_at_least_one_viable_target", "get_has_at_least_one_viable_target");
}

void Radar::_notification(int p_what) {
	if (p_what == NOTIFICATION_READY) {
		if (radarCollisionObject == nullptr) {
			radarCollisionObject = memnew(CollisionPolygon2D);
			add_child(radarCollisionObject);
		}
	}
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
	this->curX = 0.0f;
	this->scanChankSize = 0;
	this->lingerTime = 4;

	radarCollisionObject = nullptr;

	set_z_index(4096);
	set_collision_layer(4); // radars sit on 4
	set_collision_mask(2); // vehicles sit on 2
}

godot::Radar::~Radar() {
	// delete the last radar draw
	this->clear_radar_points();
}

void godot::Radar::_draw() {
	PackedVector2Array conePoints = this->get_radar_points();
	if (conePoints.size() > 2) {
		Color color = Color(0.0, 1.0, 0.25, 0.1);
		draw_colored_polygon(conePoints, color);
	}
}

Radar *godot::Radar::create_prototype(String id) {
	return GlobalManager::get_radar_from_id(id);
}

void Radar::scan_chunk(Vehicle *parent, double delta) {
    if (!parent || !is_inside_tree())
        return;

    // Calculate rotation and update linger time
    float total_sweep_distance = (float)this->get_search_area() * 2.0f;
    
    // We assume a base rotation speed to calculate the dynamic linger time
    // If you have a specific speed variable, use that instead of the hardcoded 60.0f
    float degrees_per_second = 60.0f; 
    float rotation_time = total_sweep_distance / degrees_per_second;
    this->lingerTime = rotation_time + 0.05f;

    float movement = degrees_per_second * (float)delta;
    this->set_cur_x(this->get_cur_x() + movement);

    if (this->get_cur_x() > (float)searchArea) {
        this->set_cur_x(-(float)searchArea);
    }

    // --- PHYSICAL DISTANCE CLEANUP ---
    // Immediately drop targets that have moved beyond the radar's max range
    std::deque<TrackedFrame> &history = *get_target_history();
    std::unordered_set<Vehicle *> &p_map = *get_presence_map();
    Vector3 parentPos = Vector3(parent->get_x(), parent->get_y(), parent->get_z());
    float maxRange = (float)this->get_maximum_range();

    for (std::deque<TrackedFrame>::iterator it = history.begin(); it != history.end(); ) {
        Vehicle* target = it->target;
        bool should_remove = false;

        if (target == nullptr) {
            should_remove = true;
        } else {
            Vector3 targetPos = Vector3(target->get_x(), target->get_y(), target->get_z());
            if (parentPos.distance_to(targetPos) > maxRange) {
                should_remove = true;
            }
        }

        if (should_remove) {
            if (GlobalManager::get_selected_vehicle() == parent) {
                GlobalManager::remove_target_from_list(target);
            }
            if (target != nullptr) {
                target->remove_being_tracked_by(parent);
                p_map.erase(target);
            }
            it = history.erase(it);
        } else {
            ++it;
        }
    }
    // --- END DISTANCE CLEANUP ---

    this->clear_radar_points();
    float angleSrc = parent->get_direction() + this->get_cur_x();
    float angle_to = angleSrc + (float)this->get_scan_chunk_size();

    // Draw the radar polygon
    this->add_radar_point(Vector2(0, 0)); // Origin
    int resolution = 8;
    for (int i = 0; i <= resolution; i++) {
        float angle = Math::deg_to_rad(angleSrc + (angle_to - angleSrc) * i / resolution);
        this->add_radar_point(Vector2(Math::cos(angle), Math::sin(angle)) * maxRange);
    }

    queue_redraw();
    this->set_radar_collision_polygon();

    TypedArray<Area2D> overlapping = get_overlapping_areas();

    for (int i = 0; i < overlapping.size(); i++) {
        Area2D *hitbox = Object::cast_to<Area2D>(overlapping[i]);

        if (!hitbox) {
            continue;
        }

        Radar *hit_as_radar = Object::cast_to<Radar>(hitbox);

        if (hit_as_radar == this || hit_as_radar != nullptr) {
            continue;
        }

        Node *parent_node = hitbox->get_parent();
        Vehicle *v = Object::cast_to<Vehicle>(parent_node);

        if (v && v != parent) {
            float detectionScore = this->calculate_detection_score(parent, v);
            if (detectionScore > 0.2f) {
                this->add_target_entry(v);
                v->add_being_tracked_by(parent);
            } else if (detectionScore > 0.08f && (UtilityFunctions::randi() % 10) == 1) {
                this->add_target_entry(v);
                v->add_being_tracked_by(parent);
            }
        }
    }

    double current_time = (double)Time::get_singleton()->get_ticks_msec() / 1000.0;
    double expiry_threshold = current_time - (double)this->get_linger_time();

    this->remove_older_than(expiry_threshold, parent);
}

void Radar::add_target_entry(Vehicle *target) {
	double current_time = (double)Time::get_singleton()->get_ticks_msec() / 1000.0;
	auto &p_map = *get_presence_map();
	auto &history = *get_target_history();

	if (p_map.find(target) != p_map.end()) {
		for (auto it = history.begin(); it != history.end(); ++it) {
			if (it->target == target) {
				it->timestamp = current_time; // Update timestamp

				TrackedFrame updatedEntry = *it;
				history.erase(it);
				history.push_back(updatedEntry);
				break;
			}
		}
		return;
	}

	// UtilityFunctions::print("adding new detection");
	if (GlobalManager::get_selected_vehicle() == this->get_parent()){
		GlobalManager::add_target_to_list(target);
	}
	history.push_back({ current_time, target });
	p_map.insert(target);
}

void Radar::remove_older_than(double expiry_time, Vehicle *parent) {
    std::deque<TrackedFrame> *history = this->get_target_history();
    std::unordered_set<Vehicle *> *p_map = this->get_presence_map();

    while (!history->empty() && history->front().timestamp < expiry_time) {
        UtilityFunctions::print("removing old detection");

        Vehicle* expired_vehicle = history->front().target;
		
        if (GlobalManager::get_selected_vehicle() == parent) {
            GlobalManager::remove_target_from_list(expired_vehicle);
        }

        if (expired_vehicle != nullptr) {
            expired_vehicle->remove_being_tracked_by(parent);
            p_map->erase(expired_vehicle);
        }

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

	UtilityFunctions::print("Radar sweep (distance = ", dist, " ): ", parent->get_name(), " -> ", target->get_name(), " Score: ", detection_score);

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

double godot::Radar::get_cur_x() {
	return this->curX;
}

uint8_t godot::Radar::get_scan_chunk_size() {
	return this->scanChankSize;
}

float godot::Radar::get_linger_time() {
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

Vehicle *godot::Radar::get_data_link_child() {
	return this->dataLinkChild;
}

void godot::Radar::set_id(String id) {
	this->id = id;
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
	if (this->get_radar_collision_object() && is_inside_tree()) {
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

void godot::Radar::set_cur_x(double newX) {
	this->curX = newX;
}

void godot::Radar::set_scan_chunk_size(uint8_t newSize) {
	this->scanChankSize = newSize;
}

void godot::Radar::set_search_area(uint8_t newSearchArea) {
	this->searchArea = newSearchArea;
}

void godot::Radar::set_data_link_child(Vehicle *child) {
	this->dataLinkChild = child;
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

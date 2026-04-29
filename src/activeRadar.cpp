#include "activeRadar.h"
#include "Vehicle.h"

void activeRadar::select_best_target(TypedArray<Vehicle *> possibleTargets, Vehicle *parent) {
	std::unordered_set<Vehicle*>* host_radar_targets = this->get_host_radar()->get_presence_map();
    auto data_link_target = host_radar_targets->find(this->get_launch_target());

    // if data link is active, just go for the target
    if (data_link_target != host_radar_targets->end()){
        this->set_target(*data_link_target);
        this->set_has_at_least_one_viable_target(true);
        return;
    }



    // active radar logic

    if (this->calculate_detection_score(parent, this->get_target()) >= (DETECTION_SCORE_REQUIRED_FOR_MATCH * 0.8)){
        this->set_target(*data_link_target);
        this->set_has_at_least_one_viable_target(true);
        return;
    }

    Vehicle* highest_scored_vehicle = nullptr;
    float score = 0;

    for (size_t i = 0; i < possibleTargets.size(); i++)
    {
        Vehicle* current_veh = Object::cast_to<Vehicle>(possibleTargets[i]);
        float current_score = this->calculate_detection_score(parent, current_veh) > score;
        if (current_score > score){
            score = current_score;
            highest_scored_vehicle = current_veh;
        }
    }

    if (highest_scored_vehicle){
        this->set_target(highest_scored_vehicle);
        this->set_has_at_least_one_viable_target(true);
        parent->set_move_waypoint(highest_scored_vehicle);
        return;
    } else {
        this->set_target(nullptr);
        this->set_has_at_least_one_viable_target(false);
        parent->set_move_waypoint(nullptr);
        return;
    }
}

activeRadar::activeRadar() {
    semiActiveRadar::semiActiveRadar();
}

activeRadar::~activeRadar() {

}

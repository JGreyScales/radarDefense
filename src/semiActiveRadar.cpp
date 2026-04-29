#include "semiActiveRadar.h"
#include "Vehicle.h"


void semiActiveRadar::select_best_target(TypedArray<Vehicle *> possibleTargets, Vehicle *parent) {
	std::unordered_set<Vehicle*>* host_radar_targets = this->get_host_radar()->get_presence_map();
    auto data_link_target = host_radar_targets->find(this->get_launch_target());

    // if data link is active, just go for the target
    if (data_link_target != host_radar_targets->end()){
        this->set_target(*data_link_target);
        this->set_has_at_least_one_viable_target(true);
        return;
    }

    this->set_target(nullptr);
    this->set_has_at_least_one_viable_target(false);
    parent->set_move_waypoint(nullptr);
    return;
}

semiActiveRadar::semiActiveRadar() {
}

semiActiveRadar::~semiActiveRadar() {
}

Radar *semiActiveRadar::clone() {
    semiActiveRadar* returnObject = memnew(semiActiveRadar);

    returnObject->set_name(this->get_name());
    returnObject->set_id(this->get_id());
    returnObject->set_offensive(this->get_offensive());
    returnObject->set_precision_factor(this->get_precision_factor());
    returnObject->set_maximum_range(this->get_maximum_range());
    returnObject->set_search_area(this->get_search_area());
    returnObject->set_search_elevation(this->get_search_elevation());
    returnObject->set_scan_chunk_size(this->get_scan_chunk_size());

	return returnObject;
}

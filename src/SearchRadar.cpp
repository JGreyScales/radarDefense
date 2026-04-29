#include "SearchRadar.h"
#include "Vehicle.h"

using namespace godot;

void godot::SearchRadar::_bind_methods() {
    ClassDB::bind_method(D_METHOD("select_best_target", "best_target"), &SearchRadar::select_best_target);

}

// search radars dont really pick targets
void SearchRadar::select_best_target(TypedArray<Vehicle> _, Vehicle* __) {
	return;
}

Radar *godot::SearchRadar::clone() {
    Radar* returnValue = memnew(SearchRadar);

	returnValue->set_name(this->get_name());
    returnValue->set_id(this->get_id());
    returnValue->set_offensive(this->get_offensive());
    returnValue->set_has_at_least_one_viable_target(this->get_has_at_least_one_viable_target());
    returnValue->set_precision_factor(this->get_precision_factor());
    returnValue->set_maximum_range(this->get_maximum_range());
    returnValue->set_search_area(this->get_search_area());
    returnValue->set_search_elevation(this->get_search_elevation());
    returnValue->set_scan_chunk_size(this->get_scan_chunk_size());

    return returnValue;
}

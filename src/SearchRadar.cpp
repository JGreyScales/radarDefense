#include "SearchRadar.h"
#include "Vehicle.h"

using namespace godot;

void godot::SearchRadar::_bind_methods() {
    ClassDB::bind_method(D_METHOD("select_best_target", "best_target"), &SearchRadar::select_best_target);

}

// search radars dont really pick targets
Vehicle *godot::SearchRadar::select_best_target(TypedArray<Vehicle> possibleTargets) {
	return nullptr;
}
#include "Flyable.h"
#include "Globals.h"

using namespace godot;

void Flyable::_bind_methods() {
    // --- Core Guidance ---
    ClassDB::bind_method(D_METHOD("get_despawn_if_cant_hit"), &Flyable::get_despawn_if_cant_hit);
    ClassDB::bind_method(D_METHOD("set_despawn_if_cant_hit", "state"), &Flyable::set_despawn_if_cant_hit);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "despawn_if_cant_hit"), "set_despawn_if_cant_hit", "get_despawn_if_cant_hit");

    ClassDB::bind_method(D_METHOD("get_target_elevation"), &Flyable::get_target_elevation);
    ClassDB::bind_method(D_METHOD("set_target_elevation", "value"), &Flyable::set_target_elevation);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "target_elevation"), "set_target_elevation", "get_target_elevation");

    // --- Performance & Weight ---
    ClassDB::bind_method(D_METHOD("get_weight"), &Flyable::get_weight);
    ClassDB::bind_method(D_METHOD("set_weight", "value"), &Flyable::set_weight);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "weight"), "set_weight", "get_weight");

    ClassDB::bind_method(D_METHOD("get_turn_rate"), &Flyable::get_turn_rate);
    ClassDB::bind_method(D_METHOD("set_turn_rate", "value"), &Flyable::set_turn_rate);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "turn_rate"), "set_turn_rate", "get_turn_rate");

    ClassDB::bind_method(D_METHOD("get_optimal_turn_speed"), &Flyable::get_optimal_turn_speed);
    ClassDB::bind_method(D_METHOD("set_optimal_turn_speed", "value"), &Flyable::set_optimal_turn_speed);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "optimal_turn_speed"), "set_optimal_turn_speed", "get_optimal_turn_speed");

    // --- Fuel ---
    ClassDB::bind_method(D_METHOD("get_fuel_time"), &Flyable::get_fuel_time);
    ClassDB::bind_method(D_METHOD("set_fuel_time", "value"), &Flyable::set_fuel_time);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "fuel_time"), "set_fuel_time", "get_fuel_time");

    // --- PID Constants ---
    ClassDB::bind_method(D_METHOD("get_PID_prop_term"), &Flyable::get_PID_prop_term);
    ClassDB::bind_method(D_METHOD("set_PID_prop_term", "value"), &Flyable::set_PID_prop_term);
    
    ClassDB::bind_method(D_METHOD("get_PID_int_term"), &Flyable::get_PID_int_term);
    ClassDB::bind_method(D_METHOD("set_PID_int_term", "value"), &Flyable::set_PID_int_term);

    ClassDB::bind_method(D_METHOD("get_PID_der_term"), &Flyable::get_PID_der_term);
    ClassDB::bind_method(D_METHOD("set_PID_der_term", "value"), &Flyable::set_PID_der_term);

    // --- Combat ---
    ClassDB::bind_method(D_METHOD("get_proximity_fuze_radius"), &Flyable::get_proximity_fuze_radius);
    ClassDB::bind_method(D_METHOD("set_proximity_fuze_radius", "value"), &Flyable::set_proximity_fuze_radius);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "proximity_fuze_radius"), "set_proximity_fuze_radius", "get_proximity_fuze_radius");

    // --- Combat Actions ---
    ClassDB::bind_method(D_METHOD("tick", "delta_time"), &Flyable::tick);
    ClassDB::bind_method(D_METHOD("move", "target", "delta_time"), &Flyable::move);
    ClassDB::bind_method(D_METHOD("calculateOptimalFlightPath", "target", "delta_time"), &Flyable::calculateOptimalFlightPath);
}

Flyable::Flyable() {
}

Flyable::~Flyable() {}

void Flyable::set_despawn_if_cant_hit(bool state) {
    this->despawnIfCantHitTarget = state;
}

void Flyable::set_time_to_destruction(float value) {
    this->timeToDestruction = value;
}

void Flyable::set_maximum_altitude(uint8_t value) {
    this->maximumAltitude = value;
}

void Flyable::set_fuel_time(float value) {
    this->fuelTime = value;
}

void Flyable::set_fuel_weight_per_L(float value) {
    this->fuelWeightPerL = value;
}

void Flyable::set_fuel_flow_rate_L(float value) {
    this->fuelFlowRateL = value;
}

void Flyable::set_weight(uint16_t value) {
    this->weight = value;
}

void Flyable::set_wep_timer(uint8_t value) {
    this->wepTimer = value;
}

void Flyable::set_wep_overload(uint8_t value) {
    this->wepOverload = value;
}

void Flyable::set_turn_rate(uint8_t value) {
    this->turnRate = value;
}

void Flyable::set_optimal_turn_speed(uint16_t value) {
    this->optimalTurnSpeed = value;
}

void Flyable::set_lift_coefficent(uint8_t value) {
    this->liftCoefficent = value;
}

void Flyable::set_engine_thrust_output(float value) {
    this->engineThrustOutput = value;
}

void Flyable::set_PID_prop_term(float value) {
    this->PID_prop_term = value;
}

void Flyable::set_PID_int_term(float value) {
    this->PID_int_term = value;
}

void Flyable::set_PID_der_term_limit(float value) {
    this->PID_int_term_limit = value;
}

void Flyable::set_PID_der_term(float value) {
    this->PID_der_term = value;
}

void Flyable::set_integral_error_sum(float value) {
    this->integralErrorSum = value;
}

void Flyable::set_target_elevation(float value) {
    this->targetElevation = value;
}

void Flyable::set_proximity_fuze_radius(float value) {
    this->proximityFuzeRadiusM = value;
}

void Flyable::set_prev_yaw_error(float value) {
    this->prevYawError = value;
}

void Flyable::set_yaw_integral(float value) {
    this->yawIntegral = value;
}

void Flyable::set_prev_pitch_error(float value) {
    this->prevPitchError = value;
}

void Flyable::set_pitch_integral(float value) {
    this->pitchIntegral = value;
}

void Flyable::set_chaff_count(uint8_t value) {
    this->chaffCount = value;
}

void Flyable::set_flare_count(uint8_t value) {
    this->flareCount = value;
}

void Flyable::set_interception_target(MapIcon *object) {
    this->interceptionTarget = object;
}

bool Flyable::get_despawn_if_cant_hit() {
	return this->despawnIfCantHitTarget;
}

float Flyable::get_time_to_destruction() {
	return this->timeToDestruction;
}

uint8_t Flyable::get_maximum_altitude() {
	return this->maximumAltitude;
}

float Flyable::get_fuel_time() {
	return this->fuelTime;
}

float Flyable::get_fuel_weight_per_L() {
	return this->fuelWeightPerL;
}

float Flyable::get_fuel_float_rate_L() {
	return this->fuelFlowRateL;
}

uint16_t Flyable::get_weight() {
	return this->weight;
}

uint8_t Flyable::get_wep_timer() {
	return this->wepTimer;
}

uint8_t Flyable::get_wep_overload() {
	return this->wepOverload;
}

uint8_t Flyable::get_turn_rate() {
	return this->turnRate;
}

uint16_t Flyable::get_optimal_turn_speed() {
	return this->optimalTurnSpeed;
}

uint8_t Flyable::get_lift_coefficent() {
	return this->liftCoefficent;
}

float Flyable::get_engine_thrust_output() {
	return this->engineThrustOutput;
}

float Flyable::get_PID_prop_term() {
	return this->PID_prop_term;
}

float Flyable::get_PID_int_term() {
	return this->PID_int_term;
}

float Flyable::get_PID_der_term_limit() {
	return this->PID_int_term_limit;
}

float Flyable::get_PID_der_term() {
	return this->PID_der_term;
}

float Flyable::get_integral_error_sum() {
	return this->integralErrorSum;
}

float Flyable::get_target_elevation() {
	return this->targetElevation;
}

float Flyable::get_proximity_fuze_radius() {
	return this->proximityFuzeRadiusM;
}

float Flyable::get_prev_yaw_error() {
	return this->prevYawError;
}

float Flyable::get_yaw_integral() {
	return this->yawIntegral;
}

float Flyable::get_prev_pitch_error() {
	return this->prevPitchError;
}

float Flyable::get_pitch_integral() {
	return this->pitchIntegral;
}

uint8_t Flyable::get_chaff_count() {
	return this->chaffCount;
}

uint8_t Flyable::get_flare_count() {
	return this->flareCount;
}

void Flyable::tick(float deltaTime) {
    if (deltaTime <= 0.0f) return;

    // STATE SYNC
    float currentSpeed = (float)get_speed();
    float currentYawRad = Math::deg_to_rad(get_direction());
    float currentPitchRad = Math::deg_to_rad(get_pitch());
    Vector3 pos = Vector3(get_x(), get_y(), get_z());
    bool hasFuel = fuelTime > 0;

    // ROTATION AUTHORITY (PID Suggestion)
    if (this->get_move_waypoint() != nullptr) {
        this->calculateCollisionCourseMarker();
        Vector2 angleDeltas = calculateOptimalFlightPath(this->interceptionTarget, deltaTime);
        
        currentYawRad += angleDeltas.x;
        currentPitchRad += angleDeltas.y;

        set_direction(Math::rad_to_deg(currentYawRad));
        set_pitch(Math::rad_to_deg(currentPitchRad));
    }

    float altitudePenalty = 1.0f;
    if (maximumAltitude > 0) {
        float altitudeRatio = Math::clamp(pos.z / (float)maximumAltitude, 0.0f, 1.0f);
        altitudePenalty = 1.0f - (altitudeRatio * 0.4f); // Power drops at height
    }
    float effectiveMaxSpeed = get_max_speed() * altitudePenalty;

    if (hasFuel) {
        float acceleration = calculateImpulseAcceleration((float)weight, engineThrustOutput);
        if (currentSpeed < effectiveMaxSpeed) {
            currentSpeed += acceleration * deltaTime;
            if (currentSpeed > effectiveMaxSpeed) currentSpeed = effectiveMaxSpeed;
        }

        float fuelConsumption = this->fuelFlowRateL * deltaTime;
        this->weight -= fuelConsumption * this->fuelWeightPerL;
        fuelTime -= deltaTime; 
    } else {
        if (currentSpeed > 0.0f) {
            currentSpeed -= 9.81f * deltaTime; 
            if (currentSpeed < 0.0f) currentSpeed = 0.0f;
        }
    }
    set_speed((uint16_t)currentSpeed);

    // TRANSLATION (XYZ Authority)
    float cosPitch = Math::cos(currentPitchRad);
    Vector3 velocity;
    velocity.x = currentSpeed * cosPitch * Math::cos(currentYawRad);
    velocity.y = currentSpeed * cosPitch * Math::sin(currentYawRad);
    velocity.z = currentSpeed * Math::sin(currentPitchRad);

    // AERODYNAMICS (Stall/Lift Logic)
    if (pos.z > 0.0f && currentSpeed <= (float)optimalTurnSpeed) {
        float safeLiftCoef = liftCoefficent > 0.0f ? (float)liftCoefficent : 1.0f;
        float liftDeficit = 1.0f - (currentSpeed / (float)optimalTurnSpeed);
        float descentRate = (50.0f / safeLiftCoef) * liftDeficit;
        
        velocity.z -= descentRate;
    }

    // FINAL POSITION UPDATE
    pos += velocity * deltaTime;

    if (pos.z < 0.0f) {
        pos.z = 0.0f;
        if (!hasFuel) currentSpeed = 0.0f;
    }

    set_x(pos.x);
    set_y(pos.y);
    set_z(pos.z);

    if (this->despawnIfCantHitTarget){
        this->timeToDestruction -= deltaTime;
        if (this->timeToDestruction < 0){
            memdelete(this);
        }
    }
}

void Flyable::UItick(float deltaTime) {
    GlobalManager::set_ui_name_value(this->get_name());
	GlobalManager::set_ui_xyz_value("X:" + godot::String::num((int)this->get_x()) + " Y:" + godot::String::num((int)this->get_y()) + " Z:" + godot::String::num((int)this->get_z()));
}

void Flyable::move(MapIcon *target, float deltaTime) {
}

void Flyable::avoid_incoming() {
}

void Flyable::engage(Vehicle *target) {
}

Flyable *Flyable::clone() {
	Flyable* returnValue = memnew(Flyable);

	// map icon
	returnValue->set_name(this->get_name());
	returnValue->set_path(this->get_path());

	// vehicle
	returnValue->set_id(this->get_id());
	returnValue->set_radar(Radar::create_prototype(this->get_radar()->get_id()));
	returnValue->set_max_speed(this->get_max_speed());
	returnValue->set_radar_cross_section(this->get_radar_cross_section());
	returnValue->set_ir_signature(this->get_ir_signature());
	returnValue->set_line_of_sight_detection(this->get_line_of_sight_detection());
	returnValue->set_rwr_detection(this->get_rwr_detection());

    // Flyable Core Stats
    returnValue->set_despawn_if_cant_hit(this->get_despawn_if_cant_hit());
    returnValue->set_time_to_destruction(this->get_time_to_destruction());
    returnValue->set_maximum_altitude(this->get_maximum_altitude());

    // Fuel and Weight
    returnValue->set_fuel_time(this->get_fuel_time());
    returnValue->set_fuel_weight_per_L(this->get_fuel_weight_per_L());
    returnValue->set_fuel_flow_rate_L(this->get_fuel_float_rate_L());
    returnValue->set_weight(this->get_weight());

    // WEP System
    returnValue->set_wep_timer(this->get_wep_timer());
    returnValue->set_wep_overload(this->get_wep_overload());

    // Aerodynamics & Propulsion
    returnValue->set_turn_rate(this->get_turn_rate());
    returnValue->set_optimal_turn_speed(this->get_optimal_turn_speed());
    returnValue->set_lift_coefficent(this->get_lift_coefficent());
    returnValue->set_engine_thrust_output(this->get_engine_thrust_output());

    // PID Constants & Guidance
    returnValue->set_PID_prop_term(this->get_PID_prop_term());
    returnValue->set_PID_int_term(this->get_PID_int_term());
    returnValue->set_PID_der_term(this->get_PID_der_term());
    returnValue->set_PID_der_term_limit(this->get_PID_der_term_limit());
    returnValue->set_integral_error_sum(this->get_integral_error_sum());
    returnValue->set_target_elevation(this->get_target_elevation());

    // Combat & Countermeasures
    returnValue->set_proximity_fuze_radius(this->get_proximity_fuze_radius());
    returnValue->set_chaff_count(this->get_chaff_count());
    returnValue->set_flare_count(this->get_flare_count());


	return nullptr;
}

Vector3 Flyable::getTargetVelocityVector(MapIcon *target) {
	if (!target) return Vector3(0, 0, 0);

    float speed = (float)target->get_speed();
    float yaw = Math::deg_to_rad((float)target->get_direction());
    float pitch = Math::deg_to_rad((float)target->get_pitch());

    float horizontalComp = speed * Math::cos(pitch);

    float vx = horizontalComp * Math::cos(yaw);
    float vy = horizontalComp * Math::sin(yaw);
    float vz = speed * Math::sin(pitch);

    return Vector3(vx, vy, vz);
}

uint8_t Flyable::calculateMaximumRange() {
    float currentElevation = get_z();
    float altitudePenalty = 1.0f;
    float dragDecel = 9.81f; 

    if (maximumAltitude > 0) {
        float altitudeRatio = Math::clamp(currentElevation / (float)maximumAltitude, 0.0f, 1.0f);
        altitudePenalty = 1.0f - (altitudeRatio * 0.4f);
    }
    
    float effectiveMaxSpeed = get_max_speed() * altitudePenalty;
    float stallSpeed = (float)optimalTurnSpeed;

    float poweredDistance = effectiveMaxSpeed * fuelTime;

    float coastDistance = 0.0f;
    if (effectiveMaxSpeed > stallSpeed) {
        float coastTime = (effectiveMaxSpeed - stallSpeed) / dragDecel;
        float avgCoastSpeed = (effectiveMaxSpeed + stallSpeed) / 2.0f;
        coastDistance = avgCoastSpeed * coastTime;
    }

    float descentDistance = 0.0f;
    if (currentElevation > 0.0f) {
        float safeLiftCoef = liftCoefficent > 0.0f ? (float)liftCoefficent : 1.0f;
        float descentRate = 50.0f / safeLiftCoef;
        float descentTime = currentElevation / descentRate;
        
        descentDistance = stallSpeed * descentTime;
    }

    float totalDistance = poweredDistance + coastDistance + descentDistance;
    float scaledDistance = totalDistance / 1000.0f;
    return (uint8_t)Math::clamp(scaledDistance, 0.0f, 255.0f); 
}

uint8_t Flyable::calculateMaximumEffectiveRange() {
    float launchElevation = get_z();
    float altitudePenalty = 1.0f;
    float dragDecel = 9.81f;

    if (maximumAltitude > 0) {
        float altitudeRatio = Math::clamp(launchElevation / (float)maximumAltitude, 0.0f, 1.0f);
        altitudePenalty = 1.0f - (altitudeRatio * 0.4f);
    }
    
    float effectiveMaxSpeed = get_max_speed() * altitudePenalty;
    float stallSpeed = (float)optimalTurnSpeed;

    float poweredDistance = effectiveMaxSpeed * fuelTime;

    float coastDistance = 0.0f;
    if (effectiveMaxSpeed > stallSpeed) {
        float coastTime = (effectiveMaxSpeed - stallSpeed) / dragDecel;
        float avgCoastSpeed = (effectiveMaxSpeed + stallSpeed) / 2.0f;
        coastDistance = avgCoastSpeed * coastTime;
    }
    
    float totalDistance = poweredDistance + coastDistance; 

    float scaledDistance = totalDistance / 1000.0f;
    return (uint8_t)Math::clamp(scaledDistance, 0.0f, 255.0f); 
}

float Flyable::calculateImpulseAcceleration(float mass, float thrust) {
    if (mass <= 0.0f) return 0.0f;
    return thrust / mass; 
}

Vector2 Flyable::calculateOptimalFlightPath(MapIcon* target, float deltaTime) {
    if (!target || deltaTime <= 0.0f) return Vector2(0, 0);

    float currentSpeed = (float)get_speed();
    float optSpeed = (float)optimalTurnSpeed;

    // --- Turn Authority Logic ---
    float turnAuthority = 1.0f;
    if (optSpeed > 0.0f) {
        if (currentSpeed < optSpeed) {
            turnAuthority = currentSpeed / optSpeed;
        } else {
            float overspeedRatio = currentSpeed / optSpeed;
            turnAuthority = 1.0f / overspeedRatio;
        }
    }
    turnAuthority = Math::clamp(turnAuthority, 0.1f, 1.0f);
    float maxTurnStep = Math::deg_to_rad((float)turnRate * turnAuthority) * deltaTime;

    // --- Error Calculation ---
    float dx = target->get_x() - get_x();
    float dy = target->get_y() - get_y();
    float dz = target->get_z() - get_z();
    float groundDist = Math::sqrt(dx * dx + dy * dy);

    // proxy fuze is in meters
    if (groundDist < (proximityFuzeRadiusM * METERS_TO_GODOT_UNIT)){
        memdelete(this);
        memdelete(this->get_move_waypoint());
        queue_redraw();
        return Vector2(0, 0);
    }

    if (this->despawnIfCantHitTarget) {
        float desiredRelativeHeight = groundDist * targetElevation;
        bool significantlyBelow = (dz > desiredRelativeHeight);
        bool closeRange = (groundDist < (6 * KM_TO_GODOT_UNIT));

        if (!closeRange && !significantlyBelow) {
            dz = (target->get_z() + desiredRelativeHeight) - get_z();
        }
    }

    float desiredYaw = Math::atan2(dy, dx); 
    float desiredPitch = Math::atan2(dz, Math::max(groundDist, 0.01f));

    float currentYaw = Math::deg_to_rad(get_direction());
    float currentPitch = Math::deg_to_rad(get_pitch());

    float yawError = desiredYaw - currentYaw;
    while (yawError > Math_PI) yawError -= 2.0f * Math_PI;
    while (yawError < -Math_PI) yawError += 2.0f * Math_PI;

    float pitchError = desiredPitch - currentPitch;

    // YAW
    yawIntegral = Math::clamp(yawIntegral + (yawError * deltaTime), -PID_int_term_limit, PID_int_term_limit);
    float yawDeriv = (yawError - prevYawError) / deltaTime;
    float yawOutput = (PID_prop_term * yawError) + (PID_int_term * yawIntegral) + (PID_der_term * yawDeriv);
    prevYawError = yawError;

    // PITCH
    pitchIntegral = Math::clamp(pitchIntegral + (pitchError * deltaTime), -PID_int_term_limit, PID_int_term_limit);
    float pitchDeriv = (pitchError - prevPitchError) / deltaTime;
    float pitchOutput = (PID_prop_term * pitchError) + (PID_int_term * pitchIntegral) + (PID_der_term * pitchDeriv);
    prevPitchError = pitchError;

    // Clamp the PID outputs to the physical limits of the airframe
    float clampedYawDelta = Math::clamp(yawOutput, -maxTurnStep, maxTurnStep);
    float clampedPitchDelta = Math::clamp(pitchOutput, -maxTurnStep, maxTurnStep);

    return Vector2(clampedYawDelta, clampedPitchDelta);
}

void Flyable::calculateCollisionCourseMarker() {
    if (!this->interceptionTarget){
        this->interceptionTarget = new MapIcon();
    }

	if (!this->get_move_waypoint()){
        return;
    }

    MapIcon* target = this->get_move_waypoint();
    
    Vector3 targetPos = Vector3(target->get_x(), target->get_y(), target->get_z());
    Vector3 myPos = Vector3(get_x(), get_y(), get_z());
    
    Vector3 targetVel = getTargetVelocityVector(target);
    Vector3 myVel = getTargetVelocityVector(this);

    Vector3 deltaPos = targetPos - myPos;
    float distance = deltaPos.length();

    Vector3 relVel = myVel - targetVel;
    float Vc = relVel.dot(deltaPos.normalized());

    float tti = distance / (Math::max(Vc, 0.1f));

    tti = Math::clamp(tti, 0.0f, 10.0f);

    Vector3 leadPoint = targetPos + (targetVel * tti);


    float gravityDrop = 0.5f * 9.81f * (tti * tti);
    
    this->interceptionTarget->set_x(leadPoint.x);
    this->interceptionTarget->set_y(leadPoint.y);
    this->interceptionTarget->set_z(leadPoint.z + gravityDrop);
}

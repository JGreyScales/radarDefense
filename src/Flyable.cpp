#include "Flyable.h"
#include "Globals.h"

using namespace godot;

Flyable::Flyable() {
    fuelTime = 255;
    weight = 5000;
    engineThrustOutput = 150000.0f;
    maximumAltitude = 100;
    optimalTurnSpeed = 150;
    liftCoefficent = 10;
}

Flyable::~Flyable() {}

void Flyable::tick(float deltaTime) {
    elevation = get_z();
    float currentSpeed = get_speed();
    bool hasFuel = fuelTime > 0;

    if (this->get_move_waypoint() != nullptr) {
        this->calculateCollisionCourseMarker();
        calculateOptimalFlightPath(this->interceptionTarget, deltaTime);
    }

    float altitudePenalty = 1.0f;
    if (maximumAltitude > 0) {
        float altitudeRatio = Math::clamp(elevation / (float)maximumAltitude, 0.0f, 1.0f);
        altitudePenalty = 1.0f - (altitudeRatio * 0.4f);
    }
    float effectiveMaxSpeed = get_max_speed() * altitudePenalty;

    if (hasFuel) {
        float acceleration = calculateImpulseAcceleration((float)weight, engineThrustOutput);
        
        if (currentSpeed < effectiveMaxSpeed) {
            currentSpeed += acceleration * deltaTime;
            if (currentSpeed > effectiveMaxSpeed) currentSpeed = effectiveMaxSpeed;
        }
        fuelTime -= deltaTime; 

    } else if (elevation > 0.0f) {
        if (currentSpeed > 0.0f) {
            currentSpeed -= 9.81f * deltaTime; 
            if (currentSpeed < 0.0f) currentSpeed = 0.0f;
        }

        if (currentSpeed <= (float)optimalTurnSpeed) {
            float safeLiftCoef = liftCoefficent > 0.0f ? (float)liftCoefficent : 1.0f;
            float descentRate = 50.0f / safeLiftCoef;
            elevation -= descentRate * deltaTime;
        } 
    } else {
        currentSpeed = 0.0f;
    }

    set_speed((uint16_t)currentSpeed);

    float radYaw = Math::deg_to_rad(get_direction());
    float radPitch = Math::deg_to_rad(get_pitch());

    
    float horizontalComp = currentSpeed * Math::cos(radPitch);
    
    float dx = horizontalComp * Math::cos(radYaw) * deltaTime;
    float dy = horizontalComp * Math::sin(radYaw) * deltaTime;
    float dz = currentSpeed * Math::sin(radPitch) * deltaTime;

    // Apply the position changes
    set_x(get_x() + dx);
    set_y(get_y() + dy);
    
    elevation += dz;
    if (elevation < 0.0f) elevation = 0.0f;
    set_z(elevation);
}

void Flyable::UItick(float deltaTime) {
    GlobalManager::set_ui_name_value(this->get_name());
	GlobalManager::set_ui_xyz_value("X:" + godot::String::num((int)this->get_x()) + " Y:" + godot::String::num((int)this->get_y()) + " Z:" + godot::String::num((int)this->get_z()));
}

Vector3 Flyable::getTargetVelocityVector(MapIcon* target) {
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

float Flyable::calculateOptimalFlightPath(MapIcon* target, float deltaTime) {
    if (!target || deltaTime <= 0.0f) return 0.0f;

    float currentSpeed = (float)get_speed();
    
    float turnAuthority = 1.0f;
    float optSpeed = (float)optimalTurnSpeed;

    if (optSpeed > 0.0f) {
        if (currentSpeed < optSpeed) {
            turnAuthority = currentSpeed / optSpeed;
        } else {
            float overspeedRatio = currentSpeed / optSpeed;
            turnAuthority = 1.0f / overspeedRatio; 
        }
    }

    turnAuthority = Math::clamp(turnAuthority, 0.1f, 1.0f);
    float effectiveTurnRate = (float)turnRate * turnAuthority;
    float maxTurnStep = Math::deg_to_rad(effectiveTurnRate) * deltaTime;

    float dx = target->get_x() - get_x();
    float dy = target->get_y() - get_y();
    float dz = target->get_z() - get_z();

    float groundDist = Math::sqrt(dx * dx + dy * dy);

    float desiredYaw = Math::atan2(dy, dx); 
    float desiredPitch = Math::atan2(dz, groundDist);

    float currentYaw = Math::deg_to_rad(get_direction());
    float currentPitch = Math::deg_to_rad(get_pitch());

    float yawError = desiredYaw - currentYaw;
    while (yawError > Math_PI) yawError -= 2.0f * Math_PI;
    while (yawError < -Math_PI) yawError += 2.0f * Math_PI;

    float pitchError = desiredPitch - currentPitch;

    
    // YAW PID
    yawIntegral = Math::clamp(yawIntegral + (yawError * deltaTime), -PID_int_term_limit, PID_int_term_limit);
    float yawDeriv = (yawError - prevYawError) / deltaTime;
    float yawOutput = (PID_prop_term * yawError) + (PID_int_term * yawIntegral) + (PID_der_term * yawDeriv);
    prevYawError = yawError;

    // PITCH PID
    pitchIntegral = Math::clamp(pitchIntegral + (pitchError * deltaTime), -PID_int_term_limit, PID_int_term_limit);
    float pitchDeriv = (pitchError - prevPitchError) / deltaTime;
    float pitchOutput = (PID_prop_term) + (PID_int_term * pitchIntegral) + (PID_der_term * pitchDeriv);
    prevPitchError = pitchError;

    float maxTurn = Math::deg_to_rad((float)turnRate) * deltaTime;

    float finalYaw = currentYaw + Math::clamp(yawOutput, -maxTurnStep, maxTurnStep);
    set_direction(Math::rad_to_deg(finalYaw));

    float finalPitch = currentPitch + Math::clamp(pitchOutput, -maxTurnStep, maxTurnStep);
    set_pitch(Math::rad_to_deg(finalPitch));

    float totalError = Math::sqrt(yawError * yawError + pitchError * pitchError);
    float alignment = Math::clamp((float)(1.0f - (totalError / (float)Math_PI)), 0.0f, 1.0f);

    return totalError; 
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

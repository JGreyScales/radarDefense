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

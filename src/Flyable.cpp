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
    float currentDirection = Math::deg_to_rad(get_direction());
    bool hasFuel = fuelTime > 0;

    float altitudePenalty = 1.0f;
    if (maximumAltitude > 0) {
        float altitudeRatio = Math::clamp(elevation / (float)maximumAltitude, 0.0f, 1.0f);
        altitudePenalty = 1.0f - (altitudeRatio * 0.4f);
    }
    float effectiveMaxSpeed = get_max_speed() * altitudePenalty;

    if (hasFuel) {
        // --- POWERED STAGE ---
        float acceleration = calculateImpulseAcceleration(weight, engineThrustOutput);
        
        if (currentSpeed < effectiveMaxSpeed) {
            currentSpeed += acceleration * deltaTime;
            if (currentSpeed > effectiveMaxSpeed) currentSpeed = effectiveMaxSpeed;
        }
        if (fuelTime > 0) fuelTime -= deltaTime; 

    } else if (elevation > 0.0f) {
        // --- UNPOWERED STAGE ---
        
        if (currentSpeed > 0.0f) {
            currentSpeed -= 9.81f * deltaTime; 
            if (currentSpeed < 0.0f) currentSpeed = 0.0f;
        }

        if (currentSpeed <= (float)optimalTurnSpeed) {
            float safeLiftCoef = liftCoefficent > 0.0f ? (float)liftCoefficent : 1.0f;
            float descentRate = 50.0f / safeLiftCoef;
            
            elevation -= descentRate * deltaTime;
            if (elevation < 0.0f) elevation = 0.0f;
            set_z(elevation);
        } 

    } else {
        currentSpeed = 0.0f;
    }

    set_speed((uint16_t)currentSpeed);

    float dx = currentSpeed * Math::cos(currentDirection) * deltaTime;
    float dy = currentSpeed * Math::sin(currentDirection) * deltaTime;

    set_x(get_x() + dx);
    set_y(get_y() + dy);
}

void Flyable::UItick(float deltaTime) {
    GlobalManager::set_ui_name_value(this->get_name());
	GlobalManager::set_ui_xyz_value("X:" + godot::String::num((int)this->get_x()) + " Y:" + godot::String::num((int)this->get_y()) + " Z:" + godot::String::num((int)this->get_z()));
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
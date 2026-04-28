#include "Vehicle.h"


#define JET_FUEL_WEIGHT_PER_L_LBS 1.81;
#define SOLIDE_STATE_WEIGHT_PER_L_LBS 0.65;

#include <godot_cpp/core/math.hpp>


using namespace godot;
class Flyable : public Vehicle
{
private:
    bool despawnIfCantHitTarget;
    uint8_t maximumAltitude;
    float elevation;

    float fuelTime;
    float fuelWeightPerL;
    uint16_t weight;
    

    uint8_t wepTimer;
    uint8_t wepOverload;

    uint8_t turnRate;
    uint8_t optimalTurnSpeed;
    uint8_t liftCoefficent;
    float thrustDelta;
    float engineThrustOutput;

    float PID_prop_term;
    float PID_int_term;
    float PID_int_term_limit;
    float PID_der_term;
    float integralErrorSum;

    // Yaw PID state
    float prevYawError;
    float yawIntegral;

    // Pitch PID state
    float prevPitchError;
    float pitchIntegral;


    uint8_t chaffCount;
    uint8_t flareCount;

    MapIcon* interceptionTarget;

public:
    Flyable();
    ~Flyable();

    uint8_t calculateMaximumRange();
    uint8_t calculateMaximumEffectiveRange();

    float calculateImpulseAcceleration(float mass, float thrust);
    Vector2 Flyable::calculateOptimalFlightPath(MapIcon* target, float deltaTime);
    void calculateCollisionCourseMarker();
    

    void tick(float deltaTime);
    void UItick(float deltaTime);
    void move(MapIcon* target, float deltaTime);
    void avoidIncoming();
    void engage(Vehicle* target);
    Flyable* clone();

    Vector3 Flyable::getTargetVelocityVector(MapIcon* target);


};



#include "Vehicle.h"


#define JET_FUEL_WEIGHT_PER_L_LBS 1.81;
#define SOLIDE_STATE_WEIGHT_PER_L_LBS 0.65;

#include <godot_cpp/core/math.hpp>


using namespace godot;
class Flyable : public Vehicle
{
private:
    bool despawnIfCantHitTarget;
    float timeToDestruction;
    uint8_t maximumAltitude;

    float fuelTime;
    float fuelFlowRateL;
    float fuelWeightPerL;
    uint16_t weight;
    

    bool wepping;
    uint8_t wepTimer;
    uint8_t wepOverload;

    uint8_t turnRate;
    uint8_t optimalTurnSpeed;
    uint8_t liftCoefficent;
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

    void set_despawn_if_cant_hit(bool state);
    void set_time_to_destruction(float value);
    void set_maximum_altitude(uint8_t value);

    void set_fuel_time(float value);
    void set_fuel_weight_per_L(float value);
    void set_weight(uint16_t value);

    void set_wep_timer(uint8_t value);
    void set_wep_overload(uint8_t value);

    void set_turn_rate(uint8_t value);
    void set_optiomal_turn_speed(uint8_t value);
    void set_lift_coefficent(uint8_t value);
    void set_engine_thrust_output(float value);

    void set_PID_prop_term(float value);
    void set_PID_int_term(float value);
    void set_PID_der_term_limit(float value);
    void set_PPID_der_term(float value);
    void set_integral_error_sum(float value);

    void set_prev_yaw_error(float value);
    void set_yaw_integral(float value);
    void set_prev_pitch_error(float value);
    void set_pitch_integral(float value);

    void set_chaff_count(uint8_t value);
    void set_flare_count(uint8_t value);

    void set_interception_target(MapIcon* object);

    

    uint8_t calculateMaximumRange();
    uint8_t calculateMaximumEffectiveRange();

    float calculateImpulseAcceleration(float mass, float thrust);
    Vector2 Flyable::calculateOptimalFlightPath(MapIcon* target, float deltaTime);
    void calculateCollisionCourseMarker();
    

    void tick(float deltaTime);
    void UItick(float deltaTime);
    void move(MapIcon* target, float deltaTime);
    void avoid_incoming();
    void engage(Vehicle* target);
    Flyable* clone();

    Vector3 Flyable::getTargetVelocityVector(MapIcon* target);


};



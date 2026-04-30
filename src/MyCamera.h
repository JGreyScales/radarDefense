#ifndef CAMERA_PANNER_H
#define CAMERA_PANNER_H
#pragma once


#include <godot_cpp/classes/camera2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>

#include "Vehicle.h"
#include "Globals.h"

namespace godot {

class MyCamera : public Camera2D {
    GDCLASS(MyCamera, Camera2D)

private:

    static MyCamera *singleton;

    Vehicle* follow_target = nullptr;
    bool is_following = false;
    Vector2 drag_start = Vector2(0, 0);
    bool is_selecting = false;
    bool is_panning = false;
    double pan_speed = 1.0; 
    double zoom_speed = 0.1;

protected:
    static void _bind_methods();

public:
    MyCamera();
    ~MyCamera();

    static MyCamera* getSingleton();
    void set_follow_target(Vehicle* p_vehicle);
    void _process(double deltatime);
    void _perform_box_selection(Vector2 p_begin, Vector2 p_end);
    void _input(const Ref<InputEvent> &p_event) override;
};

}

#endif
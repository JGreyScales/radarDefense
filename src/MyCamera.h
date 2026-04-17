#ifndef CAMERA_PANNER_H
#define CAMERA_PANNER_H


#include <godot_cpp/classes/camera2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>

namespace godot {

class MyCamera : public Camera2D {
    GDCLASS(MyCamera, Camera2D)

private:
    bool is_panning = false;
    double pan_speed = 1.0; 
    double zoom_speed = 0.1;

protected:
    static void _bind_methods();

public:
    MyCamera();
    ~MyCamera();

    void _input(const Ref<InputEvent> &p_event) override;
};

}

#endif
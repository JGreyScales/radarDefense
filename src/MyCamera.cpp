#include "MyCamera.h"
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void MyCamera::_bind_methods() {}

MyCamera::MyCamera() {
    set_process_input(true);
}

MyCamera::~MyCamera() {}

void MyCamera::_input(const Ref<InputEvent> &p_event) {
    Ref<InputEventMouseButton> mb = p_event;
    if (mb.is_valid()) {
        // --- PANNING LOGIC ---
        if (mb->get_button_index() == MOUSE_BUTTON_MIDDLE) {
            is_panning = mb->is_pressed();
        }

        // --- ZOOMING LOGIC ---
        if (mb->is_pressed()) {
            Vector2 current_zoom = get_zoom();
            
            if (mb->get_button_index() == MOUSE_BUTTON_WHEEL_UP) {
                current_zoom += Vector2(zoom_speed, zoom_speed);
            } 
            else if (mb->get_button_index() == MOUSE_BUTTON_WHEEL_DOWN) {
                current_zoom -= Vector2(zoom_speed, zoom_speed);
            }

            current_zoom.x = Math::clamp(current_zoom.x, (real_t)0.05, (real_t)10.0);
            current_zoom.y = Math::clamp(current_zoom.y, (real_t)0.05, (real_t)10.0);
            
            set_zoom(current_zoom);
        }
    }

    // --- MOTION LOGIC ---
    Ref<InputEventMouseMotion> mm = p_event;
    if (mm.is_valid() && is_panning) {
        Vector2 relative_motion = mm->get_relative();
        Vector2 zoom_factor = get_zoom();

        // We divide by zoom so that the pan stays "pinned" to the cursor
        Vector2 offset = relative_motion / zoom_factor;
        set_position(get_position() - offset);
    }
}
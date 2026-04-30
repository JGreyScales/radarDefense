#include "MyCamera.h"
#include <godot_cpp/variant/rect2.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

MyCamera *MyCamera::singleton = nullptr;
void MyCamera::_bind_methods() {}

MyCamera::MyCamera() {
	set_process_input(true);
    MyCamera::singleton = this;
}

MyCamera::~MyCamera() {
    if (singleton == this){
        singleton = nullptr;
    }
}

void MyCamera::_process(double delta) {
    if (is_following && follow_target != nullptr) {
        Vector2 target_pos = follow_target->get_global_position();
        Vector2 current_pos = get_global_position();
        set_global_position(current_pos.lerp(target_pos, 5.0 * delta)); 
    }
}

void MyCamera::_perform_box_selection(Vector2 p_begin, Vector2 p_end) {
	Rect2 selection_rect = Rect2(p_begin, Vector2(0, 0));
	selection_rect.expand_to(p_end);

	Vector<Vehicle *> vehicles = GlobalManager::get_singleton()->get_active_vehicles();

	for (int i = 0; i < vehicles.size(); i++) {
		Vehicle *v = vehicles[i];
		if (!v)
			continue;

		if (selection_rect.has_point(v->get_global_position())) {
			GlobalManager::get_singleton()->set_selected_vehicle(v);
			break;
		}
	}
}

void MyCamera::_input(const Ref<InputEvent> &p_event) {
    // 1. HANDLE MOUSE BUTTONS (Click, Release, Scroll)
    Ref<InputEventMouseButton> mb = p_event;
    if (mb.is_valid()) {

        // --- SELECTION LOGIC ---
        if (mb->get_button_index() == MOUSE_BUTTON_LEFT) {
            if (mb->is_pressed()) {
                is_selecting = true;
                // We use global position so the box stays anchored to the world map
                drag_start = get_global_mouse_position();
            } else if (is_selecting) {
                is_selecting = false;
                _perform_box_selection(drag_start, get_global_mouse_position());
                queue_redraw(); // Clear the box from the screen
            }
        }

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

            current_zoom.x = Math::clamp(current_zoom.x, (real_t)0.01, (real_t)11.0);
            current_zoom.y = Math::clamp(current_zoom.y, (real_t)0.01, (real_t)11.0);
            
            set_zoom(current_zoom);
        }
    }

    Ref<InputEventMouseMotion> mm = p_event;
    if (mm.is_valid()) {
        if (is_panning) {
            is_following = false;
            follow_target = nullptr;

            Vector2 relative_motion = mm->get_relative();
            Vector2 zoom_factor = get_zoom();
            Vector2 offset = relative_motion / zoom_factor;
            set_position(get_position() - offset);
        }

        if (is_selecting) {
            queue_redraw();
        }
    }
}

MyCamera* godot::MyCamera::getSingleton() {
    return MyCamera::singleton;
}

void MyCamera::set_follow_target(Vehicle *p_vehicle) {
	follow_target = p_vehicle;
    is_following = (p_vehicle != nullptr);
}
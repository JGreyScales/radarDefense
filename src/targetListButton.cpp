#include "targetListButton.h"

using namespace godot;

void TargetListButton::_bind_methods() {
    ClassDB::bind_method(D_METHOD("setup", "p_text", "p_obj", "p_group"), &TargetListButton::setup);
    ClassDB::bind_method(D_METHOD("_on_toggled", "p_pressed"), &TargetListButton::_on_toggled);

    ADD_SIGNAL(MethodInfo("vehicle_selected", PropertyInfo(Variant::OBJECT, "vehicle_ptr")));
    ADD_SIGNAL(MethodInfo("vehicle_deselected", PropertyInfo(Variant::OBJECT, "vehicle_ptr")));
}

TargetListButton::TargetListButton() {
    
}

TargetListButton::~TargetListButton() {
}

void TargetListButton::setup(String p_text, Vehicle *p_obj, Ref<ButtonGroup> p_group) {
	set_text(p_text);
    this->targetObject = p_obj;

    set_toggle_mode(true);

    if (p_group.is_valid()) {
        set_button_group(p_group);
    }

    if (!is_connected("toggled", Callable(this, "_on_toggled"))) {
        connect("toggled", Callable(this, "_on_toggled"));
    }
}

Vehicle *TargetListButton::get_target_object() {
	return this->targetObject;
}

void TargetListButton::_on_toggled(bool p_pressed) {
    if (this->targetObject == nullptr) return;

    if (p_pressed) {
        add_theme_color_override("font_color", Color(0.0, 1.0, 1.0)); 

        emit_signal("vehicle_selected", this->targetObject);
    } else {
        remove_theme_color_override("font_color");

        emit_signal("vehicle_deselected", this->targetObject);
    }
}
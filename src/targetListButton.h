#ifndef TARGET_LIST_BUTTON_H
#define TARGET_LIST_BUTTON_H

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/button_group.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "Vehicle.h"

using namespace godot;

class TargetListButton : public Button {
    GDCLASS(TargetListButton, Button);

private:
    Vehicle* targetObject = nullptr;

protected:
    static void _bind_methods();

public:
    TargetListButton();
    ~TargetListButton();

    void setup(String p_text, Vehicle* p_obj, Ref<ButtonGroup> p_group);
    Vehicle* get_target_object();
    void _on_toggled(bool p_pressed);
};

#endif
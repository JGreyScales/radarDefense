#ifndef MAP_ICON_H
#define MAP_ICON_H

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/theme_db.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/node.hpp>

namespace godot {
class GlobalManager;

class MapIcon : public Sprite2D {
    GDCLASS(MapIcon, Sprite2D)

private:

    String name;
    String iconPath;

    float x;
    float y;
    float z;

protected:   
    static void _bind_methods();
    void _notification(int p_what);

public:

    float currentScaleFactor;

    MapIcon();
    MapIcon(String path, String name);
    ~MapIcon();

    virtual void _draw() override;
    virtual String get_display_content();

    void force_redraw();

    // Getter and Setter for the property
    void set_path(const String path);
    void set_name(const String name);
    void set_x(float x);
    void set_y(float y);
    void set_z(float z);

    String get_path();
    String get_name();
    float get_x();
    float get_y();
    float get_z();
};

}

#endif
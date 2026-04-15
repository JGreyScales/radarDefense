#ifndef MAP_ICON_H
#define MAP_ICON_H

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

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

public:
    MapIcon();
    MapIcon(String path, String name);
    ~MapIcon();

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
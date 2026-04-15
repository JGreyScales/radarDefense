#include "MapIcon.h"
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void MapIcon::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_path"), &MapIcon::get_path);
    ClassDB::bind_method(D_METHOD("set_path", "path"), &MapIcon::set_path);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "icon_path", PROPERTY_HINT_FILE, "*.svg,*.png"), "set_path", "get_path");

    ClassDB::bind_method(D_METHOD("get_name"), &MapIcon::get_name);
    ClassDB::bind_method(D_METHOD("set_name", "name"), &MapIcon::set_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "icon_name"), "set_name", "get_name");

    ClassDB::bind_method(D_METHOD("get_x"), &MapIcon::get_x);
    ClassDB::bind_method(D_METHOD("set_x", "x"), &MapIcon::set_x);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "x"), "set_x", "get_x");

    ClassDB::bind_method(D_METHOD("get_y"), &MapIcon::get_y);
    ClassDB::bind_method(D_METHOD("set_y", "y"), &MapIcon::set_y);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "y"), "set_y", "get_y");

    ClassDB::bind_method(D_METHOD("get_z"), &MapIcon::get_z);
    ClassDB::bind_method(D_METHOD("set_z", "z"), &MapIcon::set_z);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "z"), "set_z", "get_z");
}

MapIcon::MapIcon() {
    iconPath = "";
    name = "New Icon";
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

// calls default constructor first
MapIcon::MapIcon(String iconPath, String name) : MapIcon() {
    this->iconPath = iconPath;
    this->name = name;
}

MapIcon::~MapIcon() {}

// Setters
void MapIcon::set_path(const String path) {
    iconPath = path;
    if (!iconPath.is_empty()) {
        Ref<Texture2D> texture = ResourceLoader::get_singleton()->load(iconPath);
        if (texture.is_valid()) {
            set_texture(texture);
        }
    }
}

void MapIcon::set_name(const String p_name) {
    name = p_name;
}

void MapIcon::set_x(float p_x) {
    x = p_x;
    Vector2 pos = get_position();
    pos.x = x;
    set_position(pos);
}

void MapIcon::set_y(float p_y) {
    y = p_y;
    Vector2 pos = get_position();
    pos.y = y;
    set_position(pos);
}

void MapIcon::set_z(float p_z) {
    z = p_z;
    set_z_index((int)z);
}

// Getters
String MapIcon::get_path() { return iconPath; }
String MapIcon::get_name() { return name; }
float MapIcon::get_x() { return x; }
float MapIcon::get_y() { return y; }
float MapIcon::get_z() { return z; }
#include "MapIcon.h"

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

void MapIcon::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY:
			break;
	}
}

MapIcon::MapIcon() {
	this->iconPath = "";
	name = "";
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

// calls default constructor first
MapIcon::MapIcon(String iconPath, String name) : MapIcon() {
	this->set_path(iconPath);
	this->name = name;
}

MapIcon::~MapIcon() {}

void MapIcon::_draw() {
	Ref<Font> font = ThemeDB::get_singleton()->get_fallback_font();
	if (font.is_null())
		return;

	float x_offset = 0.0f;
	float y_offset = 0.0f;
	Ref<Texture2D> tex = get_texture();

	if (tex.is_valid()) {
		if (is_centered()) {
			x_offset = tex->get_width() / 2.0f;
			y_offset = tex->get_height() / 2.0f;
		} else {
			x_offset = tex->get_width();
		}
	}

	// draw the text at the top right, 10 pixels to the right of the texture edge
	// and then 5 pixels down
	draw_multiline_string(font, Vector2(x_offset + 10.0f, -y_offset + 5.0f), get_display_content());
}

String MapIcon::get_display_content() {
	if (this->name.is_empty()) {
		return "";
	}

	return "Name: " + this->get_name();
}

// Setters
void MapIcon::set_path(const String path) {
	this->iconPath = path;
	if (!iconPath.is_empty()) {
		Ref<Texture2D> texture = ResourceLoader::get_singleton()->load(iconPath);
		if (texture.is_valid()) {
			set_texture(texture);
		}
	}
}

void MapIcon::set_name(const String name) {
	this->name = name;
	queue_redraw();
}

void MapIcon::set_x(float x) {
	this->x = x;
	Vector2 pos = get_position();
	pos.x = this->x;
	set_position(pos);
	queue_redraw();
}

void MapIcon::set_y(float p_y) {
	y = p_y;
	Vector2 pos = get_position();
	pos.y = y;
	set_position(pos);
	queue_redraw();
}

void MapIcon::set_z(float p_z) {
	z = p_z;
	queue_redraw();
}

// Getters
String MapIcon::get_path() { return iconPath; }
String MapIcon::get_name() { return name; }
float MapIcon::get_x() { return x; }
float MapIcon::get_y() { return y; }
float MapIcon::get_z() { return z; }
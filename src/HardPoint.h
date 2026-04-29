#ifndef HARDPOINT_H
#define HARDPOINT_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/templates/vector.hpp>

namespace godot {
    class Weapon;

    class HardPoint : public Object {
        GDCLASS(HardPoint, Object);


    // protected:
    //     static void _bind_methods();

    private:
        Weapon* equippedWeapon;
        Vector<Weapon*> options;

    protected:
        static void _bind_methods();

    public:
        HardPoint();
        ~HardPoint();

    PackedStringArray getWeaponOptionNames();
    void addWeaponOption(Weapon*);
    Weapon* getEquippedWeapon();
    void setEquippedWeapon(Weapon*);
    };
}
#endif
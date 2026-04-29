#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "Globals.h"
#include "SearchRadar.h"
#include "MyCamera.h"
#include "targetListButton.h"
#include "Loadout.h"
#include "Flyable.h"

using namespace godot;

static GlobalManager* _global_manager = nullptr;
HashMap<String, Vehicle*> GlobalManager::vehicleRegistry;
HashMap<String, Radar*> GlobalManager::radarRegistry;
HashMap<String, Weapon*> GlobalManager::weaponRegistry;

void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	UtilityFunctions::print("Loading generics");
	GDREGISTER_CLASS(MapIcon);	
	GDREGISTER_ABSTRACT_CLASS(Vehicle);
	GDREGISTER_ABSTRACT_CLASS(Radar);
	GDREGISTER_CLASS(GlobalManager);
	GDREGISTER_CLASS(SearchRadar);
	GDREGISTER_CLASS(Driveable);
	GDREGISTER_CLASS(Flyable);
	GDREGISTER_CLASS(MyCamera);
	GDREGISTER_CLASS(TargetListButton);

	UtilityFunctions::print("Loading loadout");
	GDREGISTER_CLASS(Loadout);
	GDREGISTER_CLASS(HardPoint);
	GDREGISTER_CLASS(Weapon);

	UtilityFunctions::print("Loading radars");
	GDREGISTER_CLASS(semiActiveRadar);
	GDREGISTER_CLASS(activeRadar);

	
	UtilityFunctions::print("Loading objects done");

	_global_manager = memnew(GlobalManager);

	Engine::get_singleton()->register_singleton("GlobalManager", GlobalManager::get_singleton());
	UtilityFunctions::randomize();
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		Engine::get_singleton()->unregister_singleton("GlobalManager");
        memdelete(_global_manager);
	}
}

extern "C"
{
	// Initialization
	GDExtensionBool GDE_EXPORT radarDefense_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}
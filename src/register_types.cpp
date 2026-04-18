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

using namespace godot;

static GlobalManager* _global_manager = nullptr;
HashMap<String, Vehicle*> GlobalManager::vehicleRegistry;
HashMap<String, Radar*> GlobalManager::radarRegistry;

void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GDREGISTER_CLASS(MapIcon);
	GDREGISTER_ABSTRACT_CLASS(Vehicle);
	GDREGISTER_ABSTRACT_CLASS(Radar);
	GDREGISTER_CLASS(GlobalManager);
	GDREGISTER_CLASS(SearchRadar);
	GDREGISTER_CLASS(Driveable);
	GDREGISTER_CLASS(MyCamera);
	GDREGISTER_CLASS(TargetListButton);

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
#pragma once
#include "itemBox/ObjItemBox.h"
#include "itemBox/UIItemBox.h"
#include "ObjHomingGismo.h"
#include "custom/ObjTestBoss.h"

namespace revisited::objects {
	template<typename T>
	void registerRFL() {
		hh::fnd::RflClassNameRegistry::GetInstance()->Register(&T::rflClass);
		hh::fnd::RflTypeInfoRegistry::GetInstance()->Register(&T::rflTypeInfo);
	}

	template<typename T>
	void registerObject() {
		const hh::game::GameObjectClass* obj = T::GetClass();
		hh::game::GameObjectSystem::GetInstance()->gameObjectRegistry->AddObject(&obj);
	}

	void bootstrap() {
		registerRFL<ObjItemBoxSpawner>();
		registerRFL<ObjHomingGismoSpawner>();
		registerRFL<ObjTestBossSpawner>();
		registerObject<ObjItemBox>();
		registerObject<ObjHomingGismo>();
		registerObject<ObjTestBoss>();

		auto* allocator = hh::fnd::MemoryRouter::GetModuleAllocator();
		auto* resLoader = hh::fnd::ResourceLoader::Create(allocator);

		resLoader->LoadPackfile("stage/RevisitedObject.pac");
		resLoader->LoadPackfile("ui/ui_revisited_stage.pac");
		hh::fnd::InplaceTempUri<> uri{ "sound/revisited_sound/se_revisited_object.acb"};
		hh::fnd::ResourceLoader::Locale locale{};
		resLoader->LoadResource(uri, hh::snd::ResAtomCueSheet::GetTypeInfo(), 0, 0, locale);
	}
}

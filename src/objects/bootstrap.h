#pragma once
#include "dv/DvIslandIntro.h"
#include "itemBox/ObjItemBox.h"
#include "itemBox/UIItemBox.h"
#include "ObjHomingGismo.h"
#include "titlecard/UIStageIntro.h"
#include "result/ResultHooks.h"
#include "result/UIResultModel.h"
#include "UIGameCockpitHooks.h"
//#include "custom/ObjTestBoss.h"

HOOK(uint64_t, __fastcall, GameModeCyberStageInit, 0x1401B4100, app::game::GameMode* self) {
	auto res = originalGameModeCyberStageInit(self);
	self->gameManager->AddGameObject(hh::game::GameObject::Create<revisited::objects::UIStageIntro>(self->GetAllocator()), "UIStageIntro", false, nullptr, nullptr);
	return res;
}

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
		registerRFL<StageIntro>();
		registerRFL<Stage>();
		//registerRFL<ObjTestBossSpawner>();
		registerObject<ObjItemBox>();
		registerObject<ObjHomingGismo>();
		registerObject<UIResultModel>();
		//registerObject<ObjTestBoss>();

		auto* allocator = hh::fnd::MemoryRouter::GetModuleAllocator();
		auto* resLoader = hh::fnd::ResourceLoader::Create(allocator);

		resLoader->LoadPackfile("stage/RevisitedObject.pac");
		resLoader->LoadPackfile("ui/ui_revisited_stage.pac");
		hh::fnd::InplaceTempUri<> uri{ "sound/revisited_sound/se_revisited_object.acb"};
		hh::fnd::ResourceLoader::Locale locale{};
		resLoader->LoadResource(uri, hh::snd::ResAtomCueSheet::GetTypeInfo(), 0, 0, locale);

		INSTALL_HOOK(GameModeCyberStageInit);

		INSTALL_HOOK(ResultAddCallback);

		cockpit::bootstrap();
	}
}

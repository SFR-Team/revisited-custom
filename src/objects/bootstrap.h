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

HOOK(void, __fastcall, GameModeResourceModuleLoad, 0x1401E8C90, app::game::GameModeResourceModule* self) {
	originalGameModeResourceModuleLoad(self);
	if (auto* objInfoContainer = hh::game::GameManager::GetInstance()->GetService<hh::game::ObjInfoContainer>()) {
		auto* itemBox = revisited::objects::ObjItemBoxInfo::GetClass()->instantiator(self->GetAllocator());
		objInfoContainer->Register(itemBox->GetInfoName(), itemBox);
	}
};

namespace revisited::objects {
	template<typename T>
	void registerRFL() {
		hh::fnd::RflClassNameRegistry::GetInstance()->Register(&T::rflClass);
		hh::fnd::RflTypeInfoRegistry::GetInstance()->Register(&T::rflTypeInfo);
	}

	template<typename T>
	void registerObjInfo() {
		hh::game::ObjInfoClass* obj = (hh::game::ObjInfoClass*)T::GetClass();
		auto* instance = hh::game::ObjInfoRegistry::GetInstance();
		instance->objInfos.push_back(obj);
		instance->objInfosByName.Insert(obj->name, obj);
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
		registerObjInfo<ObjItemBoxInfo>();
		//registerRFL<ObjTestBossSpawner>();
		registerObject<ObjItemBox>();
		registerObject<ObjHomingGismo>();
		//registerObject<ObjTestBoss>();

		auto* allocator = hh::fnd::MemoryRouter::GetModuleAllocator();
		auto* resLoader = hh::fnd::ResourceLoader::Create(allocator);

		resLoader->LoadPackfile("ui/ui_revisited_stage.pac");

		INSTALL_HOOK(GameModeCyberStageInit);
		INSTALL_HOOK(GameModeResourceModuleLoad);

		INSTALL_HOOK(ResultAddCallback);

		cockpit::bootstrap();
	}
}

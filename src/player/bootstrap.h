#pragma once
#include "BlackboardRevisited.h"
#include "StateHooks.h"

HOOK(uint64_t, __fastcall, PlayerAddCallback, 0x140880810, app::player::Player* self, hh::game::GameManager* gameManager) {
	auto res = originalPlayerAddCallback(self, gameManager);
	auto& blackboard = self->GetComponent<app::player::GOCPlayerBlackboard>()->blackboard;
	revisited::player::BlackboardRevisited* blackboardRevisited = revisited::player::BlackboardRevisited::Create(blackboard->GetAllocator());
	blackboard->AddContent(blackboardRevisited);
	return res;
}

inline bool IsNonDamageMessage(unsigned int messageId) {
	return messageId != 8428 && messageId != 8384 && messageId != 8385;
}

inline bool IsDamageMessage(unsigned int messageId) {
	return messageId == 8428 || messageId == 8384 || messageId == 8385;
}

HOOK(uint64_t, __fastcall, MessageHandler, 0x14091CF80, app::player::PlayerHsmContext* self, hh::fnd::Message* message) {
	unsigned int id = (int)message->ID;
	if (auto* x = self->gocPlayerBlackboard) {
		if (auto* blackboard = x->blackboard->GetContent<revisited::player::BlackboardRevisited>()) {
			if (blackboard->flags.test(revisited::player::BlackboardRevisited::Flags::ULTRA)) {
				if (IsNonDamageMessage(id))
					return originalMessageHandler(self, message);
				else
					return true;
			}
			if (blackboard->flags.test(revisited::player::BlackboardRevisited::Flags::MAGNETIC)) {
				if (IsDamageMessage(id)) {
					blackboard->RemoveMagnetic();
					return true;
				}
			}
		}
	}
	return originalMessageHandler(self, message);
}

// Enable Cyloop in Cyberspace - TODO: Fix crash when trying to spawn ObjSequenceItem

// TODO: Map StatePluginCyloop
FUNCTION_PTR(app::player::StatePlugin<app::player::PlayerHsmContext>*, __fastcall, StatePluginCyloopCtor, 0x1409A4080, void*, csl::fnd::IAllocator*);

HOOK(void, __fastcall, InitializePlayer, 0x14088AC20, app::player::Player* self) {
	originalInitializePlayer(self);
	auto& playerHsm = self->GetComponent<app::player::GOCPlayerHsm>()->statePluginManager;
	auto* plugin = StatePluginCyloopCtor(playerHsm->GetAllocator()->Alloc(216, 8), playerHsm->GetAllocator());
	plugin->context = playerHsm->context;
	playerHsm->AddPlugin(plugin);
}

// This disables any kind of ObjSkillPoint spawn using a special function, if the player is in CyberSpace
// It's a fix for Cyber Cyloop
HOOK(void, __fastcall, sub_14049AEB0, 0x14049AEB0, int64_t self, int64_t a2, float a3, char a4) {
	if (auto* stageData = hh::game::GameManager::GetInstance()->GetService<app::level::LevelInfo>()->GetStageData())
		if (stageData->GetAttributeFlag(app::level::StageData::AttributeFlags::CYBER))
			return;

	originalsub_14049AEB0(self, a2, a3, a4);
}

namespace revisited::player {
	void bootstrap() {
		auto* allocator = hh::fnd::MemoryRouter::GetModuleAllocator();
		auto* resLoader = hh::fnd::ResourceLoader::Create(allocator);
		hh::fnd::InplaceTempUri<> uri{ "sound/revisited_sound/bgm_revisited.acb" };
		hh::fnd::ResourceLoader::Locale locale{};
		resLoader->LoadResource(uri, hh::snd::ResAtomCueSheet::GetTypeInfo(), 0, 0, locale);

		INSTALL_HOOK(PlayerAddCallback);
		INSTALL_HOOK(MessageHandler);
		INSTALL_HOOK(InitializePlayer);
		INSTALL_HOOK(sub_14049AEB0);

		bootstrapStates();
	}
}
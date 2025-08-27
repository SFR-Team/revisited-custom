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

namespace revisited::player {
	void bootstrap() {
		auto* allocator = hh::fnd::MemoryRouter::GetModuleAllocator();
		auto* resLoader = hh::fnd::ResourceLoader::Create(allocator);
		hh::fnd::InplaceTempUri<> uri{ "sound/revisited_sound/bgm_revisited.acb" };
		hh::fnd::ResourceLoader::Locale locale{};
		resLoader->LoadResource(uri, hh::snd::ResAtomCueSheet::GetTypeInfo(), 0, 0, locale);

		INSTALL_HOOK(PlayerAddCallback);
		INSTALL_HOOK(MessageHandler);

		bootstrapStates();
	}
}
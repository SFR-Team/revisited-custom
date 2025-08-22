#pragma once
#include "BlackboardRevisited.h"
#include "StateHooks.h"

HOOK(uint64_t, __fastcall, PlayerAddCallback, 0x140880810, app::player::Player* self, hh::game::GameManager* gameManager) {
	auto res = originalPlayerAddCallback(self, gameManager);
	revisited::player::BlackboardRevisited* blackboardRevisited = revisited::player::BlackboardRevisited::Create(self->GetAllocator());
	self->GetComponent<app::player::GOCPlayerBlackboard>()->blackboard->contents.push_back(blackboardRevisited);
	return res;
}

HOOK(uint64_t, __fastcall, MessageHandler, 0x14091CF80, app::player::GOCPlayerHsm* self, hh::fnd::Message* message) {
	if (auto* x = GetPlayer()->GetComponent<app::player::GOCPlayerBlackboard>()) {
		if (auto* blackboard = x->blackboard->GetContent<revisited::player::BlackboardRevisited>()) {
			if (blackboard->flags.test(revisited::player::BlackboardRevisited::Flags::ULTRA)) {
				if ((int)message->ID != 8428 && (int)message->ID != 8384 && (int)message->ID != 8385)
					return originalMessageHandler(self, message);
				else
					return true;
			}
			if (blackboard->flags.test(revisited::player::BlackboardRevisited::Flags::MAGNETIC)) {
				if ((int)message->ID == 8428 || (int)message->ID == 8384 || (int)message->ID == 8385) {
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
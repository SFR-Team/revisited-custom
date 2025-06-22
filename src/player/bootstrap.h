#pragma once
#include "BlackboardRevisited.h"

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

HOOK(bool, __fastcall, GOCAnimatorChangeState, 0x140E92B00, hh::anim::GOCAnimator* self, const char* stateName) {
	return originalGOCAnimatorChangeState(self, stateName);
}

namespace revisited::player {
	void bootstrap() {
		INSTALL_HOOK(PlayerAddCallback);
		INSTALL_HOOK(MessageHandler);

		INSTALL_HOOK(GOCAnimatorChangeState);
	}
}
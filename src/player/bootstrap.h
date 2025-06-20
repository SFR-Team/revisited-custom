#pragma once
#include "BlackboardRevisited.h"

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
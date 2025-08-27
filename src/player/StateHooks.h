#pragma once

void DamageWhileBoost(app::player::PlayerHsmContext* ctx) {
	static bool wasDamageSet = false;

	if (auto* collision = ctx->gocPlayerHsm->statePluginManager->GetPlugin<app::player::StatePluginCollision>()) {
	if (auto* battle = ctx->gocPlayerBlackboard->blackboard->GetContent<app::player::BlackboardBattle>()) {
		if (auto* status = ctx->blackboardStatus) {
			bool boosting = status->GetStateFlag(app::player::BlackboardStatus::StateFlag::BOOST);
			//bool powerBoost = status->GetWorldFlag(app::player::BlackboardStatus::WorldFlag::POWER_BOOST);
			if (boosting &&
				//powerBoost &&
				!wasDamageSet)
			{
				collision->SetTypeAndRadius(2, true);
				battle->SetFlag020(false);
				wasDamageSet = true;
			}
			else if (wasDamageSet) {
				collision->SetTypeAndRadius(2, false);
				battle->SetFlag020(true);
				wasDamageSet = false;
			}
		}
	}
	}
}

HOOK(bool, __fastcall, StateRunUpdateAsync, 0x140964B00, app::player::PlayerStateBase* self, app::player::PlayerHsmContext* ctx) {
	auto res = originalStateRunUpdateAsync(self, ctx);

	DamageWhileBoost(ctx);

	return res;
}

namespace revisited::player {
	void bootstrapStates() {
		INSTALL_HOOK(StateRunUpdateAsync);
	}
}

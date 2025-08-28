#pragma once

void DamageWhileBoost(app::player::PlayerHsmContext* ctx, bool boost = false) {
	static bool wasDamageSet = false;

	if (auto* collision = ctx->gocPlayerHsm->statePluginManager->GetPlugin<app::player::StatePluginCollision>()) {
	if (auto* battle = ctx->gocPlayerBlackboard->blackboard->GetContent<app::player::BlackboardBattle>()) {
		if (auto* status = ctx->blackboardStatus) {
			bool boosting = status->GetStateFlag(app::player::BlackboardStatus::StateFlag::BOOST);
			bool powerBoost = status->GetWorldFlag(app::player::BlackboardStatus::WorldFlag::POWER_BOOST);
			if (boost || 
				boosting &&
				powerBoost &&
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

HOOK(bool, __fastcall, StateRunStep, 0x140964B00, app::player::PlayerStateBase* self, app::player::PlayerHsmContext* ctx) {
	auto res = originalStateRunStep(self, ctx);

	DamageWhileBoost(ctx);

	return res;
}

HOOK(bool, __fastcall, StateGrindStep, 0x14B618E60, app::player::PlayerStateBase* self, app::player::PlayerHsmContext* ctx) {
	auto res = originalStateGrindStep(self, ctx);

	DamageWhileBoost(ctx);

	return res;
}

HOOK(bool, __fastcall, StateAirBoostStep, 0x14B577D60, app::player::PlayerStateBase* self, app::player::PlayerHsmContext* ctx) {
	auto res = originalStateAirBoostStep(self, ctx);

	DamageWhileBoost(ctx);

	return res;
}

HOOK(bool, __fastcall, StateLeftStepRunUpdate, 0x140965B00, app::player::PlayerStateBase* self, app::player::PlayerHsmContext* ctx) {
	auto res = originalStateLeftStepRunUpdate(self, ctx);

	DamageWhileBoost(ctx);

	return res;
}

HOOK(bool, __fastcall, StateRightStepRunUpdate, 0x140965B00, app::player::PlayerStateBase* self, app::player::PlayerHsmContext* ctx) {
	auto res = originalStateRightStepRunUpdate(self, ctx);

	DamageWhileBoost(ctx);

	return res;
}

HOOK(bool, __fastcall, StateDriftDashStep, 0x1409566D0, app::player::PlayerStateBase* self, app::player::PlayerHsmContext* ctx) {
	auto res = originalStateDriftDashStep(self, ctx);

	DamageWhileBoost(ctx, true);

	return res;
}

HOOK(void, __fastcall, StateDriftDashLeave, 0x140956450, app::player::PlayerStateBase* self, app::player::PlayerHsmContext* ctx, int nextState) {
	originalStateDriftDashLeave(self, ctx, nextState);

	DamageWhileBoost(ctx, false);
}

HOOK(bool, __fastcall, StateDropDashStep, 0x140956D20, app::player::PlayerStateBase* self, app::player::PlayerHsmContext* ctx) {
	auto res = originalStateDropDashStep(self, ctx);

	DamageWhileBoost(ctx, true);

	return res;
}

HOOK(void, __fastcall, StateDropDashLeave, 0x140956CA0, app::player::PlayerStateBase* self, app::player::PlayerHsmContext* ctx, int nextState) {
	originalStateDropDashLeave(self, ctx, nextState);

	DamageWhileBoost(ctx, false);
}

namespace revisited::player {
	void bootstrapStates() {
		INSTALL_HOOK(StateRunStep);
		INSTALL_HOOK(StateGrindStep);
		INSTALL_HOOK(StateAirBoostStep);
		INSTALL_HOOK(StateLeftStepRunUpdate);
		INSTALL_HOOK(StateRightStepRunUpdate);
		INSTALL_HOOK(StateDriftDashStep);
		INSTALL_HOOK(StateDriftDashLeave);
		INSTALL_HOOK(StateDropDashStep);
		INSTALL_HOOK(StateDropDashLeave);
	}
}

#include "BlackboardRevisited.h"

using namespace app::ui;
using namespace app::snd;

using namespace app_cmn::fsm;

using namespace revisited::player;


FUNCTION_PTR(void, __fastcall, PlayerPlayEffect, 0x1409A78E0, StatePluginBase*, int, const char*);
FUNCTION_PTR(void, __fastcall, PlayerStopEffect, 0x14B9DBBF0, StatePluginBase*, int);

void BlackboardRevisited::PreGameUpdateCallback(GameManager* gameManager, const SUpdateInfo& updateInfo)
{
	if (flags.test(Flags::ULTRA) && ultraTime > 0)
		ultraTime -= updateInfo.deltaTime;
	if (flags.test(Flags::ULTRA) && ultraTime <= 0) {
		StopEffects("ultra", invincibilityEffects);
		flags.set(Flags::ULTRA, false);
	}

	if (flags.test(Flags::INFINITE_BOOST) && infiniteBoost > 0) {
		infiniteBoost -= updateInfo.deltaTime;
		boostPlugin->boostAmount = boostPlugin->boostCapacity;
	}
	if (flags.test(Flags::INFINITE_BOOST) && infiniteBoost <= 0) {
		uiStaminaGaugeCtx->unk1 &= ~0x04;
		flags.set(Flags::INFINITE_BOOST, false);
	}

	if (flags.test(Flags::MAGNETIC))
		if (auto* moveSphereCollider = GetPlayer()->GetComponent<app::physics::GOCMoveSphereColliderQuery>())
			moveSphereCollider->radius = 12.5f;
}

void BlackboardRevisited::CutsceneEnd(const char* cutsceneName)
{
	if (flags.test(Flags::MAGNETIC))
		PlayEffects("shield", shieldEffects);
}

void BlackboardRevisited::GiveInvincibility()
{
	PlayEffects("ultra", invincibilityEffects);
	flags.set(Flags::ULTRA, true);
	ultraTime = 20;
	if (auto* soundDirector = GameManager::GetInstance()->GetService<SoundDirector>())
		soundDirector->PlayBgm({ "bgm_maintheme", 2, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0x10001, 0 });
}

void BlackboardRevisited::GiveMagnetic()
{
	if (flags.test(Flags::MAGNETIC))
		return;

	PlayEffects("shield", shieldEffects);
	flags.set(Flags::MAGNETIC, true);
}

void BlackboardRevisited::GiveInfiniteBoost()
{
	if(!boostPlugin)
		boostPlugin = GetPlayer()->GetComponent<GOCPlayerHsm>()->statePluginManager->GetPlugin<StatePluginBoost>();
	if (!uiStaminaGaugeCtx)
		uiStaminaGaugeCtx = ((GOCTinyFsm2<UIStaminaGaugeContext>*)GameManager::GetInstance()->GetGameObject("UIGameCockpit")->components[8])->targetObject;

	flags.set(Flags::INFINITE_BOOST, true);
	boostPlugin->infiniteBoostTimer = 35;
	infiniteBoost = 35;
	uiStaminaGaugeCtx->unk1 |= 0x04;
}

void BlackboardRevisited::RemoveMagnetic()
{
	StopEffects("shield", shieldEffects);
	flags.set(Flags::MAGNETIC, false);
	if (auto* moveSphereCollider = GetPlayer()->GetComponent<app::physics::GOCMoveSphereColliderQuery>())
		moveSphereCollider->radius = 1.6f;
}

template<size_t T>
void BlackboardRevisited::PlayEffects(const char* name, const char* const (&names)[T])
{
	auto* plugin = GetPlayer()->GetComponent<GOCPlayerHsm>()->statePluginManager->GetPlugin(csl::ut::HashString("StatePluginEffect"));
	for (int i = 0; i < T; i++) {
		char buffer[16];
		snprintf(buffer, sizeof(buffer), "%s%d", name, i);
		PlayerPlayEffect(plugin, csl::ut::HashString(buffer), names[i]);
	}
}

template<size_t T>
void BlackboardRevisited::StopEffects(const char* name, const char* const (&names)[T])
{
	auto* plugin = GetPlayer()->GetComponent<GOCPlayerHsm>()->statePluginManager->GetPlugin(csl::ut::HashString("StatePluginEffect"));
	for (int i = 0; i < T; i++) {
		char buffer[16];
		snprintf(buffer, sizeof(buffer), "%s%d", name, i);
		PlayerStopEffect(plugin, csl::ut::HashString(buffer));
	}
}

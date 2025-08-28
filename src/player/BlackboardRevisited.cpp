#include "BlackboardRevisited.h"

using namespace app::player;
using namespace app::ui;
using namespace app::snd;

using namespace app_cmn::fsm;

using namespace revisited::player;

void BlackboardRevisited::PreGameUpdateCallback(GameManager* gameManager, const SUpdateInfo& updateInfo)
{
	if (flags.test(Flags::ULTRA) && ultraTime.IsActive())
		ultraTime.Add(updateInfo.deltaTime);
	if (flags.test(Flags::ULTRA) && ultraTime.IsFinished()) {
		StopEffects("ultra", invincibilityEffects);
		if (auto* soundDirector = GameManager::GetInstance()->GetService<SoundDirector>())
			soundDirector->PlayBgm({ lastBgm, 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0x10001, 0 });
		flags.set(Flags::ULTRA, false);
	}

	if (flags.test(Flags::INFINITE_BOOST) && infiniteBoost.IsActive()) {
		infiniteBoost.Add(updateInfo.deltaTime);
		boostPlugin->boostAmount = boostPlugin->boostCapacity;
	}
	if (flags.test(Flags::INFINITE_BOOST) && infiniteBoost.IsFinished()) {
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
	ultraTime.Set(20);
	if (auto* soundDirector = GameManager::GetInstance()->GetService<SoundDirector>()) {
		strcpy(lastBgm, soundDirector->unkA0[0].name);
		soundDirector->PlayBgm({ "invincibility", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0x10001, 0 });
	}
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
	if (!boostPlugin)
		boostPlugin = GetPlayer()->GetComponent<GOCPlayerHsm>()->statePluginManager->GetPlugin<StatePluginBoost>();
	if (!uiStaminaGaugeCtx)
		uiStaminaGaugeCtx = ((GOCTinyFsm2<UIStaminaGaugeContext>*)GameManager::GetInstance()->GetGameObject("UIGameCockpit")->components[8])->targetObject;

	flags.set(Flags::INFINITE_BOOST, true);
	boostPlugin->infiniteBoostTimer = 35;
	infiniteBoost.Set(35);
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
	auto* plugin = GetPlayer()->GetComponent<GOCPlayerHsm>()->statePluginManager->GetPlugin<StatePluginEffect>();
	if (!plugin) return;

	for (int i = 0; i < T; i++) {
		char buffer[16];
		snprintf(buffer, sizeof(buffer), "%s%d", name, i);
		plugin->PlayEffect(csl::ut::HashString(buffer), names[i]);
	}
}

template<size_t T>
void BlackboardRevisited::StopEffects(const char* name, const char* const (&names)[T])
{
	auto* plugin = GetPlayer()->GetComponent<GOCPlayerHsm>()->statePluginManager->GetPlugin<StatePluginEffect>();
	if (!plugin) return;

	for (int i = 0; i < T; i++) {
		char buffer[16];
		snprintf(buffer, sizeof(buffer), "%s%d", name, i);
		plugin->StopEffect(csl::ut::HashString(buffer));
	}
}

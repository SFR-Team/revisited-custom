#include "ObjDoorWarpTrigger.h"

using namespace csl::fnd;

using namespace hh::fnd;
using namespace hh::game;

using namespace app;
using namespace app::game;
using namespace app::player;

using namespace revisited::objects;

GAMEOBJECT_CLASS_RFL(ObjDoorWarpTrigger, Common);

GameObject* ObjDoorWarpTrigger::Create(IAllocator* allocator) {
	return new (allocator) ObjDoorWarpTrigger{ allocator };
}

ObjDoorWarpTrigger::ObjDoorWarpTrigger(IAllocator* allocator) : GameObject{ allocator } {
	SetLayer(6);
	SetUpdateFlag(UpdatingPhase::POST_ANIM, true);
}

void ObjDoorWarpTrigger::AddCallback(GameManager* gameManager) {
	GOCContact::Description gocContactDesc{};
	gocContactDesc.float64 = 3.1415927f;
	gocContactDesc.visibleRange = 10;
	gocContactDesc.range = GetWorldDataByClass<ObjDoorWarpTriggerSpawner>()->interactionRange;
	gocContactDesc.byte6A = 0x100;
	gocContactDesc.byte60 = 0x1000000;
	gocContactDesc.byte68 = 0x101;
	auto* gocContact = CreateComponent<GOCContact>();
	gocContact->Setup(gocContactDesc);
	AddComponent(gocContact);
	gocContact->SetEnabled(true);
	gocContact->AddListener(this);
}

void ObjDoorWarpTrigger::UpdateAsync(UpdatingPhase phase, const SUpdateInfo& updateInfo, void* unkParam) {
	auto* screenFade = gameManager->GetService<ScreenFadeManager>();
	if (screenFade->IsFading() && shouldFading) {
		auto* worldData = GetWorldDataByClass<ObjDoorWarpTriggerSpawner>();

		WorldPosition worldPos{};
		if (gameManager->TryGetObjectWorldPosByID(worldData->targetObject, worldPos)) {
			MsgWarpToPoint warpToPoint{};
			warpToPoint.warpTarget = worldPos;
			ut::SendMessageImmToPlayerObject(*this, 0, warpToPoint);
		}

		player::MsgHoldRelease holdEndMsg{};
		ut::SendMessageImmToPlayerObject(*this, 0, holdEndMsg);
		GetComponent<GOCContact>()->SetEnabled(true);

		shouldFading = false;

		screenFade->Fade(worldData->fadeSpeed);
	}
};

void ObjDoorWarpTrigger::OnContact() {
	player::MsgHoldOn holdOnMsg{};
	holdOnMsg.unk2 = 152;
	ut::SendMessageToPlayerObject(*this, 0, holdOnMsg);

	GetComponent<GOCContact>()->SetEnabled(false);

	auto* screenFade = gameManager->GetService<ScreenFadeManager>();
	screenFade->FadeOut(GetWorldDataByClass<ObjDoorWarpTriggerSpawner>()->fadeSpeed);
	shouldFading = true;
}

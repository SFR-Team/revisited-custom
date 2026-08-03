#include "ObjBubble.h"

using namespace csl::fnd;

using namespace hh::anim;
using namespace hh::eff;
using namespace hh::fnd;
using namespace hh::game;
using namespace hh::gfx;
using namespace hh::physics;
using namespace hh::ui;

using namespace app;
using namespace app::game;
using namespace app::player;

using namespace revisited::objects;

GAMEOBJECT_CLASS(ObjBubble);

GameObject* ObjBubble::Create(IAllocator* allocator) {
	return new (allocator) ObjBubble{ allocator };
}

ObjBubble::ObjBubble(IAllocator* allocator) : GameObject{ allocator } {
	SetLayer(6);
	SetUpdateFlag(UpdatingPhase::POST_ANIM, true);
}

void ObjBubble::AddCallback(GameManager* gameManager) {
	/*auto* gocEffect = CreateComponent<GOCEffect>();
	gocEffect->Setup({});
	AddComponent(gocEffect);

	EffectTransFrameCreateInfo bubbleCreateInfo{ "ec_so_water_airbubble01" };
	bubbleCreateInfo.unk4a = -1;
	bubbleCreateInfo.unk1a = true;
	bubbleCreateInfo.transInfo.frame = gocEffect->frame;

	gocEffect->CreateEffectEx(bubbleCreateInfo, &bubbleEffect);*/

	GOCVisualModelDescription gocVisualModelDesc{};
	gocVisualModelDesc.model = ResourceManager::GetInstance()->GetResource<ResModel>("chr_sonic");
	gocVisualModelDesc.flags.bits = 0x2000000;
	auto* gocVisual = CreateComponent<GOCVisualModel>();
	gocVisual->Setup(gocVisualModelDesc);
	AddComponent(gocVisual);

	GOCSphereCollider::SetupInfo gocColliderDesc{};
	gocColliderDesc.radius = 0.5f;
	gocColliderDesc.layer = GOCCollider::LayerType::GIMMICK;
	gocColliderDesc.unk2 = 0;
	gocColliderDesc.overlapFlags.set(GOCCollider::OverlapFlag::ENTER);
	gocColliderDesc.hitFlags = 0x8000;

	auto* gocSphereCollider = CreateComponent<GOCSphereCollider>();
	gocSphereCollider->Setup(gocColliderDesc);
	AddComponent(gocSphereCollider);

	gocTransform = GetComponent<GOCTransform>();
}

bool ObjBubble::ProcessMessage(Message& message) {
	switch (message.ID) {
	case MessageID::TRIGGER_ENTER: {
		auto& msg = (MsgTriggerEnter&)message;
		auto* playerHsm = msg.GetActorObject()->GetComponent<GOCPlayerHsm>();
		auto* statePluginInWater = playerHsm->statePluginManager->GetPlugin<StatePluginInWater>();
		auto* inWaterProcess = (StatePluginInWater::InWaterStatusProcess*)&*statePluginInWater->waterDepthProcesses[0];
		if (inWaterProcess->isDrowning)
			inWaterProcess->drowningTimer -= oxygenRestoreAmount;
		Kill();
		break;
	}
	}
	return true;
}

void ObjBubble::UpdateAsync(UpdatingPhase phase, const SUpdateInfo& updateInfo, void* unkParam) {
	deathTimer -= updateInfo.deltaTime;

	gocTransform->SetLocalTranslation(gocTransform->transform.position + csl::math::Vector3{ 0, updateInfo.deltaTime, 0 });

	if (deathTimer <= 0) {
		Kill();
	}
};

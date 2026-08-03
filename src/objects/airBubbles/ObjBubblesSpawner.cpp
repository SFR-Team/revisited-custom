#include "ObjBubblesSpawner.h"
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

using namespace revisited::objects;

GAMEOBJECT_CLASS_RFL_OBJINFO(ObjBubblesSpawner, Common);

GameObject* ObjBubblesSpawner::Create(IAllocator* allocator) {
	return new (allocator) ObjBubblesSpawner{ allocator };
}

ObjBubblesSpawner::ObjBubblesSpawner(IAllocator* allocator) : GameObject{ allocator } {
	SetLayer(6);
	SetUpdateFlag(UpdatingPhase::POST_ANIM, true);
}

void ObjBubblesSpawner::AddCallback(GameManager* gameManager) {
	auto* objInfo = GetObjInfo<ObjBubblesSpawnerInfo>(gameManager);
	auto* spawnerData = GetWorldDataByClass<ObjBubblesSpawnerSpawner>();

	bigBubbleFrequency = spawnerData->bigBubbleFrequency;
	oxygenRestoreAmount = spawnerData->oxygenRestoreAmount;

	/*auto* gocEffect = CreateComponent<GOCEffect>();
	gocEffect->Setup({});
	AddComponent(gocEffect);

	EffectTransFrameCreateInfo bubbleCreateInfo{ "ec_so_water_airbubbles01" };
	bubbleCreateInfo.unk4a = -1;
	bubbleCreateInfo.unk1a = true;
	bubbleCreateInfo.transInfo.frame = gocEffect->frame;

	gocEffect->CreateEffectEx(bubbleCreateInfo, &bubblesEffect);*/

	GOCVisualModelDescription gocVisualModelDesc{};
	gocVisualModelDesc.model = ResourceManager::GetInstance()->GetResource<ResModel>("chr_sonic");
	gocVisualModelDesc.flags.bits = 0x2000000;
	auto* gocVisual = CreateComponent<GOCVisualModel>();
	gocVisual->Setup(gocVisualModelDesc);
	AddComponent(gocVisual);
}

void ObjBubblesSpawner::UpdateAsync(UpdatingPhase phase, const SUpdateInfo& updateInfo, void* unkParam) {
	bubbleSpawnTimer += updateInfo.deltaTime;

	if (bubbleSpawnTimer >= bigBubbleFrequency) {
		bubbleSpawnTimer = 0;
		auto* bubble = GameObject::Create<ObjBubble>(pAllocator);
		bubble->oxygenRestoreAmount = oxygenRestoreAmount;
		hh::fnd::WorldPosition worldPos{};
		gameManager->AddGameObject(bubble, "bubl", false, &worldPos, this);
	}
};

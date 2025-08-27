#include "DvIslandIntro.h"
#include "utilities/GameHelpers.h"

using namespace csl::fnd;

using namespace hh::fnd;
using namespace hh::game;
using namespace hh::ui;

using namespace revisited::objects;

GAMEOBJECT_CLASS(DvIslandIntro)

GameObject* DvIslandIntro::Create(IAllocator* allocator) {
	return new (std::align_val_t(16), allocator) DvIslandIntro{ allocator };
}

DvIslandIntro::DvIslandIntro(IAllocator* allocator) : GameObject{ allocator } {
	SetLayer(19);
	SetUpdateFlag(UpdatingPhase::PRE_ANIM, true);
}

void DvIslandIntro::AddCallback(hh::game::GameManager* gameManager)
{
	auto* resMgr = ResourceManager::GetInstance();

	auto* res = resMgr->GetResource<ResSurfRideProject>("ui_islandintro");
	if (!res) return;

	GOCSprite::SetupInfo gocSpriteDesc{};
	gocSpriteDesc.name = "DvIslandIntro";
	gocSpriteDesc.resource = res;
	auto* gocSprite = CreateComponent<GOCSprite>();
	gocSprite->Setup(gocSpriteDesc);
	AddComponent(gocSprite);

	GOCUIComposition::SetupInfo gocUiCompDesc{};
	auto* gocUiComp = CreateComponent<GOCUIComposition>();
	gocUiComp->Setup(gocUiCompDesc);
	AddComponent(gocUiComp);

	gocUiComp->SetProject(gocSprite->GetProject());
	gocUiComp->Initialize();

	lc = gocSprite->CreateLayerController("ui_islandintro", "info_island", 0x6942);
}

void DvIslandIntro::Start(unsigned int island, csl::math::Vector2& position, float scale) const
{
	if (!lc) return;

	if (auto* project = lc->gocSprite->GetProject())
		if (auto* scene = project->GetScene("ui_islandintro")) {
			scene->layers[0]->casts[0]->transform->position = SurfRide::Vector3{ position.x(), position.y(), 0 };
			scene->layers[0]->casts[0]->transform->scale = SurfRide::Vector3{ scale, scale, scale };
			scene->layers[0]->casts[0]->transform->dirtyFlag.SetCellAll();
			scene->layers[0]->casts[2]->castData->data.image->cropRefs0[0].cropIndex = island;
			scene->layers[0]->casts[2]->transform->dirtyFlag.SetCellAll();
		}

	lc->ClearReservedAnimation();
	lc->ReserveAnimation("in");
	lc->ReserveAnimation("loop");
	lc->PlayReservedAnimation();
}

void DvIslandIntro::End() const
{
	if (!lc) return;

	lc->ClearReservedAnimation();
	lc->ReserveAnimation("out");
	lc->PlayReservedAnimation();
}

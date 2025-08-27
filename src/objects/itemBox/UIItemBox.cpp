#include "UIItemBox.h"
#include "utilities/GameHelpers.h"

using namespace csl::fnd;

using namespace hh::fnd;
using namespace hh::game;
using namespace hh::ui;

using namespace revisited::objects;


GAMEOBJECT_CLASS(UIItemBox)

GameObject* UIItemBox::Create(IAllocator* allocator) {
	return new (std::align_val_t(16), allocator) UIItemBox{ allocator };
}

UIItemBox::UIItemBox(IAllocator* allocator) : GameObject{ allocator } {
	SetLayer(19);
	SetUpdateFlag(UpdatingPhase::PRE_ANIM, true);
}

void UIItemBox::AddCallback(GameManager* gameManager)
{
	auto* resMgr = ResourceManager::GetInstance();

	GOCSprite::SetupInfo gocSpriteDesc{};
	gocSpriteDesc.name = "UIItemBox";
	gocSpriteDesc.resource = resMgr->GetResource<ResSurfRideProject>("ui_itembox");
	auto* gocSprite = CreateComponent<GOCSprite>();
	gocSprite->Setup(gocSpriteDesc);
	AddComponent(gocSprite);

	GOCUIComposition::SetupInfo gocUiCompDesc{};
	auto* gocUiComp = CreateComponent<GOCUIComposition>();
	gocUiComp->Setup(gocUiCompDesc);
	AddComponent(gocUiComp);

	lc = gocSprite->CreateLayerController("ui_itembox", "info_item", 0x6942);

	gocUiComp->SetProject(gocSprite->GetProject());
	gocUiComp->Initialize();
}

void UIItemBox::Update(UpdatingPhase phase, const SUpdateInfo& updateInfo)
{
	if (visibleCounter.IsActive()) {
		visibleCounter.Add(updateInfo.deltaTime);
		if(!lc->IsVisible())
			lc->SetVisible(true);
	}
	if (visibleCounter.IsFinished()) {
		if (lc->IsVisible())
			lc->SetVisible(false);
	}
}

void UIItemBox::SetVisible(ObjItemBoxSpawner::ItemType type)
{
	visibleCounter.Set(3.5f);
	if (auto* project = lc->gocSprite->GetProject())
		if (auto* scene = project->GetScene("ui_itembox")) {
			lc->PlayAnimation("popup");
			scene->layers[0]->casts[0]->castData->data.image->cropRefs0[0].cropIndex = (char)type;
			scene->layers[0]->casts[0]->transform->dirtyFlag.SetCellAll();
		}
}

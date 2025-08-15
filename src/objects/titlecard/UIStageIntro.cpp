#include "UIStageIntro.h"
#include "utilities/GameHelpers.h"

using namespace csl::fnd;

using namespace hh::fnd;
using namespace hh::game;
using namespace hh::ui;

using namespace revisited::objects;

GAMEOBJECT_CLASS(UIStageIntro)

GameObject* UIStageIntro::Create(IAllocator* allocator) {
	return new (std::align_val_t(16), allocator) UIStageIntro{ allocator };
}

UIStageIntro::UIStageIntro(IAllocator* allocator) : GameObject{ allocator }, stageIntro{ allocator } {
	SetLayer(19);
	SetUpdateFlag(UpdatingPhase::PRE_ANIM, true);
}

void UIStageIntro::AddCallback(GameManager* gameManager)
{
	auto* resMgr = ResourceManager::GetInstance();

	GOCSprite::SetupInfo gocSpriteDesc{};
	gocSpriteDesc.name = "UIStageIntro";
	gocSpriteDesc.resource = resMgr->GetResource<ResSurfRideProject>("ui_stageintro");
	auto* gocSprite = CreateComponent<GOCSprite>();
	gocSprite->Setup(gocSpriteDesc);
	AddComponent(gocSprite);

	GOCUIComposition::SetupInfo gocUiCompDesc{};
	auto* gocUiComp = CreateComponent<GOCUIComposition>();
	gocUiComp->Setup(gocUiCompDesc);
	AddComponent(gocUiComp);

	gocUiComp->SetProject(gocSprite->GetProject());
	gocUiComp->Initialize();

	ResReflectionT<StageIntro>* stageIntroRes = (ResReflectionT<StageIntro>*)resMgr->GetResource<ResReflection>("stageintro_rfl");
	if (!stageIntroRes) return;

	app::level::LevelInfo* levelInfo = gameManager->GetService<app::level::LevelInfo>();
	const char* stageId = levelInfo->stageData->cyberName.c_str();
	for (auto& x : stageIntroRes->GetData()->stages)
		if (!strcmp(x.stageId, stageId)) {
			stageIntro = x;
			hasStageIntro = true;
			break;
		}

	lc = gocSprite->CreateLayerController("ui_stageintro", "info_stage", 0x6942);

	fadeCounter = 3.5f;
}

void UIStageIntro::Update(UpdatingPhase phase, const SUpdateInfo& updateInfo)
{
	if (fadeCounter > 0)
		fadeCounter -= updateInfo.deltaTime;
	if (fadeCounter <= 0 && fadeCounter != -1) {
		fadeCounter = -1;
		if (!lc->IsVisible())
			SetVisible(true);
	}

	if (visibleCounter > 0) {
		visibleCounter -= updateInfo.deltaTime;
		if (!lc->IsVisible())
			lc->SetVisible(true);
	}
	if (visibleCounter <= 0) {
		visibleCounter = 0;
		if (lc->IsVisible())
			lc->SetVisible(false);
	}
}

void UIStageIntro::SetVisible(bool visible)
{
	if (!hasStageIntro) return;

	if (auto* project = lc->gocSprite->GetProject())
		if (auto* scene = project->GetScene("ui_stageintro")) {
			visibleCounter = 3.5f;
			lc->SetVisible(true);
			lc->PlayAnimation("popup");
			scene->layers[0]->casts[0]->castData->data.image->cropRefs0[0].cropIndex = stageIntro.cropId;
			scene->layers[0]->casts[0]->transform->dirtyFlag.SetCellAll();
		}
}

const GameObjectClass* UIStageIntro::GetClass()
{
	return &gameObjectClass;
}

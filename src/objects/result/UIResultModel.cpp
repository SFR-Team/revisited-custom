#include "UIResultModel.h"
#include "../../utilities/GameHelpers.h"

using namespace csl::fnd;

using namespace hh::anim;
using namespace hh::fnd;
using namespace hh::game;
using namespace hh::gfnd;
using namespace hh::gfx;
using namespace hh::needle;
using namespace hh::ui;

using namespace app::ui;

using namespace revisited::objects;

GAMEOBJECT_CLASS(UIResultModel);

GameObject* UIResultModel::Create(IAllocator* allocator) {
	return new (std::align_val_t(16), allocator) UIResultModel{ allocator };
}

UIResultModel::UIResultModel(IAllocator* allocator) : GameObject{ allocator } {
	SetLayer(19);
}

void* UIResultModel::GetRuntimeTypeInfo() const
{
	return nullptr;
}

void UIResultModel::AddCallback(GameManager* gameManager)
{
	auto* resMgr = ResourceManager::GetInstance();

	auto* gocTransform = CreateComponent<GOCTransform>();
	AddComponent(gocTransform);

	GOCRenderTextureUI::Description gocRenderTextureDesc{};
	gocRenderTextureDesc.textureListName = "ui_resultmenu_img";
	gocRenderTextureDesc.textureIdx = 0;
	gocRenderTextureDesc.cameraSettings.globalLightPosition = { 0, 0.5f, 0 };
	gocRenderTextureDesc.cameraSettings.globalLightColor = { 1, 1, 1 };
	gocRenderTextureDesc.cameraSettings.cameraPosition = csl::math::Vector3{ 0, 0.53f, 2.52f };
	gocRenderTextureDesc.cameraSettings.cameraTargetPos = csl::math::Vector3{ 0, 0.5f, 0 };
	gocRenderTextureDesc.cameraSettings.nearClip = 0.1f;
	gocRenderTextureDesc.cameraSettings.farClip = 10000;
	gocRenderTextureDesc.shaderSettings.name = "uiResult";
	gocRenderTextureDesc.shaderSettings.sceneName = "ResultCharacter";
	auto* gocRenderTexture = CreateComponent<GOCRenderTextureUI>();
	gocRenderTexture->Setup(gocRenderTextureDesc);
	AddComponent(gocRenderTexture);

	if (gocSprite) gocRenderTexture->SetGOCSprite(gocSprite);

	auto* renderManager = (hh::gfx::RenderManager*)hh::gfnd::RenderManagerBase::GetInstance();
	auto* supportFx = renderManager->GetNeedleResourceDevice()->GetSupportFX();
	auto* renderPipeline = new RenderTexturePipeline{ pAllocator, supportFx->modelInstanceRenderer, gocRenderTexture->renderTextureHandle };
	gocRenderTexture->renderTextureHandle->ChangePipeline(renderPipeline);
	gocRenderTexture->SetUnk0(true);

	auto* shLightField = resMgr->GetResource<ResSHLightField>("results_lf");
	SupportFX::LightFieldDescription supportFxLfDesc{};
	supportFxLfDesc.binaryData = shLightField->binaryData;
	supportFxLfDesc.textures = &shLightField->textures;
	supportFx->SetupLightField(supportFxLfDesc, "ResultCharacter");

	auto* renderingEngine = renderManager->implementation->renderingEngine;
	auto* iblComponent = (IBLComponent*)renderingEngine->GetComponentByHash(csl::ut::HashString("IBLComponent"));
	
	auto* iblTexture = resMgr->GetResource<ResTexture>("defaultibl_results");
	auto* texture = iblTexture->GetTexture();
	iblComponent->SetIBLTexture(&texture, "ResultCharacter");

	const char* modelName = "chr_sonic";
	if (gameManager->GetDLCSonicCostume() != 0)
		modelName = gameManager->GetDLCSonicCostumeModelName();

	auto* model = resMgr->GetResource<ResModel>(modelName);
	auto* skeleton = resMgr->GetResource<ResSkeletonPxd>("chr_sonic");

	GOCVisualModelDescription gocVisualModelDesc{};
	gocVisualModelDesc.flags.set(GOCVisualModelDescription::Flag::NO_MATERIAL_OPTIMIZE);
	gocVisualModelDesc.model = model;
	gocVisualModelDesc.unk320 = -1;
	gocVisualModelDesc.nameHash = csl::ut::HashString("mainModel");
	auto* gocVisualModel = CreateComponent<GOCVisualModel>();
	gocVisualModel->Setup(gocVisualModelDesc);
	AddComponent(gocVisualModel);

	auto* world = gocRenderTexture->GetWorldByIdx(0);
	gocVisualModel->SetWorld(world);

	GOCAnimationSimple::SetupInfo gocAnimationSimpleDesc{};
	gocAnimationSimpleDesc.skeleton = skeleton;
	gocAnimationSimpleDesc.animationCount = 1;
	gocAnimationSimpleDesc.setUnk6Flag = 1;
	gocAnimationSimpleDesc.gocVisualModelNameHash = csl::ut::HashString("mainModel");
	auto* gocAnimationSimple = CreateComponent<GOCAnimationSimple>();
	gocAnimationSimple->Setup(gocAnimationSimpleDesc);
	AddComponent(gocAnimationSimple);

	auto* idleAnimation = resMgr->GetResource<ResAnimationPxd>("chr_sonic@idle_loop");

	gocAnimationSimple->Add("idle", idleAnimation, PlayPolicy::NORMAL);

	gocAnimationSimple->Play("idle");
}

void UIResultModel::Setup(app::ui::UIResult* uiResult)
{
	if (!uiResult) return;

	gocSprite = uiResult->GetComponent<GOCSprite>();
}

const GameObjectClass* UIResultModel::GetClass()
{
	return &gameObjectClass;
}

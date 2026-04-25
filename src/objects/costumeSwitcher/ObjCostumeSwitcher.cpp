#include "ObjCostumeSwitcher.h"

using namespace csl::fnd;

using namespace hh::fnd;
using namespace hh::game;
using namespace hh::gfx;
using namespace hh::physics;
using namespace hh::ui;

using namespace app;
using namespace app::game;
using namespace app::save;
using namespace app::ui;

using namespace revisited::objects;

static CostumeDescription costumeDescriptions[]{
	{ // Default
		.displayName = "extra_item_0_choice_0",
		.levelName = nullptr,
		.saveId = 0,
		.condition = nullptr
	},
	{ // Adventurer's Gloves and Shoes
		.displayName = "extra_item_1_choice_1",
		.levelName = "dlcddx",
		.saveId = 1,
		.condition = nullptr,
		.dlc = true,
		.dlcType = dlc::DLCType::DELUXE
	},
	{ // Rathalos
		.displayName = "extra_item_1_choice_2",
		.levelName = "dlcmhr_costume_hunter",
		.saveId = 2,
		.condition = nullptr,
		.dlc = true,
		.dlcType = dlc::DLCType::MONSTERHUNTER
	},
	{ // Felyne Rathalos
		.displayName = "extra_item_1_choice_3",
		.levelName = "dlcmhr_costume_airou",
		.saveId = 3,
		.condition = nullptr,
		.dlc = true,
		.dlcType = dlc::DLCType::MONSTERHUNTER
	},
	{ // Korone
		.displayName = "extra_item_1_choice_7",
		.levelName = "dlcholoA",
		.saveId = 4,
		.condition = nullptr,
		.dlc = true,
		.dlcType = dlc::DLCType::KORONE
	},
	{ // Soap Shoes
		.displayName = "extra_item_1_choice_8",
		.levelName = "dlccrm",
		.saveId = 5,
		.condition = nullptr,
		.dlc = true,
		.dlcType = dlc::DLCType::SOAPSHOES
	},
	{ // Xmas
		.displayName = "extra_item_1_choice_9",
		.levelName = "dlcxmas",
		.saveId = 6,
		.condition = nullptr,
		.dlc = true,
		.dlcType = dlc::DLCType::XMAS
	}
};

GAMEOBJECT_CLASS_RFL_OBJINFO(ObjCostumeSwitcher, Common);

GameObject* ObjCostumeSwitcher::Create(IAllocator* allocator) {
	return new (allocator) ObjCostumeSwitcher{ allocator };
}

ObjCostumeSwitcher::ObjCostumeSwitcher(IAllocator* allocator) : GameObject{ allocator }, options{ allocator } {
	SetLayer(6);
	SetUpdateFlag(UpdatingPhase::POST_ANIM, true);
}

void ObjCostumeSwitcher::AddCallback(GameManager* gameManager) {
	auto* objInfo = GetObjInfo<ObjCostumeSwitcherInfo>(gameManager);

	auto model = objInfo->model;
	GOCVisualModelDescription gocVisualModelDesc{};
	gocVisualModelDesc.model = model;
	auto* gocVisual = CreateComponent<GOCVisualModel>();
	gocVisual->Setup(gocVisualModelDesc);
	AddComponent(gocVisual);

	GOCMeshCollider::SetupInfo gocMeshColDesc{};
	gocMeshColDesc.meshResource = objInfo->col;
	gocMeshColDesc.layer = GOCCollider::LayerType::SOLID;
	gocMeshColDesc.hitFlags = 0x8000;
	auto* gocMeshCol = CreateComponent<GOCMeshCollider>();
	gocMeshCol->Setup(gocMeshColDesc);
	AddComponent(gocMeshCol);

	GOCContact::Description gocContactDesc{};
	gocContactDesc.float64 = 3.1415927f;
	gocContactDesc.visibleRange = 10;
	gocContactDesc.range = 4;
	gocContactDesc.byte6A = 0x100;
	gocContactDesc.byte60 = 0x1000000;
	gocContactDesc.byte68 = 0x101;
	auto* gocContact = CreateComponent<GOCContact>();
	gocContact->Setup(gocContactDesc);
	AddComponent(gocContact);
	gocContact->SetEnabled(true);
	gocContact->AddListener(this);
}

void ObjCostumeSwitcher::UpdateAsync(UpdatingPhase phase, const SUpdateInfo& updateInfo, void* unkParam) {
	if (overlayJobId != -1) {
		UIOverlayService::OverlayJobStatus status{};
		status.unk0 = true;
		gameManager->GetService<UIOverlayService>()->GetJobStatus(overlayJobId, status);
		if (status.unkStatus == 1) {
			if (status.unk2 == 1) {
				auto& option = options[status.selectionIdx];

				if (option.type == Option::Type::SKIN) {
					auto& costumeDesc = costumeDescriptions[option.skinIdx];

					OptionAc optionAc{ GetOptionAccessor(gameManager) };
					OptionGamePlayAc optionGamePlay{ optionAc.GetOptionGamePlayAc() };
					auto& prevCostumeDesc = costumeDescriptions[optionGamePlay.data->dlcSonicCostume];

					optionGamePlay.data->dlcSonicCostume = costumeDesc.saveId;
					gameManager->GetService<SaveManager>()->saveInterface->SaveOptionData();

					if (auto* levelManager = gameManager->GetService<LevelManager>())
					{
						if (prevCostumeDesc.levelName)
							levelManager->UnloadLevel(prevCostumeDesc.levelName);
						if (costumeDesc.levelName) {
							auto* level = levelManager->GetLevelByName(costumeDesc.levelName);
							auto* mlevelData = level->masterLevel->resource->masterLevelData;
							auto* levels = mlevelData->levels;
							auto levelCount = mlevelData->levelCount;
							for (auto x = 0; x < levelCount; x++)
								if (strcmp(levels[x]->name, costumeDesc.levelName) == 0) {
									ResourceLoader::Locale locale{};
									locale.localeId = 1;
									level->resourceLoader->LoadPackfile(levels[x]->resources[0]->path, locale);
								}
							levelManager->LoadLevel(costumeDesc.levelName);
						}
					}

					player::MsgHoldRelease holdEndMsg{};
					ut::SendMessageImmToPlayerObject(*this, 0, holdEndMsg);
					GetComponent<GOCContact>()->SetEnabled(true);

					RespawnPlayer();

					overlayJobId = -1;
				}

				if (option.type == Option::Type::NEXT_PAGE) {
					if (!lastPage) page++;
					else page = 0;
					options.clear();
					overlayJobId = -1;
					CreateList();
				}

				if (option.type == Option::Type::EXIT) {
					player::MsgHoldRelease holdEndMsg{};
					ut::SendMessageImmToPlayerObject(*this, 0, holdEndMsg);
					GetComponent<GOCContact>()->SetEnabled(true);

					overlayJobId = -1;
				}
			}
		}
	}
};
 
void ObjCostumeSwitcher::OnContact() {
	player::MsgHoldOn holdOnMsg{};
	holdOnMsg.unk2 = 152;
	ut::SendMessageToPlayerObject(*this, 0, holdOnMsg);

	GetComponent<GOCContact>()->SetEnabled(false);

	page = 0;
	options.clear();
	CreateList();
}

void ObjCostumeSwitcher::CreateList() {
	lastPage = true;

	auto* overlayRequest = RequestOverlayCaptionSelectBox::Create(gameManager->GetAllocator());
	overlayRequest->time = -1;
	for (auto x = page * 3; x < ARRAY_SIZE(costumeDescriptions); x++) {
		auto& costumeDesc = costumeDescriptions[x];
		if (costumeDesc.dlc)
			if (!dlc::IsDLCAvailable(gameManager, costumeDesc.dlcType))
				continue;
		if (costumeDesc.condition)
			if (!costumeDesc.condition())
				continue;
		if (options.size() == 3) {
			lastPage = false;
			break;
		}

		overlayRequest->options.push_back({ GetAllocator() });
		overlayRequest->options[overlayRequest->options.size() - 1].copyFrom(costumeDesc.displayName, strlen(costumeDesc.displayName));
		auto& option = options.emplace_back();
		option.type = Option::Type::SKIN;
		option.skinIdx = x;
	}

	AddNextOption(overlayRequest);
	AddExitOption(overlayRequest);

	overlayRequest->block = 1;
	overlayRequest->dword9C = 0.1f;
	overlayRequest->defaultSelectedOption = 0;
	if (auto* uiOverlayService = gameManager->GetService<UIOverlayService>())
		overlayJobId = uiOverlayService->QueueBeginRequest(overlayRequest);
}

void ObjCostumeSwitcher::AddNextOption(RequestOverlayCaptionSelectBox* overlay) {
	overlay->options.push_back({ GetAllocator() });
	overlay->options[overlay->options.size() - 1].copyFrom("hud_btnguide_next");
	auto& option = options.emplace_back();
	option.type = Option::Type::NEXT_PAGE;
}

void ObjCostumeSwitcher::AddExitOption(RequestOverlayCaptionSelectBox* overlay) {
	overlay->options.push_back({ GetAllocator() });
	overlay->options[overlay->options.size() - 1].copyFrom("ko9000_177");
	auto& option = options.emplace_back();
	option.type = Option::Type::EXIT;
}

void ObjCostumeSwitcher::RespawnPlayer() {
	if (auto* levelInfo = gameManager->GetService<level::LevelInfo>())
		if (auto* gameMode = static_cast<MyApplication*>(MyApplication::GetInstance())->GetExtension<game::ApplicationSequenceExtension>()->GetCurrentGameMode())
			if (auto* fxParamMgr = gameManager->GetService<gfx::FxParamManager>()) {
				player::CharacterIdU8 prevCharId{ player::CharacterIdU8::SONIC };

				auto* playerInfo = levelInfo->GetPlayerInformation(0);

				if (playerInfo->characterId.has_value())
					prevCharId = playerInfo->characterId.value();

				player::Player::Kill(gameManager, 0);

				gameManager->ShutdownPendingObjects();

				switch (prevCharId) {
				case player::CharacterIdU8::SONIC: {
					if (auto* objInfoContainer = gameManager->GetService<ObjInfoContainer>()) {
						objInfoContainer->Unregister("SonicInfo");
						objInfoContainer->Register("SonicInfo", new (MemoryRouter::GetModuleAllocator()) player::SonicInfo{ MemoryRouter::GetModuleAllocator() });
					}
					break;
				}
				};

				auto& stageConfig = fxParamMgr->sceneParameters[fxParamMgr->currentSceneParameters]->sceneData->stageConfig;

				player::Player::PlayerSetupInfo setupInfo{};
				setupInfo.playerId = 0;
				setupInfo.characterId = prevCharId;
				setupInfo.worldPos.m_Position = playerInfo->position.value();
				setupInfo.worldPos.m_Rotation = playerInfo->rotation.value();
				setupInfo.deadline = stageConfig.common.deadline;
				setupInfo.deadFallTime = stageConfig.common.deadFallTime;
				setupInfo.oceanSurface = stageConfig.common.oceanSurface;
				setupInfo.unk6 = 1;
				setupInfo.startType = player::Player::StartType::STAND;

				player::Player::Spawn(gameManager, setupInfo);

				ui::MsgUIChangePlayerCharacter msgUIChangePlayerCharacter{ prevCharId };
				msgUIChangePlayerCharacter.Mask = 0x4000000;
				ut::SendMessageToUIObjects(*gameMode, gameManager, msgUIChangePlayerCharacter);

				player::MsgAddNotifyPreDeadListener msgAddNotifyPreDeadListener{};
				ut::SendMessageToPlayerObject(*gameMode, gameManager, 0, msgAddNotifyPreDeadListener);
			}
}

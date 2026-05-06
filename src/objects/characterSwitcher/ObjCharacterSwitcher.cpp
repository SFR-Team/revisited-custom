#include "ObjCharacterSwitcher.h"

using namespace csl::fnd;

using namespace hh::anim;
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

GAMEOBJECT_CLASS_RFL_OBJINFO(ObjCharacterSwitcher, Common);

GameObject* ObjCharacterSwitcher::Create(IAllocator* allocator) {
	return new (allocator) ObjCharacterSwitcher{ allocator };
}

ObjCharacterSwitcher::ObjCharacterSwitcher(IAllocator* allocator) : GameObject{ allocator }, options{ allocator } {
	SetLayer(6);
	SetUpdateFlag(UpdatingPhase::POST_ANIM, true);
}

void ObjCharacterSwitcher::AddCallback(GameManager* gameManager) {
	auto* objInfo = GetObjInfo<ObjCharacterSwitcherInfo>(gameManager);

	auto model = objInfo->model;
	auto skl = objInfo->skl;
	GOCVisualModelDescription gocVisualModelDesc{};
	gocVisualModelDesc.model = model;
	gocVisualModelDesc.skeleton = skl;
	gocVisualModelDesc.flags.bits = 0x2000000;
	auto* gocVisual = CreateComponent<GOCVisualModel>();
	gocVisual->Setup(gocVisualModelDesc);
	gocVisual->SetNameHash("Body");
	AddComponent(gocVisual);

	GOCAnimator::SetupInfo gocAnimatorDesc{};
	gocAnimatorDesc.asmResourceManager = objInfo->asmRes;
	gocAnimatorDesc.setPose = true;
	auto* gocAnimator = CreateComponent<GOCAnimator>();
	gocAnimator->Setup(gocAnimatorDesc);
	AddComponent(gocAnimator);
	gocAnimator->ChangeState("idle");

	GOCSphereCollider::SetupInfo gocSphereColDesc{};
	gocSphereColDesc.layer = GOCCollider::LayerType::SOLID;
	gocSphereColDesc.radius = 1.5f;
	gocSphereColDesc.hitFlags = 0x8000;
	auto* gocSphereCol = CreateComponent<GOCSphereCollider>();
	gocSphereCol->Setup(gocSphereColDesc);
	AddComponent(gocSphereCol);

	/*auto* charIkInfo = new (GetAllocator()) CharacterIkInfo{GetAllocator()};
	charIkInfo->lookAtIkInfo = new (GetAllocator()) LookAtIkInfo{ GetAllocator() };
	auto lookAtIkInfo = charIkInfo->lookAtIkInfo;
	lookAtIkInfo->maxLookDownAngle = -0.52359879f;
	lookAtIkInfo->maxLookLeftAngle = 1.5707964f;
	lookAtIkInfo->maxLookRightAngle = -1.5707964f;
	lookAtIkInfo->headBoneIndex = skl->GetBoneIndex("Head");
	lookAtIkInfo->neckBoneIndex = skl->GetBoneIndex("Neck");
	lookAtIkInfo->bool78 = true;
	lookAtIkInfo->SetLookAt(skl, { 0, 0, 1 }, { 0, 1, 0 });

	GOCCharacterIkPxd::SetupInfo gocCharIkPxdDesc{};
	gocCharIkPxdDesc.characterIkInfo = charIkInfo;
	auto* gocCharIkPxd = CreateComponent<GOCCharacterIkPxd>();
	gocCharIkPxd->Setup(gocCharIkPxdDesc);
	gocCharIkPxd->SetNameHash("Body");
	AddComponent(gocCharIkPxd);

	GOCLookAt::SetupInfo gocLookAtDesc{};
	gocLookAtDesc.gocCharacterIkNameHash = name_hash("Body");
	auto* gocLookAt = CreateComponent<GOCLookAt>();
	gocLookAt->Setup(gocLookAtDesc);
	AddComponent(gocLookAt);*/

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

void ObjCharacterSwitcher::UpdateAsync(UpdatingPhase phase, const SUpdateInfo& updateInfo, void* unkParam) {
	if (overlayJobId != -1) {
		UIOverlayService::OverlayJobStatus status{};
		status.unk0 = true;
		gameManager->GetService<UIOverlayService>()->GetJobStatus(overlayJobId, status);
		if (status.unkStatus == 1) {
			if (status.unk2 == 1) {
				if (status.selectionIdx == 3) {
					player::MsgHoldRelease holdEndMsg{};
					ut::SendMessageImmToPlayerObject(*this, 0, holdEndMsg);
					GetComponent<GOCContact>()->SetEnabled(true);
				}
				else {
					MsgChangePlayerCharacter msgChangePlayer{};
					msgChangePlayer.newCharId = options[status.selectionIdx];
					gameManager->SendMessageImm(msgChangePlayer);
					SpawnPlayer(msgChangePlayer.newCharId);

					player::MsgHoldRelease holdEndMsg{};
					ut::SendMessageImmToPlayerObject(*this, 0, holdEndMsg);

					GetComponent<GOCContact>()->SetEnabled(true);
				}

				overlayJobId = -1;
			}
		}
	}
};
 
void ObjCharacterSwitcher::OnContact() {
	player::MsgHoldOn holdOnMsg{};
	holdOnMsg.unk2 = 152;
	ut::SendMessageToPlayerObject(*this, 0, holdOnMsg);

	GetComponent<GOCContact>()->SetEnabled(false);

	CreateList();
}

static const char* optionNames[]{
	"ko9000_172", // Swap to Sonic
	"ko9000_173", // Swap to Amy
	"ko9000_174", // Swap to Knuckles
	"ko9000_175"  // Swap to Tails
};

void ObjCharacterSwitcher::CreateList() {
	options.clear();

	auto curCharId = player::CharacterIdU8::SONIC;

	if (auto* levelInfo = gameManager->GetService<level::LevelInfo>()) {
		auto* playerInfo = levelInfo->GetPlayerInformation(0);

		if (playerInfo->characterId.has_value())
			curCharId = playerInfo->characterId.value();
	}

	auto* overlayRequest = RequestOverlayCaptionSelectBox::Create(gameManager->GetAllocator());
	overlayRequest->time = -1;
	overlayRequest->subtitleText.copyFrom("egg_010");
	for (unsigned char x = 0; x < 4; x++) {
		if (x == (unsigned char)curCharId)
			continue;

		overlayRequest->options.push_back({ GetAllocator() });
		overlayRequest->options[overlayRequest->options.size() - 1].copyFrom(optionNames[x], strlen(optionNames[x]));
		options.push_back((player::CharacterIdU8)x);
	}

	AddExitOption(overlayRequest);

	overlayRequest->block = 1;
	overlayRequest->dword9C = 0.1f;
	overlayRequest->defaultSelectedOption = 0;
	if (auto* uiOverlayService = gameManager->GetService<UIOverlayService>())
		overlayJobId = uiOverlayService->QueueBeginRequest(overlayRequest);
}

void ObjCharacterSwitcher::SpawnPlayer(app::player::CharacterIdU8 charId) {
	if (auto* levelInfo = gameManager->GetService<level::LevelInfo>())
		if (auto* gameMode = static_cast<MyApplication*>(MyApplication::GetInstance())->GetExtension<game::ApplicationSequenceExtension>()->GetCurrentGameMode())
			if (auto* fxParamMgr = gameManager->GetService<gfx::FxParamManager>()) {
				auto* playerInfo = levelInfo->GetPlayerInformation(0);

				player::Player::Kill(gameManager, 0);

				gameManager->ShutdownPendingObjects();

				StageObjectModule::CharacterFilter filter = (StageObjectModule::CharacterFilter)(1 << static_cast<uint8_t>(charId));
				StageObjectModule::LoadCharacterObjInfo(gameManager, &filter, MemoryRouter::GetModuleAllocator());

				auto& stageConfig = fxParamMgr->sceneParameters[fxParamMgr->currentSceneParameters]->sceneData->stageConfig;

				player::Player::PlayerSetupInfo setupInfo{};
				setupInfo.playerId = 0;
				setupInfo.characterId = charId;
				setupInfo.worldPos.m_Position = playerInfo->position.value();
				setupInfo.worldPos.m_Rotation = playerInfo->rotation.value();
				setupInfo.deadline = stageConfig.common.deadline;
				setupInfo.deadFallTime = stageConfig.common.deadFallTime;
				setupInfo.oceanSurface = stageConfig.common.oceanSurface;
				setupInfo.unk6 = 1;
				setupInfo.startType = player::Player::StartType::STAND;

				player::Player::Spawn(gameManager, setupInfo);

				ui::MsgUIChangePlayerCharacter msgUIChangePlayerCharacter{ charId };
				msgUIChangePlayerCharacter.Mask = 0x4000000;
				ut::SendMessageToUIObjects(*gameMode, gameManager, msgUIChangePlayerCharacter);

				player::MsgAddNotifyPreDeadListener msgAddNotifyPreDeadListener{};
				ut::SendMessageToPlayerObject(*gameMode, gameManager, 0, msgAddNotifyPreDeadListener);
			}
}

void ObjCharacterSwitcher::AddExitOption(RequestOverlayCaptionSelectBox* overlay) {
	overlay->options.push_back({ GetAllocator() });
	overlay->options[overlay->options.size() - 1].copyFrom("ko9000_177");
}

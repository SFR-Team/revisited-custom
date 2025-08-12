#pragma once

enum SuperSonicState : unsigned char {
	NORMAL,
	SUPER,
	SUPER2
};

void SetCrop(unsigned short cropIdx) {
	auto* gameManager = hh::game::GameManager::GetInstance();
	if (auto* object = gameManager->GetGameObject("UIGameCockpit")) {
		if (auto* sprite = object->GetComponent<hh::ui::GOCSprite>()) {
			auto* project = sprite->GetProject();
			if (auto* scene = project->GetScene("ui_cockpitindicator_cmn"))
				if (auto* layer = scene->GetLayer("info_ring_num"))
					if (auto* cast = static_cast<SurfRide::ImageCast*>(layer->GetCast("sfr_characters"))) {
						cast->imageCastData->cropRefs0[0].cropIndex = cropIdx;
						cast->transform->dirtyFlag.SetCellAll();
					}
		}
	}
}

inline void SetSuperSonicIcon(SuperSonicState state) {
	if (state == NORMAL)
		SetCrop(state);
	else
		SetCrop(state + static_cast<char>(app::player::CharacterIdU8::TAILS));
}

inline void SetCharacterIcon(app::player::CharacterIdU8 icon) {
	SetCrop(static_cast<unsigned short>(icon));
}

HOOK(uint64_t, __fastcall, SetSuperSonicState, 0x1409C4D90, app::player::GOCPlayerHsm* self, bool state) {
	SetSuperSonicIcon(static_cast<SuperSonicState>(state));
	return originalSetSuperSonicState(self, state);
}

// Added these two only because of the Super Sonic Transformation mod, since in that mod it doesn't use messages, but manually sets everything
HOOK(uint64_t, __fastcall, SuperSonicVisualSet, 0x1409DF8D0, app::player::PlayerVisual* self) {
	SetSuperSonicIcon(SUPER);
	return originalSuperSonicVisualSet(self);
}

HOOK(uint64_t, __fastcall, SuperSonic2VisualSet, 0x1409DF740, app::player::PlayerVisual* self) {
	SetSuperSonicIcon(SUPER2);
	return originalSuperSonic2VisualSet(self);
}

// This is a more clean and correct way to detect SS2
HOOK(uint64_t, __fastcall, PlayerMessageHandler, 0x14091CF80, app::player::GOCPlayerHsm* self, hh::fnd::Message* message) {
	if (message->ID == hh::fnd::MessageID::TRANSFORM_TO_SS2)
		SetSuperSonicIcon(SUPER2);
	else if (message->ID == hh::fnd::MessageID::TRANSFORM_FROM_SS2)
		SetSuperSonicIcon(SUPER);

	return originalPlayerMessageHandler(self, message);
}

HOOK(app::player::Player*, __fastcall, PlayerSpawn, 0x140883F00, hh::game::GameManager* gameManager, const app::player::Player::PlayerSetupInfo& setupInfo) {
	SetCharacterIcon(setupInfo.characterId);
	return originalPlayerSpawn(gameManager, setupInfo);
}

namespace revisited::objects::cockpit {
	void bootstrap() {
		INSTALL_HOOK(SetSuperSonicState);

		// Added these two only because of the Super Sonic Transformation mod, since in that mod it doesn't use messages, but manually sets everything
		INSTALL_HOOK(SuperSonicVisualSet);
		INSTALL_HOOK(SuperSonic2VisualSet);

		// This is a more clean and correct way to detect SS2
		INSTALL_HOOK(PlayerMessageHandler);

		INSTALL_HOOK(PlayerSpawn);
	}
}

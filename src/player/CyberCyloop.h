#pragma once

HOOK(void, __fastcall, InitializePlayer, 0x14088AC20, app::player::Player* self) {
	originalInitializePlayer(self);
	auto& playerHsm = self->GetComponent<app::player::GOCPlayerHsm>()->statePluginManager;
	auto* plugin = new (playerHsm->GetAllocator()) app::player::StatePluginCyloop{ playerHsm->GetAllocator() };
	plugin->context = playerHsm->context;
	playerHsm->AddPlugin(plugin);
}

// This disables any kind of cyloop item spawn, if the player is in CyberSpace
// It's a fix for Cyber Cyloop
HOOK(void, __fastcall, sub_1409A47E0, 0x1409A47E0, int64_t self) {
	if (auto* stageData = hh::game::GameManager::GetInstance()->GetService<app::level::LevelInfo>()->GetStageData())
		if (stageData->GetAttributeFlag(app::level::StageData::AttributeFlags::CYBER))
			return;

	originalsub_1409A47E0(self);
}

namespace revisited::player {
	inline void bootstrapCyberCyloop() {
		INSTALL_HOOK(InitializePlayer);
		INSTALL_HOOK(sub_1409A47E0);
	}
}

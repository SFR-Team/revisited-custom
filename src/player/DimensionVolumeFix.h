#pragma once

HOOK(void, __fastcall, sub_140915530, 0x140915530, app::player::PlayerHsmContext* self) {
	originalsub_140915530(self);

	if (auto* levelInfo = self->GetPlayerObject()->gameManager->GetService<app::level::LevelInfo>()) {
		if (levelInfo->GetStageData()->GetAttributeFlag(app::level::StageData::AttributeFlags::CYBER)) {
			auto* gocPlayerParam = self->GetPlayerObject()->GetComponent<app::player::GOCPlayerParameter>();
			gocPlayerParam->SetMode(app::player::GOCPlayerParameter::Mode::CYBERSPACE_FORWARD_VIEW);
		}
	}
}

namespace revisited::player {
	inline void bootstrapDimensionVolumeFix() {
		INSTALL_HOOK(sub_140915530);
	}
}

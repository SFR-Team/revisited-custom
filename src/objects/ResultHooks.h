#pragma once
#include <iostream>

HOOK(void, __fastcall, ResultAddCallback, 0x140AC6B30, app::ui::UIResult* self, hh::game::GameManager* gameManager) {
	originalResultAddCallback(self, gameManager);
	
	auto* lc = self->mainLayerController;

	if (auto* cast = static_cast<SurfRide::ImageCast*>(lc->GetCast("sfr_background_results"))) {
		auto* castData = cast->imageCastData;
		if (castData->cropRef0Count > 0) {
			auto* project = lc->gocSprite->GetProject();
			auto& textureList = project->projectData->textureLists[1];

			if (auto* levelInfo = gameManager->GetService<app::level::LevelInfo>()) {
				auto* cyberName = levelInfo->GetStageData()->cyberName.c_str();

				static const char* prefix = "sfr_results_";
				static unsigned int prefixLen = strlen(prefix);

				for (auto x = 0; x < textureList.textureCount; x++) {
					auto& texture = textureList.textures[x];

					if (strncmp(texture.name, prefix, prefixLen) != 0)
						continue;

					char cyberTextureName[64];
					strcpy(cyberTextureName, &texture.name[prefixLen]);
					cyberTextureName[1] = '-';

					if (strcmp(cyberTextureName, cyberName) != 0)
						continue;

					castData->cropRefs0[0].textureIndex = x;
					cast->transform->dirtyFlag.SetCellAll();
					break;
				}
			}
		}
	}
}

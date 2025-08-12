#pragma once

HOOK(uint64_t, __fastcall, SetSuperSonicState, 0x1409C4D90, app::player::GOCPlayerHsm* self, bool state) {
	if (auto* object = hh::game::GameManager::GetInstance()->GetGameObject("UIGameCockpit")) {
		if (auto* sprite = object->GetComponent<hh::ui::GOCSprite>()) {
			auto* project = sprite->GetProject();
			if (auto* scene = project->GetScene("ui_cockpitindicator_cmn"))
				if (auto* layer = scene->GetLayer("info_ring_num"))
					if (auto* cast = static_cast<SurfRide::ImageCast*>(layer->GetCast("sfr_characters"))) {
						cast->imageCastData->cropRefs0[0].cropIndex = static_cast<short>(state);
						cast->transform->dirtyFlag.SetCellAll();
					}
		}
	}

	return originalSetSuperSonicState(self, state);
}

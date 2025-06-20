#pragma once
#include "ObjItemBoxSpawner.h"

namespace revisited::objects {
	class UIItemBox : public hh::game::GameObject {
		GAMEOBJECT_CLASS_DECLARATION(UIItemBox);

		float visibleCounter{ 0 };
		hh::fnd::Reference<hh::ui::LayerController> lc;
	public:
		virtual void AddCallback(hh::game::GameManager* gameManager) override;
		virtual void Update(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo) override;

		void SetVisible(ObjItemBoxSpawner::ItemType type);
	};
}

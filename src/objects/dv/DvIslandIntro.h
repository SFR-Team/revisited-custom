#pragma once

namespace revisited::objects {
	class DvIslandIntro : public hh::game::GameObject {
		GAMEOBJECT_CLASS_DECLARATION(DvIslandIntro);

		hh::fnd::Reference<hh::ui::LayerController> lc{};
	public:
		virtual void AddCallback(hh::game::GameManager* gameManager) override;

		void Start(unsigned int island, csl::math::Vector2& position, float scale) const;
		void End() const;
	};
}

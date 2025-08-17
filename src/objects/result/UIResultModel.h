#pragma once

namespace revisited::objects {
	class UIResultModel : public hh::game::GameObject {
		GAMEOBJECT_CLASS_DECLARATION(UIResultModel)
	public:
		hh::fnd::Reference<hh::ui::GOCSprite> gocSprite;

		virtual void* GetRuntimeTypeInfo() const override;
		virtual void AddCallback(hh::game::GameManager* gameManager) override;

		void Setup(app::ui::UIResult* uiResult);
	};
}

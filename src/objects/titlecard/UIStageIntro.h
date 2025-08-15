#pragma once

namespace revisited::objects {
	struct Stage {
		csl::ut::VariableString stageId;
		short cropId;

		static const hh::fnd::RflClass rflClass;
		static const hh::fnd::RflTypeInfo rflTypeInfo;
	};

	struct StageIntro {
		csl::ut::MoveArray<Stage> stages;

		static const hh::fnd::RflClass rflClass;
		static const hh::fnd::RflTypeInfo rflTypeInfo;
	};

	class UIStageIntro : public hh::game::GameObject {
		GAMEOBJECT_CLASS_DECLARATION(UIStageIntro);

		Stage stageIntro;
		hh::fnd::Reference<hh::ui::LayerController> lc{};
		float visibleCounter{ 0 };
		float fadeCounter{ -1 };
		bool hasStageIntro{ false };
	public:
		virtual void AddCallback(hh::game::GameManager* gameManager) override;
		virtual void Update(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo) override;

		void SetVisible(bool visible);
	};
}

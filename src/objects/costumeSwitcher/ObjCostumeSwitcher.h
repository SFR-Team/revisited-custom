#pragma once

namespace revisited::objects {
	struct ObjCostumeSwitcherSpawner {
		int dummy;

		static const hh::fnd::RflClass rflClass;
		static const hh::fnd::RflTypeInfo rflTypeInfo;
	};

	class ObjCostumeSwitcherInfo : public hh::game::ObjInfo {
	public:
		hh::fnd::Reference<hh::gfx::ResModel> model;

		OBJINFO_CLASS_DECLARATION(ObjCostumeSwitcherInfo);

		virtual void Load() override;
		virtual void Initialize(hh::game::GameManager* gameManager) override;
		virtual const char* GetInfoName() override;
	};

	using SkinCondFuncType = bool(*)();

	struct CostumeDescription {
		const char* displayName;
		const char* levelName;
		unsigned char saveId;
		SkinCondFuncType condition;
		bool dlc{ false };
		app::dlc::DLCType dlcType;
	};

	class ObjCostumeSwitcher : public hh::game::GameObject, public app::game::ContactListener {
		GAMEOBJECT_CLASS_DECLARATION(ObjCostumeSwitcher);

		struct Option {
			enum class Type : unsigned char {
				SKIN,
				EXIT,
				NEXT_PAGE,
			};

			Type type;
			unsigned char skinIdx{};
		};

		csl::ut::InplaceMoveArray32<Option, 5> options;
		unsigned char page{ 0 };
		bool lastPage{ false };
		int overlayJobId{ -1 };
	public:
		virtual void AddCallback(hh::game::GameManager* gameManager) override;
		virtual void UpdateAsync(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo, void* unkParam) override;
		virtual void OnContact() override;
		void CreateList();
		void AddNextOption(app::ui::RequestOverlayCaptionSelectBox* overlay);
		void AddExitOption(app::ui::RequestOverlayCaptionSelectBox* overlay);
		void RespawnPlayer();
	};
}

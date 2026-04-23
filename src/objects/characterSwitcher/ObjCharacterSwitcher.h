#pragma once

namespace revisited::objects {
	struct ObjCharacterSwitcherSpawner {
		int dummy;

		static const hh::fnd::RflClass rflClass;
		static const hh::fnd::RflTypeInfo rflTypeInfo;
	};

	class ObjCharacterSwitcherInfo : public hh::game::ObjInfo {
	public:
		hh::fnd::Reference<hh::gfx::ResModel> model;

		OBJINFO_CLASS_DECLARATION(ObjCharacterSwitcherInfo);

		virtual void Load() override;
		virtual void Initialize(hh::game::GameManager* gameManager) override;
		virtual const char* GetInfoName() override;
	};

	class ObjCharacterSwitcher : public hh::game::GameObject, public app::game::ContactListener {
		GAMEOBJECT_CLASS_DECLARATION(ObjCharacterSwitcher);

		csl::ut::InplaceMoveArray32<app::player::CharacterIdU8, 3> options;
		int overlayJobId{ -1 };
	public:
		virtual void AddCallback(hh::game::GameManager* gameManager) override;
		virtual void UpdateAsync(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo, void* unkParam) override;
		virtual void OnContact() override;
		void CreateList();
		void SpawnPlayer(app::player::CharacterIdU8 charId);
		void AddExitOption(app::ui::RequestOverlayCaptionSelectBox* overlay);
	};
}

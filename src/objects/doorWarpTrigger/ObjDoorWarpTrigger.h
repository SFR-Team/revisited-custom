#pragma once

namespace revisited::objects {
	struct ObjDoorWarpTriggerSpawner {
		float interactionRange;
		float fadeSpeed;
		hh::game::ObjectId targetObject;

		static const hh::fnd::RflClass rflClass;
		static const hh::fnd::RflTypeInfo rflTypeInfo;
	};

	class ObjDoorWarpTrigger : public hh::game::GameObject, public app::game::ContactListener {
		GAMEOBJECT_CLASS_DECLARATION(ObjDoorWarpTrigger);

		bool shouldFading{ false };
	public:
		virtual void AddCallback(hh::game::GameManager* gameManager) override;
		virtual void UpdateAsync(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo, void* unkParam) override;
		virtual void OnContact() override;
	};
}

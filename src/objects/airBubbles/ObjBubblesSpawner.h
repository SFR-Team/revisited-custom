#pragma once

namespace revisited::objects {
	struct ObjBubblesSpawnerSpawner {
		float bigBubbleFrequency;
		float oxygenRestoreAmount;

		static const hh::fnd::RflClass rflClass;
		static const hh::fnd::RflTypeInfo rflTypeInfo;
	};

	class ObjBubblesSpawnerInfo : public hh::game::ObjInfo {
	public:
		OBJINFO_CLASS_DECLARATION(ObjBubblesSpawnerInfo);

		virtual void Load() override;
		virtual void Initialize(hh::game::GameManager* gameManager) override;
		virtual const char* GetInfoName() override;
	};

	class ObjBubblesSpawner : public hh::game::GameObject {
		GAMEOBJECT_CLASS_DECLARATION(ObjBubblesSpawner);
		
		hh::eff::EffectHandle bubblesEffect;
		float bubbleSpawnTimer{ 0 };
		float bigBubbleFrequency{ 10 };
		float oxygenRestoreAmount{ 5 };
	public:
		virtual void AddCallback(hh::game::GameManager* gameManager) override;
		virtual void UpdateAsync(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo, void* unkParam) override;
	};
}

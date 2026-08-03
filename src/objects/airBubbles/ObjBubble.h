#pragma once

namespace revisited::objects {
	class ObjBubble : public hh::game::GameObject {
		GAMEOBJECT_CLASS_DECLARATION(ObjBubble);

		hh::eff::EffectHandle bubbleEffect;
		float oxygenRestoreAmount{ 5 };
		float deathTimer{ 4 };
		hh::game::GOCTransform* gocTransform;
	public:
		virtual void AddCallback(hh::game::GameManager* gameManager) override;
		virtual bool ProcessMessage(hh::fnd::Message& message) override;
		virtual void UpdateAsync(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo, void* unkParam) override;
	};
}

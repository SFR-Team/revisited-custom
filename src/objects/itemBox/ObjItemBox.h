#pragma once
#include "../../utilities/GameHelpers.h"
#include "ObjItemBoxSpawner.h"
#include "UIItemBox.h"

namespace revisited::objects {
	class ObjItemBox : public hh::game::GameObject {
		GAMEOBJECT_CLASS_DECLARATION(ObjItemBox)

		ObjItemBoxSpawner::ItemType type;
		bool canAirDashRocket;
		bool doBounce;
	public:
		virtual void* GetRuntimeTypeInfo() const override;
		virtual bool ProcessMessage(hh::fnd::Message& message) override;
		virtual void AddCallback(hh::game::GameManager* gameManager) override;

		void DestroyCallback();
		inline void GivePlayerRings(unsigned int amount) { GiveObject(amount, app::MsgTakeObject::Type::RING); }
		void GiveObject(unsigned int amount, app::MsgTakeObject::Type type);
		void SetEnabled(bool enabled);
		csl::math::Vector3 CalculateBounce();
		void SendBounceMessage();
	};
}

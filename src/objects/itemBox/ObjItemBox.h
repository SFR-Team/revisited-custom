#pragma once
#include "../../utilities/GameHelpers.h"
#include "ObjItemBoxSpawner.h"
#include "UIItemBox.h"

namespace revisited::objects {
	class ObjItemBox : public hh::game::GameObject {
		GAMEOBJECT_CLASS_DECLARATION(ObjItemBox)

		ObjItemBoxSpawner::ItemType type;
		bool canAirDashRocket;
		hh::fnd::Handle<UIItemBox> ui;
	public:
		virtual void* GetRuntimeTypeInfo() const override;
		virtual bool ProcessMessage(hh::fnd::Message& message) override;
		virtual void AddCallback(hh::game::GameManager* gameManager) override;

		void DestroyCallback();
		void GivePlayerRings(unsigned int amount);
		void GiveObject(unsigned int amount, app::MsgTakeObject::Type type);
		void SetEnabled(bool enabled);
	};
}

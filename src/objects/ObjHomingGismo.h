#pragma once

namespace revisited::objects {
	struct ObjHomingGismoSpawner {
		csl::ut::VariableString modelName;
		csl::ut::VariableString particleName;
		csl::ut::VariableString soundName;

		static const hh::fnd::RflClass rflClass;
		static const hh::fnd::RflTypeInfo rflTypeInfo;
	};

	class ObjHomingGismo : public hh::game::GameObject {
		GAMEOBJECT_CLASS_DECLARATION(ObjHomingGismo)

		csl::ut::VariableString particleName;
		csl::ut::VariableString soundName;
	public:
		virtual bool ProcessMessage(hh::fnd::Message& message) override;
		virtual void AddCallback(hh::game::GameManager* gameManager) override;

		void DestroyCallback();
	};
}

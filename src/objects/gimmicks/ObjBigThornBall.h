#pragma once

namespace revisited::objects {
	struct ObjBigThornBallSpawner {
		float scale;

		static const hh::fnd::RflClass rflClass;
		static const hh::fnd::RflTypeInfo rflTypeInfo;
	};

	class ObjBigThornBallInfo : public hh::game::ObjInfo {
	public:
		hh::fnd::Reference<hh::gfx::ResModel> model{};

		OBJINFO_CLASS_DECLARATION(ObjBigThornBallInfo);

		virtual void Load() override;
		virtual void Initialize(hh::game::GameManager* gameManager) override;
		virtual const char* GetInfoName() override;
	};

	class ObjBigThornBall : public hh::game::GameObject {
	public:
		GAMEOBJECT_CLASS_DECLARATION(ObjBigThornBall);

		virtual void AddCallback(hh::game::GameManager* gameManager) override;
		virtual bool ProcessMessage(hh::fnd::Message& msg) override;
		void UpdateScale();
	};
}

#pragma once
#include "../../utilities/GameHelpers.h"

namespace revisited::objects {
	struct ObjItemBoxSpawner {
		enum class ItemType : unsigned char {
			ROCKET,
			TEN_RING,
			TWENTY_RING,
			FIFTY_RING,
			ULTRA,
			MAGNETIC,
			INFINITE_BOOST
		};

		ItemType type;
		bool isAir;
		bool canAirDashRocket;

		static const hh::fnd::RflClass rflClass;
		static const hh::fnd::RflTypeInfo rflTypeInfo;
	};

	class ObjItemBoxInfo : public hh::game::ObjInfo {
		struct TypeSet {
			csl::ut::MoveArray<hh::fnd::Reference<hh::gfx::ResModel>> models;
			hh::fnd::Reference<hh::anim::ResSkeleton> skeleton{};
			hh::fnd::Reference<hh::anim::ResAnimation> idleAnim{};
			hh::fnd::Reference<hh::anim::ResAnimation> getAnim{};

			TypeSet(csl::fnd::IAllocator* allocator) : models{ allocator } {}

			void LoadSet(const char* type);
		};

		TypeSet groundType;
		TypeSet airType;
	public:
		OBJINFO_CLASS_DECLARATION(ObjItemBoxInfo);

		virtual void Load() override;
		virtual void Initialize(hh::game::GameManager* gameManager) override;
		virtual const char* GetInfoName() override;

		hh::fnd::Reference<hh::gfx::ResModel> GetModel(bool isAir, ObjItemBoxSpawner::ItemType item) const;
		hh::fnd::Reference<hh::anim::ResSkeleton> GetSkeleton(bool isAir) const;
		hh::fnd::Reference<hh::anim::ResAnimation> GetIdle(bool isAir) const;
		hh::fnd::Reference<hh::anim::ResAnimation> GetGet(bool isAir) const;
	};

	class ObjItemBox : public hh::game::GameObject {
		GAMEOBJECT_CLASS_DECLARATION(ObjItemBox)

		ObjItemBoxSpawner::ItemType type{ ObjItemBoxSpawner::ItemType::ROCKET };
		bool canAirDashRocket{ false };
		app::Timer animationTimer{};
	public:
		virtual bool ProcessMessage(hh::fnd::Message& message) override;
		virtual void AddCallback(hh::game::GameManager* gameManager) override;
		virtual void Update(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo) override;

		void DestroyCallback();
		void PostDestroyCallback();
		inline void GivePlayerRings(unsigned int amount) { GiveObject(amount, app::MsgTakeObject::Type::RING); }
		void GiveObject(unsigned int amount, app::MsgTakeObject::Type type);
		csl::math::Vector3 CalculateBounce();
		void SendBounceMessage();
	};
}

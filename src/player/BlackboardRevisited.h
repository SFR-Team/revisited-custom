#pragma once
#include "utilities/GameHelpers.h"

using namespace hh::game;
using namespace hh::eff;
using namespace hh::fnd;

using namespace app::evt;
using namespace app::player;


namespace revisited::player {
	class BlackboardRevisited : public BlackboardContent, public GameUpdateListener, public EventPlayerListener {
	public:
		static constexpr const char* name = "BlackboardRevisited";

		enum class Flags : unsigned char {
			MAGNETIC,
			ULTRA,
			INFINITE_BOOST
		};

		static constexpr const char* invincibilityEffects[]{
			"ec_invincibility_trail01",
			"ec_invincibility_spark01",
			"ec_invincibility_glow01"
		};

		static constexpr const char* shieldEffects[]{
			"ec_magneticshield_lightningsphere01",
			"ec_magneticshield_circle01",
			"ec_magneticshield_light01"
		};

		csl::ut::Bitset<Flags> flags{};
		float ultraTime{ 0 };
		float infiniteBoost{ 0 };
		Reference<StatePluginBoost> boostPlugin;
		Reference<app::ui::UIStaminaGaugeContext> uiStaminaGaugeCtx;

		virtual unsigned int GetNameHash() const override {
			return csl::ut::HashString(name);
		}

		virtual void PreGameUpdateCallback(GameManager* gameManager, const SUpdateInfo& updateInfo) override;
		virtual void CutsceneEnd(const char* cutsceneName) override;

		void GiveInvincibility();
		void GiveMagnetic();
		void GiveInfiniteBoost();
		void RemoveMagnetic();
		template<size_t T>
		void PlayEffects(const char* name, const char* const (&names)[T]);
		template<size_t T>
		void StopEffects(const char* name, const char* const (&names)[T]);

		BlackboardRevisited(csl::fnd::IAllocator* allocator) : BlackboardContent{ allocator }, flags{}, ultraTime{ 0 } 
		{ 
			auto* gameManager = hh::game::GameManager::GetInstance();
			gameManager->AddGameUpdateListener(this); 
			if (auto* evtPlayer = gameManager->GetService<EventPlayer>())
				evtPlayer->AddListener(this);
		}
		virtual ~BlackboardRevisited() override 
		{ 
			auto* gameManager = hh::game::GameManager::GetInstance();
			gameManager->RemoveGameUpdateListener(this);
			if (auto* evtPlayer = gameManager->GetService<EventPlayer>())
				evtPlayer->RemoveListener(this);
		}
		static BlackboardRevisited* Create(csl::fnd::IAllocator* allocator) {
			return new (std::align_val_t(16),  allocator) BlackboardRevisited{ allocator };
		}
	};
}

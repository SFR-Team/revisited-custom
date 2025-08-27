#pragma once
#include <utilities/GameHelpers.h>

namespace revisited::objects {
	class BossActionPluginTest : public app::game::BossActionPluginBase {
	public:
		BossActionPluginTest(csl::fnd::IAllocator* allocator) : BossActionPluginBase{ allocator } {}

		static BossActionPluginTest* Create(csl::fnd::IAllocator* allocator) {
			return new (std::align_val_t(16), allocator) BossActionPluginTest{ allocator };
		}

		virtual int GetNameHash() const override { return csl::ut::HashString("BossActionPluginTest"); }
	};

	class BossTestContext : public app::BossBaseContext {
	public:
		hh::game::GameObject* object;
		float hitTimer = 0;
		bool canRotate = true;

		virtual void HandleDamage(app::MsgDamage& msg) override {
			app::BossBaseContext::HandleDamage(msg);
			hitTimer = 1;
			canRotate = false;
		}

		virtual void Update(hh::fnd::UpdatingPhase phase, hh::fnd::SUpdateInfo& updateInfo) override {
			app::BossBaseContext::Update(phase, updateInfo);
			if (hitTimer > 0)
				hitTimer -= updateInfo.deltaTime;
			else
				canRotate = true;
		}

		virtual int64_t BBCtx_UnkFunc5() override { return 0; }
		virtual bool SetCurrentState(int stateIdx) override { 
			return 0; 
		}
		virtual const char* GetBossStateName(int stateIdx) override {
			switch (stateIdx) {
			case 0:
				return "Root";
			case 1:
				return "TestAttack";
			}
			return "---";
		}

		BossTestContext(csl::fnd::IAllocator* allocator) : BossBaseContext{ allocator } {}
	};

	static constexpr float rotationSpeed = 5;
	class BossTestContextState {
	public:
		class Root : public app_cmn::fsm::GOCStateBase<BossTestContext> {
		public:
			static hh::ut::StateDesc stateDesc;

			Root(csl::fnd::IAllocator* allocator) : GOCStateBase<BossTestContext>{ allocator } {}

			static Root* Create(csl::fnd::IAllocator* allocator) {
				return new (std::align_val_t(16), allocator) Root{ allocator };
			}

			virtual bool DoInit() override { return false; }
			virtual void DoEnter(int previousStateId) override {}
			virtual void DoLeave(int nextStateId) override {}
			virtual bool DoStep(float deltaTime) override { return Step(*static_cast<BossTestContext*>(context), deltaTime); }
			virtual bool DoUpdate(hh::fnd::UpdatingPhase phase, float deltaTime) override { return true; }
			virtual bool DoUpdateAsync(hh::fnd::UpdatingPhase phase, float deltaTime) override { return false; }
			virtual bool DoProcessMessage(const hh::fnd::Message& message) override { return false; }

			virtual bool Step(BossTestContext& context, float deltaTime) override {
				if (context.canRotate) {
					auto* model = context.object->GetComponent<hh::gfx::GOCVisualModel>();
					auto worldMat = TransformToAffine3f(model->frame2->fullTransform);
					auto worldPos = worldMat * Eigen::Vector3f{ 0.0f, 0.0f, 0.0f };
					auto* gocTransform = GetPlayer()->GetComponent<hh::game::GOCTransform>();
					float t = 1.0f - std::exp(-rotationSpeed * deltaTime);
					model->SetLocalRotation({ Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f{ 0.0f, 0.0f, 1.0f }, worldMat.inverse() * Eigen::Vector3f{ gocTransform->frame->fullTransform.position.x(), model->frame2->fullTransform.position.y(), gocTransform->frame->fullTransform.position.z() }) });
				}
				return true;
			}
		};

		class TestAttack : public app_cmn::fsm::GOCStateBase<BossTestContext> {
		public:
			static hh::ut::StateDesc stateDesc;

			TestAttack(csl::fnd::IAllocator* allocator) : GOCStateBase<BossTestContext>{ allocator } {}

			static TestAttack* Create(csl::fnd::IAllocator* allocator) {
				return new (std::align_val_t(16), allocator) TestAttack{ allocator };
			}

			virtual bool DoInit() override { return false; }
			virtual void DoEnter(int previousStateId) override { Enter(*static_cast<BossTestContext*>(context), previousStateId); }
			virtual void DoLeave(int nextStateId) override {}
			virtual bool DoStep(float deltaTime) override { return false; }
			virtual bool DoUpdate(hh::fnd::UpdatingPhase phase, float deltaTime) override { return true; }
			virtual bool DoUpdateAsync(hh::fnd::UpdatingPhase phase, float deltaTime) override { return false; }
			virtual bool DoProcessMessage(const hh::fnd::Message& message) override { return false; }

			virtual void Enter(BossTestContext& context, int previousStateId) override {
				context.object->GetComponent<hh::gfx::GOCVisualModel>()->SetLocalScale({ 20, 20, 20 });
			}
		};
	};

	struct StateDescc {
		unsigned int idx;
		hh::ut::StateDesc* stateDesc;
	};

	static StateDescc states[]{
		{ 0, &BossTestContextState::Root::stateDesc },
		{ 1, &BossTestContextState::TestAttack::stateDesc }
	};

	struct ObjTestBossSpawner {
		int dummy;

		static const hh::fnd::RflClass rflClass;
		static const hh::fnd::RflTypeInfo rflTypeInfo;
	};

	class ObjTestBoss : public hh::game::GameObject, public app::game::HealthListener {
		GAMEOBJECT_CLASS_DECLARATION(ObjTestBoss);

	public:
		BossTestContext* context;
		float timer = 22.523f;
		bool timerDone = false;
		hh::gfx::ModelNodeHFrame* referenceBoneFrame;
		hh::game::GOCTransform* transform;
		hh::physics::GOCCollider* collider;

		virtual bool ProcessMessage(hh::fnd::Message& message) override;
		virtual void AddCallback(hh::game::GameManager* gameManager) override;
		virtual void UpdateAsync(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo, void* unkParam) override;
		virtual void DamageDealt(app::MsgDamage& message, int damage, bool stunned, bool staggered) override;
	};
}

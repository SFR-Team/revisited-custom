#include "ObjTestBoss.h"

using namespace hh::eff;
using namespace hh::fnd;
using namespace hh::game;
using namespace hh::gfx;
using namespace hh::physics;
using namespace hh::snd;

using namespace revisited::objects;

hh::ut::StateDesc BossTestContextState::Root::stateDesc{ "Root", reinterpret_cast<hh::ut::internal::StateDescImpl::Instantiator>(&BossTestContextState::Root::Create), -1 };
hh::ut::StateDesc BossTestContextState::TestAttack::stateDesc{ "TestAttack", reinterpret_cast<hh::ut::internal::StateDescImpl::Instantiator>(&BossTestContextState::TestAttack::Create), 0 };

CAPTION_ATTRIBUTE(dummy)

const RflClassMember spawnerMembers[]{
	{ "dummy", nullptr, nullptr, RflClassMember::Type::SINT32, RflClassMember::Type::VOID, 0, 0, offsetof(ObjTestBossSpawner, dummy), &dummyAttributes }
};

RFL_CLASS(ObjTestBossSpawner)

RFL_TYPE_INFO_FUNCS(ObjTestBossSpawner, {
	self->dummy = 0;
})

RFL_TYPE_INFO(ObjTestBossSpawner);

GAMEOBJECT_CLASS_RFL(ObjTestBoss, Test)

GameObject* ObjTestBoss::Create(csl::fnd::IAllocator* allocator) {
	return new (std::align_val_t(16), allocator) ObjTestBoss{ allocator };
}

ObjTestBoss::ObjTestBoss(csl::fnd::IAllocator* allocator) : GameObject{ allocator } {
	SetLayer(7);
	context = new (std::align_val_t(16), allocator) BossTestContext{ allocator };
	hh::fnd::PropertyValue prop{};
	prop.m_Data = 2;
	SetProperty(0x4001, prop);
	app::game::SetPropertyFlag(this, 0x5001, 0);
	app::game::SetPropertyFlag(this, 0x5001, 5);
	app::game::SetPropertyFlag(this, 0x5001, 4);
	app::game::SetPropertyFlag(this, 0x5001, 0xB);
	SetUpdateFlag(hh::fnd::UpdatingPhase::PRE_ANIM, 1);
}

void* ObjTestBoss::GetRuntimeTypeInfo() const
{
	return nullptr;
}

bool ObjTestBoss::ProcessMessage(hh::fnd::Message& message)
{
	
	switch (message.ID) {
	case MessageID::GET_HOMING_TARGET_INFO: {
		auto& msg = (app::player::MsgGetHomingTargetInfo&)message;
		auto comp = **(hh::fnd::Handle<hh::game::GOComponent>*)&msg.homingTargetInfo.unk1;
		csl::math::Vector3 pos{ referenceBoneFrame->fullTransform.position + csl::math::Vector3{0, 2, 0} };

		WorldPosition worldPos{};
		worldPos.m_Position = pos;
		worldPos.m_Rotation = GetComponent<GOCVisualModel>()->frame2->fullTransform.rotation;

		msg.homingTargetInfo.unk2 = 3;
		msg.homingTargetInfo.SetUnk1(pos, csl::math::Vector3::UnitY(), 1);
		msg.homingTargetInfo.SetSize(1, 1);
		msg.homingTargetInfo.SetWorldPosition(worldPos);
		return true;
	}
	default:
		return GameObject::ProcessMessage(message);
	}
}

void ObjTestBoss::AddCallback(GameManager* gameManager)
{
	auto* resMgr = ResourceManager::GetInstance();
	auto* worldData = GetWorldDataByClass<ObjTestBossSpawner>();
	auto state = status->GetObjectState(0);

	if (state == 1)
		return;

	transform = GetComponent<GOCTransform>();

	auto* resLoader = hh::fnd::ResourceLoader::Create(pAllocator);
	resLoader->LoadPackfile("customBoss.pac");

	app_cmn::rfl::GOCRflParameter::SetupInfo gocRflParamDesc{};
	gocRflParamDesc.capacity = 1;
	auto* gocRflParam = CreateComponent<app_cmn::rfl::GOCRflParameter>();
	gocRflParam->Setup(gocRflParamDesc);
	auto* rflSrc = resMgr->GetResource<hh::fnd::ResReflection>("boss_rifle");
	auto* rfl = static_cast<heur::rfl::BossRifleConfig*>(rflSrc->unpackedBinaryData);
	rfl->status.cameraParam.mainLookAtNodeName = csl::ut::VariableString("Head", 5, pAllocator);
	gocRflParam->SetParameter(rflSrc, 0);
	AddComponent(gocRflParam);

	context->gocRfl = gocRflParam;
	context->cameraParams = rfl->status.cameraParam;
	context->SetGameObject(gameManager, this);
	context->object = this;

	app::game::GOCBossActionSelect::Description gocBossActionSelectDesc{};
	gocBossActionSelectDesc.context = context;
	auto* gocBossActionSelect = CreateComponent<app::game::GOCBossActionSelect>();
	gocBossActionSelect->Setup(gocBossActionSelectDesc);
	auto* plugin = BossActionPluginTest::Create(pAllocator);
	plugin->context = context;
	gocBossActionSelect->bossActionPluginMgr->plugins.push_back(plugin);
	AddComponent(gocBossActionSelect);

	app_cmn::fsm::GOCHsm2::Description gocHsmDesc{};
	gocHsmDesc.stateCount = 1;
	gocHsmDesc.stateCapacity = 1;
	gocHsmDesc.stateStart = 0;
	gocHsmDesc.updatePriority = -2;
	gocHsmDesc.hsmContext = context;
	gocHsmDesc.preAnimUpdate = 1;
	gocHsmDesc.states = &states;
	auto* gocHsm = CreateComponent<app_cmn::fsm::GOCHsm2>();
	gocHsm->Setup(gocHsmDesc);
	AddComponent(gocHsm);
	context->gocHsm2 = gocHsm;

	context->SetCurrentPhase(0);

	auto* model = resMgr->GetResource<ResModel>("bossMdl");
	auto* skel = resMgr->GetResource<hh::anim::ResSkeletonPxd>("bossSkl");
	GOCVisualModelDescription gocVisualModelDesc{};
	gocVisualModelDesc.model = model;
	gocVisualModelDesc.skeleton = skel;
	gocVisualModelDesc.nameHash = csl::ut::HashString("boosModel");
	gocVisualModelDesc.flags.bits |= 0x62000000;
	auto* gocVisual = CreateComponent<GOCVisualModel>();
	gocVisual->Setup(gocVisualModelDesc);
	AddComponent(gocVisual);

	GOCSphereCollider::SetupInfo gocColliderDesc{};
	gocColliderDesc.radius = 1;
	gocColliderDesc.filterCategory = 16;
	gocColliderDesc.unk5 = 0x2000001;
	gocColliderDesc.unk2 = 2;
	gocColliderDesc.SetPosition(csl::math::Vector3{ 0.0, 0.5f, 0.0 });

	auto* gocCollider = CreateComponent<GOCSphereCollider>();
	gocCollider->Setup(gocColliderDesc);
	AddComponent(gocCollider);
	collider = gocCollider;

	GOCEffect::Description gocEffectDesc{};
	auto* gocEffect = CreateComponent<GOCEffect>();
	gocEffect->Setup(gocEffectDesc);
	AddComponent(gocEffect);

	GOCSound::SetupInfo gocSoundDesc{};
	auto* gocSound = CreateComponent<GOCSound>();
	gocSound->Setup(gocSoundDesc);
	AddComponent(gocSound);

	app::game::GOCHealth::SetupInfo gocHealthDesc{};
	gocHealthDesc.currentHealth = 9999999999999999;
	gocHealthDesc.maxHealth = 9999999999999999;
	auto* gocHealth = CreateComponent<app::game::GOCHealth>();
	gocHealth->Setup(gocHealthDesc);
	AddComponent(gocHealth);
	gocHealth->AddListener(this);

	hh::anim::GOCAnimator::SetupInfo gocAnimatorDesc{};
	gocAnimatorDesc.skeleton = skel;
	gocAnimatorDesc.asmResourceManager = hh::anim::AsmResourceManager::Load("bossAsm", gocAnimatorDesc.skeleton, pAllocator);
	gocAnimatorDesc.gocVisualModelNameHash = csl::ut::HashString("bossModel");
	gocAnimatorDesc.nameHash = csl::ut::HashString("bossAnimator");
	gocAnimatorDesc.setUnk6Flag = 1;
	auto* gocAnimator = CreateComponent<hh::anim::GOCAnimator>();
	gocAnimator->Setup(gocAnimatorDesc);
	AddComponent(gocAnimator);
	gocAnimator->ChangeState("IDLE01");

	referenceBoneFrame = new (std::align_val_t(16), pAllocator) ModelNodeHFrame(pAllocator, gocVisual, "Reference");
	gocVisual->frame2->AddChild(referenceBoneFrame);
}

void ObjTestBoss::UpdateAsync(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo, void* unkParam)
{
	csl::math::Vector3 newPos{ (referenceBoneFrame->fullTransform.position + csl::math::Vector3{ 0, 2, 0 }) - transform->GetFrame().fullTransform.position };
	collider->localWorldPosition.m_Position = newPos;
	collider->frame->MarkUpdate();
	collider->flags.set(hh::physics::GOCCollider::Flag::FRAME_DIRTY);

	if (context)
		context->Update(phase, const_cast<hh::fnd::SUpdateInfo&>(updateInfo));
	if (timer < 0 && !timerDone)
	{
		timerDone = true;
		GetComponent<hh::anim::GOCAnimator>()->ChangeState("IDLE02");
	}
	else if (timer > 0 && !timerDone) {
		timer -= updateInfo.deltaTime;
	}
}

void ObjTestBoss::DamageDealt(app::MsgDamage& message, int damage, bool stunned, bool staggered)
{
	message.dword110 |= 1;
	if (context)
		context->HandleDamage(message);
}

const hh::game::GameObjectClass* ObjTestBoss::GetClass()
{
	return &gameObjectClass;
}

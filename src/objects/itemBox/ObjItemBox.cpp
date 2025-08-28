#include "ObjItemBox.h"
#include "UIItemBox.h"
#include "player/BlackboardRevisited.h"

using namespace csl::fnd;

using namespace hh::anim;
using namespace hh::eff;
using namespace hh::fnd;
using namespace hh::game;
using namespace hh::gfx;
using namespace hh::physics;
using namespace hh::snd;
using namespace hh::ui;

using namespace app;
using namespace app::game;
using namespace app::player;

using namespace revisited::objects;
using namespace revisited::player;


GAMEOBJECT_CLASS_RFL_OBJINFO(ObjItemBox, Common);

GameObject* ObjItemBox::Create(IAllocator* allocator) {
	return new (std::align_val_t(16), allocator) ObjItemBox{ allocator };
}

ObjItemBox::ObjItemBox(IAllocator* allocator) : GameObject{ allocator }, type{ ObjItemBoxSpawner::ItemType::ROCKET } {
	SetLayer(6);
	SetUpdateFlag(UpdatingPhase::PRE_ANIM, true);
	SetPropertyFlag(this, 0x5001, 0);
}

bool ObjItemBox::ProcessMessage(Message& message)
{
	switch (message.ID) {
	case MessageID::TRIGGER_ENTER: {
		if (auto* gocCollider = GetComponent<GOCSphereCollider>())
			if (gocCollider->flags.test(GOCCollider::Flag::ENABLED))
				DestroyCallback();
		return true;
	}
	case MessageID::GET_HOMING_TARGET_INFO: {
		auto& msg = (MsgGetHomingTargetInfo&)message;
		csl::math::Vector3 pos{ GetComponent<GOCTransform>()->GetFrame().fullTransform.position + csl::math::Vector3{0, GetComponent<GOCCollider>()->localWorldPosition.m_Position.y(), 0}};

		WorldPosition worldPos{};
		worldPos.m_Position = pos;

		msg.homingTargetInfo.SetSize(.5f, .5f);
		msg.homingTargetInfo.SetWorldPosition(worldPos);
		return true;
	}
	default:
		return GameObject::ProcessMessage(message);
	}
}

void ObjItemBox::AddCallback(GameManager* gameManager)
{
	if (!gameManager->GetGameObject("UIItemBox"))
		gameManager->AddGameObject(GameObject::Create<UIItemBox>(pAllocator), "UIItemBox", false, nullptr, nullptr);

	auto* worldData = GetWorldDataByClass<ObjItemBoxSpawner>();
	auto state = status->GetObjectState(0);

	if (state == 1)
		return;

	type = worldData->type;
	canAirDashRocket = worldData->canAirDashRocket;
	doBounce = worldData->doBounce;
	bool isAir = worldData->isAir;

	auto* objInfo = GetObjInfo<ObjItemBoxInfo>(gameManager);

	auto model = objInfo->GetModel(isAir, type);
	auto skeleton = objInfo->GetSkeleton(isAir);
	GOCVisualModelDescription gocVisualModelDesc{};
	gocVisualModelDesc.model = model;
	gocVisualModelDesc.skeleton = skeleton;
	gocVisualModelDesc.flags.set(GOCVisualModelDescription::Flag::NO_MATERIAL_OPTIMIZE);
	gocVisualModelDesc.useGISG = true;
	gocVisualModelDesc.useGIPRT = true;
	gocVisualModelDesc.name = csl::ut::HashString("mainModel");
	auto* gocVisual = CreateComponent<GOCVisualModel>();
	gocVisual->Setup(gocVisualModelDesc);
	AddComponent(gocVisual);

	GOCSphereCollider::SetupInfo gocColliderDesc{};
	gocColliderDesc.radius = .5f;
	gocColliderDesc.filterCategory = 18;
	gocColliderDesc.unk2 = 0;
	gocColliderDesc.overlapFlags.set(GOCCollider::OverlapFlag::ENTER);
	gocColliderDesc.unk4 = 0x8000;
	gocColliderDesc.SetPosition(csl::math::Vector3{ 0, 0.8f, 0 });

	auto* gocCollider = CreateComponent<GOCSphereCollider>();
	gocCollider->Setup(gocColliderDesc);
	AddComponent(gocCollider);

	GOCSound::SetupInfo gocSoundDesc{};
	auto* gocSound = CreateComponent<GOCSound>();
	gocSound->Setup(gocSoundDesc);
	AddComponent(gocSound);

	GOCEffect::Description gocEffectDesc{};
	auto* gocEffect = CreateComponent<GOCEffect>();
	gocEffect->Setup(gocEffectDesc);
	AddComponent(gocEffect);

	GOCAnimationSimple::SetupInfo gocAnimationSimpleDesc{};
	gocAnimationSimpleDesc.skeleton = skeleton;
	gocAnimationSimpleDesc.animationCount = 2;
	gocAnimationSimpleDesc.setPose = true;
	gocAnimationSimpleDesc.modelComponentName = csl::ut::HashString("mainModel");
	auto* gocAnimationSimple = CreateComponent<GOCAnimationSimple>();
	gocAnimationSimple->Setup(gocAnimationSimpleDesc);
	AddComponent(gocAnimationSimple);

	gocAnimationSimple->Add("idle", objInfo->GetIdle(isAir), PlayPolicy::REPEAT);
	gocAnimationSimple->Add("get", objInfo->GetGet(isAir), PlayPolicy::NORMAL);

	gocAnimationSimple->Play("idle");

	auto* gocVibration = CreateComponent<GOCVibration>();
	AddComponent(gocVibration);
}

void ObjItemBox::Update(UpdatingPhase phase, const SUpdateInfo& updateInfo)
{
	animationTimer.Add(updateInfo.deltaTime);
	if (auto* gocAnim = GetComponent<GOCAnimationSimple>())
		if (!animationTimer.IsActive() && gocAnim->IsPlaying("get"))
			PostDestroyCallback();
}

void ObjItemBox::DestroyCallback()
{
	auto* gocSound = GetComponent<GOCSound>();
	switch (type) {
	case ObjItemBoxSpawner::ItemType::TEN_RING:
		GivePlayerRings(10);
		break;
	case ObjItemBoxSpawner::ItemType::TWENTY_RING:
		GivePlayerRings(20);
		break;
	case ObjItemBoxSpawner::ItemType::FIFTY_RING:
		GivePlayerRings(50);
		break;
	case ObjItemBoxSpawner::ItemType::ULTRA: {
		if (auto* gocBlackboard = GetPlayer()->GetComponent<GOCPlayerBlackboard>())
		if (auto& blackboard = gocBlackboard->blackboard)
		if (auto* blackboardRev = blackboard->GetContent<BlackboardRevisited>())
			blackboardRev->GiveInvincibility();
		break;
	}
	case ObjItemBoxSpawner::ItemType::MAGNETIC: {
		if (auto* gocBlackboard = GetPlayer()->GetComponent<GOCPlayerBlackboard>())
		if (auto& blackboard = gocBlackboard->blackboard)
		if (auto* blackboardRev = blackboard->GetContent<BlackboardRevisited>())
			blackboardRev->GiveMagnetic();
		break;
	}
	case ObjItemBoxSpawner::ItemType::ROCKET: {
		MsgTakeNitroBoost msgNitro{};
		msgNitro.canAirBoost = canAirDashRocket;
		ut::SendMessageImmToPlayerObject(*this, 0, msgNitro);
		gocSound->Play("sys_upd_cyber_nitrobottle_get", 1);
		break;
	}
	case ObjItemBoxSpawner::ItemType::INFINITE_BOOST: {
		if (auto* gocBlackboard = GetPlayer()->GetComponent<GOCPlayerBlackboard>())
		if (auto& blackboard = gocBlackboard->blackboard)
		if (auto* blackboardRev = blackboard->GetContent<BlackboardRevisited>())
			blackboardRev->GiveInfiniteBoost();
		break;
	}
	}

	auto* gocAnim = GetComponent<GOCAnimationSimple>();
	gocAnim->playingAnimations.clear();
	gocAnim->Play("get");
	animationTimer.Set(gocAnim->GetDuration("get") - 0.01f);

	GetComponent<GOCSphereCollider>()->SetEnabled(false);

	GetComponent<GOCSound>()->Play3D("obj_itembox", GetComponent<GOCTransform>()->GetFrame().fullTransform.position, 0);

	GetComponent<GOCEffect>()->CreateEffect("ef_ob_itembox_get01", nullptr);

	if (auto* ui = (UIItemBox*)gameManager->GetGameObject("UIItemBox"))
		ui->SetVisible(type);

	GetComponent<GOCVibration>()->PlayVibration("low", 0);

	if (doBounce)
		SendBounceMessage();
}

void ObjItemBox::PostDestroyCallback()
{
	status->SetObjectState(0, false);
	GetComponent<GOCActivator>()->enabled = false;
	GetComponent<GOCVisualModel>()->SetVisible(false);
}

void ObjItemBox::GiveObject(unsigned int amount, MsgTakeObject::Type type)
{
	MsgTakeObject msg{};
	msg.amount = amount;
	msg.showEffect = true;
	msg.objectType = type;
	ut::SendMessageImmToPlayerObject(*this, 0, msg);
}

csl::math::Vector3 ObjItemBox::CalculateBounce()
{
	const float speedMax = 5;
	const float speedMin = 0;
	const float upSpeed = 10;

	csl::math::Vector3 playerVelocity{};

	if (auto* levelInfo = GetService<app::level::LevelInfo>())
		if (auto* playerInfo = levelInfo->GetPlayerInformation(0))
			playerVelocity = playerInfo->vector490.value();

	csl::math::Vector3 horizontalVel = playerVelocity;
	horizontalVel.y() = 0;

	float horizontalSpeed = horizontalVel.norm();

	float clampedSpeed = horizontalSpeed;
	if (speedMax >= horizontalSpeed) 
		if (horizontalSpeed < speedMin) 
			clampedSpeed = speedMin;
	else
		clampedSpeed = speedMax;

	if (speedMin <= 0.0f) clampedSpeed = speedMax;

	csl::math::Vector3 bounceVel = upSpeed * csl::math::Vector3::UnitY();

	if (horizontalSpeed > 1e-8f) {
		csl::math::Vector3 horizontalDir = horizontalVel / horizontalSpeed;
		bounceVel += horizontalDir * clampedSpeed;
	}

	return bounceVel;
}

void ObjItemBox::SendBounceMessage()
{
	csl::math::Vector3 bounceVel = CalculateBounce();
	MsgSpringImpulse message{};
	message.velocity = bounceVel;
	message.outOfControlTime = 0.1f;
	message.keepVelocityTime = 0.1f;
	message.unk3 = 2082;
	ut::SendMessageImmToPlayerObject(*this, 0, message);
}

#include "ObjItemBox.h"
#include "UIItemBox.h"
#include "player/BlackboardRevisited.h"

using namespace csl::fnd;

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


GAMEOBJECT_CLASS(ObjItemBox, Common)

GameObject* ObjItemBox::Create(IAllocator* allocator) {
	return new (std::align_val_t(16), allocator) ObjItemBox{ allocator };
}

ObjItemBox::ObjItemBox(IAllocator* allocator) : GameObject{ allocator }, type{ ObjItemBoxSpawner::ItemType::ROCKET } {
	SetLayer(6);
	SetPropertyFlag(this, 0x5001, 0);
}

void* ObjItemBox::GetRuntimeTypeInfo() const
{
	return nullptr;
}

bool ObjItemBox::ProcessMessage(Message& message)
{
	switch (message.ID) {
	case MessageID::COLLIDER_QUERY_ENTER: {
		DestroyCallback();
		return true;
	}
	case MessageID::DAMAGE: {
		auto& msg = (MsgDamage&)message;
		msg.dword110 |= 1;
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
	case MessageID::PARAM_CHANGED_IN_EDITOR: {
		SetEnabled(true);
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

	ui = (UIItemBox*)gameManager->GetGameObject("UIItemBox");

	auto* resMgr = ResourceManager::GetInstance();
	auto* worldData = GetWorldDataByClass<ObjItemBoxSpawner>();
	auto state = status->GetObjectState(0);

	if (state == 1)
		return;

	type = worldData->type;
	canAirDashRocket = worldData->canAirDashRocket;

	const char* bmodelName = "";
	switch (worldData->type) {
	case ObjItemBoxSpawner::ItemType::TEN_RING:
		bmodelName = "cmn_obj_ring10";
		break;
	case ObjItemBoxSpawner::ItemType::TWENTY_RING:
		bmodelName = "cmn_obj_ring20";
		break;
	case ObjItemBoxSpawner::ItemType::FIFTY_RING:
		bmodelName = "cmn_obj_ring50";
		break;
	case ObjItemBoxSpawner::ItemType::MAGNETIC:
		bmodelName = "cmn_obj_magnet";
		break;
	case ObjItemBoxSpawner::ItemType::ULTRA:
		bmodelName = "cmn_obj_ultra";
		break;
	case ObjItemBoxSpawner::ItemType::ROCKET:
		bmodelName = "cmn_obj_rocket";
		break;
	case ObjItemBoxSpawner::ItemType::INFINITE_BOOST:
		bmodelName = "cmn_obj_infiniteboost";
		break;
	}

	const char* modelType = "ground";

	if (worldData->isAir)
		modelType = "float";

	char modelName[32];
	snprintf(modelName, sizeof(modelName), "%s%s", bmodelName, modelType);

	auto* model = resMgr->GetResource<ResModel>(modelName);
	GOCVisualModelDescription gocVisualModelDesc{};
	gocVisualModelDesc.model = model;
	auto* gocVisual = CreateComponent<GOCVisualModel>();
	gocVisual->Setup(gocVisualModelDesc);
	AddComponent(gocVisual);

	GOCSphereCollider::SetupInfo gocColliderDesc{};
	gocColliderDesc.radius = .5f;
	gocColliderDesc.filterCategory = 65554;
	gocColliderDesc.unk4 = 0x4008000;
	gocColliderDesc.SetPosition(csl::math::Vector3{ 0.0, 0.8f, 0.0 });

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
		auto* gocBlackboard = GetPlayer()->GetComponent<GOCPlayerBlackboard>();
		auto& blackboard = gocBlackboard->blackboard;
		auto* blackboardRev = blackboard->GetContent<BlackboardRevisited>();
		blackboardRev->GiveInvincibility();
		break;
	}
	case ObjItemBoxSpawner::ItemType::MAGNETIC: {
		auto* gocBlackboard = GetPlayer()->GetComponent<GOCPlayerBlackboard>();
		auto& blackboard = gocBlackboard->blackboard;
		auto* blackboardRev = blackboard->GetContent<BlackboardRevisited>();
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
		auto* gocBlackboard = GetPlayer()->GetComponent<GOCPlayerBlackboard>();
		auto& blackboard = gocBlackboard->blackboard;
		auto* blackboardRev = blackboard->GetContent<BlackboardRevisited>();
		blackboardRev->GiveInfiniteBoost();
		break;
	}
	}

	SetEnabled(false);
	gocSound->Play3D("obj_itembox", GetComponent<GOCTransform>()->GetFrame().fullTransform.position, 0);
	GetComponent<GOCEffect>()->CreateEffect("ef_bo_rifle01_step01_ge01", nullptr);
	ui->SetVisible(type);
}

void ObjItemBox::GivePlayerRings(unsigned int amount)
{
	GiveObject(amount, MsgTakeObject::Type::RING);
}

void ObjItemBox::GiveObject(unsigned int amount, MsgTakeObject::Type type)
{
	MsgTakeObject msg{};
	msg.amount = amount;
	msg.showEffect = true;
	msg.objectType = type;
	ut::SendMessageImmToPlayerObject(*this, 0, msg);
}

void ObjItemBox::SetEnabled(bool enabled)
{
	status->SetObjectState(0, !enabled);
	GetComponent<GOCActivator>()->enabled = enabled;
	GetComponent<GOCSphereCollider>()->SetEnabled(enabled);
	GetComponent<GOCVisualModel>()->SetVisible(enabled);
}

const GameObjectClass* ObjItemBox::GetClass()
{
	return &gameObjectClass;
}

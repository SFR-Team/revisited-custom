#include "ObjHomingGismo.h"
#include "../utilities/GameHelpers.h"

using namespace csl::fnd;

using namespace hh::eff;
using namespace hh::fnd;
using namespace hh::game;
using namespace hh::gfx;
using namespace hh::physics;
using namespace hh::snd;

using namespace app;
using namespace app::player;

using namespace revisited::objects;


CAPTION_ATTRIBUTE(modelName)
CAPTION_ATTRIBUTE(particleName)
CAPTION_ATTRIBUTE(soundName)

const RflClassMember spawnerMembers[]{
	{ "modelName", nullptr, nullptr, RflClassMember::Type::STRING, RflClassMember::Type::VOID, 0, 0, offsetof(ObjHomingGismoSpawner, modelName), &modelNameAttributes },
	{ "particleName", nullptr, nullptr, RflClassMember::Type::STRING, RflClassMember::Type::VOID, 0, 0, offsetof(ObjHomingGismoSpawner, particleName), &particleNameAttributes },
	{ "soundName", nullptr, nullptr, RflClassMember::Type::STRING, RflClassMember::Type::VOID, 0, 0, offsetof(ObjHomingGismoSpawner, soundName), &soundNameAttributes }
};

RFL_CLASS(ObjHomingGismoSpawner);

RFL_TYPE_INFO_FUNCS_CLEAN(ObjHomingGismoSpawner, {
	new (&self->modelName) csl::ut::VariableString{ allocator };
	new (&self->particleName) csl::ut::VariableString{ allocator };
	new (&self->soundName) csl::ut::VariableString{allocator};
}
{
	self->modelName.~VariableString();
	self->particleName.~VariableString();
	self->soundName.~VariableString();
}
);

RFL_TYPE_INFO(ObjHomingGismoSpawner)

GAMEOBJECT_CLASS_RFL(ObjHomingGismo, Common);

GameObject* ObjHomingGismo::Create(IAllocator* allocator) {
	return new (std::align_val_t(16), allocator) ObjHomingGismo{ allocator };
}

ObjHomingGismo::ObjHomingGismo(IAllocator* allocator) : GameObject{ allocator }, soundName{ allocator }, particleName{ allocator } {
	SetLayer(6);
	app::game::SetPropertyFlag(this, 0x5001, 0);
}

void* ObjHomingGismo::GetRuntimeTypeInfo() const
{
	return nullptr;
}

bool ObjHomingGismo::ProcessMessage(Message& message)
{
	switch (message.ID) {
	case MessageID::DAMAGE: {
		auto& msg = (MsgDamage&)message;
		msg.dword110 |= 1;
		DestroyCallback();
		return true;
	}
	case MessageID::GET_HOMING_TARGET_INFO: {
		auto& msg = (MsgGetHomingTargetInfo&)message;
		csl::math::Vector3 pos{ GetComponent<GOCTransform>()->GetFrame().fullTransform.position + csl::math::Vector3{0, .6f, 0} };

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

void ObjHomingGismo::AddCallback(GameManager* gameManager)
{
	auto* resMgr = ResourceManager::GetInstance();
	auto* worldData = GetWorldDataByClass<ObjHomingGismoSpawner>();
	auto state = status->GetObjectState(0);

	if (state == 1)
		return;

	particleName = worldData->particleName;
	soundName = worldData->soundName;

	auto* model = resMgr->GetResource<ResModel>(worldData->modelName);
	GOCVisualModelDescription gocVisualModelDesc{};
	gocVisualModelDesc.model = model;
	auto* gocVisual = CreateComponent<GOCVisualModel>();
	gocVisual->Setup(gocVisualModelDesc);
	AddComponent(gocVisual);

	GOCSphereCollider::SetupInfo gocColliderDesc{};
	gocColliderDesc.radius = .5f;
	gocColliderDesc.filterCategory = 18;
	gocColliderDesc.unk4 = 0x8000;
	gocColliderDesc.SetPosition(csl::math::Vector3{ 0.0, 0.6f, 0.0 });

	auto* gocCollider = CreateComponent<GOCSphereCollider>();
	gocCollider->Setup(gocColliderDesc);
	AddComponent(gocCollider);

	GOCEffect::Description gocEffectDesc{};
	auto* gocEffect = CreateComponent<GOCEffect>();
	gocEffect->Setup(gocEffectDesc);
	AddComponent(gocEffect);

	GOCSound::SetupInfo gocSoundDesc{};
	auto* gocSound = CreateComponent<GOCSound>();
	gocSound->Setup(gocSoundDesc);
	AddComponent(gocSound);
}

void ObjHomingGismo::DestroyCallback()
{
	status->SetObjectState(0, 1);
	GetComponent<GOCActivator>()->enabled = false;
	GetComponent<GOCSphereCollider>()->SetEnabled(false);
	GetComponent<GOCVisualModel>()->SetVisible(false);
	GetComponent<GOCSound>()->Play3D(soundName, GetComponent<GOCTransform>()->GetFrame().fullTransform.position, 0);
	GetComponent<GOCEffect>()->CreateEffect(particleName, nullptr);
}

const GameObjectClass* ObjHomingGismo::GetClass()
{
	return &gameObjectClass;
}

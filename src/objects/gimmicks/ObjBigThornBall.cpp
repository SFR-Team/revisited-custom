#include "ObjBigThornBall.h"

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


GAMEOBJECT_CLASS_RFL_OBJINFO(ObjBigThornBall, Common);

GameObject* ObjBigThornBall::Create(IAllocator* allocator) {
	return new (std::align_val_t(16), allocator) ObjBigThornBall{ allocator };
}

ObjBigThornBall::ObjBigThornBall(IAllocator* allocator) : GameObject{ allocator } {
	SetLayer(6);
	SetUpdateFlag(UpdatingPhase::PRE_ANIM, true);
	SetPropertyFlag(this, 0x5001, 1);
}

void ObjBigThornBall::AddCallback(GameManager* gameManager) {
	auto* params = GetWorldDataByClass<ObjBigThornBallSpawner>();
	auto* objInfo = GetObjInfo<ObjBigThornBallInfo>(gameManager);

	UpdateScale();

	auto* visualModel = CreateComponent<GOCVisualModel>();
	GOCVisualModelDescription visualModelDesc{};
	visualModelDesc.model = objInfo->model;
	visualModel->Setup(visualModelDesc);
	AddComponent(visualModel);

	auto* collider = CreateComponent<GOCSphereCollider>();
	GOCSphereCollider::SetupInfo colliderDesc{};
	colliderDesc.unk2 = 2;
	colliderDesc.unk5 = 0x50000;
	colliderDesc.radius = params->scale;
	collider->Setup(colliderDesc);
	AddComponent(collider);
}

bool ObjBigThornBall::ProcessMessage(Message& msg) {
	switch (msg.ID) {
	case MessageID::PARAM_CHANGED_IN_EDITOR: {
		UpdateScale();
		return true;
	}
	}
	return true;
}

void ObjBigThornBall::UpdateScale() {
	auto* params = GetWorldDataByClass<ObjBigThornBallSpawner>();

	auto scale = csl::math::Vector3{ params->scale, params->scale, params->scale };

	if (auto* transform = GetComponent<GOCTransform>()) {
		auto& trans = transform->GetTransform();
		trans.scale = scale;
		transform->SetLocalTransform(trans);
	}

	if (auto* collider = GetComponent<GOCSphereCollider>())
		collider->radius = params->scale;
}

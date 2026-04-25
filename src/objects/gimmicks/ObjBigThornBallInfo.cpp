#include "ObjBigThornBall.h"

using namespace csl::fnd;

using namespace hh::anim;
using namespace hh::fnd;
using namespace hh::gfx;
using namespace hh::game;
using namespace hh::snd;

using namespace revisited::objects;

OBJINFO_CLASS(ObjBigThornBallInfo, ObjBigThornBall)

ObjInfo* ObjBigThornBallInfo::Create(IAllocator* allocator) {
	return new (std::align_val_t(16), allocator) ObjBigThornBallInfo{ allocator };
}

ObjBigThornBallInfo::ObjBigThornBallInfo(IAllocator* allocator) : ObjInfo{ allocator } {}

void ObjBigThornBallInfo::Load() {
	resourceLoader->LoadPackfile("stage/RevisitedObject.pac");

	//levelLoader->LoadLevel("bigthornball", {});
}

void ObjBigThornBallInfo::Initialize(GameManager* gameManager) {
	auto* resMgr = ResourceManager::GetInstance();
	model = resMgr->GetResource<ResModel>("cmn_obj_thornball");
}

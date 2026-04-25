#include "ObjCostumeSwitcher.h"

using namespace csl::fnd;

using namespace hh::fnd;
using namespace hh::gfx;
using namespace hh::game;
using namespace hh::physics;

using namespace revisited::objects;

OBJINFO_CLASS(ObjCostumeSwitcherInfo)

ObjInfo* ObjCostumeSwitcherInfo::Create(IAllocator* allocator) {
	return new (allocator) ObjCostumeSwitcherInfo{ allocator };
}

ObjCostumeSwitcherInfo::ObjCostumeSwitcherInfo(IAllocator* allocator) : ObjInfo{ allocator } {}

void ObjCostumeSwitcherInfo::Load()
{
	resourceLoader->LoadPackfile("stage/RevisitedObject.pac");
}

void ObjCostumeSwitcherInfo::Initialize(GameManager* gameManager)
{
	model = ResourceManager::GetInstance()->GetResource<ResModel>("costume_pad");
	col = ResourceManager::GetInstance()->GetResource<ResPhysicsMeshBullet>("costume_pad_col");
}

#include "ObjCostumeSwitcher.h"

using namespace csl::fnd;

using namespace hh::fnd;
using namespace hh::gfx;
using namespace hh::game;

using namespace revisited::objects;

OBJINFO_CLASS(ObjCostumeSwitcherInfo)

ObjInfo* ObjCostumeSwitcherInfo::Create(IAllocator* allocator) {
	return new (allocator) ObjCostumeSwitcherInfo{ allocator };
}

ObjCostumeSwitcherInfo::ObjCostumeSwitcherInfo(IAllocator* allocator) : ObjInfo{ allocator } {}

void ObjCostumeSwitcherInfo::Load()
{

}

void ObjCostumeSwitcherInfo::Initialize(GameManager* gameManager)
{
	model = ResourceManager::GetInstance()->GetResource<ResModel>("chr_sonic");
}

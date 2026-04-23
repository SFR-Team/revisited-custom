#include "ObjCharacterSwitcher.h"

using namespace csl::fnd;

using namespace hh::fnd;
using namespace hh::gfx;
using namespace hh::game;

using namespace revisited::objects;

OBJINFO_CLASS(ObjCharacterSwitcherInfo)

ObjInfo* ObjCharacterSwitcherInfo::Create(IAllocator* allocator) {
	return new (allocator) ObjCharacterSwitcherInfo{ allocator };
}

ObjCharacterSwitcherInfo::ObjCharacterSwitcherInfo(IAllocator* allocator) : ObjInfo{ allocator } {}

void ObjCharacterSwitcherInfo::Load()
{

}

void ObjCharacterSwitcherInfo::Initialize(GameManager* gameManager)
{
	model = ResourceManager::GetInstance()->GetResource<ResModel>("chr_sonic");
}

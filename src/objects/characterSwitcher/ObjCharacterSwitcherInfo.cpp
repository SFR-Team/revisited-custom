#include "ObjCharacterSwitcher.h"

using namespace csl::fnd;

using namespace hh::anim;
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
	levelLoader->LoadLevel("eggmanT");
}

void ObjCharacterSwitcherInfo::Initialize(GameManager* gameManager)
{
	auto* resMgr = ResourceManager::GetInstance();
	model = resMgr->GetResource<ResModel>("chr_eggmanT");
	skl = resMgr->GetResource<ResSkeletonPxd>("chr_eggmanT");
	asmRes = AsmResourceManager::Load("chr_eggmanT", skl, GetAllocator());
}

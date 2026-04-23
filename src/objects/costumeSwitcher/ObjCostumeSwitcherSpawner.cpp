#include "ObjCostumeSwitcher.h"

using namespace hh::fnd;

using namespace revisited::objects;

CAPTION_ATTRIBUTE(dummy)

const RflClassMember spawnerMembers[]{
	{ "dummy", nullptr, nullptr, RflClassMember::Type::UINT32, RflClassMember::Type::VOID, 0, 0, offsetof(ObjCostumeSwitcherSpawner, dummy), &dummyAttributes }
};

RFL_CLASS(ObjCostumeSwitcherSpawner);

RFL_TYPE_INFO_FUNCS(ObjCostumeSwitcherSpawner, {
	self->dummy = 0;
});

RFL_TYPE_INFO(ObjCostumeSwitcherSpawner)

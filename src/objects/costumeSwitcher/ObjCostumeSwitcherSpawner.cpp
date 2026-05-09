#include "ObjCostumeSwitcher.h"

using namespace hh::fnd;

using namespace revisited::objects;

CAPTION_ATTRIBUTE(dummy)

const RflClassMember spawnerMembers[]{
	{ "interactionRange", nullptr, nullptr, RflClassMember::Type::FLOAT, RflClassMember::Type::VOID, 0, 0, offsetof(ObjCostumeSwitcherSpawner, interactionRange), &dummyAttributes }
};

RFL_CLASS(ObjCostumeSwitcherSpawner);

RFL_TYPE_INFO_FUNCS(ObjCostumeSwitcherSpawner, {
	self->interactionRange = 2;
});

RFL_TYPE_INFO(ObjCostumeSwitcherSpawner)

#include "ObjItemBox.h"
#include "utilities/GameHelpers.h"

using namespace hh::fnd;

using namespace revisited::objects;

const RflClassEnumMember itemTypeMembers[]{
	ENUM_MEM(0, ROCKET),
	ENUM_MEM(1, TEN_RING),
	ENUM_MEM(2, TWENTY_RING),
	ENUM_MEM(3, FIFTY_RING),
	ENUM_MEM(4, ULTRA),
	ENUM_MEM(5, MAGNETIC),
	ENUM_MEM(6, INFINITE_BOOST)
};

const RflClassEnum spawnerEnums[]{
	{ "ItemType", itemTypeMembers, sizeof(itemTypeMembers) / sizeof(RflClassEnumMember) }
};

CAPTION_ATTRIBUTE(type)
CAPTION_ATTRIBUTE(isAir)
CAPTION_ATTRIBUTE(canAirDashRocket)
CAPTION_ATTRIBUTE(doBounce)

const RflClassMember spawnerMembers[]{
	{ "type",			  nullptr, &spawnerEnums[0], RflClassMember::Type::ENUM, RflClassMember::Type::UINT8, 0, 0, offsetof(ObjItemBoxSpawner, type),			   &typeAttributes },
	{ "isAir",			  nullptr, nullptr,			 RflClassMember::Type::BOOL, RflClassMember::Type::VOID,  0, 0, offsetof(ObjItemBoxSpawner, isAir),			   &isAirAttributes },
	{ "canAirDashRocket", nullptr, nullptr,			 RflClassMember::Type::BOOL, RflClassMember::Type::VOID,  0, 0, offsetof(ObjItemBoxSpawner, canAirDashRocket), &canAirDashRocketAttributes },
	{ "doBounce",		  nullptr, nullptr,			 RflClassMember::Type::BOOL, RflClassMember::Type::VOID,  0, 0, offsetof(ObjItemBoxSpawner, doBounce),		   &doBounceAttributes }
};

RFL_CLASS(ObjItemBoxSpawner);

RFL_TYPE_INFO_FUNCS(ObjItemBoxSpawner, {
	self->type = ObjItemBoxSpawner::ItemType::ROCKET;
	self->isAir = false;
	self->canAirDashRocket = false;
	self->doBounce = true;
});

RFL_TYPE_INFO(ObjItemBoxSpawner)

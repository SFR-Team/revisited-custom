#include "ObjDoorWarpTrigger.h"

using namespace hh::fnd;

using namespace revisited::objects;

CAPTION_ATTRIBUTE(interactionRange)
CAPTION_ATTRIBUTE(fadeSpeed)
CAPTION_ATTRIBUTE(targetObject)

const RflClassMember spawnerMembers[]{
	{ "interactionRange", nullptr, nullptr, RflClassMember::Type::FLOAT,		RflClassMember::Type::VOID, 0, 0, offsetof(ObjDoorWarpTriggerSpawner, interactionRange), &interactionRangeAttributes },
	{ "fadeSpeed",		  nullptr, nullptr, RflClassMember::Type::FLOAT,		RflClassMember::Type::VOID, 0, 0, offsetof(ObjDoorWarpTriggerSpawner, fadeSpeed),		 &fadeSpeedAttributes },
	{ "targetObject",	  nullptr, nullptr, RflClassMember::Type::OBJECT_ID_V2, RflClassMember::Type::VOID, 0, 0, offsetof(ObjDoorWarpTriggerSpawner, targetObject),	 &targetObjectAttributes },
};

RFL_CLASS(ObjDoorWarpTriggerSpawner);

RFL_TYPE_INFO_FUNCS(ObjDoorWarpTriggerSpawner, {
	self->interactionRange = 2;
	self->fadeSpeed = 1.3f;
	self->targetObject = {};
	});

RFL_TYPE_INFO(ObjDoorWarpTriggerSpawner)

#include "ObjBigThornBall.h"

using namespace hh::fnd;

using namespace revisited::objects;

CAPTION_ATTRIBUTE(scale)

const RflClassMember spawnerMembers[]{
	{ "scale", nullptr, nullptr, RflClassMember::Type::FLOAT, RflClassMember::Type::VOID, 0, 0, offsetof(ObjBigThornBallSpawner, scale), &scaleAttributes }
};

RFL_CLASS(ObjBigThornBallSpawner);

RFL_TYPE_INFO_FUNCS(ObjBigThornBallSpawner, {
	self->scale = 1.0f;
	});

RFL_TYPE_INFO(ObjBigThornBallSpawner)

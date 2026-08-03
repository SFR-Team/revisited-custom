#include "ObjBubblesSpawner.h"

using namespace hh::fnd;

using namespace revisited::objects;

CAPTION_ATTRIBUTE(bigBubbleFrequency)
CAPTION_ATTRIBUTE(oxygenRestoreAmount)

const RflClassMember spawnerMembers[]{
	{ "bigBubbleFrequency", nullptr, nullptr, RflClassMember::Type::FLOAT, RflClassMember::Type::VOID, 0, 0, offsetof(ObjBubblesSpawnerSpawner, bigBubbleFrequency), &bigBubbleFrequencyAttributes },
	{ "oxygenRestoreAmount", nullptr, nullptr, RflClassMember::Type::FLOAT, RflClassMember::Type::VOID, 0, 0, offsetof(ObjBubblesSpawnerSpawner, oxygenRestoreAmount), &oxygenRestoreAmountAttributes }
};

RFL_CLASS(ObjBubblesSpawnerSpawner);

RFL_TYPE_INFO_FUNCS(ObjBubblesSpawnerSpawner, {
	self->bigBubbleFrequency = 5.0f;
	self->oxygenRestoreAmount = 10.0f;
	});

RFL_TYPE_INFO(ObjBubblesSpawnerSpawner)

#include "../UIStageIntro.h"
#include "utilities/GameHelpers.h"

using namespace hh::fnd;
using namespace revisited::objects;

CAPTION_ATTRIBUTE(stageId)
CAPTION_ATTRIBUTE(cropId)

const RflClassMember spawnerMembers[]{
	{ "stageId", nullptr, nullptr, RflClassMember::Type::STRING, RflClassMember::Type::VOID, 0, 0, offsetof(Stage, stageId), &stageIdAttributes },
	{ "cropId",  nullptr, nullptr, RflClassMember::Type::SINT16, RflClassMember::Type::VOID, 0, 0, offsetof(Stage, cropId),  &cropIdAttributes }
};

RFL_CLASS(Stage);

RFL_TYPE_INFO_FUNCS_CLEAN(Stage, 
{
	new (&self->stageId) csl::ut::VariableString{ allocator };
	self->cropId = 0;
}
{
	self->stageId.~VariableString();
}
);

RFL_TYPE_INFO(Stage)

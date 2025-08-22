#include "../UIStageIntro.h"
#include "utilities/GameHelpers.h"

using namespace hh::fnd;
using namespace revisited::objects;

CAPTION_ATTRIBUTE(stages)

const RflClassMember spawnerMembers[]{
	{ "stages", &Stage::rflClass, nullptr, RflClassMember::Type::ARRAY, RflClassMember::Type::STRUCT, 0, 0, offsetof(StageIntro, stages), &stagesAttributes }
};

RFL_CLASS(StageIntro);

RFL_TYPE_INFO_FUNCS_CLEAN(StageIntro, 
{
	new (&self->stages) csl::ut::MoveArray<Stage>{ allocator };
}
{
	self->stages.~MoveArray();
}
);

RFL_TYPE_INFO(StageIntro)

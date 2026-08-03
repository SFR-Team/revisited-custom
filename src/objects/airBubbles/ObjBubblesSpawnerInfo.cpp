#include "ObjBubblesSpawner.h"

using namespace csl::fnd;

using namespace hh::anim;
using namespace hh::fnd;
using namespace hh::gfx;
using namespace hh::game;

using namespace revisited::objects;

OBJINFO_CLASS(ObjBubblesSpawnerInfo, ObjBubblesSpawner)

ObjInfo* ObjBubblesSpawnerInfo::Create(IAllocator* allocator) {
	return new (allocator) ObjBubblesSpawnerInfo{ allocator };
}

ObjBubblesSpawnerInfo::ObjBubblesSpawnerInfo(IAllocator* allocator) : ObjInfo{ allocator } {}

void ObjBubblesSpawnerInfo::Load()
{

}

void ObjBubblesSpawnerInfo::Initialize(GameManager* gameManager)
{

}

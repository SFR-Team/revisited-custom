#include "ObjItemBox.h"

using namespace csl::fnd;

using namespace hh::anim;
using namespace hh::fnd;
using namespace hh::gfx;
using namespace hh::game;
using namespace hh::snd;

using namespace revisited::objects;

OBJINFO_CLASS(ObjItemBoxInfo)

ObjInfo* ObjItemBoxInfo::Create(IAllocator* allocator) {
	return new (std::align_val_t(16), allocator) ObjItemBoxInfo{ allocator };
}

ObjItemBoxInfo::ObjItemBoxInfo(IAllocator* allocator) : ObjInfo{ allocator }, groundType{ allocator }, airType{ allocator } {}

void ObjItemBoxInfo::TypeSet::LoadSet(const char* type)
{
	static constexpr const char* names[] = {
		"cmn_obj_rocket",
		"cmn_obj_ring10",
		"cmn_obj_ring20",
		"cmn_obj_ring50",
		"cmn_obj_ultra",
		"cmn_obj_magnet",
		"cmn_obj_infiniteboost",
	};

	static constexpr unsigned int nameCount = sizeof(names) / sizeof(const char*);

	auto* resMgr = ResourceManager::GetInstance();

	for (auto x = 0; x < nameCount; x++) {
		auto* itemName = names[x];

		char nameBuffer[0x100];
		snprintf(nameBuffer, sizeof(nameBuffer), "%s%s", itemName, type);
		models.push_back(resMgr->GetResource<ResModel>(nameBuffer));
	}

	char skelName[64];
	snprintf(skelName, sizeof(skelName), "cmn_obj_itembox%s", type);
	skeleton = resMgr->GetResource<ResSkeletonPxd>(skelName);

	char idleName[64];
	snprintf(idleName, sizeof(idleName), "cmn_obj_itembox%s_roll", type);
	idleAnim = resMgr->GetResource<ResAnimationPxd>(idleName);

	char getName[64];
	snprintf(getName, sizeof(getName), "cmn_obj_itembox%s_get", type);
	getAnim = resMgr->GetResource<ResAnimationPxd>(getName);
}

void ObjItemBoxInfo::Load()
{
	resourceLoader->LoadPackfile("stage/RevisitedObject.pac");
	hh::fnd::InplaceTempUri<> uri{ "sound/revisited_sound/se_revisited_object.acb" };
	hh::fnd::ResourceLoader::Locale locale{};
	resourceLoader->LoadResource(uri, ResAtomCueSheet::GetTypeInfo(), 0, 0, locale);

	//levelLoader->LoadLevel("itembox", {});
}

void ObjItemBoxInfo::Initialize(GameManager* gameManager)
{
	groundType.LoadSet("ground");
	airType.LoadSet("float");
}

Reference<ResModel> ObjItemBoxInfo::GetModel(bool isAir, ObjItemBoxSpawner::ItemType item) const {
	unsigned int idx = (int)item;
	return isAir ? airType.models[idx] : groundType.models[idx];
}

Reference<ResSkeleton> ObjItemBoxInfo::GetSkeleton(bool isAir) const {
	return isAir ? airType.skeleton : groundType.skeleton;
}

Reference<ResAnimation> ObjItemBoxInfo::GetIdle(bool isAir) const {
	return isAir ? airType.idleAnim : groundType.idleAnim;
}

Reference<ResAnimation> ObjItemBoxInfo::GetGet(bool isAir) const {
	return isAir ? airType.getAnim : groundType.getAnim;
}

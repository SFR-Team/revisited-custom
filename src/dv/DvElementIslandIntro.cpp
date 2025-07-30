#include "DvElementIslandIntro.h"

using namespace hh::dv;
using namespace hh::game;

using namespace app::dv;

using namespace revisited::dv;

DvElementIslandIntro::DvElementIslandIntro(csl::fnd::IAllocator* allocator) : AppDvElementBase{ allocator, sizeof(DvElementIslandIntro::Description)} {}

AppDvElementBase* DvElementIslandIntro::Create(csl::fnd::IAllocator* allocator) {
	return new (std::align_val_t(16), allocator) DvElementIslandIntro{ allocator };
}

bool DvElementIslandIntro::AddCallback(int currentFrame, csl::math::Transform& transform)
{
	auto* desc = GetData();
	
	if (*uiObject)
		uiObject->Start((unsigned int)desc->islandId, desc->position, desc->scale);

	return true;
}

void DvElementIslandIntro::RemoveCallback()
{
	if (*uiObject)
		uiObject->End();
}

void DvElementIslandIntro::OnDataUpdated()
{
	auto* gameManager = GetGameManager();
	auto* dvIslandIntro = GameObject::Create<objects::DvIslandIntro>(nullptr);
	gameManager->AddGameObject(dvIslandIntro, "DvIslandIntro", false, nullptr, nullptr);
	uiObject = dvIslandIntro;
}

void DvElementIslandIntro::OnDataDeleted()
{
	if (*uiObject)
		uiObject->Kill();
}

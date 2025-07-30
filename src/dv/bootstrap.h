#pragma once
#include "DvElementIslandIntro.h"

using CreateFuncType = app::dv::AppDvElementBase* (*)(csl::fnd::IAllocator*);

static const CreateFuncType RevisitedElementCreateFuncs[]{
	&revisited::dv::DvElementIslandIntro::Create
};

HOOK(void, __fastcall, DvNodeElementSetup, 0x150D2B6B0, hh::dv::DvNodeElement* self, hh::dv::DvNodeElement::DescriptionBase& desc) {
	int elementId = (int)desc.elementId;
	if (elementId >= 2000) {
		self->start = desc.start * 100;
		self->end = desc.end * 100;
		auto* createFunc = RevisitedElementCreateFuncs[elementId - 2000];
		auto* element = createFunc(self->GetAllocator());
		self->element = element;
		element->baseNode = self;
		if (element)
			element->SetData(&(&desc)[1]);
		self->SetInitializedFlag(true);
	}
	else
		originalDvNodeElementSetup(self, desc);
}

namespace revisited::dv {
	void bootstrap() {
		INSTALL_HOOK(DvNodeElementSetup)
	}
}
#pragma once
#include "objects/dv/DvIslandIntro.h"

namespace revisited::dv {
	class DvElementIslandIntro : public app::dv::AppDvElementBase {
	public:
		struct Description {
			enum class IslandID : unsigned int {
				KRONOS,
				ARES,
				CHAOS,
				RHEA,
				OURANOS
			};

			IslandID islandId;
			csl::math::Vector2 position;
			float scale;
		};

		hh::fnd::Handle<objects::DvIslandIntro> uiObject;

		virtual bool AddCallback(int currentFrame, csl::math::Transform& transform) override;
		virtual void RemoveCallback() override;
		virtual void OnDataUpdated() override;
		virtual void OnDataDeleted() override;

		DvElementIslandIntro(csl::fnd::IAllocator* allocator);

		APP_DV_ELEMENT_DECLARATION_BASE(DvElementIslandIntro)
	};
}

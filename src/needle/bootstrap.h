#pragma once
#include "RenderTexturePostEffectPipelineHooks.h"

namespace revisited::needle {
	void bootstrap() {
		INSTALL_HOOK(RenderTexturePostEffectPipelineInit)
	}
}

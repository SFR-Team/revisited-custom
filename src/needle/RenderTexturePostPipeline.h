#pragma once

namespace revisited::needle {
	class RenderTexturePostPipeline : public hh::needle::WorldRenderingPipeline {
	public:
		static const char* GetName() { return "RenderTexturePostPipeline"; }

		RenderTexturePostPipeline(csl::fnd::IAllocator* allocator, hh::needle::PBRModelInstanceRenderer* pbrModelInstanceRenderer, hh::needle::RenderTextureHandle* renderTexture);


		virtual void InitializeJobs() override;
	};
}

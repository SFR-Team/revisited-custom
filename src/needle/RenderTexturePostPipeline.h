#pragma once

namespace revisited::needle {
	class RenderTexturePostPipeline : public hh::needle::WorldRenderingPipeline {
	public:
		hh::needle::RenderTextureHandle* renderTexture;

		static const char* GetName() { return "RenderTexturePostPipeline"; }

		RenderTexturePostPipeline(csl::fnd::IAllocator* allocator, hh::needle::PBRModelInstanceRenderer* pbrModelInstanceRenderer, hh::needle::RenderTextureHandle* renderTexture);

		static void ClearBuffers(hh::needle::PipelineInfo* pipelineInfo, unsigned int viewportId, void* userData);
		static void UnkJob(hh::needle::PipelineInfo* pipelineInfo, unsigned int viewportId, void* userData);

		virtual void InitializeJobs() override;
	};
}

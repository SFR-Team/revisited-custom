#include "RenderTexturePostPipeline.h"

using namespace csl::fnd;

using namespace hh::needle;

using namespace revisited::needle;

RenderTexturePostPipeline::RenderTexturePostPipeline(IAllocator* allocator, PBRModelInstanceRenderer* pbrModelInstanceRenderer, RenderTextureHandle* renderTexture) : WorldRenderingPipeline{ allocator }
{
	name = GetName();
}

void RenderTexturePostPipeline::InitializeJobs()
{

}

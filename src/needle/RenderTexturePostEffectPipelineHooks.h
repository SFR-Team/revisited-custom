#pragma once

using namespace hh::needle;

struct UnkInfo {
	int64_t unk0;
	SurfaceType type;
	uint32_t arraySize;
	uint32_t mipLevels;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
	SurfaceFormat format;
	uint32_t sampleDescIndex;
};

FUNCTION_PTR(void, __fastcall, sub_141014af0, 0x141014AF0, CopyColor*, RenderingDeviceContext*, Texture*, RenderTarget*, UnkInfo&, UnkInfo&);

void ClearBuffers(PipelineInfo* pipelineInfo, unsigned int viewportId, void* userData)
{
	auto* renderTargetManager = pipelineInfo->supportFX->GetRenderManager()->renderTargetManager;
	renderTargetManager->ClearRenderTarget({ (unsigned int)renderTargetManager->GetUnk9() });
	renderTargetManager->ClearRenderTargets(true);
	unsigned int defaultIdx = 0;
	renderTargetManager->ClearDepthStencil({ defaultIdx }, true);
}

void UnkJob(PipelineInfo* pipelineInfo, unsigned int viewportId, void* userData)
{
	auto* pipeline = (RenderTexturePostEffectPipeline*)userData;
	auto* supportFx = pipelineInfo->supportFX;
	auto* ctx = supportFx->GetRenderingContext2();
	auto* renderTarget = (RenderTarget*)*pipeline->renderTextureHandle->GetRenderTarget(0);
	auto* copyColor = supportFx->copyColor1;
	auto* renderTargetManager = supportFx->renderManager->renderTargetManager;
	UnkInfo unkInfo{};
	unkInfo.type = renderTarget->format.type;
	unkInfo.arraySize = renderTarget->format.arraySize;
	unkInfo.mipLevels = renderTarget->format.mipLevels;
	unkInfo.width = renderTarget->format.width;
	unkInfo.height = renderTarget->format.height;
	unkInfo.depth = renderTarget->format.depth;
	unkInfo.format = renderTarget->format.format;
	unkInfo.sampleDescIndex = renderTarget->format.sampleDescIndex;
	unsigned int renderIdx = 0x36;
	sub_141014af0(copyColor, ctx, renderTargetManager->GetRenderTargetView({ renderIdx }), renderTarget, unkInfo, unkInfo);
}

HOOK(void, __fastcall, RenderTexturePostEffectPipelineInit, 0x1410249D0, hh::needle::RenderTexturePostEffectPipeline* self) {
	auto* clearBuffers = new CallbackJob{};
	clearBuffers->enabledBits = -1;
	clearBuffers->callback.function = &ClearBuffers;
	self->AddJob(clearBuffers);

	auto* shadowMap = new ShadowMapRenderJob{ self->allocator, 0, 0 };
	self->AddJob(shadowMap);

	shadowMap->gap68[0] = 0;
	shadowMap->gap68[1] = 3;
	shadowMap->gap68[2] = 3;
	shadowMap->gap68[3] = 3;

	shadowMap->gap68[4] = 3;
	shadowMap->gap68[5] = 3;
	shadowMap->gap68[6] = 3;
	shadowMap->gap68[7] = 3;

	shadowMap->gap68[8] = 3;
	shadowMap->gap68[9] = 3;
	shadowMap->gap68[10] = 3;
	shadowMap->gap68[11] = 3;

	shadowMap->gap68[12] = 3;
	shadowMap->gap68[13] = 0;
	shadowMap->gap68[14] = 0;
	shadowMap->gap68[15] = 0;

	shadowMap->gap68[16] = 0;
	shadowMap->gap68[17] = 1;
	shadowMap->gap68[18] = 4;
	shadowMap->gap68[19] = 4;

	shadowMap->gap68[20] = 4;
	shadowMap->gap68[21] = 4;
	shadowMap->gap68[22] = 4;
	shadowMap->gap68[23] = 4;

	shadowMap->gap68[24] = 4;
	shadowMap->gap68[25] = 4;

	//Sets the render targets and then the viewports width and height
	auto* setViewport = new CallbackJob{};
	setViewport->enabledBits = -1;
	setViewport->viewportId = -1;
	setViewport->callback.function = (RenderJobCallback::Function*)0x141024500;
	setViewport->callback.userData = self;
	self->AddJob(setViewport);

	self->AddJob(new RenderableRenderJob{ RenderTextureHandle::RenderStage::UNK1, 0, "Before3D Renderable" });

	auto* deferredRenderJob = new DefaultDeferredRenderJob{ self->allocator, 0 };
	self->AddJob(deferredRenderJob);

	deferredRenderJob->qword70 = 1;
	deferredRenderJob->dword78 = 1;
	deferredRenderJob->qword80 = (uint64_t)((CNameIDObjectStatic*)0x1440C9150)->uniqueObject;
	deferredRenderJob->qword88 = (uint64_t)((CNameIDObjectStatic*)0x1440C9190)->uniqueObject;
	deferredRenderJob->qword98 = 0x100000000;
	deferredRenderJob->enabledBitsCopy = 0xFFFFFFFF;

	auto* modelRenderJob = new DefaultModelRenderJob{ 0, nullptr };
	self->AddJob(modelRenderJob);

	ModelRenderJobBase::RenderLayer transparentLayer{};
	transparentLayer.name = ((CNameIDObjectStatic*)0x1440C91D0)->uniqueObject;
	transparentLayer.unk1 = 2;
	transparentLayer.unk2a = 2;
	transparentLayer.unk4 = 2;
	transparentLayer.unk5 = 2;
	modelRenderJob->SetRenderLayer(0, transparentLayer);

	modelRenderJob->onBeforeDefaultRender.function = (RenderJobCallback::Function*)0x1410248C0;
	modelRenderJob->onAfterRender.function = (RenderJobCallback::Function*)0x141024880;

	self->AddJob(new RenderableRenderJob{ RenderTextureHandle::RenderStage::UNK4, 0 });
	self->AddJob(new PostEffectPipelineJob{});

	auto* unkJob = new CallbackJob{};
	unkJob->enabledBits = -1;
	unkJob->callback.function = &UnkJob;
	unkJob->callback.userData = self;
	self->AddJob(unkJob);
}

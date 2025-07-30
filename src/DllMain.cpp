#include "Pch.h"
#include "objects/bootstrap.h"
#include "player/bootstrap.h"
#include "dv/bootstrap.h"

HOOK(uint64_t, __fastcall, GameModeBootInit, 0x1401AADF0, app::game::GameMode* self) {
	auto res = originalGameModeBootInit(self);
	revisited::objects::bootstrap();
	revisited::player::bootstrap();
	revisited::dv::bootstrap();
	return res;
}

HOOK(uint64_t, __fastcall, LevelManagerSetup, 0x150407480, hh::game::LevelManager* self, int64_t setupInfo) {
	auto res = originalLevelManagerSetup(self, setupInfo);
	hh::fnd::ResourceLoader* resLoader = hh::fnd::ResourceLoader::Create(self->GetAllocator());
	resLoader->LoadPackfile("hedgehog/level/revisited.levels");
	return res;
}

BOOL WINAPI DllMain(_In_ HINSTANCE hInstance, _In_ DWORD reason, _In_ LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		INSTALL_HOOK(GameModeBootInit);
		INSTALL_HOOK(GameModeCyberStageInit);
		INSTALL_HOOK(LevelManagerSetup);
		break;
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}

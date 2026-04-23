#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#undef CreateService
#undef max
#undef min

#include <detours.h>
#include <d3d11.h>

#include <cstdint>
#include <rangers-sdk.h>

#include <utilities/Helpers.h>
#include <utilities/GameHelpers.h>

#define ARRAY_SIZE(x) sizeof(x) / sizeof(x[0])

#ifndef PCH_H
#define PCH_H
#endif
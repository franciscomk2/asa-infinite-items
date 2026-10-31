#include "API/ARK/Ark.h"

#include <fstream>

#include "Plugin.h"
#include "Utils.h"
#include "Tests.h"
#include "Hooks.h"
#include "Commands.h"

#pragma comment(lib, "AsaApi.lib")

void OnServerReady()
{
    Log::GetLog()->info("Hello, world!");

    LoadConfig();
    RefreshInfiniteList();
    AddRemoveCommands();
}

DECLARE_HOOK(AShooterGameMode_BeginPlay, void, AShooterGameMode*);
void Hook_AShooterGameMode_BeginPlay(AShooterGameMode* _this)
{
    AShooterGameMode_BeginPlay_original(_this);

    OnServerReady();
}

extern "C" __declspec(dllexport) void Plugin_Init()
{
    Log::Get().Init(PROJECT_NAME);

    AsaApi::GetHooks().SetHook("AShooterGameMode.BeginPlay()", Hook_AShooterGameMode_BeginPlay,
        &AShooterGameMode_BeginPlay_original);

    AsaApi::GetHooks().SetHook("UPrimalItem.IncrementItemQuantity(int,bool,bool,bool,bool,bool,bool)",
        Hook_UPrimalItem_IncrementItemQuantity, &UPrimalItem_IncrementItemQuantity_original);
    AsaApi::GetHooks().SetHook("UPrimalItem.AddItemDurability(float,bool)",
        Hook_UPrimalItem_AddItemDurability, &UPrimalItem_AddItemDurability_original);

    if (AsaApi::GetApiUtils().GetStatus() == AsaApi::ServerStatus::Ready)
        OnServerReady();
}

extern "C" __declspec(dllexport) void Plugin_Unload()
{
    AsaApi::GetHooks().DisableHook("AShooterGameMode.BeginPlay()", Hook_AShooterGameMode_BeginPlay);
    AsaApi::GetHooks().DisableHook("UPrimalItem.IncrementItemQuantity(int,bool,bool,bool,bool,bool,bool)",
        Hook_UPrimalItem_IncrementItemQuantity);
    AsaApi::GetHooks().DisableHook("UPrimalItem.AddItemDurability(float,bool)",
        Hook_UPrimalItem_AddItemDurability);

    AddRemoveCommands(false);
}

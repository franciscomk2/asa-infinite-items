#pragma once

#include <set>
#include <algorithm>
#include <string>
#include <vector>

inline std::vector<std::string> g_infiniteItems;
inline std::set<std::string> g_seenNames;

// Lee la lista "InfiniteItems" del config.json
inline void RefreshInfiniteList()
{
    g_infiniteItems.clear();
    if (Plugin::config.contains("InfiniteItems"))
        g_infiniteItems = Plugin::config["InfiniteItems"].get<std::vector<std::string>>();
}

inline bool IsInfinite(AShooterWeapon* weapon)
{
    if (!weapon) return false;

    UPrimalItem* item = weapon->AssociatedPrimalItemField();
    if (!item) return false;

    std::string name = item->DescriptiveNameBaseField().ToString();

    // Apunta en el log cada nombre la primera vez que aparece
    if (g_seenNames.insert(name).second)
        Log::GetLog()->info("InfiniteItems: objeto usado = '{}'", name);

    return std::find(g_infiniteItems.begin(), g_infiniteItems.end(), name)
           != g_infiniteItems.end();
}

DECLARE_HOOK(AShooterWeapon_UseAmmo, void, AShooterWeapon*, int);
void Hook_AShooterWeapon_UseAmmo(AShooterWeapon* _this, int amount)
{
    if (IsInfinite(_this)) return;
    AShooterWeapon_UseAmmo_original(_this, amount);
}

DECLARE_HOOK(AShooterWeapon_ConsumeAmmoItem, void, AShooterWeapon*, int);
void Hook_AShooterWeapon_ConsumeAmmoItem(AShooterWeapon* _this, int qty)
{
    if (IsInfinite(_this)) return;
    AShooterWeapon_ConsumeAmmoItem_original(_this, qty);
}

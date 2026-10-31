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

inline bool InList(const std::string& s)
{
    return !s.empty() &&
           std::find(g_infiniteItems.begin(), g_infiniteItems.end(), s) != g_infiniteItems.end();
}

// Nombre interno de la clase del objeto (ej. PrimalItem_WeaponBola_C)
inline std::string ItemClassName(UPrimalItem* item)
{
    if (!item) return "";
    UClass* c = item->ClassPrivateField();
    if (!c) return "";
    return c->NamePrivateField().ToString().ToString();
}

inline bool IsInfiniteItem(UPrimalItem* item)
{
    if (!item || g_infiniteItems.empty()) return false;
    return InList(ItemClassName(item)) || InList(item->DescriptiveNameBaseField().ToString());
}

// Apunta en el log cada objeto la primera vez que aparece
inline void LogOnce(UPrimalItem* item, const std::string& tipo)
{
    std::string cls = ItemClassName(item);
    if (g_seenNames.insert(tipo + "|" + cls).second)
        Log::GetLog()->info("InfiniteItems [{}]: clase='{}' nombre='{}'",
            tipo, cls, item->DescriptiveNameBaseField().ToString());
}

inline bool IsInfiniteWeapon(AShooterWeapon* weapon)
{
    if (!weapon) return false;
    UPrimalItem* item = weapon->AssociatedPrimalItemField();
    if (!item) return false;
    LogOnce(item, "arma");
    return IsInfiniteItem(item);
}

// ---- Armas ----
DECLARE_HOOK(AShooterWeapon_UseAmmo, void, AShooterWeapon*, int);
void Hook_AShooterWeapon_UseAmmo(AShooterWeapon* _this, int amount)
{
    if (IsInfiniteWeapon(_this)) return;
    AShooterWeapon_UseAmmo_original(_this, amount);
}

DECLARE_HOOK(AShooterWeapon_ConsumeAmmoItem, void, AShooterWeapon*, int);
void Hook_AShooterWeapon_ConsumeAmmoItem(AShooterWeapon* _this, int qty)
{
    if (IsInfiniteWeapon(_this)) return;
    AShooterWeapon_ConsumeAmmoItem_original(_this, qty);
}

// ---- Cantidad (gancho, bola...) ----
DECLARE_HOOK(UPrimalItem_IncrementItemQuantity, int, UPrimalItem*, int, bool, bool, bool, bool, bool, bool);
int Hook_UPrimalItem_IncrementItemQuantity(UPrimalItem* _this, int amount, bool bReplicateToClient,
    bool bDontUpdateWeight, bool bIsFromUseConsumption, bool bIsArkTributeItem,
    bool bIsFromCraftingConsumption, bool bDontBroadcastItemUpdateEvents)
{
    if (_this && amount < 0 && !bIsFromCraftingConsumption)
    {
        LogOnce(_this, bIsFromUseConsumption ? "consumo" : "cantidad");

        if (bIsFromUseConsumption && IsInfiniteItem(_this))
            return _this->ItemQuantityField();
    }

    return UPrimalItem_IncrementItemQuantity_original(_this, amount, bReplicateToClient,
        bDontUpdateWeight, bIsFromUseConsumption, bIsArkTributeItem,
        bIsFromCraftingConsumption, bDontBroadcastItemUpdateEvents);
}

// ---- Durabilidad (paracaidas...) ----
DECLARE_HOOK(UPrimalItem_AddItemDurability, void, UPrimalItem*, float, bool);
void Hook_UPrimalItem_AddItemDurability(UPrimalItem* _this, float durabilityToAdd,
    bool bNotifyClientForEquippedItems)
{
    if (_this && durabilityToAdd < 0.0f)
    {
        LogOnce(_this, "durabilidad");
        if (IsInfiniteItem(_this)) return;
    }

    UPrimalItem_AddItemDurability_original(_this, durabilityToAdd, bNotifyClientForEquippedItems);
}

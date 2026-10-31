
#include <limits>


template <typename T>
T GetText(const std::string& section, const std::string& value, T default_value)
{
    try
    {
        return Plugin::config[section][value].get<T>();
    }
    catch (const nlohmann::json::exception& error)
    {
        // Check if the exception is due to a missing key
        if (error.id == 302)
        {
            //return T{};
            return default_value;
        }

        Log::GetLog()->error("GetText() error: {}", error.what());
        throw;
    }
}

template <typename T>
T GetJson(const std::string& section, const std::string& value)
{
    try
    {
        auto jsonValue = Plugin::config[section][value];

        // Check if the value is null
        if (jsonValue.is_null() || jsonValue.empty()) {
            return T{};
        }

        return jsonValue.get<T>();
    }
    catch (const nlohmann::json::exception& error)
    {
        Log::GetLog()->error("GetJson error: {}", error.what());
        throw;
    }
}

FString GetPriorPerm(AShooterPlayerController* player_controller)
{
    const FString& eos_id = AsaApi::IApiUtils::GetEOSIDFromController(player_controller);

    auto player_groups = Permissions::GetPlayerGroups(eos_id);

    const nlohmann::json permGroups = Plugin::config["PermissionGroups"];

    std::string defaultGroup = "Default";
    int minPriority = INT_MAX;
    nlohmann::json result;
    FString selectedPerm = "Default";

    for (const FString& param : player_groups)
    {
        if (permGroups.contains(param.ToString()))
        {
            int priority = static_cast<int>(permGroups[param.ToString()]["Priority"]);
            if (priority < minPriority)
            {
                minPriority = priority;
                result = permGroups[param.ToString()];
                selectedPerm = param;
            }
        }
    }

    // If no matching parameter is found, return the Default group
    if (result.is_null() && permGroups.contains(defaultGroup))
    {
        result = permGroups[defaultGroup];
    }

    return selectedPerm;
    //return result.firs;
}

nlohmann::json GetCommandSetting(const std::string permission, const std::string command)
{
    if (permission.empty()) return {};
    if (command.empty()) return {};


    auto configObj = Plugin::config["PermissionGroups"];
    auto permObj = configObj[permission];
    auto commandObj = permObj["Commands"];
    auto settingObj = commandObj[command];
    return settingObj;
}

bool CheckPoints(AShooterPlayerController* player_controller, int cost)
{
    if (cost == -1) return false;

    if (cost == 0) return true;

    const FString& eos_id = AsaApi::IApiUtils::GetEOSIDFromController(player_controller);
    const int points = ArkShop::Points::GetPoints(eos_id);

    if (points < cost) return true;

    return false;
}

bool SpendPoints(AShooterPlayerController* player_controller, int cost)
{
    if (cost == -1) return false;

    if (cost == 0) return true;

    const FString& eos_id = AsaApi::IApiUtils::GetEOSIDFromController(player_controller);

    const int points = ArkShop::Points::GetPoints(eos_id);

    if (points >= cost && ArkShop::Points::SpendPoints(cost, eos_id))
    {
        return true;
    }
    else
    {
        return false;
    }
}





void ReadConfig()
{
    const std::string config_path = AsaApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/" + PROJECT_NAME + "/config.json";
    std::ifstream file{ config_path };
    if (!file.is_open())
    {
        throw std::runtime_error("Can't open config.json");
    }

    file >> Plugin::config;

    Log::GetLog()->info("{} config file loaded", PROJECT_NAME);
    file.close();

    Plugin::isDebug = (GetText<bool>("General", "Debug", false) == true) ? true : false;
}

void LoadConfig()
{
    try
    {
        ReadConfig();
    }
    catch (const std::exception& error)
    {
        Log::GetLog()->error("LoadConfig error: {}", error.what());
        throw;
    }
}

void Reload()
{
    ReadConfig();
}

void ReloadConfig(APlayerController* player_controller, FString* /*unused*/, bool /*unused*/)
{
    auto* shooter_controller = static_cast<AShooterPlayerController*>(player_controller);

    try
    {
        Reload();
    }
    catch (const std::exception& error)
    {
        //Log::GetLog()->error(error.what());
        AsaApi::GetApiUtils().SendServerMessage(shooter_controller, FColorList::Red, "Failed to reload config Error: {}", error.what());
        return;
    }

    AsaApi::GetApiUtils().SendServerMessage(shooter_controller, FColorList::Green, "Reloaded config");
}

void ReloadConfigRcon(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld* /*unused*/)
{
    FString reply;

    try
    {
        Reload();
    }
    catch (const std::exception& error)
    {
        //Log::GetLog()->error(error.what());
        reply = error.what();
        rcon_connection->SendMessageW(rcon_packet->Id, 0, &reply);
        return;
    }

    reply = "Reloaded config";
    rcon_connection->SendMessageW(rcon_packet->Id, 0, &reply);
}
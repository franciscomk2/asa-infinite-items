



void AddRemoveCommands(bool addCmd = true)
{
    // COMMANDS
    const FString Test = GetText<std::string>("Commands", "TestCMD", "").c_str();
    if (!Test.IsEmpty())
    {
        if (addCmd)
        {
            AsaApi::GetCommands().AddChatCommand(Test, &TestCallback);
        }
        else
        {
            AsaApi::GetCommands().RemoveChatCommand(Test);
        }
    }

    // RELOADS
    FString reload(FString(PROJECT_NAME) + ".Reload");
    if (!reload.IsEmpty())
    {
        if (addCmd)
        {
            AsaApi::GetCommands().AddConsoleCommand(reload, &ReloadConfig);
            AsaApi::GetCommands().AddRconCommand(reload, &ReloadConfigRcon);
        }
        else
        {
            AsaApi::GetCommands().RemoveConsoleCommand(reload);
            AsaApi::GetCommands().RemoveRconCommand(reload);
        }
    }

    // HOOKS
    /*
    if (addCmd)
    {
        AsaApi::GetHooks().SetHook("AShooterCharacter.SampleHook()", &Hook_AShooterCharacter_SampleHook,
            &AShooterCharacter_SampleHook_original);
    }
    else
    {
        AsaApi::GetHooks().DisableHook("AShooterCharacter.SampleHook()", &Hook_AShooterCharacter_SampleHook);
    }
    */

    // TIMERS
    /*
    if (addCmd)
    {
        AsaApi::GetCommands().AddOnTimerCallback("TimerTick", &TimerCallback);
    }
    else
    {
        AsaApi::GetCommands().RemoveOnTimerCallback("TimerTick");
    }
    */
    

}
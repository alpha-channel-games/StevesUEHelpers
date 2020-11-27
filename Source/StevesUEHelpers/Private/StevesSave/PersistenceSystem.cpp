#include "StevesSave/PersistenceSystem.h"

#include "Engine/LevelStreaming.h"

DEFINE_LOG_CATEGORY(LogPersistenceSystem)

void UPersistenceSystem::Initialise()
{
    OnPreloadMapHandle = FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UPersistenceSystem::OnPreLoadMap);
    OnPostLoadMapHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UPersistenceSystem::OnPostLoadMap);

    if (IsValid(GetWorld()))
    {
        // In PIE, this will be the startup level. In standalone, it will be just a placeholder level e.g. /Temp/Untitled_0.Untitled:PersistentLevel
        auto Lvl = GetWorld()->GetCurrentLevel();
        UE_LOG(LogPersistenceSystem, Warning, TEXT("Startup Persistent Level: %s"), *GetWorld()->GetMapName());

        if (!Lvl->GetPathName().StartsWith("/Temp/Untitled"))
            SubscribeToStreamingEvents();

    }
}

void UPersistenceSystem::SubscribeToStreamingEvents()
{
    if (!IsValid(GetWorld()) )
        return;
    
    for (auto Stream : GetWorld()->GetStreamingLevels())
    {
        Stream->OnLevelLoaded.AddDynamic(this, &UPersistenceSystem::OnStreamLevelLoad);
        Stream->OnLevelUnloaded.AddDynamic(this, &UPersistenceSystem::OnStreamLevelUnload);
        Stream->OnLevelShown.AddDynamic(this, &UPersistenceSystem::OnStreamLevelShown);
        Stream->OnLevelHidden.AddDynamic(this, &UPersistenceSystem::OnStreamLevelHidden);
    }
            

}


void UPersistenceSystem::UnsubscribeFromStreamingEvents()
{
    if (!IsValid(GetWorld()))
        return;

    for (auto Stream : GetWorld()->GetStreamingLevels())
    {
        Stream->OnLevelLoaded.RemoveDynamic(this, &UPersistenceSystem::OnStreamLevelLoad);
        Stream->OnLevelUnloaded.RemoveDynamic(this, &UPersistenceSystem::OnStreamLevelUnload);
        Stream->OnLevelShown.RemoveDynamic(this, &UPersistenceSystem::OnStreamLevelShown);
        Stream->OnLevelHidden.RemoveDynamic(this, &UPersistenceSystem::OnStreamLevelHidden);
    }
}

void UPersistenceSystem::Deinitialise()
{
    if (OnPreloadMapHandle.IsValid())
        FCoreUObjectDelegates::PreLoadMap.Remove(OnPreloadMapHandle);
    if (OnPostLoadMapHandle.IsValid())
        FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(OnPostLoadMapHandle);
}

void UPersistenceSystem::OnPreLoadMap(const FString& Name)
{
    // Here we're given the persistent map that's about to be loaded, although current map is still in memory
    UE_LOG(LogPersistenceSystem, Warning, TEXT("OnPreLoadMap: New %s Old %s"), *Name, *GetWorld()->GetMapName());

    // Because PostLoad occurs AFTER BeginPlay, on OnStreamLevelLoad happens BEFORE BeginPlay, we have to
    // do loading

    UnsubscribeFromStreamingEvents();
}

void UPersistenceSystem::OnPostLoadMap(UWorld* World)
{
    // Here the new persistent map has been loaded
    // Sadly this is AFTER BeginPlay has occurred on actors in a persistent level
    UE_LOG(LogPersistenceSystem, Warning, TEXT("OnPostLoadMap: %s"), *World->GetMapName());
    SubscribeToStreamingEvents();
}

void UPersistenceSystem::OnStreamLevelLoad()
{
    // Unfortunately we don't know *which* streaming level has loaded, so we just need to check
    // with the list of streaming levels in this map to see what our previously known state was
    // This event happens *before* the BeginPlay on actors & level blueprint in this streaming level so we want to
    // load in our data ready for them to read
    
    UE_LOG(LogPersistenceSystem, Warning, TEXT("Some streaming level has LOADED"))
}

void UPersistenceSystem::OnStreamLevelShown()
{
    UE_LOG(LogPersistenceSystem, Warning, TEXT("Some streaming level has been SHOWN"))
    
}
void UPersistenceSystem::OnStreamLevelHidden()
{
    UE_LOG(LogPersistenceSystem, Warning, TEXT("Some streaming level has been HIDDEN"))
    
}
void UPersistenceSystem::OnStreamLevelUnload()
{
    // Unfortunately we don't know *which* streaming level has loaded, so we just need to check
    // with the list of streaming levels in this map to see what our previously known state was

    // This event happens *after* the EndPlay on all unloaded stream objects so we can be sure that
    // they will have pushed their state to us already, and we can persist this to the active state files
    // and unload it from memory.

    UE_LOG(LogPersistenceSystem, Warning, TEXT("Some streaming level has UNLOADED"))
}

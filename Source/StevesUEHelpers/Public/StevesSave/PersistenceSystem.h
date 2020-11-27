#pragma once

#include "CoreMinimal.h"

#include "PersistenceSystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPersistenceSystem, Verbose, Verbose);

/**
 * @brief The Persistence system is designed to support persistence of game data across map loads, so that
 * your actors can be unloaded and reloaded, whether from an actual game load action or simply changing maps
 * or streaming in levels, and automatically have their state retained.
 *
 * This has an overlap with the save system of course, but first there is the persistence of the active game.
 * Maps can be unloaded, streaming levels within them can be unloaded in parts, and the persistent state used by the actors
 * concerned will be saved out to disk as part of the active game. Each map / streaming level will have its own file
 * in the active persistent game for ease of reloading later when those parts are loaded back in. When saved though,
 * all the parts will be combined into a single file and compressed. Loading will involve decompressing the save file
 * into the active game data folder, then loading the global data, including the current map. Everything else will
 * then be loaded in by requesting the data from this system as they Begin Play.
 *
 * NOTE: would be useful to make the load async for streaming purposes, which means the load may not be complete at
 * the end of Begin Play. We probably need a post load event to cope with this.
 */
UCLASS()
class UPersistenceSystem : public UObject
{
    GENERATED_BODY()
protected:
    UGameInstance* GameInstance;
    FDelegateHandle OnPreloadMapHandle;
    FDelegateHandle OnPostLoadMapHandle;

    UFUNCTION()
    void OnPreLoadMap(const FString& Name);
    UFUNCTION()
    void OnPostLoadMap(UWorld* World);
    UFUNCTION()
    void OnStreamLevelLoad();
    UFUNCTION()
    void OnStreamLevelShown();
    UFUNCTION()
    void OnStreamLevelHidden();
    UFUNCTION()
    void OnStreamLevelUnload();

    void SubscribeToStreamingEvents();
    void UnsubscribeFromStreamingEvents();

public:
    void Initialise(UGameInstance* GI);
    void Deinitialise();

};
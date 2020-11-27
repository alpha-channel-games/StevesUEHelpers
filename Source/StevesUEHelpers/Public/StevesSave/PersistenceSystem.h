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
 * We don't store all the data for the currently active game in memory at once, only what is needed globally, or
 * for the currently loaded levels.
 *
 * Thus there is the concept of an active game store, which contains easily loaded / unloaded data for the current
 * game state, split into multiple files for convenience for quicker piecemeal updates. Saving a game involves flushing
 * anything in memory to  this active state, then archiving it as a single save game. Since not all the game state is in
 * memory, the disk version of the active state is the full save game.
 *
 * Loading a game means deleting the current active state on disk, extracting the save file into the active state, then loading the
 * minimum global data required to determine which map to load. Once the map is loaded, this will trigger the loading
 * of any other data required from active state.
 * 
 * Using this system, maps and streaming level parts can be unloaded and loaded, and in memory we only keep what's needed
 * for the currently loaded subset; everything else is on disk in the active state. This is therefore quite scalable.
 *
 * All persistent data is kept within a "page" of data, which maps on to a file in the active game, but which are
 * combined into a single file in a save game. Each page has a unique identifier. Pages will contain:
 *
 *  1. Global page(s)
 *       Used to store any data your game needs all the time. There will always be one of these storing the absolute
 *       minimum data for the system to operate, including what map is current. You can add additional global pages,
 *       ensuring that each has a unique identifier.
 *       Global pages are never vacated from memory unless you request it. They are loaded into memory on first access
 *       by the identifier.
 *  2. Level pages.
 *       Used to store data about actors contained in a given level, which can be a persistent map or a streaming level.
 *       Identifiers are generated from the level's unique path name.
 *       Level pages are vacated from memory when the corresponding level unloads. They are loaded on demand as the
 *       level loads; synchronously by default. If you find this becoming a source of hitches for streaming levels, you
 *       can request an asynchronous load for a streaming level at the same time you request the streaming level to be
 *       loaded. 
 */
UCLASS()
class UPersistenceSystem : public UObject
{
    GENERATED_BODY()
protected:
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
    void Initialise();
    void Deinitialise();

};
#include "StevesSave/PersistentActorComponent.h"

#include "StevesSave/PersistenceSystem.h"

void UPersistentActorComponent::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogPersistenceSystem, Warning, TEXT("PersistentActorComponent: Begin Play"))
    
    // Here we want to make a blocking call to get persistent data for this actor
    // PostLoad should have requested this in a separate thread already so hopefully should be in progress
}

void UPersistentActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    UE_LOG(LogPersistenceSystem, Warning, TEXT("PersistentActorComponent: End Play"))
}


bool UPersistentActorComponent::IsPostLoadThreadSafe() const
{
    // We want to trigger the load of persistent game data in the loading thread for better
    // behaviour in streaming levels
    return true;
}

void UPersistentActorComponent::PostLoad()
{
    Super::PostLoad();

    UE_LOG(LogPersistenceSystem, Warning, TEXT("PersistentActorComponent: Post Load"))

    // This will happen in the background thread!
    // We want to request the async load of data for this actor's level so that in BeginPlay it's already
    // ready to use (that call will be blocking)
}

#include "StevesSave/PersistentActorComponent.h"
#include "StevesSave/PersistenceSystem.h"

void UPersistentActorComponent::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogPersistenceSystem, Warning, TEXT("PersistentActorComponent: Begin Play"))
}

void UPersistentActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    UE_LOG(LogPersistenceSystem, Warning, TEXT("PersistentActorComponent: End Play"))
}

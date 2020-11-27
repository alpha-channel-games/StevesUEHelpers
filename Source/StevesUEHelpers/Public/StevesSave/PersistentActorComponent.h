#pragma once

#include "CoreMinimal.h"
#include "PersistentActorComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(StevesHelpers), meta=(BlueprintSpawnableComponent))
class STEVESUEHELPERS_API UPersistentActorComponent : public UActorComponent
{
    GENERATED_BODY()

public:


    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

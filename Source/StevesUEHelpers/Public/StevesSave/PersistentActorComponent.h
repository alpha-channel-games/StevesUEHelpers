#pragma once

#include "CoreMinimal.h"
#include "PersistentActorComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(StevesHelpers), meta=(BlueprintSpawnableComponent))
class STEVESUEHELPERS_API UPersistentActorComponent : public UActorComponent
{
    GENERATED_BODY()

public:


    virtual bool IsPostLoadThreadSafe() const override;
    virtual void PostLoad() override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

// item.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTestNyeom, Warning, All);

UCLASS()
class TEST_1_API AItem : public AActor
{
    GENERATED_BODY()

public:
    AItem();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RotationSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MaxRange;

    FVector StartLocation;
    float MoveDirection;
};
#include "Item.h"

AItem::AItem()
{
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(SceneRoot);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Resources/Props/SM_Couch.SM_Couch"));
    if (MeshAsset.Succeeded()) {
        StaticMesh->SetStaticMesh(MeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Resources/Materials/M_Gem_D.M_Gem_D"));
    if (MaterialAsset.Succeeded()) {
        StaticMesh->SetMaterial(0, MaterialAsset.Object);
    }

    PrimaryActorTick.bCanEverTick = true;
    RotationSpeed = 90.0f;
    MoveSpeed = 200.0f;
    MaxRange = 500.0f;
    MoveDirection = 1.0f;
}

void AItem::BeginPlay()
{
    Super::BeginPlay();
    StartLocation = GetActorLocation();
}

void AItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!FMath::IsNearlyZero(RotationSpeed)) {
        AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
    }

    FVector CurrentLocation = GetActorLocation();
    float DistanceMoved = (CurrentLocation - StartLocation).Size();

    if (DistanceMoved >= MaxRange)
    {
        MoveDirection *= -1.0f; // 방향 반전
    }

    FVector NewLocation = CurrentLocation + FVector(MoveSpeed * MoveDirection * DeltaTime, 0.0f, 0.0f);
    SetActorLocation(NewLocation);
}

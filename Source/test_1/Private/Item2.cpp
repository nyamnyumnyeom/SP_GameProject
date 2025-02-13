#include "Item2.h"

// Sets default values
AItem2::AItem2()
{
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(SceneRoot);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Resources/Props/SM_Star_B.SM_Star_B"));
    if (MeshAsset.Succeeded()) {
        StaticMesh->SetStaticMesh(MeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Resources/Materials/M_Gem_D.M_Gem_D"));
    if (MaterialAsset.Succeeded()) {
        StaticMesh->SetMaterial(0, MaterialAsset.Object);
    }

    PrimaryActorTick.bCanEverTick = true;
    RotationSpeed = 90.0f;
}

void AItem2::BeginPlay()
{
    Super::BeginPlay();

    //SetActorLocation(FVector(0.0f, 500.0f, 0.0f));
    // SetActorRotation(FRotator(0.0f , 90.0f , 0.0f)); // pitch , yaw , roll
    SetActorScale3D(FVector(0.7f));
}

void AItem2::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!FMath::IsNearlyZero(RotationSpeed)) {
        AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
    }
}

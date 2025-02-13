#include "MineItem.h"
#include "SP_GameBase.h"
#include "SP_GameInstance.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/Paths.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{
	ExplosionDelay = 0.5f;
	ExplosionRadius = 350.0f;
    ExplosionDamage = 15.0f;
	ItemType = "Mine";
	bHasExploded = false;

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);
}


void AMineItem::ActivateItem(AActor* Activator)
{
	if (bHasExploded) return;
	//  레벨이 증가할 때마다 데미지 5씩 증가
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USP_GameInstance* SP_GameInstance = Cast<USP_GameInstance>(GameInstance);
		if (SP_GameInstance)
		{
			ExplosionDamage = 15.0f + (SP_GameInstance->CurrentLevelIndex * 5.0f);
		}
	}
    GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AMineItem::Explode, ExplosionDelay,false);
	bHasExploded = true;
}

void AMineItem::Explode()
{
	UParticleSystemComponent* Particle = nullptr;

	if (ExplosionParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionParticle,
			GetActorLocation(),
			GetActorRotation(),
			false
		);
	}
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ExplosionSound,
			GetActorLocation()
		);
	}

    TArray<AActor*> OverlappingActors;
    ExplosionCollision->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor,                      // 데미지를 받을 액터
				ExplosionDamage,            // 데미지 양
				nullptr,                    // 데미지를 유발한 주체 (지뢰를 설치한 캐릭터가 없으므로 nullptr)
				this,                       // 데미지를 유발한 오브젝트(지뢰)
				UDamageType::StaticClass()  // 기본 데미지 유형
			);
		}
    }
    DestroyItem();
	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;
		TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[WeakParticle]()
			{
				if (WeakParticle.IsValid())
				{
					WeakParticle->DestroyComponent();
				}
			},
			2.0f,
			false
		);
	}
}
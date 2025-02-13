#include "HealingItem.h"
#include "SP_Character.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASP_Character* PlayerCharacter = Cast<ASP_Character>(Activator))
		{
			// 캐릭터의 체력을 회복
			PlayerCharacter->AddHealth(HealAmount);
		}
		DestroyItem();
	}
}

void AHealingItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Time += DeltaTime;
	float ScaleFactor = 1.0f + 0.2f * FMath::Sin(Time * 2.0f); // 크기 변화 계산

	SetActorScale3D(FVector(ScaleFactor));
}
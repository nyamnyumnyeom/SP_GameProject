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
			// ĳ������ ü���� ȸ��
			PlayerCharacter->AddHealth(HealAmount);
		}
		DestroyItem();
	}
}

void AHealingItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Time += DeltaTime;
	float ScaleFactor = 1.0f + 0.2f * FMath::Sin(Time * 2.0f); // ũ�� ��ȭ ���

	SetActorScale3D(FVector(ScaleFactor));
}
#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "HealingItem.generated.h"

UCLASS()
class TEST_1_API AHealingItem : public ABaseItem
{
	GENERATED_BODY()
	
	void Tick(float DeltaTime) override;

public:
	AHealingItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 HealAmount;

	virtual void ActivateItem(AActor* Activator) override;

private:
	float Time;
};
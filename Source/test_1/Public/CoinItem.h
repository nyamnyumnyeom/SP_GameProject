// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "CoinItem.generated.h"

/**
 * 
 */
UCLASS()
class TEST_1_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()

	void Tick(float DeltaTime) override;

public:
	ACoinItem();
	// ���� ȹ�� �� ���� ���� (�ڽ� Ŭ�������� ��ӹ޾� ���� �ٲ��� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;

	virtual void ActivateItem(AActor* Activator) override;
};
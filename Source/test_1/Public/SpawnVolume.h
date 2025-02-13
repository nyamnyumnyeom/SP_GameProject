// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"       // 우리가 정의한 구조체
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class TEST_1_API ASpawnVolume : public AActor
{
    GENERATED_BODY()

public:
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ItemDataTable;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomItem(); // 리턴 형식을 AActor* 로 변경

	FItemSpawnRow* GetRandomItem() const;
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
	FVector GetRandomPointInVolume() const;
};
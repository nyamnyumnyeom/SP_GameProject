// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item2.generated.h"


UCLASS()
class TEST_1_API AItem2 : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem2();

protected:
	USceneComponent* SceneRoot;
	UStaticMeshComponent* StaticMesh;

	float RotationSpeed;

	// 생성자																 //	- 메모리에 생김. 딱 한번만 호출.
	//virtual void PostInitializeComponents() override;						 //		- 컴포넌트가 완성된 직후 호출. 컴포넌트끼리 데이터 주고 받기, 상호작용
	virtual void BeginPlay() override;										 //		- 배치 (Spawn) 직후
	virtual void Tick(float DeltaTime) override;							 //		- 매 프레임마다 호출
	//virtual void Destroyed() override;										 //		- 삭제 되기 직전에 호출
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; //		- 게임 종료, 파괴(Destroy()), 레벨 전환

};

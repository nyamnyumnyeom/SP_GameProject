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

	// ������																 //	- �޸𸮿� ����. �� �ѹ��� ȣ��.
	//virtual void PostInitializeComponents() override;						 //		- ������Ʈ�� �ϼ��� ���� ȣ��. ������Ʈ���� ������ �ְ� �ޱ�, ��ȣ�ۿ�
	virtual void BeginPlay() override;										 //		- ��ġ (Spawn) ����
	virtual void Tick(float DeltaTime) override;							 //		- �� �����Ӹ��� ȣ��
	//virtual void Destroyed() override;										 //		- ���� �Ǳ� ������ ȣ��
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; //		- ���� ����, �ı�(Destroy()), ���� ��ȯ

};

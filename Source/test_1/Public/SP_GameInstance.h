#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SP_GameInstance.generated.h"

UCLASS()
class TEST_1_API USP_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USP_GameInstance();

	// ���� ��ü ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	// ���� ���� �ε��� (GameState������ ������ �� ������, �� ��ȯ �Ŀ��� �츮�� �ʹٸ� GameInstance�� ������ �� ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);

	float SavedLevelDuration = 60.0f;
};
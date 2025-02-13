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

	// 게임 전체 누적 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	// 현재 레벨 인덱스 (GameState에서도 관리할 수 있지만, 맵 전환 후에도 살리고 싶다면 GameInstance에 복제할 수 있음)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);

	float SavedLevelDuration = 60.0f;
};
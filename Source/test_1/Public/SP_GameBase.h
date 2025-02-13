#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SP_GameBase.generated.h"

UCLASS()
class TEST_1_API ASP_GameBase : public AGameState
{
GENERATED_BODY()

public:
    ASP_GameBase();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	// ���� �������� ������ ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	// �÷��̾ ������ ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;
	// �� ������ �����Ǵ� �ð� (�� ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;
	// ���� ���� ���� ���� �ε���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	// ��ü ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;
	// ���� ���� �� �̸� �迭. ���� �ִ� �ε����� ���ʴ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	// �� ������ ������ ������ �ð��� �帣���� �����ϴ� Ÿ�̸�
	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	// ������ ������ ������ �� (��� ���� ����) ����Ǵ� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	// ������ ������ ��, ������ ���� �� Ÿ�̸� ����
	void StartLevel();
	// ���� ���� �ð��� ����Ǿ��� �� ȣ��
	void OnLevelTimeUp();
	// ������ �ֿ��� �� ȣ��
	void OnCoinCollected();
	// ������ ���� �����ϰ� ���� ������ �̵�
	void EndLevel();

	void UpdateHUD();
};

#include "SP_GameBase.h"
#include "SP_GameInstance.h"
#include "SP_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "SP_Character.h"

ASP_GameBase::ASP_GameBase()
{
    Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 60.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void ASP_GameBase::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASP_GameBase::UpdateHUD,
		0.1f,
		true
	);
}

int32 ASP_GameBase::GetScore() const
{
	return Score;
}

void ASP_GameBase::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance()) 
	{
		USP_GameInstance* SP_GameInstance = Cast<USP_GameInstance>(GameInstance);
		if(SP_GameInstance)
		{
			SP_GameInstance->AddToScore(Amount);
		}
	}
}

void ASP_GameBase::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASP_PlayerController* SP_PlayerController = Cast<ASP_PlayerController>(PlayerController))
		{
			SP_PlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USP_GameInstance* SP_GameInstance = Cast<USP_GameInstance>(GameInstance);
		if (SP_GameInstance)
		{
			CurrentLevelIndex = SP_GameInstance->CurrentLevelIndex;

			// GameInstance에서 유지된 LevelDuration을 불러오기
			if (SP_GameInstance->SavedLevelDuration > 0.0f)
			{
				LevelDuration = SP_GameInstance->SavedLevelDuration;
			}
		}
	}


	// 레벨 시작 시, 코인 개수 초기화
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	// 현재 맵에 배치된 모든 SpawnVolume을 찾아 아이템 스폰
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				// 만약 스폰된 액터가 코인 타입이라면 SpawnedCoinCount 증가
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	// 정해진 시간 후에 OnLevelTimeUp()가 호출되도록 타이머 설정
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ASP_GameBase::OnLevelTimeUp,
		LevelDuration,
		false
	);

	UpdateHUD();

	UE_LOG(LogTemp, Warning, TEXT("Level %d Start!, Spawned %d coin"),
		CurrentLevelIndex + 1,
		SpawnedCoinCount);
}

void ASP_GameBase::OnLevelTimeUp()
{
	EndLevel();
}

void ASP_GameBase::OnCoinCollected()
{
	CollectedCoinCount++;

	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
		CollectedCoinCount,
		SpawnedCoinCount)

		// 현재 레벨에서 스폰된 코인을 전부 주웠다면 즉시 레벨 종료
		if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
		{
			EndLevel();
		}
}

void ASP_GameBase::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USP_GameInstance* SP_GameInstance = Cast<USP_GameInstance>(GameInstance);
		if (SP_GameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;

			// 레벨 시간이 20초 감소 (최소 10초는 유지)
			LevelDuration = FMath::Max(LevelDuration - 20.0f, 10.0f);

			// GameInstance에 저장해서 유지
			SP_GameInstance->CurrentLevelIndex = CurrentLevelIndex;
			SP_GameInstance->SavedLevelDuration = LevelDuration;
		}
	}

	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void ASP_GameBase::OnGameOver()
{
	UpdateHUD();
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASP_PlayerController* SP_PlayerController = Cast<ASP_PlayerController>(PlayerController))
		{
			SP_PlayerController->SetPause(true);
			SP_PlayerController->ShowMainMenu(true);
		}
	}
}

void ASP_GameBase::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ASP_PlayerController* SpartaPlayerController = Cast<ASP_PlayerController>(PlayerController);
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						USP_GameInstance* SP_GameInstance = Cast<USP_GameInstance>(GameInstance);
						if (SP_GameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SP_GameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
					{
						FString LevelName = LevelMapNames[CurrentLevelIndex].ToString();
						LevelName = LevelName.Replace(TEXT("Level"), TEXT(""));
						LevelName = LevelName.TrimStartAndEnd();
						LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level : %s"), *LevelName)));
					}
				}
			}
		}
	}
}
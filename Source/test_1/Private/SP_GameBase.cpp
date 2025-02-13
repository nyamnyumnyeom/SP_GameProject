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

			// GameInstance���� ������ LevelDuration�� �ҷ�����
			if (SP_GameInstance->SavedLevelDuration > 0.0f)
			{
				LevelDuration = SP_GameInstance->SavedLevelDuration;
			}
		}
	}


	// ���� ���� ��, ���� ���� �ʱ�ȭ
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	// ���� �ʿ� ��ġ�� ��� SpawnVolume�� ã�� ������ ����
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
				// ���� ������ ���Ͱ� ���� Ÿ���̶�� SpawnedCoinCount ����
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	// ������ �ð� �Ŀ� OnLevelTimeUp()�� ȣ��ǵ��� Ÿ�̸� ����
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

		// ���� �������� ������ ������ ���� �ֿ��ٸ� ��� ���� ����
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

			// ���� �ð��� 20�� ���� (�ּ� 10�ʴ� ����)
			LevelDuration = FMath::Max(LevelDuration - 20.0f, 10.0f);

			// GameInstance�� �����ؼ� ����
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
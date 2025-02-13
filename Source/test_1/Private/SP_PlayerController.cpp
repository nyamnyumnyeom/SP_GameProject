#include "SP_PlayerController.h"
#include "SP_GameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "SP_GameBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

ASP_PlayerController::ASP_PlayerController()
    :InputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    LookAction(nullptr),
    SprintAction(nullptr),
    HUDWidgetClass(nullptr),
    HUDWidgetInstance(nullptr),
    MainMenuWidgetClass(nullptr),
    MainMenuWidgetInstance(nullptr)
{
}

void ASP_PlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }
    if (!HUDWidgetInstance && HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();
        }
    }
    ASP_GameBase* SP_GameBase = GetWorld() ? GetWorld()->GetGameState<ASP_GameBase>() : nullptr;
    if (SP_GameBase)
    {
        SP_GameBase->UpdateHUD();
    }
    FString CurrentMapName = GetWorld()->GetMapName();
    if (CurrentMapName.Contains("MenuLevel"))
    {
        if (!MainMenuWidgetInstance)
        {
            ShowMainMenu(false);
        }
    }
}

UUserWidget* ASP_PlayerController::GetHUDWidget() const
{
    return HUDWidgetInstance;
}

void ASP_PlayerController::UpdateHUD()
{
    if (HUDWidgetInstance)
    {
        UE_LOG(LogTemp, Log, TEXT("HUD Updated"));
    }
}

void ASP_PlayerController::ShowMainMenu(bool bIsRestart)
{
	// HUD가 켜져 있다면 닫기
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	// 이미 메뉴가 떠 있으면 제거
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	// 메뉴 UI 생성
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString(TEXT("Restart")));
			}
			else
			{
				ButtonText->SetText(FText::FromString(TEXT("Start")));
			}
		}
        if (bIsRestart)
        {
            UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
            if (PlayAnimFunc)
            {
                MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
            }

            if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
            {
                if (USP_GameInstance* SP_GameInstance = Cast<USP_GameInstance>(UGameplayStatics::GetGameInstance(this)))
                {
                    TotalScoreText->SetText(FText::FromString(
                        FString::Printf(TEXT("Total Score : %d"), SP_GameInstance->TotalScore)
                    ));
                }
            }
        }
	}
}

void ASP_PlayerController::ShowGameHUD()
{
    // 기존 HUD 위젯이 있으면 제거
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }

    // 기존 메인 메뉴 위젯이 있으면 제거
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    // 새 HUD 위젯 생성
    if (HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();

            bShowMouseCursor = false;
            SetInputMode(FInputModeGameOnly());

            // HUD 업데이트
            ASP_GameBase* SP_GameBase = GetWorld() ? GetWorld()->GetGameState<ASP_GameBase>() : nullptr;
            if (SP_GameBase)
            {
                SP_GameBase->UpdateHUD();
            }
        }
    }
}

void ASP_PlayerController::StartGame()
{
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

	if (USP_GameInstance* SP_GameInstance = Cast<USP_GameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SP_GameInstance->CurrentLevelIndex = 0;
		SP_GameInstance->TotalScore = 0;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
    SetPause(false);
}
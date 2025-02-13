#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SP_PlayerController.generated.h"


class UInputMappingContext; // IMC 관련 전방 선언
class UInputAction;

UCLASS()
class TEST_1_API ASP_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASP_PlayerController();

	// 에디터에서 세팅할 IMC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	// IA_Move를 지정할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	// IA_Jump를 지정할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	// IA_Look를 지정할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	// IA_Sprint를 지정할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* HUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;

	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	void UpdateHUD();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();
	// 메인 메뉴 표시
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu(bool bIsRestart);
	// 게임 시작
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();


protected:
	virtual void BeginPlay() override;
};

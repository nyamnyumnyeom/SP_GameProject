#pragma once

#include "CoreMinimal.h"
#include "SP_PlayerController.h"
#include "GameFramework/Character.h"
#include "SP_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;

struct FInputActionValue;

UCLASS()
class TEST_1_API ASP_Character : public ACharacter
{
    GENERATED_BODY()

public:
    ASP_Character();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CameraComp;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* OverheadWidget;


    // 현재 체력을 가져오는 함수
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealth() const;

    // 체력을 회복시키는 함수
    UFUNCTION(BlueprintCallable, Category = "Health")
    void AddHealth(float Amount);

    // 사망 처리 함수 (체력이 0 이하가 되었을 때 호출)
    UFUNCTION(BlueprintCallable, Category = "Health")
    virtual void OnDeath();

    virtual void BeginPlay() override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION()
    void Move(const FInputActionValue& value);

    UFUNCTION()
    void StartJump(const FInputActionValue& value);

    UFUNCTION()
    void StopJump(const FInputActionValue& value);

    UFUNCTION()
    void Look(const FInputActionValue& value);

    UFUNCTION()
    void StartSprint(const FInputActionValue& value);

    UFUNCTION()
    void StopSprint(const FInputActionValue& value);

    // 최대 체력
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth;

    // 현재 체력
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
    float Health;

    // 데미지 처리 함수 - 외부로부터 데미지를 받을 때 호출됨
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    // HUD의 체력 바를 업데이트하는 함수 추가
    void UpdateHUDHP();

    UFUNCTION()
    void RestartGame();

    void UpdateOverheadHP();

private:
    float NormalSpeed;
    float SprintSpeedMultiplier;
    float SprintSpeed;
};

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


    // ���� ü���� �������� �Լ�
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealth() const;

    // ü���� ȸ����Ű�� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Health")
    void AddHealth(float Amount);

    // ��� ó�� �Լ� (ü���� 0 ���ϰ� �Ǿ��� �� ȣ��)
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

    // �ִ� ü��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth;

    // ���� ü��
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
    float Health;

    // ������ ó�� �Լ� - �ܺηκ��� �������� ���� �� ȣ���
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    // HUD�� ü�� �ٸ� ������Ʈ�ϴ� �Լ� �߰�
    void UpdateHUDHP();

    UFUNCTION()
    void RestartGame();

    void UpdateOverheadHP();

private:
    float NormalSpeed;
    float SprintSpeedMultiplier;
    float SprintSpeed;
};

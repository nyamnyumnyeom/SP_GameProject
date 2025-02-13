#include "SP_Character.h"
#include "SP_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "SP_GameBase.h"

ASP_Character::ASP_Character()
{
    PrimaryActorTick.bCanEverTick = false;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 300.0f;
    SpringArmComp->bUsePawnControlRotation = true;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false;

    OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
    OverheadWidget->SetupAttachment(GetMesh());
    OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

    NormalSpeed = 500.0f;
    SprintSpeedMultiplier = 2.0f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

    MaxHealth = 100.0f;
    Health = MaxHealth;
}

void ASP_Character::BeginPlay()
{
    Super::BeginPlay();
    UpdateOverheadHP();
}

void ASP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (ASP_PlayerController* PlayerController = Cast<ASP_PlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASP_Character::Move
                );
            }

            if (PlayerController->JumpAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASP_Character::StartJump
                );

                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Completed,
                    this,
                    &ASP_Character::StopJump
                );
            }

            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASP_Character::Look
                );
            }

            if (PlayerController->SprintAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASP_Character::StartSprint
                );

                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Completed,
                    this,
                    &ASP_Character::StopSprint
                );
            }
        }
    }
}

void ASP_Character::Move(const FInputActionValue& value)
{
    if (!Controller) return;

    const FVector2D MoveInput = value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
    }
}

void ASP_Character::StartJump(const FInputActionValue& value)
{
    if (value.Get<bool>())
    {
        Jump();
    }
}

void ASP_Character::StopJump(const FInputActionValue& value)
{
    if (!value.Get<bool>())
    {
        StopJumping();
    }
}

void ASP_Character::Look(const FInputActionValue& value)
{
    FVector2D LookInput = value.Get<FVector2D>();

    AddControllerYawInput(LookInput.X);
    AddControllerPitchInput(LookInput.Y);
}

void ASP_Character::StartSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void ASP_Character::StopSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

void ASP_Character::UpdateHUDHP()
{
    if (ASP_PlayerController* PlayerController = Cast<ASP_PlayerController>(GetController()))
    {
        if (PlayerController->GetHUDWidget())
        {
            // "HPBar"��� �̸��� ProgressBar ������ ������
            if (UProgressBar* HPBar = Cast<UProgressBar>(PlayerController->GetHUDWidget()->GetWidgetFromName(TEXT("HPBar"))))
            {
                float HPPercentage = FMath::Clamp(Health / MaxHealth, 0.0f, 1.0f);
                HPBar->SetPercent(HPPercentage);
            }
        }
    }
}

float ASP_Character::GetHealth() const
{
    return Health;
}

void ASP_Character::AddHealth(float Amount)
{
    // ü���� ȸ����Ŵ. �ִ� ü���� �ʰ����� �ʵ��� ������
    Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
    UE_LOG(LogTemp, Log, TEXT("Health increased to: %f"), Health);
    UpdateOverheadHP();
    UpdateHUDHP();
}

float ASP_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // �⺻ ������ ó�� ���� ȣ�� (�ʼ��� �ƴ�)
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // ü���� ��������ŭ ���ҽ�Ű��, 0 ���Ϸ� �������� �ʵ��� Clamp
    Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
    UpdateOverheadHP();
    UE_LOG(LogTemp, Warning, TEXT("Health decreased to: %f"), Health);

    UpdateHUDHP();

    // ü���� 0 ���ϰ� �Ǹ� ��� ó��
    if (Health <= 0.0f)
    {
        OnDeath();
    }

    // ���� ����� �������� ��ȯ
    return ActualDamage;
}

void ASP_Character::OnDeath()
{
    UE_LOG(LogTemp, Error, TEXT("Character is Dead!"));

    ASP_GameBase* SP_GameBase = GetWorld() ? GetWorld()->GetGameState<ASP_GameBase>() : nullptr;
    if (SP_GameBase)
    {
        SP_GameBase->OnGameOver();
    }
    // �̵� ���߱� (�߰� �ڵ�)
    GetCharacterMovement()->DisableMovement();

    // �÷��̾� �Է� ��Ȱ��ȭ
    AController* PlayerController = GetController();
    if (PlayerController)
    {
        PlayerController->DisableInput(Cast<APlayerController>(PlayerController));
    }

    // ���� ��� �����ͼ� ���� ���� ó��
    ASP_GameBase* GameMode = Cast<ASP_GameBase>(UGameplayStatics::GetGameMode(this));
    if (GameMode)
    {
        GameMode->OnGameOver();
    }

    // ���� �ð� �� ���� ������ �̵� (0.15�� ��)
    FTimerHandle RestartTimerHandle;
    GetWorldTimerManager().SetTimer(
        RestartTimerHandle,
        this,
        &ASP_Character::RestartGame,
        0.15f,
        false
    );
}

void ASP_Character::RestartGame()
{
    UGameplayStatics::OpenLevel(this, FName("MainLevel")); // ���� ������ ����
}

void ASP_Character::UpdateOverheadHP()
{
    if (!OverheadWidget) return;

    UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
    if (!OverheadWidgetInstance) return;

    if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
    {
        HPText->SetText(FText::FromString(FString::Printf(TEXT("HP : %.0f | %.0f"), Health, MaxHealth)));
    }
}
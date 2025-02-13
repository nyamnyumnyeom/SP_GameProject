#include "CoinItem.h"
#include "Engine/World.h"
#include "SP_GameBase.h"

ACoinItem::ACoinItem()
{
    PrimaryActorTick.bCanEverTick = true;
    PointValue = 0;
    ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
    if (Activator && Activator->ActorHasTag("Player"))
    {
        if (UWorld* World = GetWorld())
        {
            if (ASP_GameBase* GameState = World->GetGameState<ASP_GameBase>())
            {
                GameState->AddScore(PointValue);
                GameState->OnCoinCollected();
            }
        }
        DestroyItem();
    }
}
void ACoinItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AddActorLocalRotation(FRotator(0.0f, 90 * DeltaTime, 0.0f));
}

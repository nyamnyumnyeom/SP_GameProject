#include "SP_GameInstance.h"

USP_GameInstance::USP_GameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void USP_GameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}
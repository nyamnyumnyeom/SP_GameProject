#include "SP_Gamemode.h"
#include "SP_Character.h"
#include "SP_PlayerController.h"
#include "SP_GameBase.h"


ASP_Gamemode::ASP_Gamemode()
{
	DefaultPawnClass = ASP_Character::StaticClass();
	PlayerControllerClass = ASP_PlayerController::StaticClass();
	GameStateClass = ASP_GameBase::StaticClass();
}

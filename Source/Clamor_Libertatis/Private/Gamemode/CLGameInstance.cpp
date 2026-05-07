#include "Gamemode/CLGameInstance.h"

UCLGameInstance::UCLGameInstance()
{
	WonBattleCount = 0;
}

void UCLGameInstance::AddWonBattle()
{
	WonBattleCount++;
}

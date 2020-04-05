#include "B2Engine/GamePhaseStateMachine/GPSM_Phase.h"

#include "../BladeIIGameGameMode.h"

GPSM_Phase::GPSM_Phase()
{

}

void GPSM_Phase::Init(ABladeIIGameGameMode* GameMode)
{
	GameModeInstance = GameMode;
}

void GPSM_Phase::Tick(float DeltaSeconds)
{

}

void GPSM_Phase::End()
{

}

EGamePhase GPSM_Phase::GetStateType()
{
	return EGamePhase::None;
}

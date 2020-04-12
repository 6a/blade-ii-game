#include "B2Engine/GameState.h"

B2GameState::B2GameState(const FB2Cards& Cards)
	:Cards(FB2Cards(Cards))
{
	PlayerScore = 0;
	OpponentScore = 0;

	bAcceptPlayerInput = false;
	CursorPosition = ECardSlot::PlayerDeck;
	CursorSlotIndex = 0;

	Turn = EPlayer::Undecided;
	
	bBlastAnimationPending = false;
}

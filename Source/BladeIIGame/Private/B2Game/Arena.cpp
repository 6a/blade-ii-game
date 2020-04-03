#include "B2Game/Arena.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"

#include "B2Game/DeckSlot.h"
#include "B2Game/FieldSlot.h"
#include "B2Game/HandSlot.h"

AArena::AArena()
{
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arena Mesh"));
	RootComponent = MainMesh;

	FAttachmentTransformRules StandardAttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);

	DividerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Divider Mesh"));
	DividerMesh->AttachToComponent(MainMesh, StandardAttachmentRules);

	VSBadgeBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VS Badge Body"));
	VSBadgeBody->AttachToComponent(MainMesh, StandardAttachmentRules);

	VSBadgeText = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VS Badge Text"));
	VSBadgeText->AttachToComponent(MainMesh, StandardAttachmentRules);

	Slots = CreateDefaultSubobject<USceneComponent>(TEXT("Card Slots"));
	Slots->AttachToComponent(MainMesh, StandardAttachmentRules);

	PlayerDeck = CreateDefaultSubobject<UDeckSlot>(TEXT("Player Deck"));
	PlayerDeck->AttachToComponent(Slots, StandardAttachmentRules);

	PlayerHand = CreateDefaultSubobject<UHandSlot>(TEXT("Player Hand"));
	PlayerHand->AttachToComponent(Slots, StandardAttachmentRules);

	PlayerHandReversed = CreateDefaultSubobject<UHandSlot>(TEXT("Player Hand (Reversed)"));
	PlayerHandReversed->AttachToComponent(Slots, StandardAttachmentRules);

	PlayerField = CreateDefaultSubobject<UFieldSlot>(TEXT("Player Field"));
	PlayerField->AttachToComponent(Slots, StandardAttachmentRules);

	OpponentDeck = CreateDefaultSubobject<UDeckSlot>(TEXT("Opponent Deck"));
	OpponentDeck->AttachToComponent(Slots, StandardAttachmentRules);

	OpponentHand = CreateDefaultSubobject<UHandSlot>(TEXT("Opponent Hand"));
	OpponentHand->AttachToComponent(Slots, StandardAttachmentRules);

	OpponentField = CreateDefaultSubobject<UFieldSlot>(TEXT("Opponent Field"));
	OpponentField->AttachToComponent(Slots, StandardAttachmentRules);
}

// Called when the game starts or when spawned
void AArena::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


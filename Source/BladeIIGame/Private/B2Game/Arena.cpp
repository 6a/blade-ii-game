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

	DividerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Divider Mesh"));
	DividerMesh->SetupAttachment(MainMesh);

	VSBadgeBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VS Badge Body"));
	VSBadgeBody->SetupAttachment(MainMesh);

	VSBadgeText = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VS Badge Text"));
	VSBadgeText->SetupAttachment(MainMesh);

	Slots = CreateDefaultSubobject<USceneComponent>(TEXT("Card Slots"));
	Slots->SetupAttachment(MainMesh);

	PlayerDeck = CreateDefaultSubobject<UDeckSlot>(TEXT("Player Deck"));
	PlayerDeck->SetupAttachment(Slots);
	PlayerDeck->SetType(ECardSlot::PlayerDeck);

	PlayerHand = CreateDefaultSubobject<UHandSlot>(TEXT("Player Hand"));
	PlayerHand->SetupAttachment(Slots);
	PlayerHand->SetType(ECardSlot::PlayerHand);

	PlayerHandReversed = CreateDefaultSubobject<UHandSlot>(TEXT("Player Hand (Reversed)"));
	PlayerHandReversed->SetupAttachment(Slots);
	PlayerHandReversed->SetType(ECardSlot::PlayerHand);

	PlayerField = CreateDefaultSubobject<UFieldSlot>(TEXT("Player Field"));
	PlayerField->SetupAttachment(Slots);
	PlayerField->SetType(ECardSlot::PlayerField);

	OpponentDeck = CreateDefaultSubobject<UDeckSlot>(TEXT("Opponent Deck"));
	OpponentDeck->SetupAttachment(Slots);
	OpponentDeck->SetType(ECardSlot::OpponentDeck);

	OpponentHand = CreateDefaultSubobject<UHandSlot>(TEXT("Opponent Hand"));
	OpponentHand->SetupAttachment(Slots);
	OpponentHand->SetType(ECardSlot::OpponentHand);

	OpponentField = CreateDefaultSubobject<UFieldSlot>(TEXT("Opponent Field"));
	OpponentField->SetupAttachment(Slots);
	OpponentField->SetType(ECardSlot::OpponentField);
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


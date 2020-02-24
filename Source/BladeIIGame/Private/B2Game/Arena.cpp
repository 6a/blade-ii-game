#include "B2Game/Arena.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"

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

	PlayerDeck = CreateDefaultSubobject<UCardSlot>(TEXT("Player Deck"));
	PlayerDeck->AttachToComponent(Slots, StandardAttachmentRules);

	PlayerHand = CreateDefaultSubobject<UCardSlot>(TEXT("Player Hand"));
	PlayerHand->AttachToComponent(Slots, StandardAttachmentRules);

	PlayerField = CreateDefaultSubobject<UCardSlot>(TEXT("Player Field"));
	PlayerField->AttachToComponent(Slots, StandardAttachmentRules);

	OpponentDeck = CreateDefaultSubobject<UCardSlot>(TEXT("Opponent Deck"));
	OpponentDeck->AttachToComponent(Slots, StandardAttachmentRules);

	OpponentHand = CreateDefaultSubobject<UCardSlot>(TEXT("Opponent Hand"));
	OpponentHand->AttachToComponent(Slots, StandardAttachmentRules);

	OpponentField = CreateDefaultSubobject<UCardSlot>(TEXT("Opponent Field"));
	OpponentField->AttachToComponent(Slots, StandardAttachmentRules);

	// PrimaryActorTick.bCanEverTick = true;
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


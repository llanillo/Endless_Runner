// Fill out your copyright notice in the Description page of Project Settings.


#include "Tiles/BasicFloorTile.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "Player/PlayerBase.h"
#include "GameMode/EndlessRunnerGameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Obstacles/WallObstacle.h"

// Sets default values
ABasicFloorTile::ABasicFloorTile()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComponent);
	
	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Attach Point"));
	AttachPoint->SetupAttachment(SceneComponent);

	CenterLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Center Lane"));
	CenterLane->SetupAttachment(SceneComponent);

	RightLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Lane"));
	RightLane->SetupAttachment(SceneComponent);

	LeftLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Lane"));
	LeftLane->SetupAttachment(SceneComponent);

	FloorTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Floor Trigger Box"));
	FloorTriggerBox->SetupAttachment(SceneComponent);
	FloorTriggerBox->SetBoxExtent(FVector(32.0f, 500.0f, 200.0f));
	FloorTriggerBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

// Called when the game starts or when spawned
void ABasicFloorTile::BeginPlay()
{
	Super::BeginPlay();

	MainGameMode = Cast<AEndlessRunnerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	check(MainGameMode);

	FloorTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABasicFloorTile::OnTriggerBoxOverlap);
}

void ABasicFloorTile::SpawnItems()
{
	if(IsValid(SmallObstacleClass) && IsValid(BigObstacleClass))
	{
		SpawnLaneItem(CenterLane);
		SpawnLaneItem(RightLane);
		SpawnLaneItem(LeftLane);
	}
}

void ABasicFloorTile::SpawnLaneItem(const UArrowComponent* Lane) const
{
	const float RandValue = FMath::FRandRange(0.0f, 1.0f);
	const FAttachmentTransformRules AttachmentTransformRules (EAttachmentRule::KeepRelative, false);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	
	if(UKismetMathLibrary::InRange_FloatFloat(RandValue, 0.5f, 0.75f, true, true))
	{
		AWallObstacle* WallObstacle = GetWorld()->SpawnActor<AWallObstacle>(SmallObstacleClass, Lane->GetComponentTransform(), SpawnParams);
		WallObstacle->AttachToComponent(RootComponent, AttachmentTransformRules); // Check if works
	}
	else if(UKismetMathLibrary::InRange_FloatFloat(RandValue, 0.75f, 1.0f, true, true))
	{
		GetWorld()->SpawnActor<AWallObstacle>(BigObstacleClass, Lane->GetComponentTransform(), SpawnParams);
	}
}

void ABasicFloorTile::OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const APlayerBase* MainCharacter = Cast<APlayerBase>(OtherActor);

	if (MainCharacter)
	{
		MainGameMode->AddFloorTile(true);

		GetWorldTimerManager().SetTimer(DestroyHandle, this, &ABasicFloorTile::DestroyFloorTile, 2.0f, false);
	}
}

void ABasicFloorTile::DestroyFloorTile()
{
	if (DestroyHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DestroyHandle);
	}

	Destroy();
}
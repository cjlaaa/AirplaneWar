// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceShip.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "TimerManager.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SpawnArea->SetupAttachment(RootComponent);

	MinimumDistanceToPlayer = 12000;
	SpawnInterval = 2;
	MaxEnemyNum = 5;
	CurrentEnemyCount = 0;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	// 获取玩家对象
	SpaceShip = Cast<ASpaceShip>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(TimerHandle_Spawn, this, &AEnemySpawner::SpawnEnemy, SpawnInterval, true, 0);
}

FVector AEnemySpawner::GetGenerateLocation()
{
	float Distance = 0;
	FVector Location;
	// 在玩家的一定距离外生成
	while (Distance < MinimumDistanceToPlayer)
	{
		Location = UKismetMathLibrary::RandomPointInBoundingBox(SpawnArea->Bounds.Origin, SpawnArea->Bounds.BoxExtent);
		Distance = (Location - SpaceShip->GetActorLocation()).Size();
	}

	return Location;
}

void AEnemySpawner::SpawnEnemy()
{
	if (SpaceShip->GetBDead() == false && CurrentEnemyCount < MaxEnemyNum)
	{
		FActorSpawnParameters SpawnParams;
		if (GetWorld()->SpawnActor<AEnemy>(Enemy, GetGenerateLocation(), FRotator::ZeroRotator, SpawnParams) != NULL)
		{
			CurrentEnemyCount++;
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat(CurrentEnemyCount));
		}
	}
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::DecreaseEnemyCount()
{
	if (CurrentEnemyCount > 0) {
		CurrentEnemyCount--;
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat(CurrentEnemyCount));
	}
}


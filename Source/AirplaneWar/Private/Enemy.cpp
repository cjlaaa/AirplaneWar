// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceShip.h"
#include "Kismet/KismetMathLibrary.h"
#include "ShipGameMode.h"
#include "EnemySpawner.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	//RootComponent = RootComp;
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	//CollisionComp->SetupAttachment(RootComponent);
	RootComponent = CollisionComp;
	ShipSM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipSM"));
	ShipSM->SetupAttachment(RootComponent);
	
	Speed = 300;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// 获取玩家对象
	SpaceShip = Cast<ASpaceShip>(UGameplayStatics::GetPlayerPawn(this, 0));

	SetColor();

	TArray<AActor*> EnemySpawnerArray;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemySpawner::StaticClass(), EnemySpawnerArray);
	EnemySpawner = Cast<AEnemySpawner>(EnemySpawnerArray[0]);
	
	MyGameMode = Cast<AShipGameMode>(UGameplayStatics::GetGameMode(this));
}

void AEnemy::MoveTowardsPlayer(float DeltaTime)
{
	FVector Direction =  (SpaceShip->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	AddActorWorldOffset(Direction * Speed * DeltaTime, true);
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),SpaceShip->GetActorLocation()));
}

void AEnemy::OnDead()
{
	EnemySpawner->DecreaseEnemyCount();
	MyGameMode->IncreaseScore();
	Destroy();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpaceShip->GetBDead()==false) {
		MoveTowardsPlayer(DeltaTime);
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


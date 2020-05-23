// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy.h"
#include "Engine/BlockingVolume.h"
#include "EnemySpawner.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 设置组件
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;
	BulletSM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletSM"));
	BulletSM->SetupAttachment(RootComponent);

	// 与根组件同级,不需要绑定到根组件上
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> EnemySpawnerArray;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemySpawner::StaticClass(), EnemySpawnerArray);
	EnemySpawner = Cast<AEnemySpawner>(EnemySpawnerArray[0]);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		Enemy->Destroy();
		EnemySpawner->DecreaseEnemyCount();
		Destroy();
	}
	else if (Cast<ABlockingVolume>(OtherActor))
	{
		Destroy();
	}
}


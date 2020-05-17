 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemy;
class UBoxComponent;
class ASpaceShip;

UCLASS()
class AIRPLANEWAR_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	 
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USceneComponent* RootComp;
	
	UPROPERTY(EditAnywhere, Category = "Enemy")
	TSubclassOf<AEnemy> Enemy;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UBoxComponent* SpawnArea;

	ASpaceShip* SpaceShip;
	float MinimumDistanceToPlayer;
	FTimerHandle TimerHandle_Spawn;
	float SpawnInterval;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector GetGenerateLocation();
	void SpawnEnemy();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

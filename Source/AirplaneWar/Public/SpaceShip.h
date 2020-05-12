// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpaceShip.generated.h"

class USphereComponent;
class UCameraComponent;

UCLASS()
class AIRPLANEWAR_API ASpaceShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceShip();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere,Category="Component")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* ShipSM;

	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//UCameraComponent* CameraComp;

	APlayerController* PC;

	UPROPERTY(EditAnywhere, Category = "Move")
	float Speed;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LookAtCursor();

	void MoveUp(float Value);
	void MoveRight(float Value);
	void Move();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

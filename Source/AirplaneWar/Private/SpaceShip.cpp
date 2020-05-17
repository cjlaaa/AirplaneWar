// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShip.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Core/Public/Misc/App.h"
#include "Engine/World.h"
#include "AirplaneWar/Public/Bullet.h"
#include "TimerManager.h"

// Sets default values
ASpaceShip::ASpaceShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 设置组件
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;
	ShipSM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipSM"));
	ShipSM->SetupAttachment(RootComponent);
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetupAttachment(RootComponent);
	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(ShipSM);

	//// 设置正交相机
	//CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamaraComp"));
	//CameraComp->SetupAttachment(RootComponent);
	//CameraComp->SetProjectionMode(ECameraProjectionMode::Orthographic);
	//CameraComp->SetOrthoWidth(2500);
	
	Speed = 1000;
	TimeBetweenShot = 0.2;
}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();
	
	// 设置显示鼠标
	PC = Cast<APlayerController>(GetController());
	PC->bShowMouseCursor = true;
}

// 指向鼠标方向
void ASpaceShip::LookAtCursor()
{
	FVector MouseLocation, MouseDirection;
	// 获取鼠标的位置和方向并转为世界坐标
	PC->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
	// 只需X和Y旋转,Z不变
	FVector TargetLocation = FVector(MouseLocation.X, MouseLocation.Y, GetActorLocation().Z);
	// 获取指向目标方向的旋转角度
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	SetActorRotation(Rotation);
}

void ASpaceShip::MoveUp(float Value)
{
	AddMovementInput(FVector::ForwardVector, Value);
}

void ASpaceShip::MoveRight(float Value)
{
	AddMovementInput(FVector::RightVector, Value);
}

void ASpaceShip::Move()
{
	AddActorWorldOffset(ConsumeMovementInputVector() * Speed * FApp::GetDeltaTime(), true);
}

void ASpaceShip::Fire()
{
	if (Bullet) 
	{
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<ABullet>(Bullet, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
	}
	
}

void ASpaceShip::StartFire()
{
	GetWorldTimerManager().SetTimer(TimerHandle_BetweenShot, this, &ASpaceShip::Fire, TimeBetweenShot, true, 0);
}

void ASpaceShip::EndFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BetweenShot);
}

// Called every frame
void ASpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookAtCursor();
	Move();
}

// Called to bind functionality to input
void ASpaceShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveUp", this, &ASpaceShip::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASpaceShip::MoveRight);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASpaceShip::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASpaceShip::EndFire);
}


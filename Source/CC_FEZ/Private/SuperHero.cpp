// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperHero.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include <Runtime/Core/Public/Windows/COMPointer.h>

PRAGMA_DISABLE_OPTIMIZATION

//General Log
DEFINE_LOG_CATEGORY(YourLog);

//Logging during game startup
DEFINE_LOG_CATEGORY(YourInit);

//Logging for your AI system
DEFINE_LOG_CATEGORY(YourAI);

//Logging for Critical Errors that must always be addressed
DEFINE_LOG_CATEGORY(YourCriticalErrors);


// Sets default values
ASuperHero::ASuperHero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	direction = FVector::ZeroVector;
	SpawnPoint = FVector(50.0f, 50.0f, 100.0f);
	StoreLevel = 0;
	for (int i = 0;i < 4;i++) {
		for (int j = 0;j < 32;j++) {
			LevelsInfo[i][j].Init(0, 32);
			LevelsPoints[i][j].Init(FVector::ZeroVector, 32);
		}
	}
}

// Called when the game starts or when spawned
void ASuperHero::BeginPlay()
{
	Super::BeginPlay();
}

void ASuperHero::MoveForword(float value)
{
	if (value == 0.0f)
		return;
	direction = GetForwordDirection();
	AddMovementInput(direction, value);

}

FVector ASuperHero::GetForwordDirection()
{
	FVector ForwordDirection = FVector::ZeroVector;
	switch (FaceKind)
	{
	case 0:
		ForwordDirection.Y = 1.0f;
		break;
	case 1:
		ForwordDirection.X = -1.0f;
		break;
	case 2:
		ForwordDirection.Y = -1.0f;
		break;
	default:
		ForwordDirection.X = 1.0f;
		break;
	}
	return ForwordDirection;
}

int32 ASuperHero::GetIndex(FVector Location)
{
	int32 Index;
	switch (FaceKind)
	{
	case 0:
		Index = int(Location.Y) / 100;
		break;
	case 1:
		Index = int(Location.X) / 100;
		break;
	case 2:
		Index = int(Location.Y) / 100;
		break;
	default:
		Index = int(Location.X) / 100;
		break;
	}
	return Index;
}

int32 ASuperHero::GetNextIndex(float value, FVector &Location)
{
	int32 NextIndex;
	switch (FaceKind)
	{
	case 0:
		NextIndex = int(Location.Y + 51 * abs(value) / value) / 100;
		break;
	case 1:
		NextIndex = int(Location.X - 51 * abs(value) / value) / 100;
		break;
	case 2:
		NextIndex = int(Location.Y - 51 * abs(value) / value) / 100;
		break;
	default:
		NextIndex = int(Location.X + 51 * abs(value) / value) / 100;
		break;
	}
	return NextIndex;
}

void ASuperHero::UpdateLevelInfo(int32 Level)
{
	TArray<AActor*> BlockActors,MoveActors;
	FName ButtonTag = FName(FString::FromInt(Level));
	FName BlockTag = FName(FString::FromInt(Level+1));
	int32 Index;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), ButtonTag, MoveActors);		//GET LEVEL MOVE INFO
	for (AActor* Actor : MoveActors)
	{
		FVector TargetLocation = Actor->GetActorLocation();
		//FaceKind : 0
		Index = int(TargetLocation.Y) / 100;
		if (LevelsInfo[0][Level][Index] == 0) {
			LevelsInfo[0][Level][Index] = 1;
			LevelsPoints[0][Level][Index] = TargetLocation;
		}
		else if (LevelsInfo[0][Level][Index] == 1 && LevelsPoints[0][Level][Index].X > TargetLocation.X)
		{
			LevelsPoints[0][Level][Index] = TargetLocation;
		}
		//FaceKind : 1
		Index = int(TargetLocation.X) / 100;
		if (LevelsInfo[1][Level][Index] == 0) {
			LevelsInfo[1][Level][Index] = 1;
			LevelsPoints[1][Level][Index] = TargetLocation;
		}
		else if (LevelsInfo[1][Level][Index] == 1 && LevelsPoints[1][Level][Index].Y > TargetLocation.Y)
		{
			LevelsPoints[1][Level][Index] = TargetLocation;
		}
		//FaceKind : 2
		Index = int(TargetLocation.Y) / 100;
		if (LevelsInfo[2][Level][Index] == 0) {
			LevelsInfo[2][Level][Index] = 1;
			LevelsPoints[2][Level][Index] = TargetLocation;
		}
		else if (LevelsInfo[2][Level][Index] == 1 && LevelsPoints[2][Level][Index].X < TargetLocation.X)
		{
			LevelsPoints[2][Level][Index] = TargetLocation;
		}
		//FaceKind : 3
		Index = int(TargetLocation.X) / 100;
		if (LevelsInfo[3][Level][Index] == 0) {
			LevelsInfo[3][Level][Index] = 1;
			LevelsPoints[3][Level][Index] = TargetLocation;
		}
		else if (LevelsInfo[3][Level][Index] == 1 && LevelsPoints[3][Level][Index].Y < TargetLocation.Y)
		{
			LevelsPoints[3][Level][Index] = TargetLocation;
		}
	}
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), BlockTag, BlockActors);				//GET LEVEL BLOCK INFO
	for (AActor* Actor : BlockActors)
	{
		FVector TargetLocation = Actor->GetActorLocation();
		//FaceKind : 0
		Index = int(TargetLocation.Y) / 100;
		LevelsInfo[0][Level][Index] = 2;
		//FaceKind : 1
		Index = int(TargetLocation.X) / 100;
		LevelsInfo[1][Level][Index] = 2;
		//FaceKind : 2
		Index = int(TargetLocation.Y) / 100;
		LevelsInfo[2][Level][Index] = 2;
		//FaceKind : 3
		Index = int(TargetLocation.X) / 100;
		LevelsInfo[3][Level][Index] = 2;
	}

	for (int i = 0;i < 4;i++) {																//NO CUBE POINT USE LOWER LEVEL POINT CUBE
		for (int j = 0;j < 32;j++) {
			if (LevelsInfo[i][Level][j] == 0){
				LevelsInfo[i][Level][j] = 1;
				if(Level != 0)
					LevelsPoints[i][Level][j] = LevelsPoints[i][Level - 1][j];
			}
		}
	}
}

void ASuperHero::FaceKindAdd()
{
	FaceKind = (FaceKind + 1) % 4;
}

void ASuperHero::FaceKindSub()
{
	FaceKind--;
	if (FaceKind < 0) {
		FaceKind += 4;
	}
}

void ASuperHero::UpdateInfo()
{
	FVector Location = GetActorLocation();
	int32 Level = GetLevelBy_z(Location.Z);
	for (int i = StoreLevel;i <= Level + 1;i++) {
		UpdateLevelInfo(i);
		StoreLevel++;
	}
}

void ASuperHero::MovetoPoint()
{
	FVector Location = GetActorLocation();
	int32 Index = GetIndex(Location);
	int32 Level = GetLevelBy_z(Location.Z);
	if (Index < 0 || Index >= 32)
		return;
	FVector TargetLocation = LevelsPoints[FaceKind][Level][Index];
	TargetLocation.Z = Location.Z;
	SetActorLocation(TargetLocation);
}

int32 ASuperHero::GetLevelBy_z(float z)
{
	int32 Level = int(z) / 100 - 1;
	if (int(z) % 100 > 70)
		Level++;
	return Level;
}

bool ASuperHero::WillBeHidden()
{
	FVector Location = GetActorLocation();
	int32 Level = GetLevelBy_z(Location.Z);
	int32 Index = GetIndex(Location);
	if (LevelsInfo[FaceKind][Level][Index] == 2)
		return true;
	else
		return false;
}

// Called every frame
void ASuperHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASuperHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ASuperHero::MyJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ASuperHero::MyStopJump);
	InputComponent->BindAction("TurnRight", IE_Pressed, this, &ASuperHero::TurnRight);
	InputComponent->BindAction("TurnLeft", IE_Pressed, this, &ASuperHero::TurnLeft);
	InputComponent->BindAxis("Move", this, &ASuperHero::MoveForword);

}


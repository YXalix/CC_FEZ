// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SuperHero.generated.h"


//General Log
DECLARE_LOG_CATEGORY_EXTERN(YourLog, Log, All);

//Logging during game startup
DECLARE_LOG_CATEGORY_EXTERN(YourInit, Log, All);

//Logging for your AI system
DECLARE_LOG_CATEGORY_EXTERN(YourAI, Log, All);

//Logging for Critical Errors that must always be addressed
DECLARE_LOG_CATEGORY_EXTERN(YourCriticalErrors, Log, All);

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CC_FEZ_API ASuperHero : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASuperHero();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
	int32 FaceKind = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
	FVector SpawnPoint;

	UPROPERTY(VisibleAnywhere, Category = "MoveDirection")
	FVector direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CubeInfo")
	int32 StoreLevel;

	TArray<FVector> LevelsPoints[4][32];

	TArray<int32> LevelsInfo[4][32];

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForword(float value);

	void Flash2Cube();

	void UpdateLevelInfo(int32 Level);

	int32 GetNextIndex(float value, FVector &Location);


	UFUNCTION(BlueprintCallable)
	int32 GetIndex(FVector Location);

	FVector GetForwordDirection();

	int32 GetLevelBy_z(float z);

	UFUNCTION(BlueprintImplementableEvent)
	void MyJump();

	UFUNCTION(BlueprintImplementableEvent)
	void MyStopJump();

	UFUNCTION(BlueprintImplementableEvent)
	void TurnRight();

	UFUNCTION(BlueprintImplementableEvent)
	void TurnLeft();

	UFUNCTION(BlueprintCallable)
	bool WillBeHidden();

	UFUNCTION(BlueprintCallable)
	void FaceKindAdd();

	UFUNCTION(BlueprintCallable)
	void FaceKindSub();

	UFUNCTION(BlueprintCallable)
	void UpdateInfo();

	UFUNCTION(BlueprintCallable)
	void MovetoPoint();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

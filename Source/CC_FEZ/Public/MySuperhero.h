// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MySuperhero.generated.h"

class USphereComponent;
class USpringArmComponent;
class UCameraComponent;
UCLASS()
class CC_FEZ_API AMySuperhero : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMySuperhero();

protected:

	/*UPROPERTY(VisibleAnywhere, Category = "Collision")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UParticleSystemComponent* OurParticleSystem;

	UPROPERTY(VisibleAnywhere,Category="Component")
	UStaticMeshComponent* HeroSM;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere,Category="Component")
	UCameraComponent* Camera;*/
	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

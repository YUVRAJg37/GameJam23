// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Aircraft.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USoundCue;
class UParticleSystem;

UCLASS()
class GAMEJAM_API AAircraft : public APawn
{
	GENERATED_BODY()

private:
	float Health;
	float MaxHealth;
	float BaseDamage;

public:

	AAircraft();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
protected:

	virtual void BeginPlay() override;

	void ShootButtonPressed();

	void ShootButtonReleased();

	void AutoShootReset();

	void StartShootTimer();
	
	void Shoot();

	void SpawnParticlesAndLineTrace(FName SocketName);

	void Explode();
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Plane Properties")
		TObjectPtr<UStaticMeshComponent> Collision;

	UPROPERTY(EditAnywhere, Category = "Plane Properties")
		TObjectPtr<USkeletalMeshComponent> PlaneMesh;

	UPROPERTY(EditAnywhere, Category = "Plane Properties")
		TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditAnywhere, Category = "Plane Properties")
		TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Properties", meta = (AllowPrivateAccess = "true"))
		float CurrentThrottle = 100.f;

	UPROPERTY(EditAnywhere, Category = "Plane Properties")
		float MaxThrottle = 750.f;

	UPROPERTY(EditAnywhere, Category = "Plane Properties")
		float MaxSpeed = 50.f;

	bool MoveUpThrottle;
	bool MoveDownThrottle;
	bool ZeroThrottle;
	float MouseY;
	float MouseX;
	float Rudder;

	UPROPERTY(EditAnywhere, Category = "Mouse Input")
		float MouseTorque = -2000.f;

	UPROPERTY(EditAnywhere, Category = "Keyboard Input")
		float TurnTorque = 1250.f;

	UFUNCTION(BlueprintCallable)
		void MouseYInput(float Y);

	UFUNCTION(BlueprintCallable)
		void MouseXInput(float X);

	void MoveRight(float Value);

	void MoveLeft(float Value);

	void OnThrottle(float Value);

	void OffThrottle(float Value);

	void AddSpeed();

	//Sounds
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USoundCue> FireSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USoundCue> ExplodeSound;

	//FX
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UParticleSystem> MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UParticleSystem> ExplodeParticles;

	
	bool bShootButtonPressed;
	bool bShouldShoot;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate;

	
	FTimerHandle FireRateHandle;
	
};

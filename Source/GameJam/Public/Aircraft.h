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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Properties", meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Properties", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Properties", meta = (AllowPrivateAccess = "true"))
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Properties", meta = (AllowPrivateAccess = "true"))
	float MinFireRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Properties", meta = (AllowPrivateAccess = "true"))
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Properties", meta = (AllowPrivateAccess = "true"))
	float FireRateIncrement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Properties", meta = (AllowPrivateAccess = "true"))
	float HealthIncrement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Properties", meta = (AllowPrivateAccess = "true"))
	float ExplodeVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Properties", meta = (AllowPrivateAccess = "true"))
	float MinThrottle;
	
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

	UFUNCTION(BlueprintCallable)
	void Explode();

	
private:
	
	UPROPERTY(EditAnywhere, Category = "Plane Properties")
		TObjectPtr<UStaticMeshComponent> Collision;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Plane Properties", meta= (AllowPrivateAccess="true"))
		TObjectPtr<USkeletalMeshComponent> PlaneMesh;

	UPROPERTY(EditAnywhere, Category = "Plane Properties")
		TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditAnywhere, Category = "Plane Properties")
		TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Properties", meta = (AllowPrivateAccess = "true"))
		float CurrentThrottle = 100.f;

	UPROPERTY(EditAnywhere, Category = "Plane Properties")
		float MaxThrottle = 500.f;

	UPROPERTY(EditAnywhere, Category = "Plane Properties")
		float MinThrottle = 500.f;

	UPROPERTY(EditAnywhere, Category = "Plane Properties")
		float MaxSpeed = 50.f;

	UPROPERTY(EditAnywhere, Category = "Plane Properties")
	float ImaginaryGravity = -1500.f;
	
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

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UParticleSystem> ImpactParticles;
	
	bool bShootButtonPressed;
	bool bShouldShoot;

	double FireTimer;
	
	FTimerHandle FireRateHandle;

	FVector CurrentVelocity;
};

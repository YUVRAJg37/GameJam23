// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Aircraft.generated.h"

UCLASS()
class GAMEJAM_API AAircraft : public APawn
{
	GENERATED_BODY()

public:

	AAircraft();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;

private:


	UPROPERTY(EditAnywhere, Category = "Plane Properties")
		UStaticMeshComponent* Collision;

	UPROPERTY(EditAnywhere, Category = "Plane Properties")
		USkeletalMeshComponent* PlaneMesh;

	UPROPERTY(EditAnywhere, Category = "Plane Properties")
		class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, Category = "Plane Properties")
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Data", meta = (AllowPrivateAccess = "true"))
		float CurrentThrottle = 100.f;

	UPROPERTY(EditAnywhere, Category = "Plane Data")
		float MaxThrottle = 750.f;

	UPROPERTY(EditAnywhere, Category = "Plane Data")
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

	void Speed();
};

#include "Aircraft.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AAircraft::AAircraft() :
MinThrottle(400)
{

	PrimaryActorTick.bCanEverTick = true;

	BaseDamage = 4.f;
	Health = 10.f;
	MaxHealth = 100.f;

	CurrentThrottle = 100.f;
	MaxThrottle = 750.f;
	MaxSpeed = 50.f;
	
	bShouldShoot = true;
	FireRate = 1200.f;
	MinFireRate = 400.f;
	FireRateIncrement = -100.f;
	HealthIncrement +=50.f;
	ExplodeVelocity = 0.f;
	
	Collision = CreateDefaultSubobject<UStaticMeshComponent>("Plane Collision");
	SetRootComponent(Collision);
	Collision->SetHiddenInGame(true, false);
	Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Collision->SetSimulatePhysics(true);
	Collision->SetEnableGravity(false);
	
	PlaneMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Plane Mesh");
	PlaneMesh->SetupAttachment(Collision);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Camera Boom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 2600.f;
	CameraBoom->bEnableCameraLag = false;
	CameraBoom->bEnableCameraRotationLag = false;
	CameraBoom->AddRelativeRotation(FRotator(-20.f, 0.f, 0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = false;
}

void AAircraft::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAircraft::ShootButtonPressed()
{
	bShootButtonPressed = true;
	StartShootTimer();
}

void AAircraft::ShootButtonReleased()
{
	bShootButtonPressed = false;
}

void AAircraft::AutoShootReset()
{
	bShouldShoot = true;
	if(bShootButtonPressed)
	{
		StartShootTimer();
	}
}

void AAircraft::StartShootTimer()
{
	if(bShouldShoot)
	{
		Shoot();
		bShouldShoot = false;
		FireTimer = (1/(FireRate/60));
		GetWorldTimerManager().SetTimer(FireRateHandle, this,&AAircraft::AutoShootReset, FireTimer);
	}
}

void AAircraft::Shoot()
{
	
	this->SpawnParticlesAndLineTrace(FName("Turret_1"));
	this->SpawnParticlesAndLineTrace(FName("Turret_2"));
	this->SpawnParticlesAndLineTrace(FName("Turret_3"));
	this->SpawnParticlesAndLineTrace(FName("Turret_4"));
	
	if(FireSound)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);
	}
}


void AAircraft::SpawnParticlesAndLineTrace(FName SocketName)
{
	
	if(MuzzleFlash)
	{
		FVector SocketLocation;
		FRotator SocketRotation;
		
		PlaneMesh->GetSocketWorldLocationAndRotation(SocketName,SocketLocation,SocketRotation);
		UGameplayStatics::SpawnEmitterAtLocation(this, MuzzleFlash, SocketLocation,SocketRotation);

		FVector2D ViewportSize;

		if(GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(ViewportSize);
		}

		FVector2D CrosshairLocation =  { (ViewportSize.X / 2.f), ((ViewportSize.Y / 2.f) - 100.f) };

		FVector CrosshairWorldPosition;
		FVector CrosshairWorldDirection;
		
		bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this,0),CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);
		
		FHitResult OutHitResult;
		
		FVector TraceStart = CrosshairWorldPosition;
		FVector TraceEnd = CrosshairWorldPosition + (CrosshairWorldDirection * 50'000);
		
		if(bool bBlockingHit = GetWorld()->LineTraceSingleByChannel(OutHitResult, TraceStart, TraceEnd ,ECollisionChannel::ECC_Visibility,FCollisionQueryParams::DefaultQueryParam,FCollisionResponseParams::DefaultResponseParam))
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, ImpactParticles, OutHitResult.ImpactPoint);
            AActor* HitActor = OutHitResult.GetActor();
			/*
			 * if(AAircraft* Turret = Cast<>(HitActor))
			 * {
			 *	UGameplayStatics::ApplyDamage(Turret, BaseDamage, GetController(),this, UDamageType::StaticClass());
			 * }
			*/  
		}
	}
}

void AAircraft::Explode()
{
	if(ExplodeParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeParticles, GetActorLocation(),GetActorRotation());
	}
	if(ExplodeSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation(), GetActorRotation());
	}
	this->Destroy();
}

void AAircraft::MouseYInput(float Y)
{
	MouseY = Y;

	const FVector PlaneRightVector = FVector(Collision->GetRightVector());

	const float OnThrottleTorque = (MouseY * MouseTorque);
	const float OffThrottleTorque = (MouseY * MouseTorque) / 3.f;

	if (CurrentThrottle > 0.f)
	{
		Collision->AddTorqueInDegrees(UKismetMathLibrary::VLerp(FVector::ZeroVector, OnThrottleTorque * PlaneRightVector, 0.1), NAME_None, true);
	}
	else
	{
		Collision->AddTorqueInDegrees(UKismetMathLibrary::VLerp(FVector::ZeroVector, OffThrottleTorque * PlaneRightVector, 0.1), NAME_None, true);
	}
}

void AAircraft::MouseXInput(float X)
{
	MouseX = X;

	const FVector PlaneRightVector = FVector(Collision->GetForwardVector());

	const float OnThrottleTorque = (MouseX * MouseTorque);
	const float OffThrottleTorque = (MouseX * MouseTorque) / 3.f;

	if (CurrentThrottle > 0.f)
	{
		Collision->AddTorqueInDegrees(UKismetMathLibrary::VLerp(FVector::ZeroVector, OnThrottleTorque * PlaneRightVector, 0.1), NAME_None, true);
	}
	else
	{
		Collision->AddTorqueInDegrees(UKismetMathLibrary::VLerp(FVector::ZeroVector, OffThrottleTorque * PlaneRightVector, 0.1), NAME_None, true);
	}
}

void AAircraft::MoveRight(float Value)
{

	Rudder = Value * (CurrentThrottle / MaxThrottle);

	const FVector PlaneUpVector = Collision->GetUpVector();

	const float RightMagnitude = (Value * TurnTorque) * (CurrentThrottle / MaxThrottle);

	Collision->AddTorqueInDegrees(UKismetMathLibrary::VLerp(FVector::ZeroVector, RightMagnitude * PlaneUpVector, 0.1f), NAME_None, true);

}

void AAircraft::MoveLeft(float Value)
{
	Rudder = Value * (CurrentThrottle / MaxThrottle);

	const FVector PlaneUpVector = Collision->GetUpVector() * -1.f;

	const float RightMagnitude = (Value * TurnTorque) * (CurrentThrottle / MaxThrottle);

	Collision->AddTorqueInDegrees(UKismetMathLibrary::VLerp(FVector::ZeroVector, RightMagnitude * PlaneUpVector, 0.1f), NAME_None, true);
}

void AAircraft::OnThrottle(float Value)
{
	MoveUpThrottle = (Value) ? true : false;
}

void AAircraft::OffThrottle(float Value)
{
	MoveDownThrottle = (Value) ? true : false;
}

void AAircraft::AddSpeed()
{
	const FVector LinearVelocity = Collision->GetPhysicsLinearVelocity();

	const FVector PlaneForwardVectorScaled = ((Collision->GetForwardVector()) * (CurrentThrottle * MaxSpeed));

	Collision->SetPhysicsLinearVelocity(UKismetMathLibrary::VLerp(LinearVelocity, PlaneForwardVectorScaled, 0.1f));
}


void AAircraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (MoveUpThrottle)
	{
		CurrentThrottle = FMath::Clamp(CurrentThrottle + 1.f, MinThrottle, MaxThrottle);
	}
	else if (MoveDownThrottle)
	{
		CurrentThrottle = FMath::Clamp(CurrentThrottle - 1.f, 0.f, MaxThrottle);
		
	}

	const FVector PhysXAngularVelocity = (Collision->GetPhysicsAngularVelocityInDegrees() * -1.f) / (0.75f);

	Collision->AddTorqueInDegrees(PhysXAngularVelocity, NAME_None, true);
	AddSpeed();
	
	Collision->AddForce(FVector(0.f, 0.f, ImaginaryGravity), NAME_None, true);
	
	if (ZeroThrottle)
	{
		CurrentThrottle = UKismetMathLibrary::FInterpTo_Constant(CurrentThrottle, 0.f, UGameplayStatics::GetWorldDeltaSeconds(this), 500.f);
	}
	
}

float AAircraft::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(Health - DamageAmount <=0.f)
	{
		Health = 0.f;
		Explode();
	}
	else
	{
		Health -=BaseDamage;
	}
	return Health;
}

void AAircraft::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AAircraft::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MoveLeft"), this, &AAircraft::MoveLeft);
	PlayerInputComponent->BindAxis(TEXT("OnThrottle"), this, &AAircraft::OnThrottle);
	PlayerInputComponent->BindAxis(TEXT("OffThrottle"), this, &AAircraft::OffThrottle);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AAircraft::ShootButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Released, this, &AAircraft::ShootButtonReleased);
}



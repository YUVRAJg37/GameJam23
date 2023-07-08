#include "Aircraft.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

AAircraft::AAircraft()
{

	PrimaryActorTick.bCanEverTick = true;

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
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = false;
	CameraBoom->CameraLagSpeed = 100.f;
	CameraBoom->AddRelativeRotation(FRotator(-20.f, 0.f, 0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = false;


}

void AAircraft::BeginPlay()
{
	Super::BeginPlay();

}

void AAircraft::MouseYInput(float Y)
{
	MouseY = Y;

	FVector PlaneRightVector = FVector(Collision->GetRightVector());

	float OnThrottleTorque = (MouseY * MouseTorque);
	float OffThrottleTorque = (MouseY * MouseTorque) / 3.f;

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

	FVector PlaneRightVector = FVector(Collision->GetForwardVector());

	float OnThrottleTorque = (MouseX * MouseTorque);
	float OffThrottleTorque = (MouseX * MouseTorque) / 3.f;

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

	FVector PlaneUpVector = Collision->GetUpVector();

	float RightMagnitude = (Value * TurnTorque) * (CurrentThrottle / MaxThrottle);

	Collision->AddTorqueInDegrees(UKismetMathLibrary::VLerp(FVector::ZeroVector, RightMagnitude * PlaneUpVector, 0.1f), NAME_None, true);

}

void AAircraft::MoveLeft(float Value)
{
	Rudder = Value * (CurrentThrottle / MaxThrottle);

	FVector PlaneUpVector = Collision->GetUpVector() * -1.f;

	float RightMagnitude = (Value * TurnTorque) * (CurrentThrottle / MaxThrottle);

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

void AAircraft::Speed()
{
	FVector LinearVelocity = Collision->GetPhysicsLinearVelocity();

	FVector PlaneForwardVectorScaled = ((Collision->GetForwardVector()) * (CurrentThrottle * MaxSpeed));

	Collision->SetPhysicsLinearVelocity(UKismetMathLibrary::VLerp(LinearVelocity, PlaneForwardVectorScaled, 0.1f));
}

void AAircraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MoveUpThrottle)
	{
		CurrentThrottle = FMath::Clamp(CurrentThrottle + 1.f, 0.f, MaxThrottle);
	}
	else if (MoveDownThrottle)
	{
		CurrentThrottle = FMath::Clamp(CurrentThrottle - 1.f, 0.f, MaxThrottle);
	}

	FVector PhysXAngularVelocity = (Collision->GetPhysicsAngularVelocityInDegrees() * -1.f) / (0.75f);

	Collision->AddTorqueInDegrees(PhysXAngularVelocity, NAME_None, true);

	Speed();

	Collision->AddForce(FVector(0.f, 0.f, -15000.f), NAME_None, true);

	if (ZeroThrottle)
	{
		CurrentThrottle = UKismetMathLibrary::FInterpTo_Constant(CurrentThrottle, 0.f, UGameplayStatics::GetWorldDeltaSeconds(this), 500.f);
	}

	UE_LOG(LogTemp, Warning, TEXT("X: %f    Y: %f   Z: %f"), (Collision->GetComponentLocation()).X, (Collision->GetComponentLocation()).Y, (Collision->GetComponentLocation()).Z);
}

void AAircraft::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AAircraft::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MoveLeft"), this, &AAircraft::MoveLeft);
	PlayerInputComponent->BindAxis(TEXT("OnThrottle"), this, &AAircraft::OnThrottle);
	PlayerInputComponent->BindAxis(TEXT("OffThrottle"), this, &AAircraft::OffThrottle);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "BacteriaAgent.h"

#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GenericPlatform/GenericPlatformMath.h"

#define UPPER_BOUND Length
#define LOWER_BOUND -Length

#define UPPER_ACCEPTABLE_BOUND Width
#define LOWER_ACCEPTABLE_BOUND -Width

#define UPPER_BOUND_ROTATION 40.0f
#define LOWER_BOUND_ROTATION -40.0f

// Sets default values for this component's properties
UBacteriaAgent::UBacteriaAgent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBacteriaAgent::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle MemberTimerHandle;
	// Call RepeatingFunction once per second, starting two seconds from now.
	GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &UBacteriaAgent::DoBinaryFission, 1.0f, true, 2.0f);
	
}


// Called every frame
void UBacteriaAgent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UBacteriaAgent::TestFunction(bool A) {
	return true;
}

void UBacteriaAgent::DoBinaryFission() {

	EnergyLevel += 0.5f;

	if (EnergyLevel >= 3.0 && MyVariable) {

		EnergyLevel += 0.0f;

		FVector CurrentLocation = GetOwner()->GetActorLocation();
		FRotator CurrentRotation = GetOwner()->GetActorRotation();

		//ComplexFission(CurrentLocation, CurrentRotation);

		if ((CurrentRotation.Yaw >= 0.0f && CurrentRotation.Yaw <= 10.0f) ||
			(CurrentRotation.Yaw >= 80.0f && CurrentRotation.Yaw <= 100.0f) ||
			(CurrentRotation.Yaw >= 170.0f && CurrentRotation.Yaw <= 190.0f) ||
			(CurrentRotation.Yaw >= 260.0f && CurrentRotation.Yaw <= 280.0f)) {

			EasyFission(CurrentLocation, CurrentRotation);

		}
		else {
			ComplexFission(CurrentLocation, CurrentRotation);
		}
	}
}

void UBacteriaAgent::FissionArea() {

	FVector CurrentLocation = GetOwner()->GetActorLocation();

	float TempX = FMath::RandRange(LOWER_BOUND, UPPER_BOUND);
	while (TempX >= LOWER_ACCEPTABLE_BOUND && TempX <= UPPER_ACCEPTABLE_BOUND) {
		TempX = FMath::RandRange(LOWER_BOUND, UPPER_BOUND);
	}
	float XPoint = CurrentLocation.X + TempX;

	float TempY = FMath::RandRange(LOWER_BOUND, UPPER_BOUND);
	while (TempY >= LOWER_ACCEPTABLE_BOUND && TempX <= UPPER_ACCEPTABLE_BOUND) {
		TempY = FMath::RandRange(LOWER_BOUND, UPPER_BOUND);
	}
	float YPoint = CurrentLocation.Y + TempY;

	float TempZ = FMath::RandRange(16.0f, 20.0f);
	float ZPoint = CurrentLocation.Z + TempZ;	

	FVector Location(XPoint, YPoint, ZPoint);

	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	float RotationX = CurrentRotation.Roll;
	float RotationY = CurrentRotation.Pitch;
	float RotationZ = CurrentRotation.Yaw + FMath::RandRange(LOWER_BOUND_ROTATION, UPPER_BOUND_ROTATION);

	//float InPitch, float InYaw, float InRoll
	FRotator Rotation(RotationY, RotationZ, RotationX);

	FActorSpawnParameters SpawnInfo;

	FActorSpawnParameters ActorSpawnParameters;
	
	AActor* NewBacteria = GetWorld()->SpawnActor<AActor>(ActorToSpawn, Location, Rotation, ActorSpawnParameters);

}

void UBacteriaAgent::EasyFission(FVector CurrentLocation, FRotator CurrentRotation) {

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("EasyFission")));

	float X, Y, Z;
	float Distance = Length;

	if ((CurrentRotation.Yaw >= 0.0f && CurrentRotation.Yaw <= 10.0f) ||
		(CurrentRotation.Yaw >= 170.0f && CurrentRotation.Yaw <= 190.0f)) {

		//X constant and Y constant
		X = CurrentLocation.X;
		Z = CurrentLocation.Z + 10;
		Y = CurrentLocation.Y;

		float randomNumber = FMath::RandRange(0.0f, 50.0f);
		int option;
		if (randomNumber >= 0 && randomNumber < 25) {
			Y += Length;
			option = 1;
		}
		else {
			Y -= Length;
			option = 2;
		}

		FVector Location(X, Y, Z);
		FActorSpawnParameters ActorSpawnParameters;

		float RotationX = CurrentRotation.Roll;
		float RotationY = CurrentRotation.Pitch;
		float RotationZ = CurrentRotation.Yaw + FMath::RandRange(LOWER_BOUND_ROTATION, UPPER_BOUND_ROTATION);

		//float InPitch, float InYaw, float InRoll
		FRotator Rotation(RotationY, RotationZ, RotationX);

		AActor* NewBacteria = GetWorld()->SpawnActor<AActor>(ActorToSpawn, Location, Rotation, ActorSpawnParameters);

		if (!IsValid(NewBacteria)) {
			if (option == 1) {
				Y -= Length * 2;
			}
			else {
				Y += Length * 2;
			}
			FVector Location2(X, Y, Z);
			NewBacteria = GetWorld()->SpawnActor<AActor>(ActorToSpawn, Location2, Rotation, ActorSpawnParameters);
			if (!IsValid(NewBacteria)) {
				FissionArea();
			}
		}

	}
	else { //CurrentRotation.Yaw != 90 || CurrentRotation.Yaw != 270

		X = CurrentLocation.X;
		Y = CurrentLocation.Y;
		Z = CurrentLocation.Z + 10;

		int option;
		float randomNumber = FMath::RandRange(0.0f, 50.0f);
		if (randomNumber >= 0 && randomNumber < 25) {
			X += Length;
			option = 1;
		}
		else {
			X -= Length;
			option = 2;
		}

		FVector Location(X, Y, Z);
		FActorSpawnParameters ActorSpawnParameters;

		float RotationX = CurrentRotation.Roll;
		float RotationY = CurrentRotation.Pitch;
		float RotationZ = CurrentRotation.Yaw + FMath::RandRange(LOWER_BOUND_ROTATION, UPPER_BOUND_ROTATION);

		//float InPitch, float InYaw, float InRoll
		FRotator Rotation(RotationY, RotationZ, RotationX);

		AActor* NewBacteria = GetWorld()->SpawnActor<AActor>(ActorToSpawn, Location, Rotation, ActorSpawnParameters);

		if (!IsValid(NewBacteria)) {
			if (option == 1) {
				X -= Length * 2;
			}
			else {
				X += Length * 2;
			}
			FVector Location2(X, Y, Z);
			NewBacteria = GetWorld()->SpawnActor<AActor>(ActorToSpawn, Location2, Rotation, ActorSpawnParameters);
			if (!IsValid(NewBacteria)) {
				FissionArea();
			}
		}

	}

}

void UBacteriaAgent::ComplexFission(FVector CurrentLocation, FRotator CurrentRotation) {

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ComplexFission")));

	float X, Y, Z;

	float X1 = CurrentLocation.X;
	float Y1 = CurrentLocation.Y;

	float X2 = X1 + FGenericPlatformMath::Cos(CurrentRotation.Yaw);
	float Y2 = Y1 + FMath::Sin(CurrentRotation.Yaw);

	float Slope = (Y2 - Y1) / (X2 - X1);
	float Intercept = Y1 - Slope * X1;

	int option;
	Z = CurrentLocation.Z + 15.0f;
	float randomNumber = FMath::RandRange(0.0f, 50.0f);
	if (randomNumber >= 0.0f && randomNumber < 25.0f) {
		X = ComputeX1(Intercept, Slope, X1, Y1, Length);
		Y = ComputeY1(Intercept, Slope, X1, Y1, Length);
		option = 1;
	}
	else {
		X = ComputeX2(Intercept, Slope, X1, Y1, Length);
		Y = ComputeY2(Intercept, Slope, X1, Y1, Length);
		option = 2;
	}

	FVector Location(X, Y, Z);
	FActorSpawnParameters ActorSpawnParameters;

	float RotationX = CurrentRotation.Roll;
	float RotationY = CurrentRotation.Pitch;
	float RotationZ = CurrentRotation.Yaw + FMath::RandRange(LOWER_BOUND_ROTATION, UPPER_BOUND_ROTATION);

	//float InPitch, float InYaw, float InRoll
	FRotator Rotation(RotationY, RotationZ, RotationX);

	AActor* NewBacteria = GetWorld()->SpawnActor<AActor>(ActorToSpawn, Location, Rotation, ActorSpawnParameters);

	if (!IsValid(NewBacteria)) {

		if (option == 1) {
			X = ComputeX2(Intercept, Slope, X1, Y1, Length);
			Y = ComputeY2(Intercept, Slope, X1, Y1, Length);
		}
		else {
			X = ComputeX1(Intercept, Slope, X1, Y1, Length);
			Y = ComputeY1(Intercept, Slope, X1, Y1, Length);
		}

		FVector Location2(X, Y, Z);

		NewBacteria = GetWorld()->SpawnActor<AActor>(ActorToSpawn, Location2, Rotation, ActorSpawnParameters);

		if (!IsValid(NewBacteria)) {
			FissionArea();
		}

	}

}

float UBacteriaAgent::ComputeX1(float B, float Slope, float U, float V, float D) {
	float X;
	X = (-FGenericPlatformMath::Sqrt(-FGenericPlatformMath::Pow(B, 2) - 2 * B * Slope * U +
		2 * B * V + FGenericPlatformMath::Pow(D, 2) * FGenericPlatformMath::Pow(Slope, 2) +
		FGenericPlatformMath::Pow(D, 2) - FGenericPlatformMath::Pow(Slope, 2) * FGenericPlatformMath::Pow(U, 2)
		+ 2 * Slope * U * V - FGenericPlatformMath::Pow(V, 2))
		- B * Slope + Slope * V + U) / (FGenericPlatformMath::Pow(Slope, 2) + 1);
	return X;
}

float UBacteriaAgent::ComputeY1(float B, float Slope, float U, float V, float D) {
	float Y;
	Y = (-Slope * FGenericPlatformMath::Sqrt(-FGenericPlatformMath::Pow(B, 2) - 2 * B * Slope * U
		+ 2 * B * V + FGenericPlatformMath::Pow(D, 2) * FGenericPlatformMath::Pow(Slope, 2)
		+ FGenericPlatformMath::Pow(D, 2) - FGenericPlatformMath::Pow(Slope, 2) * FGenericPlatformMath::Pow(U, 2)
		+ 2 * Slope * U * V - FGenericPlatformMath::Pow(V, 2))
		+ B + FGenericPlatformMath::Pow(Slope, 2) * V + Slope * U) / (FGenericPlatformMath::Pow(Slope, 2) + 1);
	return Y;
}

float UBacteriaAgent::ComputeX2(float B, float Slope, float U, float V, float D) {
	float X;
	X = (FGenericPlatformMath::Sqrt(-FGenericPlatformMath::Pow(B, 2) - 2 * B * Slope * U + 2 * B * V
		+ FGenericPlatformMath::Pow(D, 2) * FGenericPlatformMath::Pow(Slope, 2) + FGenericPlatformMath::Pow(D, 2)
		- FGenericPlatformMath::Pow(Slope, 2) * FGenericPlatformMath::Pow(U, 2) + 2 * Slope * U * V
		- FGenericPlatformMath::Pow(V, 2)) - B * Slope + Slope * V + U) / (FGenericPlatformMath::Pow(Slope, 2) + 1);
	return X;
}

float UBacteriaAgent::ComputeY2(float B, float Slope, float U, float V, float D) {
	float Y;
	Y = (Slope * FGenericPlatformMath::Sqrt(-FGenericPlatformMath::Pow(B, 2) - 2 * B * Slope * U
		+ 2 * B * V + FGenericPlatformMath::Pow(D, 2) * FGenericPlatformMath::Pow(Slope, 2)
		+ FGenericPlatformMath::Pow(D, 2) - FGenericPlatformMath::Pow(Slope, 2) * FGenericPlatformMath::Pow(U, 2)
		+ 2 * Slope * U * V - FGenericPlatformMath::Pow(V, 2)) + B + FGenericPlatformMath::Pow(Slope, 2) * V
		+ Slope * U) / (FGenericPlatformMath::Pow(Slope, 2) + 1);
	return Y;
}

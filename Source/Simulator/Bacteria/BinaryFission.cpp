// Fill out your copyright notice in the Description page of Project Settings.


#include "BinaryFission.h"
#include "Kismet/KismetMathLibrary.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "DrawDebugHelpers.h"

//macros to define the range of random rotation on Z axis to spawn a bacterium
#define UPPER_BOUND_ROTATION 40.0f
#define LOWER_BOUND_ROTATION -40.0f
#define PI 3.141593

// Sets default values for this component's properties
UBinaryFission::UBinaryFission()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBinaryFission::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBinaryFission::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//Calls a function to do the binary fission according to the angle in Z axis of bacterium
void UBinaryFission::DoBinaryFission(TSubclassOf<AActor> ActorToSpawn, AActor* Owner, float Length, float Width) {

	//get current location and rotation of this bacterium
	FVector CurrentLocation = Owner->GetActorLocation();
	FRotator CurrentRotation = Owner->GetActorRotation();

	//get reference of the world
	UWorld* World = Owner->GetWorld();

	//DoRaycast(World, CurrentLocation.X, CurrentLocation.Y, Length, Width);
	
	//if rotation is between one this ranges, then the location for binary fission is found in a simple way
	if ((CurrentRotation.Yaw >= 0.0f && CurrentRotation.Yaw <= 10.0f) ||
		(CurrentRotation.Yaw <= 0.0f && CurrentRotation.Yaw >= -10.0f) ||
		(CurrentRotation.Yaw >= 80.0f && CurrentRotation.Yaw <= 100.0f) ||
		(CurrentRotation.Yaw <= -80.0f && CurrentRotation.Yaw >= -100.0f) ||
		(CurrentRotation.Yaw >= 170.0f && CurrentRotation.Yaw <= 180.0f) ||
		(CurrentRotation.Yaw <= -170.0f && CurrentRotation.Yaw >= -180.0f)) {

		SimpleFission(CurrentLocation, CurrentRotation, ActorToSpawn, Length, Width, World);

	}
	else { //if rotation is not between the previous ranges, then the location for binary fission
		//is found using some geometrical formulas
		ComplexFission(CurrentLocation, CurrentRotation, ActorToSpawn, Length, Width, World);
	}
	
}

//Spawns a bacterium on the same straight line of the mother cell, for angles in Z axis near to 0°, 90° and 180° 
void UBinaryFission::SimpleFission(FVector CurrentLocation, FRotator CurrentRotation, TSubclassOf<AActor> ActorToSpawn, 
	float Length, float Width, UWorld* World) {

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("EasyFission")));

	if ((CurrentRotation.Yaw >= 0.0f && CurrentRotation.Yaw <= 10.0f) ||
		(CurrentRotation.Yaw <= 0.0f && CurrentRotation.Yaw >= -10.0f) ||
		(CurrentRotation.Yaw >= 170.0f && CurrentRotation.Yaw <= 180.0f) ||
		(CurrentRotation.Yaw <= -170.0f && CurrentRotation.Yaw >= -180.0f)) {

		SimpleFissionOnYAxis(CurrentLocation, CurrentRotation, ActorToSpawn, Length, Width, World);

	}
	else { //CurrentRotation.Yaw != 90 || CurrentRotation.Yaw != 270

		SimpleFissionOnXAxis(CurrentLocation, CurrentRotation, ActorToSpawn, Length, Width, World);

	}

}

void UBinaryFission::SimpleFissionOnYAxis(FVector CurrentLocation, FRotator CurrentRotation, TSubclassOf<AActor> ActorToSpawn,
	float Length, float Width, UWorld* World) {

	float X, Y, Z;
	float Distance = Length;

	X = CurrentLocation.X;
	Z = CurrentLocation.Z + Width / 2;
	Y = CurrentLocation.Y;

	float randomNumber = FMath::RandRange(0.0f, 50.0f);
	int option;
	//position in Y is changed, while X remains constant
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

	//AActor* NewBacteria = World->SpawnActor<AActor>(ActorToSpawn, Location, Rotation, ActorSpawnParameters);
	//bool SpawnStatus = DoSpawn(World, ActorToSpawn, Location, Rotation);
	bool SpawnStatus = DoRaycast(World, ActorToSpawn, Location, Rotation, Length, Width);

	//if bacterium could not be spawned, then try again in the opposite direction
	if (!SpawnStatus) {
		if (option == 1) {
			Y -= Length * 2;
		}
		else {
			Y += Length * 2;
		}
		FVector Location2(X, Y, Z);
		//NewBacteria = World->SpawnActor<AActor>(ActorToSpawn, Location2, Rotation, ActorSpawnParameters);
		//SpawnStatus = DoSpawn(World, ActorToSpawn, Location2, Rotation);
		SpawnStatus = DoRaycast(World, ActorToSpawn, Location2, Rotation, Length, Width);

		//if the spawn fails again, then try a binary fission using the area approach
		if (!SpawnStatus) {
			FissionArea(CurrentLocation, CurrentRotation, ActorToSpawn, World, Length, Width);
		}
	}

}

void UBinaryFission::SimpleFissionOnXAxis(FVector CurrentLocation, FRotator CurrentRotation, TSubclassOf<AActor> ActorToSpawn,
	float Length, float Width, UWorld* World) {

	float X, Y, Z;
	float Distance = Length;

	X = CurrentLocation.X;
	Y = CurrentLocation.Y;
	//Z = CurrentLocation.Z + Width;
	Z = CurrentLocation.Z + Width / 2;

	int option;
	float randomNumber = FMath::RandRange(0.0f, 50.0f);
	//position in X is changed, while Y remains constant
	if (randomNumber >= 0 && randomNumber < 25) {
		X += Length;
		option = 1;
	}
	else {
		X -= Length;
		option = 2;
	}

	FVector Location(X, Y, Z);

	float RotationX = CurrentRotation.Roll;
	float RotationY = CurrentRotation.Pitch;
	float RotationZ = CurrentRotation.Yaw + FMath::RandRange(LOWER_BOUND_ROTATION, UPPER_BOUND_ROTATION);

	//float InPitch, float InYaw, float InRoll
	FRotator Rotation(RotationY, RotationZ, RotationX);

	//AActor* NewBacteria = World->SpawnActor<AActor>(ActorToSpawn, Location, Rotation, ActorSpawnParameters);
	//bool SpawnStatus = DoSpawn(World, ActorToSpawn, Location, Rotation);
	bool SpawnStatus = DoRaycast(World, ActorToSpawn, Location, Rotation, Length, Width);

	//if bacterium could not be spawned, then try again in the opposite direction
	if (!SpawnStatus) {
		if (option == 1) {
			X -= Length * 2;
		}
		else {
			X += Length * 2;
		}
		FVector Location2(X, Y, Z);
		//NewBacteria = World->SpawnActor<AActor>(ActorToSpawn, Location2, Rotation, ActorSpawnParameters);
		//SpawnStatus = DoSpawn(World, ActorToSpawn, Location2, Rotation);
		SpawnStatus = DoRaycast(World, ActorToSpawn, Location2, Rotation, Length, Width);

		//if the spawn fails again, then try a binary fission using the area approach
		if (!SpawnStatus) {
			FissionArea(CurrentLocation, CurrentRotation, ActorToSpawn, World, Length, Width);
		}
	}

}

//Spawns a bacterium using the intersections between a line and a circumference as center for the new bacterium
//for mother cells with rotation angles in Z axis not included in the function SimpleFission 
void UBinaryFission::ComplexFission(FVector CurrentLocation, FRotator CurrentRotation, TSubclassOf<AActor> ActorToSpawn, 
	float Length, float Width, UWorld* World) {

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ComplexFission")));

	float X, Y, Z;

	float X1 = CurrentLocation.X;
	float Y1 = CurrentLocation.Y;

	float X2, Y2;

	//depending of the angle in Z axis, X and Y coordinates change
	//X and Y change according to quadrant in XY-plane
	//functions to compute cos and sin require angle expressed in radians
	if (CurrentRotation.Yaw > 0.0f && CurrentRotation.Yaw < 90.0f) {
		float Angle = CurrentRotation.Yaw * PI / 180;
		Y2 = Y1 - FGenericPlatformMath::Cos(Angle);
		X2 = X1 + FMath::Sin(Angle);
	}
	else {
		if (CurrentRotation.Yaw > 90.0f && CurrentRotation.Yaw < 180.0f) {
			float SupplementaryAngle = 180.0f - CurrentRotation.Yaw;
			float Angle = SupplementaryAngle * PI / 180;
			Y2 = Y1 + FGenericPlatformMath::Cos(Angle);
			X2 = X1 + FMath::Sin(Angle);
		}
		else {
			if (CurrentRotation.Yaw < 0.0f && CurrentRotation.Yaw > -90.0f) {
				float Angle = -CurrentRotation.Yaw * PI / 180;
				Y2 = Y1 - FGenericPlatformMath::Cos(Angle);
				X2 = X1 - FMath::Sin(Angle);
			}
			else { //CurrentRotation.Yaw < -90.0f && CurrentRotation.Yaw > -180.0f				
				float SupplementaryAngle = 180.0f - (-CurrentRotation.Yaw);
				float Angle = SupplementaryAngle * PI / 180;
				Y2 = Y1 + FGenericPlatformMath::Cos(Angle);
				X2 = X1 - FMath::Sin(Angle);
			}
		}
	}

	//formulas to find the equation of the line that traverses the bacterium
	float Slope = (Y2 - Y1) / (X2 - X1);
	float Intercept = Y1 - Slope * X1;

	int option;
	Z = CurrentLocation.Z + Width / 2;
	float randomNumber = FMath::RandRange(0.0f, 50.0f);

	//find X and Y coordinates using intersections between the equation of the line and
	//the circumference centered in (X1, Y1) and radius equal to the length of bacterium
	if (randomNumber >= 0.0f && randomNumber < 25.0f) {
		Y = ComputeY1(Intercept, Slope, X1, Y1, Length);
		X = ComputeX1(Intercept, Slope, X1, Y1, Length);
		option = 1;
	}
	else {
		Y = ComputeY2(Intercept, Slope, X1, Y1, Length);
		X = ComputeX2(Intercept, Slope, X1, Y1, Length);
		option = 2;
	}

	FVector Location(X, Y, Z);	

	float RotationX = CurrentRotation.Roll;
	float RotationY = CurrentRotation.Pitch;
	float RotationZ = CurrentRotation.Yaw + FMath::RandRange(LOWER_BOUND_ROTATION, UPPER_BOUND_ROTATION);

	//float InPitch, float InYaw, float InRoll
	FRotator Rotation(RotationY, RotationZ, RotationX);

	//float AngleX = DoRaycast(World, X, Y, Length, Width);
	//UE_LOG(LogTemp, Warning, TEXT("AngleX: %f"), AngleX);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AngleX: %f"), AngleX));

	bool SpawnStatus = DoRaycast(World, ActorToSpawn, Location, Rotation, Length, Width);

	/*
	AActor* NewBacteria = nullptr;
	if (AngleX != -1000.0f) {
		//NewBacteria = World->SpawnActor<AActor>(ActorToSpawn, Location, Rotation, ActorSpawnParameters);
		SpawnStatus = DoSpawn(World, ActorToSpawn, Location, Rotation);
	}*/

	//if bacterium could not be spawned, then try again using the other (X, Y) solution (the other intersection)
	if (!SpawnStatus) {
		
		if (option == 1) {
			Y = ComputeY2(Intercept, Slope, X1, Y1, Length);
			X = ComputeX2(Intercept, Slope, X1, Y1, Length);
		}
		else {
			Y = ComputeY1(Intercept, Slope, X1, Y1, Length);
			X = ComputeX1(Intercept, Slope, X1, Y1, Length);
		}

		FVector Location2(X, Y, Z);

		/*AngleX = DoRaycast(World, X, Y, Length, Width);
		if (AngleX != -1000.0f) {
			//NewBacteria = World->SpawnActor<AActor>(ActorToSpawn, Location2, Rotation, ActorSpawnParameters);
			SpawnStatus = DoSpawn(World, ActorToSpawn, Location2, Rotation);
		}*/

		SpawnStatus = DoRaycast(World, ActorToSpawn, Location2, Rotation, Length, Width);
		
		//if the spawn fails again, then try a binary fission using the area approach
		if (!SpawnStatus) {
			FissionArea(CurrentLocation, CurrentRotation, ActorToSpawn, World, Length, Width);
		}

	}

}

//Computes the first X solution for intersection between the equation of the line that traverses mother cell
//and the circumference centered in the mother cell centre with radius equal to the length of bacterium
//B is intercept of the equation of the line, Slope is slope of the same equation
//U and V are the (X, Y) coordinates of mother cell center
//R is the radius of the circumference
float UBinaryFission::ComputeX1(float B, float Slope, float U, float V, float R) {
	float X;
	X = (-FGenericPlatformMath::Sqrt(-FGenericPlatformMath::Pow(B, 2) - 2 * B * Slope * U +
		2 * B * V + FGenericPlatformMath::Pow(R, 2) * FGenericPlatformMath::Pow(Slope, 2) +
		FGenericPlatformMath::Pow(R, 2) - FGenericPlatformMath::Pow(Slope, 2) * FGenericPlatformMath::Pow(U, 2)
		+ 2 * Slope * U * V - FGenericPlatformMath::Pow(V, 2))
		- B * Slope + Slope * V + U) / (FGenericPlatformMath::Pow(Slope, 2) + 1);
	return X;
}

//Computes the first Y solution for intersection between the equation of the line that traverses mother cell
//and the circumference centered in the mother cell centre with radius equal to the length of bacterium
//B is intercept of the equation of the line, Slope is slope of the same equation
//U and V are the (X, Y) coordinates of mother cell center
//R is the radius of the circumference
float UBinaryFission::ComputeY1(float B, float Slope, float U, float V, float R) {
	float Y;
	Y = (-Slope * FGenericPlatformMath::Sqrt(-FGenericPlatformMath::Pow(B, 2) - 2 * B * Slope * U
		+ 2 * B * V + FGenericPlatformMath::Pow(R, 2) * FGenericPlatformMath::Pow(Slope, 2)
		+ FGenericPlatformMath::Pow(R, 2) - FGenericPlatformMath::Pow(Slope, 2) * FGenericPlatformMath::Pow(U, 2)
		+ 2 * Slope * U * V - FGenericPlatformMath::Pow(V, 2))
		+ B + FGenericPlatformMath::Pow(Slope, 2) * V + Slope * U) / (FGenericPlatformMath::Pow(Slope, 2) + 1);
	return Y;
}

//Computes the second X solution for intersection between the equation of the line that traverses mother cell
//and the circumference centered in the mother cell centre with radius equal to the length of bacterium
//B is intercept of the equation of the line, Slope is slope of the same equation
//U and V are the (X, Y) coordinates of mother cell center
//R is the radius of the circumference
float UBinaryFission::ComputeX2(float B, float Slope, float U, float V, float R) {
	float X;
	X = (FGenericPlatformMath::Sqrt(-FGenericPlatformMath::Pow(B, 2) - 2 * B * Slope * U + 2 * B * V
		+ FGenericPlatformMath::Pow(R, 2) * FGenericPlatformMath::Pow(Slope, 2) + FGenericPlatformMath::Pow(R, 2)
		- FGenericPlatformMath::Pow(Slope, 2) * FGenericPlatformMath::Pow(U, 2) + 2 * Slope * U * V
		- FGenericPlatformMath::Pow(V, 2)) - B * Slope + Slope * V + U) / (FGenericPlatformMath::Pow(Slope, 2) + 1);
	return X;
}

//Computes the second Y solution for intersection between the equation of the line that traverses mother cell
//and the circumference centered in the mother cell centre with radius equal to the length of bacterium
//B is intercept of the equation of the line, Slope is slope of the same equation
//U and V are the (X, Y) coordinates of mother cell center
//R is the radius of the circumference
float UBinaryFission::ComputeY2(float B, float Slope, float U, float V, float R) {
	float Y;
	Y = (Slope * FGenericPlatformMath::Sqrt(-FGenericPlatformMath::Pow(B, 2) - 2 * B * Slope * U
		+ 2 * B * V + FGenericPlatformMath::Pow(R, 2) * FGenericPlatformMath::Pow(Slope, 2)
		+ FGenericPlatformMath::Pow(R, 2) - FGenericPlatformMath::Pow(Slope, 2) * FGenericPlatformMath::Pow(U, 2)
		+ 2 * Slope * U * V - FGenericPlatformMath::Pow(V, 2)) + B + FGenericPlatformMath::Pow(Slope, 2) * V
		+ Slope * U) / (FGenericPlatformMath::Pow(Slope, 2) + 1);
	return Y;
}

//Spawns a bacterium within a sorrounding area to the mother cell
void UBinaryFission::FissionArea(FVector CurrentLocation, FRotator CurrentRotation, TSubclassOf<AActor> ActorToSpawn, 
	UWorld* World, float Length, float Width) {

	float UPPER_BOUND = Length;
	float LOWER_BOUND = -Length;

	float UPPER_ACCEPTABLE_BOUND = Width;
	float LOWER_ACCEPTABLE_BOUND = -Width;

	//Ranges are established for changes in X and Y coordinates

	float TempX = FMath::RandRange(LOWER_BOUND, UPPER_BOUND);
	while (TempX == 0) {
		TempX = FMath::RandRange(LOWER_BOUND, UPPER_BOUND);
	}
	if (TempX >= LOWER_ACCEPTABLE_BOUND && TempX <= UPPER_ACCEPTABLE_BOUND) {
		if (TempX >= LOWER_ACCEPTABLE_BOUND && TempX <0) {
			TempX = FMath::RandRange(LOWER_BOUND, LOWER_ACCEPTABLE_BOUND+1);
		}
		else {
			TempX = FMath::RandRange(UPPER_ACCEPTABLE_BOUND+1, UPPER_BOUND);
		}
	}
	float XPoint = CurrentLocation.X + TempX;

	float TempY = FMath::RandRange(LOWER_BOUND, UPPER_BOUND);
	while (TempY == 0) {
		TempY = FMath::RandRange(LOWER_BOUND, UPPER_BOUND);
	}
	if (TempY >= LOWER_ACCEPTABLE_BOUND && TempX <= UPPER_ACCEPTABLE_BOUND) {
		if (TempY >= LOWER_ACCEPTABLE_BOUND && TempY < 0) {
			TempY = FMath::RandRange(LOWER_BOUND, LOWER_ACCEPTABLE_BOUND + 1);
		}
		else {
			TempY = FMath::RandRange(UPPER_ACCEPTABLE_BOUND + 1, UPPER_BOUND);
		}
	}
	float YPoint = CurrentLocation.Y + TempY;

	//float TempZ = FMath::RandRange(0.0f, Width+Width*(1/8));
	//float ZPoint = CurrentLocation.Z + TempZ;
	float ZPoint = CurrentLocation.Z + Width / 2;
	//UE_LOG(LogTemp, Warning, TEXT("ZPoint: %f"), ZPoint);

	FVector Location(XPoint, YPoint, ZPoint);

	float RotationX = CurrentRotation.Roll;
	float RotationY = CurrentRotation.Pitch;
	float RotationZ = CurrentRotation.Yaw + FMath::RandRange(LOWER_BOUND_ROTATION, UPPER_BOUND_ROTATION);

	//float InPitch, float InYaw, float InRoll
	FRotator Rotation(RotationY, RotationZ, RotationX);

	FActorSpawnParameters SpawnInfo;

	FActorSpawnParameters ActorSpawnParameters;

	DoRaycast(World, ActorToSpawn, Location, Rotation, Length, Width);
	/*
	if (AngleX != -1000.0f) {
		//AActor* NewBacteria = World->SpawnActor<AActor>(ActorToSpawn, Location, Rotation, ActorSpawnParameters);
		DoSpawn(World, ActorToSpawn, Location, Rotation);
	}
	*/
}

bool UBinaryFission::DoRaycast(UWorld* World, TSubclassOf<AActor> ActorToSpawn, FVector Location, FRotator Rotation, float Length, float Width) {

	float X1, X2;
	float Y1, Y2;
	float ZSuperiorBond, ZInferiorBond;

	float X = Location.X;
	float Y = Location.Y;
	float Z = Location.Z;

	X1 = X + Width / 2.5;
	X2 = X - Width / 2.5;
	Y1 = Y + Width / 2.5;
	Y2 = Y - Width / 2.5;
	ZSuperiorBond = 50.0f;
	ZInferiorBond = Width / 2;

	FVector NewLocation0(X, Y, ZSuperiorBond);
	FVector End0(X, Y, ZInferiorBond);
	FHitResult Hit0;
	bool HitSuccess0 = World->LineTraceSingleByChannel(Hit0, NewLocation0, End0, ECollisionChannel::ECC_GameTraceChannel1);
	//DrawDebugLine(World, NewLocation0, End0, FColor(255, 255, 255), true, -1.f, 0, 0.3);

	FVector NewLocation1(X, Y1, ZSuperiorBond);
	FVector End1(X, Y1, ZInferiorBond);
	FHitResult Hit1;
	bool HitSuccess1 = World->LineTraceSingleByChannel(Hit1, NewLocation1, End1, ECollisionChannel::ECC_GameTraceChannel1);
	//DrawDebugLine(World, NewLocation1, End1, FColor(255, 255, 255), true, -1.f, 0, 0.3);

	FVector NewLocation2(X, Y2, ZSuperiorBond);
	FVector End2(X, Y2, ZInferiorBond);
	FHitResult Hit2;
	bool HitSuccess2 = World->LineTraceSingleByChannel(Hit2, NewLocation2, End2, ECollisionChannel::ECC_GameTraceChannel1);
	//DrawDebugLine(World, NewLocation2, End2, FColor(0, 255, 255), true, -1.f, 0, 0.3);

	FVector NewLocation3(X1, Y, ZSuperiorBond);
	FVector End3(X1, Y, ZInferiorBond);
	FHitResult Hit3;
	bool HitSuccess3 = World->LineTraceSingleByChannel(Hit3, NewLocation3, End3, ECollisionChannel::ECC_GameTraceChannel1);
	//DrawDebugLine(World, NewLocation3, End3, FColor(255, 0, 0), true, -1.f, 0, 0.3);

	FVector NewLocation4(X2, Y, ZSuperiorBond);
	FVector End4(X2, Y, ZInferiorBond);
	FHitResult Hit4;
	bool HitSuccess4 = World->LineTraceSingleByChannel(Hit4, NewLocation4, End4, ECollisionChannel::ECC_GameTraceChannel1);
	//DrawDebugLine(World, NewLocation4, End4, FColor(0, 0, 255), true, -1.f, 0, 0.3);

	if (HitSuccess0 && HitSuccess1 && HitSuccess2 && HitSuccess3 && HitSuccess4) {

		if (Hit0.Actor->GetName().Contains(TEXT("Bacteria")) ) {
			return false;
		}

		//FVector NewLocation(X, Y, Hit0.Location.Z + Width/1.8);
		FVector NewLocation(X, Y, Hit0.Location.Z + Width / 1.2);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hit: %s"), *Hit0.Actor->GetName()));
		if (Hit1.Location.Z == Hit2.Location.Z && Hit3.Location.Z == Hit4.Location.Z) {
			return DoSpawn(World, ActorToSpawn, Location, Rotation);
		}
		else {
			float DifferenceZ1 = UKismetMathLibrary::Abs(Hit1.Location.Z - Hit2.Location.Z);
			float DifferenceZ2 = UKismetMathLibrary::Abs(Hit3.Location.Z - Hit4.Location.Z);
			//if (DifferenceZ1 != 0 && DifferenceZ2 == 0) {
			if (DifferenceZ1 != 0) {
				if (Y1 > Y2) { //negative angle
					float AngleRadians = UKismetMathLibrary::Atan(DifferenceZ1 / (Width / 2.5));
					float AngleDegrees = AngleRadians * 180 / PI;
					
					float RotationX = -AngleDegrees;

					//float InPitch, float InYaw, float InRoll
					FRotator NewRotation(Rotation.Pitch, Rotation.Yaw, RotationX);

					return DoSpawn(World, ActorToSpawn, NewLocation, NewRotation);
				}
				else { //positive angle
					float AngleRadians = UKismetMathLibrary::Atan(DifferenceZ1 / (Width / 2.5));
					float AngleDegrees = AngleRadians * 180 / PI;
					
					float RotationX = AngleDegrees;

					//float InPitch, float InYaw, float InRoll
					FRotator NewRotation(Rotation.Pitch, Rotation.Yaw, RotationX);

					return DoSpawn(World, ActorToSpawn, NewLocation, NewRotation);

				}
			}
			else {
				return DoSpawn(World, ActorToSpawn, NewLocation, Rotation);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Here")));
				
				if (DifferenceZ1 == 0 && DifferenceZ2 != 0) {
					//return 0.0f;
				}
			}
		}
	}

	return DoSpawn(World, ActorToSpawn, Location, Rotation);
}

bool UBinaryFission::DoSpawn(UWorld* World, TSubclassOf<AActor> ActorToSpawn, FVector Location, FRotator Rotation) {

	FActorSpawnParameters ActorSpawnParameters;
	AActor* NewBacteria = World->SpawnActor<AActor>(ActorToSpawn, Location, Rotation, ActorSpawnParameters);
	if (!IsValid(NewBacteria)) {
		return false;
	}
	else {
		return true;
	}

}
// Fill out your copyright notice in the Description page of Project Settings.


#include "BinaryFission.h"
#include "Kismet/KismetMathLibrary.h"
#include "GenericPlatform/GenericPlatformMath.h"

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

int UBinaryFission::SelectBinaryFissionMethod(FRotator CurrentRotation) {

	if ((CurrentRotation.Yaw >= 0.0f && CurrentRotation.Yaw <= 10.0f) ||
		(CurrentRotation.Yaw <= 0.0f && CurrentRotation.Yaw >= -10.0f) ||
		(CurrentRotation.Yaw >= 80.0f && CurrentRotation.Yaw <= 100.0f) ||
		(CurrentRotation.Yaw <= -80.0f && CurrentRotation.Yaw >= -100.0f) ||
		(CurrentRotation.Yaw >= 170.0f && CurrentRotation.Yaw <= 180.0f) ||
		(CurrentRotation.Yaw <= -170.0f && CurrentRotation.Yaw >= -180.0f)) {

		return 1; // simple fission

	}
	else { //if rotation is not between the previous ranges, then the location for binary fission
		//is found using some geometrical formulas
		return 2; // complex fission
	}
}

int UBinaryFission::SelectSimpleFissionWay(FRotator CurrentRotation) {

	if ((CurrentRotation.Yaw >= 0.0f && CurrentRotation.Yaw <= 10.0f) ||
		(CurrentRotation.Yaw <= 0.0f && CurrentRotation.Yaw >= -10.0f) ||
		(CurrentRotation.Yaw >= 170.0f && CurrentRotation.Yaw <= 180.0f) ||
		(CurrentRotation.Yaw <= -170.0f && CurrentRotation.Yaw >= -180.0f)) {

		return 1; //on Y axis
	}
	else { //CurrentRotation.Yaw != 90 || CurrentRotation.Yaw != 270

		return 2; //on X axis
	}
	
}

FVector UBinaryFission::GetCoordinatesFissionOnYAxis(FVector CurrentLocation, float Length, bool choice) {
	float X, Y, Z;
	X = CurrentLocation.X;
	Y = CurrentLocation.Y;
	Z = CurrentLocation.Z;
	if (choice) {
		Y += Length;
	}
	else {
		Y -= Length;
	}
	FVector Location(X, Y, Z);
	return Location;
}

FVector UBinaryFission::GetCoordinatesFissionOnXAxis(FVector CurrentLocation, float Length, bool choice) {
	float X, Y, Z;
	X = CurrentLocation.X;
	Y = CurrentLocation.Y;
	Z = CurrentLocation.Z;
	if (choice) {
		X += Length;
	}
	else {
		X -= Length;
	}
	FVector Location(X, Y, Z);
	return Location;
}

void UBinaryFission::GetEquationLine(FVector CurrentLocation, FRotator CurrentRotation, float &Slope, 
	float &Intercept, float Pi) {

	float X1 = CurrentLocation.X;
	float Y1 = CurrentLocation.Y;

	float X2, Y2;

	//depending of the angle in Z axis, X and Y coordinates change
	//X and Y change according to quadrant in XY-plane
	//functions to compute cos and sin require angle expressed in radians
	if (CurrentRotation.Yaw > 0.0f && CurrentRotation.Yaw < 90.0f) {
		float Angle = CurrentRotation.Yaw * Pi / 180;
		Y2 = Y1 - FGenericPlatformMath::Cos(Angle);
		X2 = X1 + FMath::Sin(Angle);
	}
	else {
		if (CurrentRotation.Yaw > 90.0f && CurrentRotation.Yaw < 180.0f) {
			float SupplementaryAngle = 180.0f - CurrentRotation.Yaw;
			float Angle = SupplementaryAngle * Pi / 180;
			Y2 = Y1 + FGenericPlatformMath::Cos(Angle);
			X2 = X1 + FMath::Sin(Angle);
		}
		else {
			if (CurrentRotation.Yaw < 0.0f && CurrentRotation.Yaw > -90.0f) {
				float Angle = -CurrentRotation.Yaw * Pi / 180;
				Y2 = Y1 - FGenericPlatformMath::Cos(Angle);
				X2 = X1 - FMath::Sin(Angle);
			}
			else { //CurrentRotation.Yaw < -90.0f && CurrentRotation.Yaw > -180.0f				
				float SupplementaryAngle = 180.0f - (-CurrentRotation.Yaw);
				float Angle = SupplementaryAngle * Pi / 180;
				Y2 = Y1 + FGenericPlatformMath::Cos(Angle);
				X2 = X1 - FMath::Sin(Angle);
			}
		}
	}

	//formulas to find the equation of the line that traverses the bacterium
	Slope = (Y2 - Y1) / (X2 - X1);
	Intercept = Y1 - Slope * X1;

}

FVector UBinaryFission::GetIntersectionSolution(FVector CurrentLocation, float Intercept, float Slope, 
	float Length, bool choice) {

	float X, Y, Z;

	float X1 = CurrentLocation.X;
	float Y1 = CurrentLocation.Y;

	if (choice) {		
		X = ComputeX1(Intercept, Slope, X1, Y1, Length);
		Y = ComputeY1(Intercept, Slope, X1, Y1, Length);
	}
	else {		
		X = ComputeX2(Intercept, Slope, X1, Y1, Length);
		Y = ComputeY2(Intercept, Slope, X1, Y1, Length);
	}

	Z = CurrentLocation.Z;

	FVector Location(X, Y, Z);
	return Location;
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

FVector UBinaryFission::GetVectorFissionArea(FVector CurrentLocation, float Length, float Width) {

	float UPPER_BOUND = Length;
	float LOWER_BOUND = -Length;

	float UPPER_ACCEPTABLE_BOUND = Width;
	float LOWER_ACCEPTABLE_BOUND = -Width;

	float TempX = FMath::RandRange(LOWER_BOUND, UPPER_BOUND);
	while (TempX == 0) {
		TempX = FMath::RandRange(LOWER_BOUND, UPPER_BOUND);
	}
	if (TempX >= LOWER_ACCEPTABLE_BOUND && TempX <= UPPER_ACCEPTABLE_BOUND) {
		if (TempX >= LOWER_ACCEPTABLE_BOUND && TempX < 0) {
			TempX = FMath::RandRange(LOWER_BOUND, LOWER_ACCEPTABLE_BOUND + 1);
		}
		else {
			TempX = FMath::RandRange(UPPER_ACCEPTABLE_BOUND + 1, UPPER_BOUND);
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

	float ZPoint = CurrentLocation.Z + Width / 2;

	FVector Location(XPoint, YPoint, ZPoint);

	return Location;

}
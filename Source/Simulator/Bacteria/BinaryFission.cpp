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

//Returns the binary fission type to try according to the angle of bacterium on Z axis
//For angles near to 0°, 90° and 180° the position to spawn is easily calculated using the simple fission
//Otherwise the complex fission is used
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

//Returns the simple binary fission way to try according to the angle of bacterium on Z axis
//For angles near to 0° and 180° the position requires the change of Y coordinate only
//Otherwise the change of X coordinate only is required
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

//Returns the coordinates to try spawn, with Y coordinate varying and X constant (if option is 1)
//or X coordinate varying and Y constant (if option is 2)
FVector UBinaryFission::GetCoordinatesSimpleFission(FVector CurrentLocation, float Length, bool Choice, int Option) {
	
	float X, Y, Z;
	X = CurrentLocation.X;
	Y = CurrentLocation.Y;
	Z = CurrentLocation.Z;

	if (Option == 1) { //on Y axis
		if (Choice) {
			Y += Length;
		}
		else {
			Y -= Length;
		}
	}
	else { //on X axis
		if (Choice) {
			X += Length;
		}
		else {
			X -= Length;
		}
	}
	
	FVector Location(X, Y, Z);
	return Location;
}

//Returns (by reference) the slope and intercept of equation of the line that traverses the bacterium 
//through the center in the XY plane
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

//Gets an intersection solution in the XY plane for the intersection between the equation of the line
//that traverses through the center of bacterium and the the circumference centered in the mother cell 
//centre with radius equal to the length of bacterium
FVector UBinaryFission::GetIntersectionSolution(FVector CurrentLocation, float Intercept, float Slope, 
	float Length, bool Choice) {

	float X, Y, Z;

	float X1 = CurrentLocation.X;
	float Y1 = CurrentLocation.Y;

	if (Choice) {		
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

//Gets a random location to spawn which is located within the inmediate surrounding area of bacterium
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

	float ZPoint = CurrentLocation.Z;

	FVector Location(XPoint, YPoint, ZPoint);

	return Location;

}

FVector UBinaryFission::GetVectorEasyFission(FVector CurrentLocation, float Length) {
	int LowerBound = 0;
	int UpperBound = 7;
	int X, Y;
	int RandomValue = FMath::RandRange(LowerBound, UpperBound);
	switch (RandomValue) {
	case 0:
		X = CurrentLocation.X - Length;
		Y = CurrentLocation.Y + Length;
		break;
	case 1:
		X = CurrentLocation.X;
		Y = CurrentLocation.Y + Length;
		break;
	case 2:
		X = CurrentLocation.X + Length;
		Y = CurrentLocation.Y + Length;
		break;
	case 3:
		X = CurrentLocation.X - Length;
		Y = CurrentLocation.Y;
		break;
	case 4:
		X = CurrentLocation.X + Length;
		Y = CurrentLocation.Y;
		break;
	case 5:
		X = CurrentLocation.X - Length;
		Y = CurrentLocation.Y - Length;
		break;
	case 6:
		X = CurrentLocation.X;
		Y = CurrentLocation.Y - Length;
		break;
	case 7:
		X = CurrentLocation.X + Length;
		Y = CurrentLocation.Y - Length;
		break;
	}
	FVector Location(X, Y, CurrentLocation.Z);
	return Location;
}
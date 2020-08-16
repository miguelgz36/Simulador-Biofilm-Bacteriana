// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumeNutrients.h"

// Sets default values for this component's properties
UConsumeNutrients::UConsumeNutrients()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UConsumeNutrients::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UConsumeNutrients::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UConsumeNutrients::GetNutrients(float AvailableNutrients, float EnergyLevel) {

	if (AvailableNutrients > 0.5) {
		return EnergyLevel + 0.5;
	}
	else {
		return EnergyLevel;
	}

}


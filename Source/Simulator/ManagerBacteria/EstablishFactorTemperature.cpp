// Fill out your copyright notice in the Description page of Project Settings.


#include "EstablishFactorTemperature.h"

// Sets default values for this component's properties
UEstablishFactorTemperature::UEstablishFactorTemperature()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEstablishFactorTemperature::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEstablishFactorTemperature::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//Computes the factor temperature following an average and simplistic mesophile behaviour
float UEstablishFactorTemperature::ComputeFactorTemperature(float TemperatureEnvironment)
{

	float FactorTemperature;

	if (TemperatureEnvironment < 10.0f) {
		FactorTemperature = 0.0f;
	}
	else {
		if (TemperatureEnvironment >= 10.0f && TemperatureEnvironment < 20.0f) {
			FactorTemperature = 0.2f;
		}
		else {
			if (TemperatureEnvironment >= 20.0f && TemperatureEnvironment < 25.0f) {
				FactorTemperature = 0.4f;
			}
			else {
				if (TemperatureEnvironment >= 25.0f && TemperatureEnvironment < 30.0f) {
					FactorTemperature = 0.6f;
				}
				else {
					if (TemperatureEnvironment >= 30.0f && TemperatureEnvironment <= 37.0f) {
						FactorTemperature = TemperatureEnvironment/37.0f;
					}
					else {
						if (TemperatureEnvironment > 37.0f && TemperatureEnvironment <= 40.0f) {
							FactorTemperature = 1.0f - ((TemperatureEnvironment-37.0f) / 37.0f);
						}
						else {
							if (TemperatureEnvironment > 40.0f && TemperatureEnvironment < 41.0f) {
								FactorTemperature = 0.7f;
							}
							else {
								if (TemperatureEnvironment >= 41.0f && TemperatureEnvironment < 42.0f) {
									FactorTemperature = 0.5f;
								}
								else {
									if (TemperatureEnvironment >= 42.0f && TemperatureEnvironment < 43.0f) {
										FactorTemperature = 0.3f;
									}
									else {
										if (TemperatureEnvironment >= 43.0f && TemperatureEnvironment < 44.0f) {
											FactorTemperature = 0.1f;
										}
										else { //TemperatureEnvironment greater or equal than 44
											FactorTemperature = 0.0f;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	return FactorTemperature;

}


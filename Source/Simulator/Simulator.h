// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Simulator.generated.h"

USTRUCT(BlueprintType)
struct FS_SimulationDataPerTimeAux
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, Category = Variables)
		int NumberTicks;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		int NumberBacteria;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		float AreaBiofilm;
};

USTRUCT(BlueprintType)
struct FS_SimulationConfiguration
{
	GENERATED_USTRUCT_BODY();

	//basic parameters

	UPROPERTY(BlueprintReadWrite, Category = Variables)
		FString Bacteria;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		FString Surface;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		float FactorNutrientUptakePerSize;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		bool DoPresimulation;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		int PresimulationGranularity;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		int BlocksPerSide;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		float Temperature;

	//advanced parameters

	UPROPERTY(BlueprintReadWrite, Category = Variables)
		float InitialEnergyLevel;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		float MaximumEnergyLevel;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		float NutrientLevelThresholdToMove;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		float MinimumEnergyLevel;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		float EnergySpentToMove;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		float EnergySpentToSurvive;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		int MaximumMovementAttempts;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		float MinimumNutrientLevel;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		float UnitsRadiusToMove;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		int InitialNumberObjectPool;

	friend FArchive& operator<<(FArchive& Archive, FS_SimulationConfiguration& Configuration)
	{
		Archive << Configuration.Bacteria;
		Archive << Configuration.Surface;		
		Archive << Configuration.FactorNutrientUptakePerSize;
		Archive << Configuration.DoPresimulation;
		Archive << Configuration.PresimulationGranularity;
		Archive << Configuration.BlocksPerSide;
		Archive << Configuration.Temperature;

		Archive << Configuration.InitialEnergyLevel;
		Archive << Configuration.MaximumEnergyLevel;
		Archive << Configuration.NutrientLevelThresholdToMove;
		Archive << Configuration.MinimumEnergyLevel;
		Archive << Configuration.EnergySpentToMove;
		Archive << Configuration.EnergySpentToSurvive;
		Archive << Configuration.MaximumMovementAttempts;
		Archive << Configuration.MinimumNutrientLevel;
		Archive << Configuration.UnitsRadiusToMove;
		Archive << Configuration.InitialNumberObjectPool;

		return Archive;
	}
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"

#include "../Simulator.h"

#include "BinaryFileManager.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATOR_API UBinaryFileManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	//Save and load configuration binary file

	UFUNCTION(BlueprintCallable, Category = "BinaryFile", meta = (Keywords = "Save"))
		static bool SaveConfigurationFile(FString SaveDirectory, FString FileName,
			FS_SimulationConfiguration Configuration, bool AllowOverWriting);

	UFUNCTION(BlueprintCallable, Category = "BinaryFile", meta = (Keywords = "Save"))
		static bool LoadConfigurationFile(FString SaveDirectory, FString FileName,
			FS_SimulationConfiguration& Configuration);
	
};

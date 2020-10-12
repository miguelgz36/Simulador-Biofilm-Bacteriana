// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Templates/SharedPointer.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Containers/Array.h"
#include "JsonObjectConverter.h"
#include "JsonObjectWrapper.h"

#include "../Simulator.h"

#include "TextFileManager.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATOR_API UTextFileManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "TextFile", meta = (Keywords = "Save"))
		static bool SaveArrayStringsToFile(FString SaveDirectory, FString FileName, TArray<FString> ArrayStrings,
			bool AllowOverWriting);

	UFUNCTION(BlueprintCallable, Category = "TextFile", meta = (Keywords = "Save"))
		static FString SimulationDataToJsonString(FS_SimulationConfigurationAux SimulationConfiguration, 
			TArray<FS_SimulationDataPerTimeAux> ArraySimulationDataPerTime);

	UFUNCTION(BlueprintCallable, Category = "TextFile", meta = (Keywords = "Save"))
		static bool SaveStringToFile(FString SaveDirectory, FString FileName, FString StringToSave,
			bool AllowOverWriting);

};

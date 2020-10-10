// Fill out your copyright notice in the Description page of Project Settings.


#include "TextFileManager.h"

bool UTextFileManager::SaveArrayStringsToFile(FString SaveDirectory, FString FileName, TArray<FString> ArrayStrings,
	bool AllowOverWriting = false)
{

	//Set complete file path
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if (!AllowOverWriting) {

		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory)) {
			return false;
		}

	}

	return FFileHelper::SaveStringArrayToFile(ArrayStrings, *SaveDirectory);
}

FString UTextFileManager::ArrayBacteriaPerTimeToJsonString(FS_SimulationConfiguration Configuration, TArray<FS_BacteriaAreaPerTime> StructBacteriaPerTime)
{
	TSharedPtr< FJsonObject > JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetStringField("Surface", Configuration.SurfaceType);
	JsonObject->SetStringField("Bacteria", Configuration.BacteriaType);
	JsonObject->SetNumberField("FactorNutrientUptakePerSize", Configuration.FactorNutrientUptakePerSize);
	JsonObject->SetNumberField("Temperature", Configuration.Temperature);

	TArray< TSharedPtr<FJsonValue> > JsonValueArray;
	TSharedPtr< FJsonObject > JsonObjectInside;

	for (FS_BacteriaAreaPerTime& Each : StructBacteriaPerTime)
	{
		JsonObjectInside = MakeShareable(new FJsonObject);

		JsonObjectInside = MakeShareable(new FJsonObject);
		JsonObjectInside->SetNumberField("Time", Each.NumberTicks);
		JsonObjectInside->SetNumberField("NumberBacteria", Each.NumberBacteria);
		JsonObjectInside->SetNumberField("AreaBiofilm", Each.TotalArea);

		TSharedRef< FJsonValueObject > JsonValue = MakeShareable(new FJsonValueObject(JsonObjectInside));
		JsonValueArray.Add(JsonValue);
	}

	JsonObject->SetArrayField("SimulationData", JsonValueArray);

	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *OutputString);
	return OutputString;
}

bool UTextFileManager::SaveStringToFile(FString SaveDirectory, FString FileName, FString StringToSave,
	bool AllowOverWriting = false)
{

	//Set complete file path
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if (!AllowOverWriting) {

		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory)) {
			return false;
		}

	}

	return FFileHelper::SaveStringToFile(StringToSave, *SaveDirectory);
}

bool UTextFileManager::SaveConfigurationFile(FString SaveDirectory, FString FileName, FS_SimulationConfiguration Configuration,
	bool AllowOverWriting = false)
{

	//Set complete file path
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if (!AllowOverWriting) {

		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory)) {
			return false;
		}

	}

	TSharedPtr< FJsonObject > JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetStringField("Surface", Configuration.SurfaceType);
	JsonObject->SetStringField("Bacteria", Configuration.BacteriaType);
	JsonObject->SetNumberField("FactorNutrientUptakePerSize", Configuration.FactorNutrientUptakePerSize);
	JsonObject->SetNumberField("Temperature", Configuration.Temperature);

	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return FFileHelper::SaveStringToFile(OutputString, *SaveDirectory);
}

FS_SimulationConfiguration UTextFileManager::LoadConfigurationFile(FString SaveDirectory, FString FileName) {

	//Set complete file path
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	FS_SimulationConfiguration Configuration;
	Configuration.SurfaceType = "";

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory)) {
		return Configuration;
	}

	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *SaveDirectory);

	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(JsonString);

	TSharedPtr< FJsonObject > JsonObject = MakeShareable(new FJsonObject);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		Configuration.SurfaceType = JsonObject->GetStringField("Surface");
		Configuration.BacteriaType = JsonObject->GetStringField("Bacteria");
		Configuration.FactorNutrientUptakePerSize = JsonObject->GetNumberField("FactorNutrientUptakePerSize");
		Configuration.Temperature = JsonObject->GetNumberField("Temperature");
	}

	return Configuration;
}
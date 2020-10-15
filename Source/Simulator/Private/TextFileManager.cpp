// Fill out your copyright notice in the Description page of Project Settings.


#include "TextFileManager.h"

//Saves an array of string to a text file
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

//Returns a JSON string using a FJsonObject build with the specified SimulationConfiguration and SimulationData
FString UTextFileManager::SimulationDataToJsonString(FS_SimulationConfigurationAux SimulationConfiguration,
	TArray<FS_SimulationDataPerTimeAux> ArraySimulationDataPerTime)
{
	TSharedPtr< FJsonObject > GeneralJsonObject = MakeShareable(new FJsonObject);

	TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	FJsonObjectConverter::UStructToJsonObject(FS_SimulationConfigurationAux::StaticStruct(), &SimulationConfiguration, JsonObject, 0, 0);

	GeneralJsonObject->SetField(TEXT("simulationConfiguration"), MakeShareable(new FJsonValueObject(JsonObject)));

	TArray< TSharedPtr<FJsonValue> > JsonValueArray;

	for (FS_SimulationDataPerTimeAux& Each : ArraySimulationDataPerTime)
	{
		TSharedRef<FJsonObject> JsonObjectItem = MakeShareable(new FJsonObject);
		FJsonObjectConverter::UStructToJsonObject(FS_SimulationDataPerTimeAux::StaticStruct(), &Each, JsonObjectItem, 0, 0);

		TSharedRef< FJsonValueObject > JsonValue = MakeShareable(new FJsonValueObject(JsonObjectItem));
		JsonValueArray.Add(JsonValue);
	}

	GeneralJsonObject->SetArrayField("simulationData", JsonValueArray);

	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(GeneralJsonObject.ToSharedRef(), Writer);

	return OutputString;
}

//Saves a string to a text file
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
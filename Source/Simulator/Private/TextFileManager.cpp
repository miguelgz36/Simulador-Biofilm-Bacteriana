// Fill out your copyright notice in the Description page of Project Settings.


#include "TextFileManager.h"

//Saves an array of string to a text file. The boolean result of operation is returned.
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

//Creates a string in JSON format using a FJsonObject build with the specified configuration and data from simulation.
//The boolean result of operation is returned. String is returned using the variable OutputString.
bool UTextFileManager::SimulationDataToJsonString(FS_SimulationConfigurationAux SimulationConfiguration,
	TArray<FS_SimulationDataPerTimeAux> ArraySimulationDataPerTime, FString& OutputString)
{
	//Create JsonObject that is going to contain the whole JSON structure
	TSharedPtr< FJsonObject > GeneralJsonObject = MakeShareable(new FJsonObject);

	//Convert SimulationConfiguration to JSON format and save it in JsonObject
	TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	FJsonObjectConverter::UStructToJsonObject(FS_SimulationConfigurationAux::StaticStruct(), &SimulationConfiguration, JsonObject, 0, 0);

	//Add item "simulationConfiguration" to GeneralJsonObject 
	GeneralJsonObject->SetField(TEXT("simulationConfiguration"), MakeShareable(new FJsonValueObject(JsonObject)));

	//Create array of JsonValue to save each data row of ArraySimulationDataPerTime
	TArray< TSharedPtr<FJsonValue> > JsonValueArray;

	TSharedRef<FJsonObject> JsonObjectItem = MakeShareable(new FJsonObject);
	TSharedRef< FJsonValueObject > JsonValue = MakeShareable(new FJsonValueObject(JsonObjectItem));

	for (FS_SimulationDataPerTimeAux& Each : ArraySimulationDataPerTime)
	{
		//Convert a row in ArraySimulationDataPerTime to JSON format and save it in JsonObjectItem
		JsonObjectItem = MakeShareable(new FJsonObject);
		FJsonObjectConverter::UStructToJsonObject(FS_SimulationDataPerTimeAux::StaticStruct(), &Each, JsonObjectItem, 0, 0);

		//Save JsonObjectItem as JsonValue and add this to the array
		JsonValue = MakeShareable(new FJsonValueObject(JsonObjectItem));
		JsonValueArray.Add(JsonValue);
	}

	//Add item "simulationData" to GeneralJsonObject. 
	//This item contains the values of each row in ArraySimulationDataPerTime
	GeneralJsonObject->SetArrayField("simulationData", JsonValueArray);

	//Convert GeneralJsonObject to string and save it in OutputString
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);	

	bool Result = FJsonSerializer::Serialize(GeneralJsonObject.ToSharedRef(), Writer);

	Writer->Close();

	return Result;
}

//Saves a string to a text file. The boolean result of operation is returned.
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

bool UTextFileManager::BacteriaPositionsToJsonString(TArray<FString> BacteriaPositionsPerTime, FString& OutputString)
{
	TSharedPtr< FJsonObject > GeneralJsonObject = MakeShareable(new FJsonObject);

	//Create array of JsonValue to save each data row of ArraySimulationDataPerTime
	TArray< TSharedPtr<FJsonValue> > JsonValueArray;

	TSharedRef<FJsonObject> JsonObjectItem = MakeShareable(new FJsonObject);
	TSharedRef< FJsonValueObject > JsonValue = MakeShareable(new FJsonValueObject(JsonObjectItem));

	TArray<FString> ElementsEachLine;
	for (FString& Each : BacteriaPositionsPerTime)
	{
		Each.ParseIntoArray(ElementsEachLine, TEXT(","), true);

		//Convert a row in ArraySimulationDataPerTime to JSON format and save it in JsonObjectItem
		JsonObjectItem = MakeShareable(new FJsonObject);

		JsonObjectItem->SetNumberField("numberTicks", FCString::Atof(*ElementsEachLine[0]));
		JsonObjectItem->SetNumberField("positionX", FCString::Atof(*ElementsEachLine[1]));
		JsonObjectItem->SetNumberField("positionY", FCString::Atof(*ElementsEachLine[2]));
		JsonObjectItem->SetNumberField("positionZ", FCString::Atof(*ElementsEachLine[3]));

		//Save JsonObjectItem as JsonValue and add this to the array
		JsonValue = MakeShareable(new FJsonValueObject(JsonObjectItem));
		JsonValueArray.Add(JsonValue);
	}

	//Add item "simulationData" to GeneralJsonObject. 
	//This item contains the values of each row in ArraySimulationDataPerTime
	GeneralJsonObject->SetArrayField("bacteriaPositionsPerTime", JsonValueArray);

	//Convert GeneralJsonObject to string and save it in OutputString
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);

	bool Result = FJsonSerializer::Serialize(GeneralJsonObject.ToSharedRef(), Writer);

	Writer->Close();

	return Result;
}
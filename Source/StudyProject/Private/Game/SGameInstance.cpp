// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameInstance.h"
#include "SUnrealObjectClass.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Examples/SFlyable.h"
#include "Examples/SPigeon.h"
#include "JsonObjectConverter.h"
#include "UObject/SavePackage.h"

USGameInstance::USGameInstance()
{
	Name = TEXT("USGameInstance Class Default Object");
}

void USGameInstance::Init()
{
	Super::Init();

	FBirdData SrcRawData(TEXT("Pigeon17"), 17);
	UE_LOG(LogTemp, Log, TEXT("SrcRawDataName: %s SrcRawDataID: %d"), *SrcRawData.Name, SrcRawData.ID);

	const FString SaveDir = FPaths::Combine(FPlatformMisc::ProjectDir(), TEXT("Saved"));
	UE_LOG(LogTemp, Log, TEXT("SaveDir: %s"), *SaveDir);

	const FString RawDataFileName(TEXT("RawData.bin"));
	FString AbsolutePathForRawData = FPaths::Combine(SaveDir, RawDataFileName);
	UE_LOG(LogTemp, Log, TEXT("AbsolutePathForRawData: %s"), *AbsolutePathForRawData);

	FArchive* RawFileWriterAr = IFileManager::Get().CreateFileWriter(*AbsolutePathForRawData);
	if(nullptr != RawFileWriterAr)
	{
		*RawFileWriterAr << SrcRawData;
		RawFileWriterAr->Close();
		delete RawFileWriterAr;
		RawFileWriterAr = nullptr;
	}

	FBirdData DstRawData;
	FArchive* RawFileReaderAr = IFileManager::Get().CreateFileReader(*AbsolutePathForRawData);
	if(nullptr != RawFileReaderAr)
	{
		*RawFileReaderAr << DstRawData;
		RawFileReaderAr->Close();
		delete RawFileReaderAr;
		RawFileReaderAr = nullptr;

		UE_LOG(LogTemp, Log, TEXT("[DstRawData] Name: %s, ID: %d"), *DstRawData.Name, DstRawData.ID);
	}

	SerializedPigeon = NewObject<USPigeon>();
	SerializedPigeon->SetName(TEXT("Pigeon76"));
	SerializedPigeon->SetID(76);
	UE_LOG(LogTemp, Log, TEXT("SerializedPigeon->GetName(): %s, SerializedPigeon->GetID(): %d"), *SerializedPigeon->GetName(), SerializedPigeon->GetID());

	const FString ObjectDataFileName(TEXT("ObjectData.bin"));
	FString AbsolutePathForObjectData = FPaths::Combine(*SaveDir, *ObjectDataFileName);
	FPaths::MakeStandardFilename(AbsolutePathForObjectData);

	TArray<uint8> BufferArray;
	FMemoryWriter MemoryWriter(BufferArray);
	SerializedPigeon->Serialize(MemoryWriter);

	TUniquePtr<FArchive> ObjectDataFileWriterAr = TUniquePtr<FArchive>(IFileManager::Get().CreateFileReader(*AbsolutePathForObjectData));
	if(nullptr != ObjectDataFileWriterAr)
	{
		*ObjectDataFileWriterAr <<	BufferArray;
		ObjectDataFileWriterAr->Close();
		ObjectDataFileWriterAr = nullptr; //delete ObjectDataFileWriterAr; 와 같은 효과.
	}

	TArray<uint8> BufferArrayFromObjectDataFile;
	TUniquePtr<FArchive> ObjectDataFileReaderAr = TUniquePtr<FArchive>(IFileManager::Get().CreateFileReader(*AbsolutePathForObjectData));
	if(nullptr != ObjectDataFileReaderAr)
	{
		*ObjectDataFileReaderAr << BufferArrayFromObjectDataFile;
		ObjectDataFileReaderAr->Close();
		ObjectDataFileReaderAr = nullptr;
	}

	FMemoryReader MemoryReaderAr(BufferArrayFromObjectDataFile);
	USPigeon* Pigeon77 = NewObject<USPigeon>();
	Pigeon77->Serialize(MemoryReaderAr);
	UE_LOG(LogTemp, Log, TEXT("[SerializedPigeon] Name: %s, ID: %d"), *Pigeon77->GetName(), Pigeon77->GetID());

	const FString JsonDataFileName(TEXT("StudyFile.txt"));
	FString AbsolutePathForJsonData = FPaths::Combine(*SaveDir, *JsonDataFileName);
	FPaths::MakeStandardFilename(AbsolutePathForJsonData);

	TSharedRef<FJsonObject> SrcJsonObject = MakeShareable(new FJsonObject);
	FJsonObjectConverter::UStructToJsonObject(SerializedPigeon->GetClass(), SerializedPigeon, SrcJsonObject);

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonOutString);
	if(true == FJsonSerializer::Serialize(SrcJsonObject, JsonWriter))
	{
		FFileHelper::SaveStringToFile(JsonOutString, *AbsolutePathForJsonData);
	}

	FString JsonInString;
	FFileHelper::LoadFileToString(JsonInString, *AbsolutePathForJsonData);
	TSharedRef<TJsonReader<TCHAR>> JsonReaderAr = TJsonReaderFactory<TCHAR>::Create(JsonInString);

	TSharedPtr<FJsonObject> DstJsonObject;
	if(true == FJsonSerializer::Deserialize(JsonReaderAr, DstJsonObject))
	{
		USPigeon* Pigeon78 = NewObject<USPigeon>();
		if(true == FJsonObjectConverter::JsonObjectToUStruct(DstJsonObject.ToSharedRef(), Pigeon78->GetClass(), Pigeon78))
		{
			UE_LOG(LogTemp, Log, TEXT("[Pigeon78] Name: %s, ID: %d"), *Pigeon78->GetName(), Pigeon78->GetID());
		}
	}
}

void USGameInstance::Shutdown()
{
	Super::Shutdown();
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("PrintString()"));
}

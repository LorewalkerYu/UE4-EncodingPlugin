// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TextEncodingLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ENCODING_API UTextEncodingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:


	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FString GB2312ToUtf8(const FString& GB2312Str);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FString GB2312BytesToUtf8String(const TArray<uint8>& GB2312Bytes);
};

// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Keys/GeneralBlackboardKey.h"
#include "GeneralBlackboardKey_Object.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, meta = (DisplayName = "Object"), collapseCategories)
class GENERALBLACKBOARD_API UGeneralBlackboardKey_Object : public UGeneralBlackboardKey
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (HideInChildren, DisplayThumbnail = false))
	UObject* DefaultValue;

	UPROPERTY(EditAnywhere, meta = (AllowAbstract = true))
	UClass* BaseClass;
	
	UPROPERTY()
	UObject* CurrentValue;

	typedef UObject* FValueType;
	static const FValueType InvalidValue;

	virtual void Reset() override
	{
		CurrentValue = DefaultValue;
	}

	UObject* GetValue() const
	{
		return CurrentValue;
	}

	void SetValue(UObject* NewValue)
	{
		CurrentValue = NewValue;
	}

	virtual bool ImportFromString(const FString& String) override
	{
		CurrentValue = FSoftObjectPath(String).ResolveObject();
		return true;
	}
	
	virtual bool ExportToString(FString& String) const override
	{
		String = FSoftObjectPath(CurrentValue).ToString();
		return true;
	}
};

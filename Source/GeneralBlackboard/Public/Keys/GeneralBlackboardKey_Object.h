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
	UObject* Value;

	UPROPERTY(EditAnywhere, meta = (AllowAbstract = true))
	UClass* BaseClass;
	

	typedef UObject* FValueType;
	static const FValueType InvalidValue;


	UObject* GetValue() const
	{
		return Value;
	}

	void SetValue(UObject* NewValue)
	{
		Value = NewValue;
	}

	virtual void SetFrom(const UGeneralBlackboardKey* Other) override
	{
		if (const ThisClass* Casted = Cast<ThisClass>(Other))
		{
			SetValue(Casted->GetValue());
		}
	}

	virtual bool ImportFromString(const FString& String) override
	{
		Value = FSoftObjectPath(String).ResolveObject();
		return true;
	}
	
	virtual bool ExportToString(FString& String) const override
	{
		String = FSoftObjectPath(Value).ToString();
		return true;
	}
};

// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Keys/GeneralBlackboardKey.h"
#include "GeneralBlackboardKey_String.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, meta = (DisplayName = "String"), collapseCategories)
class GENERALBLACKBOARD_API UGeneralBlackboardKey_String : public UGeneralBlackboardKey
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (HideInChildren))
	FString Value;


	typedef FString FValueType;
	static const FValueType InvalidValue;


	FString GetValue() const
	{
		return Value;
	}	

	bool SetValue(const FString& NewValue)
	{
		if (Value != NewValue)
		{
			Value = NewValue;
			return true;
		}
		return false;
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
		Value = String;
		return true;
	}

	virtual bool ExportToString(FString& String) const override
	{
		String = Value;
		return true;
	}
};

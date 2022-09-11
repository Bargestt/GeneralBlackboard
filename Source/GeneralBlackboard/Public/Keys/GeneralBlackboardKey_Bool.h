// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Keys/GeneralBlackboardKey.h"
#include "GeneralBlackboardKey_Bool.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, meta = (DisplayName = "Bool"), collapseCategories)
class GENERALBLACKBOARD_API UGeneralBlackboardKey_Bool : public UGeneralBlackboardKey
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (HideInChildren))
	bool Value;


	typedef bool FValueType;
	static const FValueType InvalidValue;

	
	bool GetValue() const
	{
		return Value;
	}

	bool SetValue(const bool& NewValue)
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
		Value = String.Equals(TEXT("True")) || String.Equals(TEXT("1"));
		return true;
	}

	virtual bool ExportToString(FString& String) const override
	{
		String = Value ? TEXT("1") : TEXT("0");
		return true;
	}
};


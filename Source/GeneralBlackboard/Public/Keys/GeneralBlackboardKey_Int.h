// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Keys/GeneralBlackboardKey.h"
#include "GeneralBlackboardKey_Int.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, meta = (DisplayName = "Int"), collapseCategories)
class GENERALBLACKBOARD_API UGeneralBlackboardKey_Int : public UGeneralBlackboardKey
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (HideInChildren))
	int32 Value;


	typedef int32 FValueType;
	static const FValueType InvalidValue;


	int32 GetValue() const
	{
		return Value;
	}

	bool SetValue(const int32& NewValue)
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
		Value = FCString::Atoi(*String);
		return true;
	}

	virtual bool ExportToString(FString& String) const override
	{
		String = FString::FromInt(Value);
		return true;
	}
};

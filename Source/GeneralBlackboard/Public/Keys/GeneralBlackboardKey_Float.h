// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Keys/GeneralBlackboardKey.h"
#include "GeneralBlackboardKey_Float.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, meta = (DisplayName = "Float"), collapseCategories)
class GENERALBLACKBOARD_API UGeneralBlackboardKey_Float : public UGeneralBlackboardKey
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (HideInChildren))
	float Value;


	typedef float FValueType;
	static const FValueType InvalidValue;


	float GetValue() const
	{
		return Value;
	}

	void SetValue(const float& NewValue)
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
		Value = FCString::Atof(*String);
		return true;
	}

	virtual bool ExportToString(FString& String) const override
	{
		String = FString::SanitizeFloat(Value);
		return true;
	}
};

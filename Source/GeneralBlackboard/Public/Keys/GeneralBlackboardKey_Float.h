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
	float DefaultValue;

	UPROPERTY()
	float CurrentValue;

	typedef float FValueType;
	static const FValueType InvalidValue;

	virtual void Reset() override
	{
		CurrentValue = DefaultValue;
	}

	float GetValue() const
	{
		return CurrentValue;
	}

	void SetValue(const float& NewValue)
	{
		CurrentValue = NewValue;
	}

	virtual bool ImportFromString(const FString& String) override
	{
		CurrentValue = FCString::Atof(*String);
		return true;
	}

	virtual bool ExportToString(FString& String) const override
	{
		String = FString::SanitizeFloat(CurrentValue);
		return true;
	}
};

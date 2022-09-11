// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Keys/GeneralBlackboardKey.h"
#include "GeneralBlackboardKey_Vector.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, meta = (DisplayName = "Vector"), collapseCategories)
class GENERALBLACKBOARD_API UGeneralBlackboardKey_Vector : public UGeneralBlackboardKey
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (HideInChildren))
	FVector DefaultValue;

	UPROPERTY()
	FVector	CurrentValue;

	typedef FVector FValueType;
	static const FValueType InvalidValue;

	virtual void Reset() override
	{
		CurrentValue = DefaultValue;
	}

	FVector GetValue() const
	{
		return CurrentValue;
	}

	void SetValue(const FVector& NewValue)
	{
		CurrentValue = NewValue;
	}

	virtual bool ImportFromString(const FString& String) override
	{
		CurrentValue.InitFromString(String);
		return true;
	}

	
	virtual bool ExportToString(FString& String) const override
	{
		String = CurrentValue.ToString();
		return true;
	}
};

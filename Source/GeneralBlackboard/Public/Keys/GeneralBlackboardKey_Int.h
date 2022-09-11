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
	int32 DefaultValue;

	UPROPERTY()
	int32 CurrentValue;

	typedef int32 FValueType;
	static const FValueType InvalidValue;

	virtual void Reset() override
	{
		CurrentValue = DefaultValue;
	}

	int32 GetValue() const
	{
		return CurrentValue;
	}

	void SetValue(const int32& NewValue)
	{
		CurrentValue = NewValue;
	}

	virtual bool ImportFromString(const FString& String) override
	{
		CurrentValue = FCString::Atoi(*String);
		return true;
	}

	virtual bool ExportToString(FString& String) const override
	{
		String = FString::FromInt(CurrentValue);
		return true;
	}
};

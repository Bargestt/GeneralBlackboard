// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Keys/GeneralBlackboardKey.h"
#include "GeneralBlackboardKey_Name.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, meta = (DisplayName = "Name"), collapseCategories)
class GENERALBLACKBOARD_API UGeneralBlackboardKey_Name : public UGeneralBlackboardKey
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (HideInChildren))
	FName Value;


	typedef FName FValueType;
	static const FValueType InvalidValue;


	FName GetValue() const
	{
		return Value;
	}

	void SetValue(const FName& NewValue)
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
		Value = *String;
		return true;
	}

	virtual bool ExportToString(FString& String) const override
	{
		String = Value.ToString();
		return true;
	}
};

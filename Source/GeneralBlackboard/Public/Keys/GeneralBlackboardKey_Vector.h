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
	FVector Value;

	typedef FVector FValueType;
	static const FValueType InvalidValue;

	FVector GetValue() const
	{
		return Value;
	}

	void SetValue(const FVector& NewValue)
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
		Value.InitFromString(String);
		return true;
	}

	
	virtual bool ExportToString(FString& String) const override
	{
		String = Value.ToString();
		return true;
	}
};

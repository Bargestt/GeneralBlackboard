// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GeneralBlackboardTypes.h"
#include "GeneralBlackboard.generated.h"

class UGeneralBlackboardKey;




/**  */
USTRUCT()
struct GENERALBLACKBOARD_API FGeneralBlackboardKeyData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FName KeyName;	

	UPROPERTY(EditAnywhere, Instanced)
	UGeneralBlackboardKey* KeyType;

	bool IsValid() const
	{
		return !KeyName.IsNone() && KeyType;
	}

	bool operator==(const FGeneralBlackboardKeyData& Other) const
	{
		return KeyName == Other.KeyName && KeyType == Other.KeyType;
	}
};


/**  */
USTRUCT()
struct FGeneralBlackboardController
{
	GENERATED_BODY()	
};


/** */
UENUM()
enum class EBlackboardAppenRule : uint8
{	
	/** Old values will be preserved */
	UseOld,
	/** New values will override old */
	UseNew
};

/**
 * 
 */
UCLASS(Blueprintable, collapseCategories, meta = (DontUseGenericSpawnObject = "True"))
class GENERALBLACKBOARD_API UGeneralBlackboard : public UObject
{
	GENERATED_BODY()
public:

#if WITH_EDITOR
	UPROPERTY(EditAnywhere)
	FGeneralBlackboardController Controller;

	/** Map view with preserved order */
	UPROPERTY(EditAnywhere, EditFixedSize)
	TArray<FGeneralBlackboardKeyData> KeyDisplay;

	UPROPERTY()
	bool bShowKeys;

#endif //WITH_EDITOR

protected:
	UPROPERTY(VisibleAnywhere, meta = (EditCondition = "bShowKeys", EditConditionHides = true))
	TMap<FName, FGeneralBlackboardKeyData> Keys;

	uint8 bInitialized : 1;

public:
	UGeneralBlackboard();
	virtual void PostLoad() override;

public:
#if WITH_EDITOR
	void UpdateKeyDisplay();
#endif //WITH_EDITOR

public:
	/** Initialize blackboard before using it */
	UFUNCTION(BlueprintCallable, Category = "General Blackboard")
	void Initialize();

	/** Reset all keys values */
	UFUNCTION(BlueprintCallable, Category = "General Blackboard")
	void Reset();
	
	UFUNCTION(BlueprintCallable, Category = "General Blackboard")
	void Append(UGeneralBlackboard* Other, EBlackboardAppenRule Rule);


	UFUNCTION(BlueprintCallable, Category = "General Blackboard")
	void ExportToMap(TMap<FString, FString>& OutValueMap);

	UFUNCTION(BlueprintCallable, Category = "General Blackboard")
	void ImportFromMap(const TMap<FString, FString>& ValueMap);


public:
	const TMap<FName, FGeneralBlackboardKeyData>& GetKeyMap() const { return Keys; }

	UGeneralBlackboardKey* AddNewKey(FName NewKeyName, TSubclassOf<UGeneralBlackboardKey> KeyType);

	bool SetKeyType(FName KeyName, TSubclassOf<UGeneralBlackboardKey> NewKeyType);

	bool RemoveKey(FName KeyName);

	bool RenameKey(FName KeyName, FName NewKeyName);	

	FName GenerateKeyName(FName BaseName, TSubclassOf<UGeneralBlackboardKey> KeyType);	

	UGeneralBlackboardKey* GetKey(FName Name) const;

	bool HasBlackboardKey(const FName& Key, TSubclassOf<UGeneralBlackboardKey> Class) const;

	TSubclassOf<UGeneralBlackboardKey> GetBlackboardKeyClass(const FName& Key) const;

public:

	UFUNCTION(BlueprintCallable, Category = "General Blackboard")
	TArray<FName> GetBlackboardKeys() const;

	/** Get blackboard keys filtered by class */
	UFUNCTION(BlueprintCallable, Category = "General Blackboard")
	TArray<FName> GetBlackboardKeysOfClass(TSubclassOf<UGeneralBlackboardKey> Class) const;



	/** 
	 * Returns true if blackboard has key with specified Name and Class. 
	 * Use empty class to check by name only 
	 */
	UFUNCTION(BlueprintCallable, Category = "General Blackboard", meta = (DisplayName = "HasBlackboardKey"))
	bool BP_HasBlackboardKey(const FGeneralBlackboardKeySelector& Key, TSubclassOf<UGeneralBlackboardKey> Class) const
	{
		return HasBlackboardKey(Key.Name, Class);
	}	

	UFUNCTION(BlueprintCallable, Category = "General Blackboard", meta = (DisplayName = "GetBlackboardKeyClass"))
	TSubclassOf<UGeneralBlackboardKey> BP_GetBlackboardKeyClass(const FGeneralBlackboardKeySelector& Key) const
	{
		return GetBlackboardKeyClass(Key.Name);
	}	


	/*--------------------------------------------
		Access
	 *--------------------------------------------*/
protected:
	template<class TKeyClass, typename TValueType>
	typename bool GetKeyValue(FName Name, TValueType& OutValue) const
	{
		static_assert(TIsSame<TKeyClass::FValueType, TValueType>::Value, "TKeyClass::FValueType must be same as TValueType");

		UGeneralBlackboardKey* Key = GetKey(Name);
		if (Key == nullptr || Key->GetClass() != TKeyClass::StaticClass())
		{
			OutValue = TKeyClass::InvalidValue;
			return false;
		}

		OutValue = CastChecked<TKeyClass>(Key)->GetValue();
		return true;
	}

	template<class TKeyClass, typename TValueType>
	typename bool SetKeyValue(FName Name, TValueType& NewValue)
	{
		static_assert(TIsSame<TKeyClass::FValueType, TValueType>::Value, "TKeyClass::FValueType must be same as TValueType");

		UGeneralBlackboardKey* Key = GetKey(Name);
		if (Key == nullptr || Key->GetClass() != TKeyClass::StaticClass())
		{
			return false;
		}

		CastChecked<TKeyClass>(Key)->SetValue(NewValue);
		return true;
	}
public:
	
	UFUNCTION(BlueprintPure, Category = "General Blackboard|Values")
	bool GetValueAsBool(const FGeneralBlackboardKeySelector& Key, bool& Value) const;

	UFUNCTION(BlueprintPure, Category = "General Blackboard|Values")
	bool GetValueAsFloat(const FGeneralBlackboardKeySelector& Key, float& Value) const;

	UFUNCTION(BlueprintPure, Category = "General Blackboard|Values")
	bool GetValueAsInt(const FGeneralBlackboardKeySelector& Key, int32& Value) const;

	UFUNCTION(BlueprintPure, Category = "General Blackboard|Values")
	bool GetValueAsName(const FGeneralBlackboardKeySelector& Key, FName& Value) const;

	UFUNCTION(BlueprintPure, Category = "General Blackboard|Values")
	bool GetValueAsString(const FGeneralBlackboardKeySelector& Key, FString& Value) const;

	UFUNCTION(BlueprintPure, Category = "General Blackboard|Values")
	bool GetValueAsText(const FGeneralBlackboardKeySelector& Key, FText& Value) const;

	UFUNCTION(BlueprintPure, Category = "General Blackboard|Values")
	bool GetValueAsVector(const FGeneralBlackboardKeySelector& Key, FVector& Value) const;

	UFUNCTION(BlueprintPure, Category = "General Blackboard|Values")
	bool GetValueAsObject(const FGeneralBlackboardKeySelector& Key, UObject*& Value) const;

	UFUNCTION(BlueprintPure, Category = "General Blackboard|Values")
	bool GetValueAsActor(const FGeneralBlackboardKeySelector& Key, AActor*& Value) const;



	UFUNCTION(BlueprintCallable, Category = "General Blackboard|Values")
	bool SetValueAsBool(const FGeneralBlackboardKeySelector& Key, bool Value);

	UFUNCTION(BlueprintCallable, Category = "General Blackboard|Values")
	bool SetValueAsFloat(const FGeneralBlackboardKeySelector& Key, float Value);

	UFUNCTION(BlueprintCallable, Category = "General Blackboard|Values")
	bool SetValueAsInt(const FGeneralBlackboardKeySelector& Key, int32 Value);

	UFUNCTION(BlueprintCallable, Category = "General Blackboard|Values")
	bool SetValueAsName(const FGeneralBlackboardKeySelector& Key, FName Value);

	UFUNCTION(BlueprintCallable, Category = "General Blackboard|Values")
	bool SetValueAsString(const FGeneralBlackboardKeySelector& Key, FString Value);

	UFUNCTION(BlueprintCallable, Category = "General Blackboard|Values")
	bool SetValueAsText(const FGeneralBlackboardKeySelector& Key, FText Value);

	UFUNCTION(BlueprintCallable, Category = "General Blackboard|Values")
	bool SetValueAsVector(const FGeneralBlackboardKeySelector& Key, FVector Value);

	UFUNCTION(BlueprintCallable, Category = "General Blackboard|Values")
	bool SetValueAsObject(const FGeneralBlackboardKeySelector& Key, UObject* Value);

};

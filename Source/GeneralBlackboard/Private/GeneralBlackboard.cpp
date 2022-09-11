// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.


#include "GeneralBlackboard.h"
#include "Keys/GeneralBlackboardKey.h"
#include <Templates/SubclassOf.h>

#include "Keys/GeneralBlackboardKey_Bool.h"
#include "Keys/GeneralBlackboardKey_Float.h"
#include "Keys/GeneralBlackboardKey_Int.h"
#include "Keys/GeneralBlackboardKey_Name.h"
#include "Keys/GeneralBlackboardKey_String.h"
#include "Keys/GeneralBlackboardKey_Text.h"
#include "Keys/GeneralBlackboardKey_Vector.h"
#include "Keys/GeneralBlackboardKey_Object.h"

UGeneralBlackboard::UGeneralBlackboard()
{

}

void UGeneralBlackboard::PostLoad()
{
	Super::PostLoad();

#if WITH_EDITOR
	UpdateKeyDisplay();
#endif //WITH_EDITOR
}


void UGeneralBlackboard::UpdateKeyDisplay()
{	
	Keys.Remove(NAME_None);

	TSet<FName> DisplayedKeys;

	// Update current display
	for (FGeneralBlackboardKeyData& Display : KeyDisplay)
	{
		if (Display.KeyName != NAME_None)
		{
			const FGeneralBlackboardKeyData* Data = Keys.Find(Display.KeyName);
			if (Data)
			{
				Display = *Data;
				DisplayedKeys.Add(Display.KeyName);
			}
		}		
	}

	//Add new
	for (auto& Pair : Keys)
	{
		if(!DisplayedKeys.Contains(Pair.Key))
		{
			KeyDisplay.Add(Pair.Value);
		}
	}

	// Remove old
	KeyDisplay.RemoveAll([this](const FGeneralBlackboardKeyData& Data)
	{
		return !Keys.Contains(Data.KeyName);
	});
}

void UGeneralBlackboard::SetValues(const UGeneralBlackboard* Other)
{
	if (Other)
	{
		for (const auto& Pair : Keys)
		{
			if (Pair.Value.KeyType)
			{
				Pair.Value.KeyType->SetFrom(Other->GetBlackboardKey(Pair.Key));
			}
		}
	}	

#if WITH_EDITOR
	UpdateKeyDisplay();
#endif //WITH_EDITOR
}

void UGeneralBlackboard::Append(const UGeneralBlackboard* Other, EBlackboardAppenRule Rule)
{
	if (Other)
	{
		for (const auto& Pair : Other->Keys)
		{
			if (Pair.Key != NAME_None && Pair.Value.KeyType)
			{
				UGeneralBlackboardKey* ExistingKey = GetBlackboardKey(Pair.Key);
				if (ExistingKey == nullptr || Rule == EBlackboardAppenRule::UseNew)
				{
					AddKey(Pair.Key, DuplicateObject<UGeneralBlackboardKey>(Pair.Value.KeyType, this));
				}
			}
		}
	}

#if WITH_EDITOR
	UpdateKeyDisplay();
#endif //WITH_EDITOR
}

void UGeneralBlackboard::ExportToMap(TMap<FString, FString>& OutValueMap)
{
	OutValueMap.Reset();
	for (const auto& Pair : Keys)
	{
		if (Pair.Key != NAME_None && Pair.Value.KeyType)
		{
			FString Value;
			if (Pair.Value.KeyType->ExportToString(Value))
			{
				OutValueMap.Add(Pair.Key.ToString(), Value);
			}
		}
	}
}

void UGeneralBlackboard::ImportFromMap(const TMap<FString, FString>& ValueMap)
{
	for (const auto& Pair : ValueMap)
	{
		FName KeyName = FName(Pair.Key);
		if (KeyName != NAME_None)
		{
			UGeneralBlackboardKey* Key = GetBlackboardKey(*Pair.Key);
			if (Key)
			{
				Key->ImportFromString(Pair.Value);
			}
		}
	}

#if WITH_EDITOR
	UpdateKeyDisplay();
#endif //WITH_EDITOR
}

UGeneralBlackboardKey* UGeneralBlackboard::AddNewKey(FName NewKeyName, TSubclassOf<UGeneralBlackboardKey> KeyType)
{
	if (KeyType)
	{
		FGeneralBlackboardKeyData& NewKeyData = AddKey(GenerateKeyName(NewKeyName, KeyType), NewObject<UGeneralBlackboardKey>(this, KeyType));

#if WITH_EDITOR
		UpdateKeyDisplay();
#endif //WITH_EDITOR

		return NewKeyData.KeyType;
	}	

	return nullptr;
}

bool UGeneralBlackboard::SetKeyType(FName KeyName, TSubclassOf<UGeneralBlackboardKey> NewKeyType)
{
	if (KeyName != NAME_None && NewKeyType)
	{
		FGeneralBlackboardKeyData* KeyPtr = Keys.Find(KeyName);
		if (KeyPtr)
		{
			KeyPtr->KeyType = NewObject<UGeneralBlackboardKey>(this, NewKeyType);

#if WITH_EDITOR
			UpdateKeyDisplay();
#endif //WITH_EDITOR

			return true;
		}
	}

	return false;
}

bool UGeneralBlackboard::RemoveKey(FName KeyName)
{
	bool bRemoved = Keys.Remove(KeyName) > 0;
#if WITH_EDITOR
	UpdateKeyDisplay();
#endif //WITH_EDITOR

	return bRemoved;
}


bool UGeneralBlackboard::RenameKey(FName KeyName, FName NewKeyName)
{
	if (KeyName != NAME_None && NewKeyName != NAME_None && KeyName != NewKeyName)
	{
		FGeneralBlackboardKeyData Data;
		Keys.RemoveAndCopyValue(KeyName, Data);

		if (Data.KeyType != nullptr)
		{
			AddKey(NewKeyName, Data.KeyType);

#if WITH_EDITOR
			UpdateKeyDisplay();
#endif //WITH_EDITOR

			return true;
		}
	}
	return false;
}

FName UGeneralBlackboard::GenerateKeyName(FName BaseName, TSubclassOf<UGeneralBlackboardKey> KeyType)
{
	FString NewKeyNameString;
	if (BaseName == NAME_None)
	{
		NewKeyNameString = KeyType ? KeyType->GetDisplayNameText().ToString() : TEXT("None");
		NewKeyNameString = NewKeyNameString.Replace(TEXT(" "), TEXT("_"));
		NewKeyNameString += TEXT("Key");
	}
	else
	{
		NewKeyNameString = BaseName.ToString();
	}

	int32 IndexSuffix = -1;
	auto DuplicateFunction = [&](const FGeneralBlackboardKeyData& Key)
	{
		if (Key.KeyName.ToString() == NewKeyNameString)
		{
			IndexSuffix = FMath::Max(0, IndexSuffix);
		}
		if (Key.KeyName.ToString().StartsWith(NewKeyNameString))
		{
			const FString ExistingSuffix = Key.KeyName.ToString().RightChop(NewKeyNameString.Len());
			if (ExistingSuffix.IsNumeric())
			{
				IndexSuffix = FMath::Max(FCString::Atoi(*ExistingSuffix) + 1, IndexSuffix);
			}
		}
	};

	// check for existing keys of the same name
	for (const auto& Pair : Keys) { DuplicateFunction(Pair.Value); };
	if (IndexSuffix != -1)
	{
		NewKeyNameString += FString::Printf(TEXT("%d"), IndexSuffix);
	}

	return FName(*NewKeyNameString);
}

UGeneralBlackboardKey* UGeneralBlackboard::GetBlackboardKey(FName Name) const
{
	if (Name != NAME_None)
	{
		const FGeneralBlackboardKeyData* KeyPtr = Keys.Find(Name);
		if (KeyPtr)
		{
			return KeyPtr->KeyType;
		}
	}

	return nullptr;
}

bool UGeneralBlackboard::HasBlackboardKey(const FName& Key, TSubclassOf<UGeneralBlackboardKey> Class) const
{
	UGeneralBlackboardKey* KeyType = GetBlackboardKey(Key);
	return KeyType && (!Class || KeyType->IsA(Class));
}

TSubclassOf<UGeneralBlackboardKey> UGeneralBlackboard::GetBlackboardKeyClass(const FName& Key) const
{
	UGeneralBlackboardKey* KeyType = GetBlackboardKey(Key);
	return KeyType ? KeyType->GetClass() : nullptr;
}

TArray<FName> UGeneralBlackboard::GetBlackboardKeys() const
{
	return GetBlackboardKeysOfClass(nullptr);
}

TArray<FName> UGeneralBlackboard::GetBlackboardKeysOfClass(TSubclassOf<UGeneralBlackboardKey> Class) const
{
	TArray<FName> Arr;
	for (const auto& Pair : Keys)
	{
		if (Pair.Key != NAME_None && Pair.Value.KeyType && (!Class || Pair.Value.KeyType->IsA(Class)))
		{
			Arr.Add(Pair.Key);
		}
	}
	return Arr;
}



/*--------------------------------------------
 	Getters
 *--------------------------------------------*/

bool UGeneralBlackboard::GetValueAsBool(const FGeneralBlackboardKeySelector& Key, bool& Value) const
{
	return GetKeyValue<UGeneralBlackboardKey_Bool>(Key.Name, Value);
}

bool UGeneralBlackboard::GetValueAsFloat(const FGeneralBlackboardKeySelector& Key, float& Value) const
{
	return GetKeyValue<UGeneralBlackboardKey_Float>(Key.Name, Value);
}

bool UGeneralBlackboard::GetValueAsInt(const FGeneralBlackboardKeySelector& Key, int32& Value) const
{
	return GetKeyValue<UGeneralBlackboardKey_Int>(Key.Name, Value);
}

bool UGeneralBlackboard::GetValueAsName(const FGeneralBlackboardKeySelector& Key, FName& Value) const
{
	return GetKeyValue<UGeneralBlackboardKey_Name>(Key.Name, Value);
}

bool UGeneralBlackboard::GetValueAsString(const FGeneralBlackboardKeySelector& Key, FString& Value) const
{
	return GetKeyValue<UGeneralBlackboardKey_String>(Key.Name, Value);
}

bool UGeneralBlackboard::GetValueAsText(const FGeneralBlackboardKeySelector& Key, FText& Value) const
{
	return GetKeyValue<UGeneralBlackboardKey_Text>(Key.Name, Value);
}

bool UGeneralBlackboard::GetValueAsVector(const FGeneralBlackboardKeySelector& Key, FVector& Value) const
{
	return GetKeyValue<UGeneralBlackboardKey_Vector>(Key.Name, Value);
}

bool UGeneralBlackboard::GetValueAsObject(const FGeneralBlackboardKeySelector& Key, UObject*& Value) const
{
	return GetKeyValue<UGeneralBlackboardKey_Object>(Key.Name, Value);
}

bool UGeneralBlackboard::GetValueAsActor(const FGeneralBlackboardKeySelector& Key, AActor*& Value) const
{
	UObject* Object = nullptr;
	if (GetKeyValue<UGeneralBlackboardKey_Object>(Key.Name, Object))
	{
		Value = nullptr;
		return false;
	}

	Value = Cast<AActor>(Object);
	return true;
}


/*--------------------------------------------
 	Setters
 *--------------------------------------------*/

bool UGeneralBlackboard::SetValueAsBool(const FGeneralBlackboardKeySelector& Key, bool Value)
{
	return SetKeyValue<UGeneralBlackboardKey_Bool>(Key.Name, Value);
}

bool UGeneralBlackboard::SetValueAsFloat(const FGeneralBlackboardKeySelector& Key, float Value)
{
	return SetKeyValue<UGeneralBlackboardKey_Float>(Key.Name, Value);
}

bool UGeneralBlackboard::SetValueAsInt(const FGeneralBlackboardKeySelector& Key, int32 Value)
{
	return SetKeyValue<UGeneralBlackboardKey_Int>(Key.Name, Value);
}

bool UGeneralBlackboard::SetValueAsName(const FGeneralBlackboardKeySelector& Key, FName Value)
{
	return SetKeyValue<UGeneralBlackboardKey_Name>(Key.Name, Value);
}

bool UGeneralBlackboard::SetValueAsString(const FGeneralBlackboardKeySelector& Key, FString Value)
{
	return SetKeyValue<UGeneralBlackboardKey_String>(Key.Name, Value);
}

bool UGeneralBlackboard::SetValueAsText(const FGeneralBlackboardKeySelector& Key, FText Value)
{
	return SetKeyValue<UGeneralBlackboardKey_Text>(Key.Name, Value);
}

bool UGeneralBlackboard::SetValueAsVector(const FGeneralBlackboardKeySelector& Key, FVector Value)
{
	return SetKeyValue<UGeneralBlackboardKey_Vector>(Key.Name, Value);
}

bool UGeneralBlackboard::SetValueAsObject(const FGeneralBlackboardKeySelector& Key, UObject* Value)
{
	return SetKeyValue<UGeneralBlackboardKey_Object>(Key.Name, Value);
}

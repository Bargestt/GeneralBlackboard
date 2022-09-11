// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include <ClassViewerModule.h>
#include <ClassViewerFilter.h>



struct FGeneralBlackboardCustomizationHelpers
{
	static TSharedRef<SWidget> CreateKeyTypePickerOptions(const struct FClassFilterIntersection& ClassFilter, FOnClassPicked OnPicked);

	static const FSlateBrush* GetKeyTypeBrush(UClass* KeyType);

	/** Invoke Operation on each blackboard */
	static void BlackboardOperation(FText TransactionName, TSharedPtr<IPropertyHandle> Property, TFunctionRef<EPropertyChangeType::Type(class UGeneralBlackboard*)> Operation);

};


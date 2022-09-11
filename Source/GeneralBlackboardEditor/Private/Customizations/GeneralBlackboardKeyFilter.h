// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include <ClassViewerFilter.h>

#include "Keys/GeneralBlackboardKey.h"



/**  
 * Combined intersection of multiple class filters
 */
struct FClassFilterIntersection : public IClassViewerFilter
{
	FClassFilterIntersection()
		: bAllowAll(true)
	{
		
	}

	void AddFilters(const TArray<TSubclassOf<UGeneralBlackboardKey>>& NewFilter)
	{
		if (bAllowAll)
		{
			for (const auto& Class : NewFilter)
			{
				if (Class)
				{
					AllowedClasses.Add(Class);
				}
			}

			// Keep allow all if no class was added to filter
			bAllowAll = AllowedClasses.Num() == 0;
		}
		else
		{
			TSet<UClass*> Filter; 
			for (const auto& Class : NewFilter)
			{
				if (Class)
				{
					Filter.Add(Class);
				}
			}
			AllowedClasses = AllowedClasses.Intersect(Filter);
		}	
	}

	/** No class will pass this filter */
	bool AllFilteredOut() const
	{
		return !bAllowAll && AllowedClasses.Num() == 0;
	}

	bool IsClassAllowed(const UClass* InClass) const
	{
		if (InClass != nullptr)
		{
			return bAllowAll || AllowedClasses.Contains(InClass);
		}
		return false;
	}

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		return IsClassAllowed(InClass);
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InUnloadedClassData, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		return InUnloadedClassData->IsChildOf(UGeneralBlackboardKey::StaticClass());
	}

private:
	bool bAllowAll;
	TSet<UClass*> AllowedClasses;
};

class FGeneralBlackboardClassFilter : public IClassViewerFilter
{
	FClassFilterIntersection Filter;
public:
	FGeneralBlackboardClassFilter(const FClassFilterIntersection& InClassFilter)
		: Filter(InClassFilter)
	{

	}

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		if (InClass != nullptr)
		{
			return !InClass->HasAnyClassFlags(CLASS_Abstract | CLASS_HideDropDown) &&
				InClass->HasAnyClassFlags(CLASS_EditInlineNew) &&
				InClass->IsChildOf(UGeneralBlackboardKey::StaticClass()) &&
				Filter.IsClassAllowed(InClass);
		}
		return false;
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InUnloadedClassData, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		return InUnloadedClassData->IsChildOf(UGeneralBlackboardKey::StaticClass());
	}
};


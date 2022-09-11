// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.


#include "GeneralBlackboardFactory.h"
#include "GeneralBlackboard.h"

UGeneralBlackboardFactory::UGeneralBlackboardFactory()
{
	SupportedClass = UGeneralBlackboard::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UGeneralBlackboardFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(Class->IsChildOf(UGeneralBlackboard::StaticClass()));
	return NewObject<UGeneralBlackboard>(InParent, Class, Name, Flags);;
}

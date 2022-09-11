// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.


#include "GeneralBlackboardFunctionLibrary.h"
#include "GeneralBlackboardInterface.h"

UGeneralBlackboard* UGeneralBlackboardFunctionLibrary::CreateGeneralBlackboard(UObject* Outer)
{
	UGeneralBlackboard* Blackboard = NewObject<UGeneralBlackboard>(Outer, NAME_None);

	return Blackboard;
}

UGeneralBlackboard* UGeneralBlackboardFunctionLibrary::GetGeneralBlackboardFromObject(UObject* Object)
{
	if (Object && Object->Implements<UGeneralBlackboardInterface>())
	{
		return IGeneralBlackboardInterface::Execute_GetGeneralBlackboard(Object);
	}
	return nullptr;
}

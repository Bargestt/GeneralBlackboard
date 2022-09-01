// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#include "GeneralBlackboardModule.h"

DEFINE_LOG_CATEGORY(LogGeneralBlackboard);

#define LOCTEXT_NAMESPACE "FGeneralBlackboardModule"

class FGeneralBlackboardModule : public IGeneralBlackboardModule
{
public:

	virtual void StartupModule() override
	{

	}
	virtual void ShutdownModule() override
	{

	}
};

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGeneralBlackboardModule, GeneralBlackboard)
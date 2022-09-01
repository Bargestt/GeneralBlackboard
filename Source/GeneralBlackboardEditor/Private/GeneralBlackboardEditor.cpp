// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#include "GeneralBlackboardEditor.h"




#define LOCTEXT_NAMESPACE "FGeneralBlackboardEditorModule"



class FGeneralBlackboardEditorModule : public IGeneralBlackboardEditorModule
{
	void StartupModule() override
	{		

	}

	void ShutdownModule() override
	{
		
	}
};


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGeneralBlackboardEditorModule, GeneralBlackboardEditor)
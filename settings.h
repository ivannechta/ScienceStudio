#pragma once
#include "header.h"
#include "tablevars.h"


const uint16_t Settings_Max_Modules=100;
typedef TVar* (*ModuleFuncType)();
struct TSettings_Record{
	char* AliasName;	// Name in Studio
	char* Namespace;
	int ParamCount;
	char* ModuleFileName;
	char* FunctionName; // Name in dll file
};
class TSettings
{
private:
	uint16_t SettingsSize;
	struct TSettings_Record** SettingsRecord;
	TTableVars* Table;
public:
	ModuleFuncType* ModuleFunction;
public:
	void ParceSettings();
	int LoadFunction(TSettings_Record* _rec);
	int Search(TSettings_Record* _rec);
	bool CmpAlias(TSettings_Record* _a, TSettings_Record* _b);

public:
	TSettings(TTableVars* _table) {
		Table = _table;
		SettingsRecord = new TSettings_Record*[Settings_Max_Modules];
		ModuleFunction = new ModuleFuncType[Settings_Max_Modules];
		for (uint16_t i = 0; i < Settings_Max_Modules; i++) {
			ModuleFunction[i] = NULL;
		}
		SettingsSize = 0;
	}
	~TSettings() {
		delete[] SettingsRecord;
		delete ModuleFunction;
	}
};
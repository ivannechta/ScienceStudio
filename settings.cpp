#include "settings.h"
#include <windows.h>

void TSettings::ParceSettings()
{
    FILE* f; 
    fopen_s(&f,"Settings.ini","rt");
    if (!f) {
        SHOW("Failed to open Settings.ini\n");
        return;
    }
    while (!feof(f)) {
        char* Alias = new char[256];
        char* Namespace = new char[256];
        char* ModuleFileName = new char[256];
        char* FunctionName = new char[256];
        fscanf_s(f, "%s", Alias, 255);
        fscanf_s(f, "%s", Namespace, 255);
        fscanf_s(f, "%s", ModuleFileName, 255);
        fscanf_s(f, "%s", FunctionName, 255);

        TSettings_Record* rec = new TSettings_Record;
        rec->AliasName = Alias;
        rec->Namespace = Namespace;
        rec->ModuleFileName = ModuleFileName;
        rec->FunctionName = FunctionName;
        LoadFunction(rec);
    }
    fclose(f);
}

int TSettings::LoadFunction(TSettings_Record* _rec)
{
	HINSTANCE hDll = LoadLibraryA(_rec->ModuleFileName);
    if (hDll == NULL) {
        printf("Failed to load module. Module '%s', func '%s'\n", _rec->ModuleFileName, _rec->FunctionName);
        return -1;
    }    
    ModuleFunction[SettingsSize] = (ModuleFuncType)GetProcAddress(hDll, _rec->FunctionName);

    if (!ModuleFunction[SettingsSize]) {
        printf("Failed to find function. Module '%s', func '%s'\n", _rec->ModuleFileName, _rec->FunctionName);
        return -1;
    }
    SettingsRecord[SettingsSize++] = _rec;
    return 0;
}

bool TSettings::CmpAlias(TSettings_Record* _a, TSettings_Record* _b) {
    if (strcmp(_a->AliasName, _b->AliasName)) return false;
    if (strcmp(_a->Namespace, _b->Namespace)) return false;
    return true;
}

int TSettings::Search(TSettings_Record* _rec)
{
    for (int16_t i = 0; i < SettingsSize; i++) {
        if (CmpAlias(_rec, SettingsRecord[i])) {
            return i;
        }
    }
	return -1;
}

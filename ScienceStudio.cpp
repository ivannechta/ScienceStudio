#include <iostream>
#include "core.h"
#include "interface.h"
#include <windows.h>
#include "parcecmd.h"
#include "grammar.h"
#include "tablevars.h"

bool isRunning = true;

TVar* CreateArray1D(double a, double b) {
	double* d1 = new double;
	*d1 = a;
	TVar* A = new TVar(NULL, d1, sizeof(double));
	A->VarType = EVAR_TYPE_DOUBLE;
	A->TensorSize = 1;

	double* d2 = new double;
	*d2 = b;
	TVar* B = new TVar(NULL, d2, sizeof(double));
	B->VarType = EVAR_TYPE_DOUBLE;
	B->TensorSize = 1;

	TVar* C = new TVar((char*)"MyArray1D", NULL, sizeof(double));
	C->VarType = EVAR_TYPE_ARRAY;
	C->TensorSize = 2;
	C->Value = new TVar * *[C->TensorSize];
	TVar** _p = (TVar**)C->Value;
	_p[0] = A; _p[1] = B;	
	return C;
}
TVar* CreateArray2D() {
	TVar* A = CreateArray1D(11, 12);
	TVar* B = CreateArray1D(21, 22);

	TVar* C = new TVar((char*)"MyArray2D", NULL, sizeof(double));
	C->VarType = EVAR_TYPE_ARRAY;
	C->TensorSize = 2;
	C->Value = new TVar * *[C->TensorSize];
	for (int i = 0; i < 2; i++) {
		TVar** _p = (TVar**)C->Value;
		_p[0] = A; _p[1] = B;
		return C;
	}
}


int main()
{	
	TCore Core;
	HANDLE HandleConsoleThread = CreateThread(NULL, 0, Console_Thread, &Core, 0, NULL);
	Sleep(100); // Wait until pipe created
	Core.Settings->ParceSettings();
	
	TGrammar g(Core.Table, Core.Context_lnk);
	//Core.Table->ShowTable();
	//Core.Table->Add((char*)"MyNewArray", CreateArray2D());
	//g.CalcExpr((char*)"a=1+2");
	//g.CalcExpr((char*)"b=a");
	//g.CalcExpr((char*)"a=sum(1,2)");
	//g.CalcExpr((char*)"b=1+a*5");
	//g.CalcExpr((char*)"m=[sum(5,2),3+8]");
	//g.CalcExpr((char*)"y=[[5,2],[6,sum(4,1)]]");
	/*g.CalcExpr((char*)"y=MyArray2D[5,2]");
	*/
	//g.CalcExpr((char*)"m=[5]");

	/*g.CalcExpr((char*)"m=[1,2,3,4,5]");
	g.CalcExpr((char*)"y=array1D(m)");*/

	g.CalcExpr((char*)"m=[[1,2],[3,4],[5,6]]");
	//g.CalcExpr((char*)"y=Math.array2D(m,3,2)");
	//g.CalcExpr((char*)"y=m[1]");
	g.CalcExpr((char*)"m[1,1]=0");

	//g.CalcExpr((char*)"y.e=5");
	
	Core.Table->ShowTable();

	SHOW("Enter commands\n");
	while (isRunning) {
		Core.Prompt();
		gets_s(Core.ConsoleInput, ConsoleCommandSize);
		Core.Execute();
	}
}
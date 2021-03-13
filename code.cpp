#include<Windows.h>
#include <CommDlg.h>
#include <mmsystem.h>
#include <time.h>
#include"resource.h"
#include"clases.h"
#pragma warning(disable : 4996) 

#define TM_RELOJ 3000
#define IDR_MENU1 MENU

// aber 
HWND txbUser;
HWND txbPass;

#pragma region Instancias
HINSTANCE hInst;
HINSTANCE hInstGlob;
HWND hMain = 0;
HWND hLogin = 0;
HWND hCits = 0;
HWND hMecs = 0;
HWND hClients = 0;
HWND hServs = 0;
HMENU hMenu = 0;
#pragma endregion

#pragma region Variables de Timer
time_t unixsecs;
time_t unixsecs2;
struct tm* _actualTime;
struct tm* _actualTime2;
HWND hLbl_Reloj;
HWND hLbl_Reloj2;

#pragma endregion

#pragma region Variables Globales de busqueda
Mechs::Nodo* found = NULL;
Servs::Node* serFound = NULL;
Servs::Node* ser2Found = NULL;

#pragma endregion

BOOL CALLBACK hMainFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK hLoginFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK hMecsFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK hCitsFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK hClientsFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK hServsFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

//funcion principal
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR cmdLine, int showCmd)
{
	hInst = hInstance;
	// Create the modeless dialog window. This program is a pure
	// dialog window application, and the dialog window is the
	// "main" window.
	hMain = CreateDialog(hInstance, // Application instance.
		MAKEINTRESOURCE(IDD_MAIN), // Dialog resource ID.
		0, // Parent window--null for no parent.
		(DLGPROC)hMainFun);// Dialog window procedure.
	//hLogin = CreateDialog(hInstance, // Application instance.
	//	MAKEINTRESOURCE(IDD_LOGIN), // Dialog resource ID.
	//	hMain, // Parent window--null for no parent.
	//	(DLGPROC)hLoginFun);
	//hCits = CreateDialog(hInstance, // Application instance.
	//	MAKEINTRESOURCE(IDD_CITAS), // Dialog resource ID.
	//	hMain, // Parent window--null for no parent.
	//	(DLGPROC)hCitsFun);
	//hMecs = CreateDialog(hInstance, // Application instance.
	//	MAKEINTRESOURCE(IDD_MEDICOS), // Dialog resource ID.
	//	hMain, // Parent window--null for no parent.
	//	(DLGPROC)hMecsFun);
	//hPacs = CreateDialog(hInstance, // Application instance.
	//	MAKEINTRESOURCE(IDD_PACIENTES), // Dialog resource ID.
	//	hMain, // Parent window--null for no parent.
	//	(DLGPROC)hClientsFun);
	//hServs = CreateDialog(hInstance, // Application instance.
	//	MAKEINTRESOURCE(IDD_SERVICES), // Dialog resource ID.
	//	hMain, // Parent window--null for no parent.
	//	(DLGPROC)hServsFun);
	hMenu = GetMenu(hMain);
	
	//se inicia el timer en las ventanas que queramos
	SetTimer(hMain, TM_RELOJ, 1000, NULL);


	// Show the dialog.
	ShowWindow(hMain, 0);
	hInst = hInstance;
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (GetMessage(&msg, 0, 0, 0))
	{
		if (hMain == 0 || !IsDialogMessage(hMain, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

//callback ventana principal
BOOL CALLBACK hMainFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		//vamos a crear Servs
		/*Servs* Especialidades = new Servs();
		Especialidades->ServsIsEmpty();*/
		                               // esto es un label, un static text en la ventana
		hLbl_Reloj = GetDlgItem(hDlg, Lbl_Timer);
		
		Usuario.loadData();
		//LO PRIMERO QUE SE VE ES EL LOGIN
		ShowWindow(hMain, SW_HIDE);
		hLogin=CreateDialog(hInstGlob, MAKEINTRESOURCE(IDD_LOGIN), hMain, hLoginFun);
		ShowWindow(hLogin, SW_SHOW);
		//DESPUES DE ESTO CARGAMOS EL NOMBRE COMPLETO DEL USUARIO Y SU FOTO PA Q SE VEA BIEN
		//LBL_USERNAME ES EL LABEL DONDE PONDREMOS EL NOMBRE DEL USUARIO QUE ACCESO
		/*SendMessageA(GetDlgItem(hMain, LBL_USERNAME) , WM_SETTEXT, 0, (LPARAM)Usuario.cUser);*/
		
		SetWindowTextA(GetDlgItem(hDlg, LBL_USERNAME), Usuario.cUser);

		HWND PICM = GetDlgItem(hDlg, PC_USERMA);
		loadPic(PICM, Usuario.cpicU, 150, 250);

	}
	break;
	case WM_COMMAND:
	{

		switch (wParam) {
		case 40014:
			ShowWindow(hMain, SW_HIDE);
			hServs=CreateDialog(hInst, MAKEINTRESOURCE(IDD_SERVICES), hMain, hServsFun);
			ShowWindow(hServs, SW_SHOW);
			break;
		case 40015:
			ShowWindow(hMain, SW_HIDE);
			hMecs=CreateDialog(hInst, MAKEINTRESOURCE(IDD_MECHS), hMain, hMecsFun);
			ShowWindow(hMecs, SW_SHOW);
			break;
		case 40016:
			ShowWindow(hMain, SW_HIDE);
			hCits=CreateDialog(hInst, MAKEINTRESOURCE(IDD_CITAS), hMain, hCitsFun);
			ShowWindow(hCits, SW_SHOW);
			break;
		case 40017:
			ShowWindow(hMain, SW_HIDE);
			hClients=CreateDialog(hInst, MAKEINTRESOURCE(IDD_CLIENTS), hMain, hClientsFun);
			ShowWindow(hClients, SW_SHOW);
			break;
		case ID_SALIR:
			DestroyWindow(hDlg);
			break;
		}
		break;
	}
	case WM_TIMER: {
		time(&unixsecs);
		_actualTime = localtime(&unixsecs);

		char reloj[80];
		strftime(reloj, 80, "%d-%m-%Y %I:%M:%S", _actualTime);
		/*SetWindowText(hLbl_Reloj, reloj);*/
		SendMessageA(hLbl_Reloj, WM_SETTEXT, 0, (LPARAM)reloj);
	}
	break;
	case WM_CLOSE:
	{
		KillTimer(hMain,TM_RELOJ);
		/*DestroyWindow(hDlg);*/
		break;
	}
	case WM_DESTROY:
	{
		//aqui se guarda todo
		PostQuitMessage(0);
		break;;
	}
	}
	return 0;
}
BOOL CALLBACK hLoginFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		txbUser = GetDlgItem(hDlg, TB_USER);
		txbPass = GetDlgItem(hDlg, TB_PASS);
		HWND PICL= GetDlgItem(hDlg, PB_AppL);
		loadPic(PICL, Usuario.cpicA, 200, 120);
	}
	case WM_COMMAND:
	{

		switch (LOWORD(wParam))
		{
		case IDOK: {
			char userN[20] = { NULL };
			char passN[20] = { NULL };

			int lengthUser = GetWindowTextLength(txbUser) + 1;
			int lengthPass = GetWindowTextLength(txbPass) + 1;

			GetWindowTextA(txbUser, userN, lengthUser);
			GetWindowTextA(txbPass, passN, lengthPass);

			string sUseR(userN);
			string sPasS(passN);
	
			if (!Usuario.loadData()) {
				MessageBoxA(hLogin,"Error de archivo", "error", MB_ICONERROR);
			}
			else {
		
				if (!Usuario.compData(sUseR, sPasS)) {
					MessageBoxA(hLogin,"Usuario o contrasena incorrectos", "error", MB_ICONERROR);
					SendMessageA(GetDlgItem(hDlg, TB_USER), WM_SETTEXT, 0, (LPARAM)"");
					SendMessageA(GetDlgItem(hDlg, TB_PASS), WM_SETTEXT, 0, (LPARAM)"");
				}else {
				
					
					//PUEDO ESCONDER EL LOGIN SI QUIERO, PERO YA NO ME SIRVE, QUE SE MUERA
					ShowWindow(hMain, SW_SHOW);
					EndDialog(hLogin, 0);
				}
			}

			//Recuerda validar para un usuario, aunque sea una sola letra :3
			//Pd: TQM ;v

			break;
		}
		case BN_SALIR: {
			EndDialog(hDlg, 0);
			DestroyWindow(hMain);
			
			break;
		}
		}
		break;
	}

	case WM_CLOSE:
	{
	/*	DestroyWindow(hMain);*/
		EndDialog(hDlg, 0);
		break;
	}
	case WM_DESTROY:
	{
		break;
	}
	}
	return 0;
}
BOOL CALLBACK hMecsFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		//sin servicios no registras ni mecanicos
		if (Services.lista == NULL) {
			MessageBoxA(NULL, "No hay servicios. Registre uno primero", "Error", MB_ICONEXCLAMATION);
		}
		else {
			Services.aux = Services.lista;
			while (Services.aux != NULL) {
				SendMessageA(GetDlgItem(hDlg, CB_SERVM), CB_ADDSTRING, 0, (LPARAM)Services.aux->serv);
				Services.aux = Services.aux->next;
			}
			Services.aux = Services.origen;

			Mechanics.aux = Mechanics.lista;
			while (Mechanics.aux != NULL) {
				SendMessageA(GetDlgItem(hDlg, LB_MECS), LB_ADDSTRING, 0, (LPARAM)Mechanics.aux->name);
				Mechanics.aux = Mechanics.aux->next;
			}
			Mechanics.aux = Mechanics.origen;

			/*SendMessageA(GetDlgItem(hMecs, CB_SERVM), CB_SETCURSEL, -1, NULL);*/
		}

		EnableWindow(GetDlgItem(hDlg, BTN_EDITM), false);
		EnableWindow(GetDlgItem(hDlg, BTN_DELETEM), false);
		
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == CB_SERVM && HIWORD(wParam) == CBN_SELCHANGE) {
			char testServ[50];
		
			int selBIndex = (int)SendMessageA(GetDlgItem(hMecs, CB_SERVM), CB_GETCURSEL, 0, 0);
			if (selBIndex == -1) {

			}
			else {
				SendMessageA(GetDlgItem(hMecs, CB_SERVM), CB_GETLBTEXT, (WPARAM)selBIndex, (LPARAM)testServ);

				Servs::Node* var = Services.findItem(testServ);
				if (!Services.fullSpace(var)) {
					if (Services.freeSpace(var, 0) || Services.freeSpace(var, 1) || Services.freeSpace(var, 2)) {
						SendMessageA(GetDlgItem(hDlg, CB_SCHEDW), CB_ADDSTRING, 0, (LPARAM)"LUNES A VIERNES");
						if (Services.freeSpace(var, 1)) {
							SendMessageA(GetDlgItem(hDlg, CB_SCHEDH), CB_ADDSTRING, 0, (LPARAM)"8AM-12PM");
						}
						if (Services.freeSpace(var, 2)) {
							SendMessageA(GetDlgItem(hDlg, CB_SCHEDH), CB_ADDSTRING, 0, (LPARAM)"12PM-4PM");
						}
						if (Services.freeSpace(var, 3)) {
							SendMessageA(GetDlgItem(hDlg, CB_SCHEDH), CB_ADDSTRING, 0, (LPARAM)"4PM-8PM");
						}
					}
					if (Services.freeSpace(var, 3)) {
						SendMessageA(GetDlgItem(hMecs, CB_SCHEDW), CB_ADDSTRING, 0, (LPARAM)"SABADO");
						SendMessageA(GetDlgItem(hMecs, CB_SCHEDH), CB_ADDSTRING, 0, (LPARAM)"8AM-2PM");
					}
					SendMessageA(GetDlgItem(hDlg, CB_SCHEDW), CB_SETCURSEL, -1, NULL);
					SendMessageA(GetDlgItem(hDlg, CB_SCHEDH), CB_SETCURSEL, -1, NULL);
				}
				else {
					MessageBoxA(NULL, "El servicio ya no tiene mas espacios", "Error", MB_ICONEXCLAMATION);
					SendMessageA(GetDlgItem(hDlg, CB_SERVM), CB_SETCURSEL, -1, NULL);
					SendMessageA(GetDlgItem(hDlg, CB_SCHEDW), CB_SETCURSEL, -1, NULL);
					SendMessageA(GetDlgItem(hDlg, CB_SCHEDH), CB_SETCURSEL, -1, NULL);
				}
			}
		}
		else if (LOWORD(wParam) == LB_MECS && HIWORD(wParam) == LBN_SELCHANGE) {
			char testMec[50];

			int selIndex = (int)SendMessageA(GetDlgItem(hMecs, LB_MECS), LB_GETCURSEL, 0, 0);
			if (selIndex == -1) {

			}
			else {
#pragma region Deactivate buttons
				EnableWindow(GetDlgItem(hMecs, TB_NAMEM), false);
				EnableWindow(GetDlgItem(hMecs, TB_TELM), false);
				EnableWindow(GetDlgItem(hMecs, CB_SERVM), false);
				EnableWindow(GetDlgItem(hMecs, CB_SCHEDW), false);
				EnableWindow(GetDlgItem(hMecs, CB_SCHEDH), false);
				//BUTTONS deactivate
				EnableWindow(GetDlgItem(hMecs, BTN_LOADM), false);
				//buttons activate
				EnableWindow(GetDlgItem(hMecs, BTN_EDITM), true);
				EnableWindow(GetDlgItem(hMecs, BTN_DELETEM), true);




#pragma endregion


				SendMessageA(GetDlgItem(hMecs, LB_MECS), LB_GETTEXT, (WPARAM)selIndex, (LPARAM)testMec);
				found = Mechanics.findItem(testMec);
				if (found != NULL) {
#pragma region fillCombos
					Services.aux = Services.lista;
					while (Services.aux != NULL) {
						SendMessage(GetDlgItem(hDlg, CB_SERVM), CB_ADDSTRING, 0, (LPARAM)Services.aux->serv);
						Services.aux = Services.aux->next;
					}Services.aux = Services.lista;

					SendMessage(GetDlgItem(hDlg, CB_SCHEDW), CB_ADDSTRING, 0, (LPARAM)"LUNES A VIERNES");
					SendMessage(GetDlgItem(hDlg, CB_SCHEDW), CB_ADDSTRING, 0, (LPARAM)"SABADO");

					SendMessage(GetDlgItem(hDlg, CB_SCHEDH), CB_ADDSTRING, 0, (LPARAM)"8AM-12PM");
					SendMessage(GetDlgItem(hDlg, CB_SCHEDH), CB_ADDSTRING, 0, (LPARAM)"12PM-4PM");
					SendMessage(GetDlgItem(hDlg, CB_SCHEDH), CB_ADDSTRING, 0, (LPARAM)"4PM-8PM");
					SendMessage(GetDlgItem(hDlg, CB_SCHEDH), CB_ADDSTRING, 0, (LPARAM)"8AM-2PM");


#pragma endregion

					char limpia[20] = "";
					SendMessageA(GetDlgItem(hDlg, TB_NAMEM), WM_SETTEXT, 0, (LPARAM)found->name);
					SendMessageA(GetDlgItem(hDlg, TB_TELM), WM_SETTEXT, 0, (LPARAM)found->telephone);
					SendMessageA(GetDlgItem(hDlg, LBL_NUMEMP), WM_SETTEXT, 0, (LPARAM)found->idEmp);

					SetWindowTextA(GetDlgItem(hDlg, CB_SERVM),found->occupation);
					char weekSched[20] = { "" };
					char hourSched[20] = { "" };
					if (found->empSched.weekSche == 1) {
						strcpy(weekSched, "LUNES A VIERNES");

						if (found->empSched.hourSche == 1) {
							strcpy(hourSched, "8AM-12PM");
						}
						else if (found->empSched.hourSche == 2) {
							strcpy(hourSched, "12PM-4PM");
						}
						else if (found->empSched.hourSche == 3) {
							strcpy(hourSched, "4PM-8PM");
						}
					}
					else {
						strcpy(weekSched, "SABADO");
						if (found->empSched.hourSche == 4) {
							strcpy(hourSched, "8AM-2PM");
						}
					}

					SetWindowTextA(GetDlgItem(hDlg, CB_SCHEDW), weekSched);
					SetWindowTextA(GetDlgItem(hDlg, CB_SCHEDH), hourSched);
					
					SendMessage(GetDlgItem(hDlg, LBL_PIC), WM_SETTEXT, 0, (LPARAM)found->pic);
					SendMessage(GetDlgItem(hDlg, PC_MECH), WM_SETTEXT, sizeof(limpia), (LPARAM)limpia);
					loadPic(GetDlgItem(hDlg, PC_MECH),found->pic,50,50);
					
				}
				
			}
		}
		else if (LOWORD(wParam) == BTN_LOADM && HIWORD(wParam) == BN_CLICKED) {
			char PICBuff[MAX_PATH] = "";
			HWND PIC = GetDlgItem(hMecs, PC_MECH);
			examinar(hMecs, GetDlgItem(hMecs, LBL_PIC));
			GetDlgItemTextA(hDlg, LBL_PIC, PICBuff, MAX_PATH);
			loadPic(PIC, PICBuff, 50, 50);
		}
		else if (LOWORD(wParam) == BTN_EDITM && HIWORD(wParam) == BN_CLICKED) {
		EnableWindow(GetDlgItem(hMecs, TB_NAMEM), true);
		EnableWindow(GetDlgItem(hMecs, TB_TELM), true);
		EnableWindow(GetDlgItem(hMecs, CB_SERVM), true);
		EnableWindow(GetDlgItem(hMecs, CB_SCHEDW), true);
		EnableWindow(GetDlgItem(hMecs, CB_SCHEDH), true);
		EnableWindow(GetDlgItem(hMecs, BTN_LOADM), true);
		EnableWindow(GetDlgItem(hMecs, LB_MECS), false);
}
		else if (LOWORD(wParam) == BTN_DELETEM && HIWORD(wParam) == BN_CLICKED) {
		char tempname[50] = "";
		SendMessageA(GetDlgItem(hDlg, TB_NAMEM), WM_GETTEXT, sizeof(tempname), (LPARAM)tempname);
		found = Mechanics.findItem(tempname);
		Mechanics.delItem(found);

		char limpia[20] = "";
		SendMessageA(GetDlgItem(hDlg, TB_NAMEM), WM_SETTEXT, 0, (LPARAM)"");
		SendMessageA(GetDlgItem(hDlg, TB_TELM), WM_SETTEXT, 0, (LPARAM)"");

		SendMessage(GetDlgItem(hDlg, CB_SERVM), CB_SETCURSEL, -1, NULL);
		SendMessage(GetDlgItem(hDlg, CB_SCHEDW), CB_SETCURSEL, -1, NULL);
		SendMessage(GetDlgItem(hDlg, CB_SCHEDH), CB_SETCURSEL, -1, NULL);

		SendMessage(GetDlgItem(hDlg, LBL_PIC), WM_SETTEXT, sizeof(limpia), (LPARAM)limpia);
		SendMessage(GetDlgItem(hDlg, PC_MECH), WM_SETTEXT, sizeof(limpia), (LPARAM)limpia);

		EnableWindow(GetDlgItem(hDlg, LB_MECS), true);
		SendMessageA(GetDlgItem(hDlg, LB_MECS), LB_SETCURSEL, -1, NULL);
		EnableWindow(GetDlgItem(hMecs, BTN_DELETEM), false);
		EnableWindow(GetDlgItem(hMecs, BTN_EDITM), false);
}
		else if (LOWORD(wParam) == IDOK && HIWORD(wParam) == BN_CLICKED) {
			if (IsDlgButtonChecked(hMecs, RB_REGM) == BST_CHECKED && IsWindowEnabled(GetDlgItem(hMecs, BTN_EDITM)) == 0) {
				char tempname[50];
				char temphone[13];
				char tempserv[30];
				char tempwsched[30];
				char temphsched[20];
				char temppic[MAX_PATH];
				bool OK1=false, OK2 = false, OK3 = false, OK4 = false, OK5 = false, OK6 = false;

				//ok1

#pragma region NAME
				SendMessageA(GetDlgItem(hDlg, TB_NAMEM), WM_GETTEXT, sizeof(tempname), (LPARAM)tempname);
				int cont = 0;
				int j = 0;
				if (tempname[j] == '\0') {
					MessageBoxA(NULL, "El nombre esta vacio.", "ERROR", MB_ICONERROR);
				}
				else {
					while (tempname[j] != '\0')
					{
						if (((tempname[j] >= 97) && (tempname[j] <= 122)) || ((tempname[j] >= 65) && (tempname[j] <= 90)) || (tempname[j] == 32))
						{
							if (j == 0)
							{
								if ((tempname[j] >= 65) && (tempname[j] <= 90)) {
									tempname[j] = tempname[j];
								}

								else if ((tempname[j] >= 97) && (tempname[j] <= 122))
								{
									tempname[j] = tempname[j] - 32;
								}
								else if (tempname[j] == 32) {
									tempname[j] = tempname[j];
								}
							}
							else if (j > 0)
							{
								if ((tempname[j] >= 97) && (tempname[j] <= 122))
								{
									tempname[j] = tempname[j];
								}
								else if ((tempname[j] >= 65) && (tempname[j] <= 90))
								{
									tempname[j] = tempname[j] + 32;
								}
								else if (tempname[j] == 32) {
									tempname[j] = tempname[j];
								}

								if (tempname[j - 1] == 32 && (tempname[j] >= 65 && tempname[j] <= 90)) {
									tempname[j] = tempname[j];
								}
								else if (tempname[j - 1] == 32 && (tempname[j] >= 97 && tempname[j] <= 122)) {
									tempname[j] = tempname[j]-32;
								}

							}
							j++;
						}

						else {
							tempname[j] = tempname[j];
							cont = cont + 1;
							j++;
						}

					}
					if (cont > 0) {
						MessageBoxA(NULL, "El nombre solo debe tener letras", "ERROR", MB_ICONERROR);
						OK1 = false;
					}
					else {
						OK1 = true;
					}
				}

#pragma endregion

				//ok2

#pragma region SERVICE
				int indexS = SendMessageA(GetDlgItem(hMecs, CB_SERVM), CB_GETCURSEL, 0, 0);
				if (indexS == -1) {
					MessageBoxA(NULL, "Seleccione algo del combo", "Error", MB_OK);
					break;
				}
				else {
					/*Obtenemos el texto del combo y lo mostramos*/
					int lengthS = GetWindowTextLength(GetDlgItem(hMecs, CB_SERVM));
					GetWindowTextA(GetDlgItem(hMecs, CB_SERVM), tempserv, lengthS + 1);
					OK2 = true;
				}

#pragma endregion

				// ok3 & ok4 

#pragma region Schedule 

				int indexW = SendMessageA(GetDlgItem(hMecs, CB_SCHEDW), CB_GETCURSEL, 0, 0);
				if (indexW == -1) {
					MessageBoxA(NULL, "Seleccione algo del combo", "Error", MB_OK);
					OK3 = false;
					break;
				}
				else {
					/*Obtenemos el texto del combo y lo mostramos*/
					int lengthW = GetWindowTextLength(GetDlgItem(hMecs, CB_SCHEDW));
					GetWindowTextA(GetDlgItem(hMecs, CB_SCHEDW), tempwsched, lengthW + 1);
					OK3 = true;

					int indexH = SendMessageA(GetDlgItem(hMecs, CB_SCHEDH), CB_GETCURSEL, 0, 0);
					if (indexH == -1) {
						MessageBoxA(NULL, "Seleccione algo del combo", "Error", MB_OK);
						OK4 = false;
						break;
					}
					else {
						/*Obtenemos el texto del combo y lo mostramos*/
						int lengthH = GetWindowTextLength(GetDlgItem(hMecs, CB_SCHEDH));
						GetWindowTextA(GetDlgItem(hMecs, CB_SCHEDH), temphsched, lengthH + 1);

					}
					if (strcmp(tempwsched, "LUNES A VIERNES") == 0) {
						if (strcmp(temphsched, "8AM-12PM") == 0 || strcmp(temphsched, "12PM-4PM") == 0 || strcmp(temphsched, "4PM-8PM") == 0) {
							OK4 = true;
						}
						else {
							OK4 = false;
						}
					}
					else if (strcmp(tempwsched, "SABADO") == 0) {
						if (strcmp(temphsched, "8AM-2PM")==0) {
							OK4 = true;
						}
						else {
							OK4 = false;;
						}
					}
				}


#pragma endregion
				//ok5

#pragma region Phone
				SendMessageA(GetDlgItem(hMecs, TB_TELM), WM_GETTEXT, sizeof(temphone), (LPARAM)temphone);
				int cont3 = 0, f = 0;
				int car = 0;
				if (temphone[f] == '\0') {
					MessageBoxA(hDlg, "Llene la informacion", "Error", MB_ICONASTERISK);
				}
				else {
					while (temphone[f] != '\0') {
						if (isdigit(temphone[f])) {
							cont3 = cont3 + 1;
						}
						else {
							car = car + 1;
						}
						f++;
					}
				}
				if (car > 0) {
					MessageBoxA(hDlg, "El numero no debe contenerletras", "Error", MB_ICONERROR);
				}
				else if (cont3 > 0) {
					if (cont3 == 8) {
						OK5 = true;
					}
					else if (cont3 == 10) {
						OK5 = true;
					}
					else if (cont3 == 12) {
						OK5 = true;
					}
					else {
						MessageBoxA(hDlg, "El numero debe tener 8, 10 o 12 digitos", "Error", MB_ICONERROR);
						OK5 = false;
					}
				}
#pragma endregion

				//ok6
#pragma region Picture
				SendMessageA(GetDlgItem(hMecs, LBL_PIC), WM_GETTEXT, sizeof(temppic), (LPARAM)temppic);
				if (!(strcmp(temppic, "") == 0)) {
					OK6 = true;
				}
				else {
					MessageBoxA(hMecs, "No selecciono foto", "error", MB_ICONASTERISK);
					OK6 = false;
				}
#pragma endregion

				//si no fue edicion
				if (IsWindowEnabled(GetDlgItem(hMecs, BTN_EDITM)) == 0) {
					if (OK1 && OK2 && OK3 && OK4 && OK5 && OK6) {
						if (!Mechanics.lookDouble(tempname)) {
							//aqui se crea el nodo y todo feliz
							//aqui hay un error, no puedo crear el nodo 
							//por que no es de acceso publico
							Mechs::Nodo* nuevo = Mechanics.creItem(tempname, temppic, temphone, tempserv, tempwsched, temphsched);
							Mechanics.inItem(nuevo);
							Servs::Node* itemS = Services.findItem(tempserv);
							int option = 0;
							if (strcmp(temphsched, "8AM-12PM")) {
								option = 0;
							}
							else if (strcmp(temphsched, "12PM-4PM")) {
								option = 1;
							}
							else if (strcmp(temphsched, "4PM-8PM")) {
								option = 2;
							}
							else {
								option = 3;
							}
							bool done =Services.addMechs(nuevo, itemS, option);
							if (done) {
								MessageBoxA(hMecs, "MECANICO GUARDADO CON EXITO", "ALELUYA", MB_OK);
							}

							char limpia[20] = "";
							SendMessageA(GetDlgItem(hDlg, TB_NAMEM), WM_SETTEXT, 0, (LPARAM)"");
							SendMessageA(GetDlgItem(hDlg, TB_TELM), WM_SETTEXT, 0, (LPARAM)"");

							SendMessage(GetDlgItem(hDlg, CB_SERVM), CB_SETCURSEL, -1, NULL);
							SendMessage(GetDlgItem(hDlg, CB_SCHEDW), CB_SETCURSEL, -1, NULL);
							SendMessage(GetDlgItem(hDlg, CB_SCHEDH), CB_SETCURSEL, -1, NULL);

							SendMessage(GetDlgItem(hDlg, LBL_PIC), WM_SETTEXT, sizeof(limpia), (LPARAM)limpia);
							SendMessage(GetDlgItem(hDlg, PC_MECH), WM_SETTEXT, sizeof(limpia), (LPARAM)limpia);


							SendMessageA(GetDlgItem(hDlg, LB_MECS), LB_ADDSTRING, 0, (LPARAM)Mechanics.tail->name);
							SendMessageA(GetDlgItem(hDlg, LB_MECS), LB_SETCURSEL, -1, NULL);

						}
						else {
							MessageBoxA(hServs, "Error", "Ya existe mecánico en ese servicio", MB_ICONERROR);
						}
					}
					else {
						MessageBoxA(hMecs, "Algo hiciste mal", "error", MB_ICONERROR);
					}
				}
				//si si fue edicion
				else {
					if (OK1 && OK2 && OK3 && OK4 && OK5 && OK6) {
						
						int week = 0;
						int hour = 0;
						if (strcmp(tempwsched, "LUNES A VIERNES") == 0) {
							week = 1;
						}
						else if (strcmp(tempwsched, "SABADO") == 0) {
							week = 2;
						}

						if (strcmp(temphsched, "8AM-12PM") == 0) {
							hour = 0;
						}
						else if (strcmp(temphsched, "12PM-4PM") == 0) {
							hour = 1;
						}
						else if (strcmp(temphsched, "4PM-8PM") == 0) {
							hour = 2;
						}
						else if (strcmp(temphsched, "8AM-2PM") == 0) {
							hour = 3;
						}

						serFound = Services.findItem(tempserv);
						ser2Found= Services.findItem(found->occupation);
						bool regist = false;
						//el servicio es el mismo
						if (strcmp(found->occupation, tempserv) == 0) {
							//la semana es la misma
							if (found->empSched.weekSche == week) {
								//las horas son las mismas
								if (found->empSched.hourSche == hour) {
									//no se cambia nada uwu
								}
								//horas diferentes
								else {
									Services.deleteMechs(found, serFound, found->empSched.hourSche);
									regist=Services.addMechs(found, serFound, hour);
								}
							}//la semana es diferente
							else {
								Services.deleteMechs(found, serFound, found->empSched.hourSche);
								regist=Services.addMechs(found, serFound, hour);
							}
							
						}
						//el servicio es diferente
						else {
							Services.deleteMechs(found, ser2Found, found->empSched.hourSche);
							regist=Services.addMechs(found, serFound, hour);
						}

						Mechanics.editItem(found, tempname, temppic, temphone, tempserv, tempwsched, temphsched);
						
						char limpia[20] = "";
						SendMessageA(GetDlgItem(hDlg, TB_NAMEM), WM_SETTEXT, 0, (LPARAM)"");
						SendMessageA(GetDlgItem(hDlg, TB_TELM), WM_SETTEXT, 0, (LPARAM)"");

						SendMessage(GetDlgItem(hDlg, CB_SERVM), CB_SETCURSEL, -1, NULL);
						SendMessage(GetDlgItem(hDlg, CB_SCHEDW), CB_SETCURSEL, -1, NULL);
						SendMessage(GetDlgItem(hDlg, CB_SCHEDH), CB_SETCURSEL, -1, NULL);

						SendMessage(GetDlgItem(hDlg, LBL_PIC), WM_SETTEXT, sizeof(limpia), (LPARAM)limpia);
						SendMessage(GetDlgItem(hDlg, PC_MECH), WM_SETTEXT, sizeof(limpia), (LPARAM)limpia);


						SendMessageA(GetDlgItem(hDlg, LB_MECS), LB_SETCURSEL, -1, NULL);

						EnableWindow(GetDlgItem(hDlg, BTN_DELETEM), false);
						EnableWindow(GetDlgItem(hDlg, BTN_EDITM), false);

					}
					else {
						MessageBoxA(NULL, "NO SE NI QUE SALIO MAL, PERO ESTA MAL", "ERROR", MB_ICONERROR);
					}
				}

			}
		}
		else if (LOWORD(wParam) == IDCANCEL && HIWORD(wParam) == BN_CLICKED) {
		ShowWindow(hMain, SW_SHOW);
		EndDialog(hMecs, 0);
		
}
		return 0;
	}
	break;
	case WM_CLOSE:
	{
		ShowWindow(hMain, SW_SHOW);
		EndDialog(hMecs, 0);
		break;
	}
	case WM_DESTROY:
	{
		break;
	}
	}
	return 0;
}
BOOL CALLBACK hCitsFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{

	}
	case WM_COMMAND:
	{

		switch (LOWORD(wParam))
		{


		}
		return true;
	}
	break;
	case WM_CLOSE:
	{
		ShowWindow(hMain, SW_SHOW);
		EndDialog(hCits, 0);
		break;
	}
	case WM_DESTROY:
	{
		break;
	}
	}
	return 0;
}
BOOL CALLBACK hClientsFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{

	}
	case WM_COMMAND:
	{

		switch (LOWORD(wParam))
		{


		}
		return 0;
	}
	break;
	case WM_CLOSE:
	{
		ShowWindow(hMain, SW_SHOW);
		EndDialog(hClients, 0);
		break;
	}
	case WM_DESTROY:
	{
		ShowWindow(hMain, SW_SHOW);
		EndDialog(hClients, 0);
		break;
	}
	}
	return 0;
}
BOOL CALLBACK hServsFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		//aqui se deben cargar las especialidades

		Services.aux = Services.lista;
		while (Services.aux != NULL) {
			SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_ADDSTRING, 0, (LPARAM)Services.aux->serv);
			Services.aux = Services.aux->next;
		}
		Services.aux = Services.origen;
	}
	case WM_COMMAND:
	{

	
		if (LOWORD(wParam) == BN_OKESP && HIWORD(wParam) == BN_CLICKED) {
			if (IsDlgButtonChecked(hServs, RB_REGIST) == BST_CHECKED) {
				bool fail = true;
				bool OK1 = false, OK2 = false, OK3 = false;
				char tempname[30] = { "" };
				char tempdesc[300] = { "" };
				char tempcost[10] = { "" };
				//validacion de la especialidad y de su descripcion

					//obtener nombre de la especialidad

				SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_GETTEXT, sizeof(tempname), (LPARAM)tempname);
				//obtener la descripcion
				SendMessageA(GetDlgItem(hDlg, TB_DESC), WM_GETTEXT, sizeof(tempdesc), (LPARAM)tempdesc);
				//obtener el precio
				SendMessageA(GetDlgItem(hDlg, TB_COST), WM_GETTEXT, sizeof(tempcost), (LPARAM)tempcost);
				//valida el nombre
				int cont = 0;
				int j = 0;
				if (tempname[j] == '\0') {
					MessageBoxA(NULL, "El nombre esta vacio.", "ERROR", MB_ICONERROR);
				}
				else {
					while (tempname[j] != '\0')
					{
						if (((tempname[j] >= 97) && (tempname[j] <= 122)) || ((tempname[j] >= 65) && (tempname[j] <= 90)) || (tempname[j] == 32))
						{
							if (j == 0)
							{
								if ((tempname[j] >= 65) && (tempname[j] <= 90)) {
									tempname[j] = tempname[j];
								}

								else if ((tempname[j] >= 97) && (tempname[j] <= 122))
								{
									tempname[j] = tempname[j] - 32;
								}
								else if (tempname[j] == 32) {
									tempname[j] = tempname[j];
								}
							}
							else if (j > 0)
							{
								if ((tempname[j] >= 97) && (tempname[j] <= 122))
								{
									tempname[j] = tempname[j];
								}
								else if ((tempname[j] >= 65) && (tempname[j] <= 90))
								{
									tempname[j] = tempname[j] + 32;
								}
								else if (tempname[j] == 32) {
									tempname[j] = tempname[j];
								}

							}
							j++;
						}

						else {
							tempname[j] = tempname[j];
							cont = cont + 1;
							j++;
						}

					}
					if (cont > 0) {
						MessageBox(NULL, (LPCWSTR)"El nombre solo debe tener letras", (LPCWSTR)"ERROR", MB_ICONERROR);
						OK2 = false;
					}
					else {
						OK1 = true;
					}
				}
				//valida la descripcion
				int cont1 = 0;
				int k = 0;
				if (tempdesc[k] == '\0') {
					MessageBoxA(NULL, "Descripcion vacía", "ERROR", MB_ICONERROR);
				}
				else {
					while (tempdesc[k] != '\0')
					{
						if (((tempdesc[k] >= 97) && (tempdesc[k] <= 122)) || ((tempdesc[k] >= 65) && (tempdesc[k] <= 90)) || (tempdesc[k] == 32))
						{
							if (k == 0)
							{
								if ((tempdesc[k] >= 65) && (tempdesc[k] <= 90)) {
									tempdesc[k] = tempdesc[k];
								}

								else if ((tempdesc[k] >= 97) && (tempdesc[k] <= 122))
								{
									tempdesc[k] = tempdesc[k] - 32;
								}
								else if (tempdesc[k] == 32) {
									tempdesc[k] = tempdesc[k];
								}
							}
							else if (k > 0)
							{
								if ((tempdesc[k] >= 97) && (tempdesc[k] <= 122))
								{
									tempdesc[k] = tempdesc[k];
								}
								else if ((tempdesc[k] >= 65) && (tempdesc[k] <= 90))
								{
									tempdesc[k] = tempdesc[k] + 32;
								}
								else if (tempdesc[k] == 32) {
									tempdesc[k] = tempdesc[k];
								}

							}
							k++;
						}

						else {
							tempdesc[k] = tempdesc[k];
							cont1 = cont1 + 1;
							k++;
						}

					}
					if (cont1 > 0) {
						MessageBox(NULL, (LPCWSTR)"El nombre solo debe tener letras", (LPCWSTR)"ERROR", MB_ICONERROR);
						OK2 = false;
					}
					else {
						OK2 = true;
					}
				}
				//valida el costo
				int cont2 = 0;
				int point = 0;
				int car2 = 0;
				int large = strlen(tempcost);
				int l = 0;
				if (tempcost[l] == '\0') {
					MessageBoxA(NULL, "El costo esta vacio.", "ERROR", MB_ICONERROR);
				}
				else {
					while (tempcost[l] != '\0') {
						if (isdigit(tempcost[l])) {
							cont2 = cont2 + 1;
						}
						else if (tempcost[l] == 46) {
							point = point + 1;
						}
						else {
							car2 = car2 + 1;
						}
						l++;
					}
					if (car2 > 0) {
						MessageBoxA(hDlg, "El precio no puede contener letras", "Error", MB_ICONERROR);
					}
					else if (large > 0 && large < 10) {
						if (cont2 > 0) {
							if (point < 2) {
								float tmpCost=stof(tempcost);
								if (tmpCost > 0) {
									OK3 = true;
								}
								else {
									MessageBoxA(hDlg, "El precio debe ser mayor a 0", "Error", MB_ICONERROR);
									OK3 = false;
								}
							}
							else if (point > 1) {
								MessageBoxA(hDlg, "El precio contiene mas de un punto decimal", "Error", MB_ICONERROR);
								OK3 = false;
							}
						}
					}
					else {
						MessageBoxA(hDlg, "El precio tiene muchos digitos", "Error", MB_ICONERROR);
						OK3 = false;
					}

				}

			

			if (OK1 && OK2&&OK3)
				fail = false;
			else
				fail = true;

		
					if (!fail) {
						//SI NO FUE EDICION
						if (IsWindowEnabled(GetDlgItem(hMecs, BTN_EDITM)) == 0) {
							if (!Services.lookDouble(tempname)) {
								//aqui se crea el nodo y todo feliz
								//aqui hay un error, no puedo crear el nodo 
								//por que no es de acceso publico
								Servs::Node* nuevo = Services.creItem(tempname, tempdesc, tempcost);
								Services.inItem(nuevo);
								MessageBoxA(hDlg, "SERVICIO GUARDADO CON EXITO", "ALELUYA", MB_OK);
								SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, TB_DESC), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, TB_COST), WM_SETTEXT, 0, (LPARAM)"");


								SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_ADDSTRING, 0, (LPARAM)Services.tail->serv);

							}
							else {
								MessageBoxA(hServs, "Error", "Ya existe ese servicio", MB_ICONERROR);
							}
						}
						//si si fue edición
						else {
							serFound = Services.findItem(tempname);
							if (serFound != NULL) {
								if (Services.emptySpaceM(serFound)) {
									Services.editItem(serFound, tempname, tempdesc, tempcost);

									SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_SETTEXT, 0, (LPARAM)"");
									SendMessageA(GetDlgItem(hDlg, TB_DESC), WM_SETTEXT, 0, (LPARAM)"");
									SendMessageA(GetDlgItem(hDlg, TB_COST), WM_SETTEXT, 0, (LPARAM)"");
									MessageBoxA(NULL, "YA SE EDITO", "ENHORABUENA", MB_OK);

									EnableWindow(GetDlgItem(hDlg, BN_EDIT), false);
									EnableWindow(GetDlgItem(hDlg, BTN_DELETES), false);
									EnableWindow(GetDlgItem(hDlg, LB_SERV), true);
									SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_SETCURSEL, -1, NULL);


								}
								else {
									MessageBoxA(NULL, "NO SE PUEDE EDITAR SI TIENE MECANICOS", "ERROR", MB_ICONERROR);
								}
							}
						}
					}else { //si si fall+o, se indica donde
						
						MessageBoxA(hServs, "EN ALGO TE EQUIVOCASTE", "ERROR", MB_ICONERROR);

					}

			
			}
	
		}
		if (LOWORD(wParam) == BN_EDIT && HIWORD(wParam) == BN_CLICKED) {
			EnableWindow(GetDlgItem(hDlg, TB_NAME), true);
			EnableWindow(GetDlgItem(hDlg, TB_DESC), true);
			EnableWindow(GetDlgItem(hDlg, TB_COST), true);
			EnableWindow(GetDlgItem(hDlg, LB_SERV), false);

		}
		if (LOWORD(wParam) == LB_SERV && HIWORD(wParam) == LBN_SELCHANGE) {
			char testServ[50];

			int selIndex = (int)SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_GETCURSEL, 0, 0);
			if (selIndex == -1) {

			}
			else {
#pragma region Deactivate buttons
				EnableWindow(GetDlgItem(hDlg, TB_NAME), false);
				EnableWindow(GetDlgItem(hDlg, TB_DESC), false);
				EnableWindow(GetDlgItem(hDlg, TB_COST), false);
				//buttons activate
				EnableWindow(GetDlgItem(hDlg, BN_EDIT), true);
				EnableWindow(GetDlgItem(hDlg, BTN_DELETES), true);




#pragma endregion


				SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_GETTEXT, (WPARAM)selIndex, (LPARAM)testServ);
				serFound = Services.findItem(testServ);
				if (serFound != NULL) {
					SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_SETTEXT, 0, (LPARAM)serFound->serv);
					SendMessageA(GetDlgItem(hDlg, TB_DESC), WM_SETTEXT, 0, (LPARAM)serFound->desc);
					SendMessageA(GetDlgItem(hDlg, TB_COST), WM_SETTEXT, 0, (LPARAM)serFound->costSer);
				}

			}
		}
		if (LOWORD(wParam) == BTN_DELETES && HIWORD(wParam) == BN_CLICKED) {
			char tempname[50] = "";
			SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_GETTEXT, sizeof(tempname), (LPARAM)tempname);
			serFound = Services.findItem(tempname);
			bool empty=Services.emptySpaceM(serFound);
			if (empty) {
				Services.delItem(serFound);
			}
			else {
				MessageBoxA(NULL, "NO PUEDES ELIMINAR SI HAY MECANICOS REGISTRADOS", "ERROR", MB_ICONERROR);
			}
			char limpia[20] = "";
			SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_SETTEXT, 0, (LPARAM)"");
			SendMessageA(GetDlgItem(hDlg, TB_DESC), WM_SETTEXT, 0, (LPARAM)"");
			SendMessageA(GetDlgItem(hDlg, TB_COST), WM_SETTEXT, 0, (LPARAM)"");
			EnableWindow(GetDlgItem(hDlg, LB_SERV), true);
			SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_SETCURSEL, -1, NULL);
		}
		if (LOWORD(wParam) ==BN_CANCELESP && HIWORD(wParam) == BN_CLICKED) {
			ShowWindow(hMain, SW_SHOW);
			EndDialog(hServs, 0);

		}
		
	}
	break;
	case WM_CLOSE:
	{
		ShowWindow(hMain, SW_SHOW);
		EndDialog(hServs, 0);
		break;
	}
	case WM_DESTROY:
	{
	
		break;
	}
	}
	return 0;
}
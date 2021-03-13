#ifndef  _clases_
#define _clases_


#include"mechs.h"

int folioBase = 100000;




class Servs
{
public:

	struct Node
	{
		int ID;
		char serv[30];
		char desc[300];
		float costSer=0;
		struct sched {
			int weekSche=0;
			int hourSche=0;
		}ConEsp[4];
		Node* next;
		Node* prev;
		Mechs::Nodo* mechs[4] = { 0,0,0,0 };
		}*lista,*origen,*tail ,* aux;
	fstream servArchive;

	//-------------------------------------------------
	Servs()
	{
		lista=tail=origen = aux = NULL;
		//aqui se cargan las especialidades del archivo binario
		readArchive();
	}
	Node *creItem(char esps[],char desc[],char cost[]) {
		Node* nuevo_nodo = new Node;
		strcpy_s(nuevo_nodo->serv, esps);
		strcpy_s(nuevo_nodo->desc, desc);
		nuevo_nodo->costSer = stof(cost);

		nuevo_nodo->ID = folioBase;
		folioBase++;

		//agregar el horario semanal 1=lun a viernes, 2 el sabado
		for (int i = 0; i < 3; i++) {
			nuevo_nodo->ConEsp[i].weekSche = 1;
		}
		nuevo_nodo->ConEsp[3].weekSche = 2;
		//como recien se creo la eservialidad, los horarios estan vacios
		//cuando se llenen 1 es de 8am -12pm , 2 es de 12-4 pm y 3 es de 4-8pm
		//de ser 2, el horario es el sabado de 8am-2pm con el numero 4
		for (int i = 0; i < 4; i++) {
			nuevo_nodo->ConEsp[i].hourSche = 0;
		}

		nuevo_nodo->prev = NULL;
		nuevo_nodo->next = NULL;

		//nomas va a haber 1 espacio de trabajo por cada especialidad, nada de 2 mecanicos
		//con la misma especialidad, atendiendo el mismo dia y horario, en lados diferentes
		//no caben
		return nuevo_nodo;
	}
	void editItem(Node*& edit,char esps[], char desc[], char cost[]) {

	}
	bool addMechs(Mechs::Nodo*&mechs, Servs::Node*& ptr,int x) {
		if (freeSpace(ptr,x)) {
			for (int i = 0; i < 4; i++) {
				if (x == i) {
					ptr->mechs[i] = mechs;
					return true;
				}
			}
		}
		else {
			return false;
		}
	}
	void deleteMechs(Mechs::Nodo*& mechs, Servs::Node*& ptr,int x) {
		for (int i =0 ; i < 4; i++) {
			if (x == i && ptr->mechs[i] != NULL) {
				ptr->mechs[i] = NULL;
				break;
			}
		}
	}
	bool freeSpace(Servs::Node*&var, int x) {
		if (var->mechs[x] == NULL) {
			return true;
		}
		return false;
	}
	bool fullSpace(Servs::Node*& var) {
		int cont = 0;
		for (int i = 0; i < 4; i++) {
			if (var->mechs[i]!=NULL) {
				cont++;
			}
		}
		if (cont == 4) {
			return true;
		}
		else {
			return false;
		}
	}
	bool emptySpaceM(Servs::Node*& var) {
		int cont = 0;
		for (int i = 0; i < 4; i++) {
			if (var->mechs[i] == NULL) {
				cont++;
			}
		}
		if (cont == 4) {
			return true;
		}
		else {
			return false;
		}
	}
	void inItem(Node *&nuevo)
	{
	
		aux = origen;
		if (origen == NULL)
		{
			nuevo->next = NULL;
			this->origen = nuevo;
			this->tail = nuevo;
			this->lista = origen;
		}
		else
		{
			//esto es por si quiero insertar ordenado por nombre

//#pragma region Ordenamiento
//			while (aux->next != NULL && nuevo->serv[0] <= aux->serv[0]) {
//
//				aux = aux->next;
//			}
//			//si es el primer elemento
//			if (aux = origen) {
//				nuevo->prev = NULL;
//				nuevo->next = aux;
//				aux->prev = nuevo;
//				origen = nuevo;
//			}
//			//caso 2: nada de la lista es mayor al nuevo nodo. 
//			else if (aux->next == NULL) {
//				nuevo->prev = aux;
//				nuevo->next = NULL;
//				aux->next = nuevo;
//			}
//			else {
//				//caso 3: Hay algo mayor que el nuevo nodo y se coloca en medio
//				nuevo->prev = aux->prev;
//				nuevo->next = aux->next;
//				aux->next->prev = nuevo;
//				aux->prev->next = nuevo;
//			}
//#pragma endregion

			//insertar todo al final de la lista
			this->tail->next = nuevo;
			nuevo->prev =this-> tail;
			nuevo->next = NULL;
			this->tail = nuevo;
		}
	}
	bool lookDouble(char servName[]) {
		if (!ServsIsEmpty()) {
			aux = origen;
			while (aux->next != NULL) {
				if (strcmp(aux->serv, servName) == 0) {
					return true;
				}
				else {
					aux = aux->next;
				}
			}
			return false;
		}
		else {
			//si esta vacio, no hay nada que hacer
			return false;
		}
		
	}
	Node *findItem(char servName[]) {
		if (!ServsIsEmpty()) {
			aux = origen;
			while (aux!= NULL) {
				if (strcmp(aux->serv, servName) == 0) {
					return aux;
				}
				else {
					aux = aux->next;
				}
			}
			return NULL;
		}
		else {
			//si esta vacio, no hay nada que hacer
			return NULL;
		}
	}
	bool ServsIsEmpty() {
		if (lista == NULL) {
			return true;
		}
		else {
			return false;
		}
	}

	void writeArchive() {
		servArchive.open("Data\\servs.bin", ios::out | ios::trunc | ios::binary);
		if (!servArchive.is_open()) {
			MessageBoxA(NULL, "No se cargo el archivo", "error", MB_ICONASTERISK);
		}
		else {
			Node* temp = origen;
			while (temp != NULL) {
				servArchive.write(reinterpret_cast<char*> (temp), sizeof(Node));
				temp = temp->next;
			}
		}
		servArchive.close();
		MessageBoxA(NULL, "Guardado de servicios exitoso", "ok", MB_OK);
	}
	void readArchive() {
		servArchive.open("Data\\servs.bin", ios::in | ios::binary | ios::ate);
		if (!servArchive.is_open()) {
			aux = origen = NULL;
			MessageBoxA(NULL, "No se cargo el archivo", "error", MB_ICONASTERISK);
			servArchive.close();
		}
		else {
			int size = servArchive.tellg();
			if (size == 0) {
				MessageBoxA(NULL, "Archivo de citas vacio", "error", MB_ICONASTERISK);

			}
			for (unsigned int i = 0; i < (size / sizeof(Node)); i++) {

				if (origen == NULL) {
					origen = new Node;
					aux = origen;
					aux->prev = NULL;
					lista = origen;
				}
				else {
					while (aux->next != NULL)
						aux = aux->next;
					aux->next = new Node;
					aux->next->prev = aux;
					aux = aux->next;
				}

				Node* Temp = new Node;
				servArchive.seekg(i * sizeof(Node));
				servArchive.read(reinterpret_cast<char*>(Temp), sizeof(Node));


				aux->ID = Temp->ID;
				for (int f = 0; f < 4; f++) {
					aux->ConEsp[f] = Temp->ConEsp[f];
				}
				aux->costSer = Temp->costSer;
				strcpy(aux->desc, Temp->desc);
				for (int f = 0; f < 4; f++) {
					aux->mechs[f] = Temp->mechs[f];
				}
				strcpy(aux->serv, Temp->serv);

				delete reinterpret_cast<char*>(Temp);

				aux->next = NULL;
				aux = origen;
			}
			servArchive.close();
			//aqui hay que reemplazar el global id con el ultimo id
			aux = origen;
			while (aux != NULL) {
				if (aux->next == NULL) {
					folioBase = aux->ID;
					folioBase++;
				}
				aux = aux->next;
			}
			aux = origen;
			MessageBoxA(NULL, "Cargado de servicios exitoso", "OK", MB_OK);
		}
	}
	void delItem(Node*& var)
	{
		Node* auxD = var;

		if ((auxD->prev == NULL) && (auxD->next == NULL)) /*Caso de unico elemento*/ {
			delete auxD;
			origen = NULL;
		}
		else if (auxD->next == NULL) /*Caso de ultimo elemento*/ {
			auxD->prev->next = NULL;
			delete auxD;
		}
		else if (auxD->prev == NULL) /*Caso de primer elemento*/ {
			auxD->next->prev = NULL;
			origen = auxD->next;
			delete aux;
		}
		else { /*cualquier otro*/
			auxD->next->prev = auxD->prev;
			auxD->prev->next = auxD->next;
			delete auxD;
		}
	}
	~Servs()
	{
		writeArchive();
		aux = lista;
		while (aux!= NULL) {
			Node* borr = aux;
			aux = aux->next;
			delete borr;
		}
	}
}Services;


class User {
private:
	
	string username="";
	string pass="";
	ifstream miTxt;
	string name = "";
	string picUs = "";
	string picApp = "";
public:
	char cUser[30];
	char cpicU[MAX_PATH];
	char cpicA[MAX_PATH];
	User() {
		//aqui se crea y se carga
		name = "";
		username = "";
		pass = "";

	}
	bool loadData() {
		miTxt.open("login.txt");
		if (!miTxt.is_open()) {
			/*miTxt.close();*/
			return false;
		}
		else {
			getline(miTxt, this->name);
			getline(miTxt, this->username);
			getline(miTxt,this-> pass);
			getline(miTxt,this-> picUs);
			getline(miTxt,this-> picApp);
			miTxt.close();
			strcpy(this->cUser, name.c_str());
			strcpy(this->cpicU, picUs.c_str());
			strcpy(this->cpicA, picApp.c_str());
			return true;
		}
	}
	bool compData(string user,string passe) {
		if (user.compare(username) == 0 && passe.compare(pass) == 0) {

			return true;
		}
		else {
			return false;
		}
	}
	void copyData(string&name,string&username,string&pass) {
		name = this->name;
		username = this->username;
		pass = this->pass;
	}
	/*bool saveData() {
		miTxt.open("login.txt",ios::out,ios::trunc);
		if (!miTxt.is_open()) {
			miTxt.close();
			return false;
		}
		else {
			miTxt << name<<endl;
			miTxt << username<<endl;
			miTxt << pass<<endl;

			miTxt.close();
			return true;
		}
	}*/
	~User() {
		//aqui se guarda y destruye
	}
}Usuario;

void examinar(HWND Ventana, HWND hDestino)
{

	char Empty[MAX_PATH] = { "" };


	OPENFILENAME ofn;
	/*OPENFILENAME ofn;*/       // common dialog box structure
	char szFile[260];       // buffer for file name
							//HANDLE hf;            // file handle

							// Propiedades OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Ventana;
	ofn.lpstrFile = (LPWSTR)szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = (LPCWSTR)"Imagenes bmp\0*.bmp\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == TRUE)
	{

		SendMessage(hDestino, WM_SETTEXT, 0, (LPARAM)ofn.lpstrFile);
		//folder = ofn.lpstrFile;
	}
	else
	{
		MessageBoxA(Ventana, "No eligio un archivo", "Aviso", MB_ICONEXCLAMATION);
		//folder = Empty;
		SendMessage(hDestino, WM_SETTEXT, 0, (LPARAM)"");
	}
}
void loadPic(HWND hImagen, char filebmp[MAX_PATH], int x, int y) {

	static HBITMAP bmp1;
	static HBITMAP bmp2;

	bmp1 = (HBITMAP)SendMessage(hImagen, STM_GETIMAGE, IMAGE_BITMAP, 0);
	bmp2 = (HBITMAP)LoadImageA(NULL, filebmp, IMAGE_BITMAP, x, y, LR_LOADFROMFILE);
	SendMessage(hImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);

}
#endif
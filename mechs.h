#ifndef  _mechs_
#define _mechs_
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#pragma warning(disable : 4996) 
using namespace std;
int folioEmp = 700000;

class Mechs {
public:
	struct Nodo
	{
		int idEmp;
		char name[70];
		char pic[300];
		char telephone[12];
		char occupation[30];
		struct sched {
			int weekSche = 0;
			int hourSche = 0;
		}empSched;
		Nodo* next;
		Nodo* prev;
	}*lista, * origen, * tail, * aux;
	fstream mechsArch;
	Mechs() {
		lista = tail = origen = aux = NULL;
		readArchive();
	}
	Nodo* creItem(char name[], char pic[], char tel[], char ocup[], char wsched[], char hsched[]) {
		Nodo* nuevo_nodo = new Nodo;
		strcpy_s(nuevo_nodo->name, name);
		strcpy_s(nuevo_nodo->pic, pic);
		strcpy_s(nuevo_nodo->telephone, tel);
		strcpy_s(nuevo_nodo->occupation, ocup);

		nuevo_nodo->idEmp = folioEmp;
		folioEmp++;

		//agregar el horario semanal 1=lun a viernes, 2 el sabado
		if (strcmp(wsched, "LUNES A VIERNES") == 0) {
			nuevo_nodo->empSched.weekSche = 1;
		}
		else {
			nuevo_nodo->empSched.weekSche = 2;
		}

		//como recien se creo la eservialidad, los horarios estan vacios
		//cuando se llenen 1 es de 8am -12pm , 2 es de 12-4 pm y 3 es de 4-8pm
		//de ser 2, el horario es el sabado de 8am-2pm con el numero 4
		if (strcmp(hsched, "8AM-12PM")) {
			nuevo_nodo->empSched.hourSche = 1;
		}
		else if (strcmp(hsched, "12PM-4PM")) {
			nuevo_nodo->empSched.hourSche = 2;
		}
		else if (strcmp(hsched, "4PM-8PM")) {
			nuevo_nodo->empSched.hourSche = 3;
		}
		else {
			nuevo_nodo->empSched.hourSche = 4;
		}

		nuevo_nodo->prev = NULL;
		nuevo_nodo->next = NULL;

		//nomas va a haber 1 espacio de trabajo por cada especialidad, nada de 2 mecanicos
		//con la misma especialidad, atendiendo el mismo dia y horario, en lados diferentes
		//no caben
		return nuevo_nodo;
	}
	void editItem(Nodo*& edit,char name[], char pic[], char tel[], char ocup[], char wsched[], char hsched[]) {
		if (strcmp(edit->name, name) == 0) {
			//keeps the same
		}
		else {
			strcpy(edit->name, name);
		}

		if (strcmp(edit->pic, pic) == 0) {

		}
		else {
			strcpy(edit->pic, pic);
		}

		if (strcmp(edit->telephone, tel) == 0) {

		}
		else {
			strcpy(edit->telephone, tel);
		}

		if (strcmp(wsched, "LUNES A VIERNES") == 0) {
			edit->empSched.weekSche = 1;
		}
		else {
			edit->empSched.weekSche = 2;
		}

		//como recien se creo la eservialidad, los horarios estan vacios
		//cuando se llenen 1 es de 8am -12pm , 2 es de 12-4 pm y 3 es de 4-8pm
		//de ser 2, el horario es el sabado de 8am-2pm con el numero 4
		if (strcmp(hsched, "8AM-12PM")) {
			edit->empSched.hourSche = 1;
		}
		else if (strcmp(hsched, "12PM-4PM")) {
			edit->empSched.hourSche = 2;
		}
		else if (strcmp(hsched, "4PM-8PM")) {
			edit->empSched.hourSche = 3;
		}
		else {
			edit->empSched.hourSche = 4;
		}




		

	}
	void inItem(Nodo*& nuevo)
	{

		aux = origen;
		if (origen == NULL)
		{
			origen = nuevo;
			nuevo->next = NULL;
			tail = nuevo;
			lista = origen;

		}
		else
		{
			//insertar todo al final de la lista
			tail->next = nuevo;
			nuevo->prev = tail;
			nuevo->next = NULL;
			tail = nuevo;
		}
	}
	bool MechsIsEmpty() {
		if (lista == NULL) {
			return true;
		}
		else {
			return false;
		}
	}
	bool lookDouble(char mechName[]) {
		if (!MechsIsEmpty()) {
			aux = origen;
			while (aux->next != NULL) {
				if (strcmp(aux->name, mechName) == 0) {
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
	Nodo* findItem(char mecName[]) {
		if (!MechsIsEmpty()) {
			aux = origen;
			while (aux != NULL) {
				if (strcmp(aux->name, mecName) == 0) {
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
    void  delItem(Nodo*&mec)
	{
		Nodo*auxD = mec;

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
	void writeArchive() {
		mechsArch.open("Data\\mechs.bin", ios::out | ios::trunc | ios::binary);
		if (!mechsArch.is_open()) {
			MessageBoxA(NULL, "No se cargo el archivo", "error", MB_ICONASTERISK);
		}
		else {
			Nodo* temp = origen;
			while (temp != NULL) {
				mechsArch.write(reinterpret_cast<char*> (temp), sizeof(Nodo));
				temp = temp->next;
			}
		}
		mechsArch.close();
		MessageBoxA(NULL, "Guardado de servicios exitoso", "ok", MB_OK);
	}
	void readArchive() {
		mechsArch.open("Data\\mechs.bin", ios::in | ios::binary | ios::ate);
		if (!mechsArch.is_open()) {
			aux = origen = NULL;
			MessageBoxA(NULL, "No se cargo el archivo", "error", MB_ICONASTERISK);
			mechsArch.close();
		}
		else {
			int size = mechsArch.tellg();
			if (size == 0) {
				MessageBoxA(NULL, "Archivo de mecanicos vacio", "error", MB_ICONASTERISK);

			}
			for (unsigned int i = 0; i < (size / sizeof(Nodo)); i++) {

				if (origen == NULL) {
					origen = new Nodo;
					aux = origen;
					aux->prev = NULL;
					lista = origen;
				}
				else {
					while (aux->next != NULL)
						aux = aux->next;
					aux->next = new Nodo;
					aux->next->prev = aux;
					aux = aux->next;
				}

				Nodo* Temp = new Nodo;
				mechsArch.seekg(i * sizeof(Nodo));
				mechsArch.read(reinterpret_cast<char*>(Temp), sizeof(Nodo));


				aux->idEmp = Temp->idEmp;
				strcpy(aux->name, Temp->name);
				strcpy(aux->occupation, Temp->occupation);
				strcpy(aux->pic, Temp->pic);
				strcpy(aux->telephone, Temp->telephone);
				aux->empSched.weekSche = Temp->empSched.weekSche;
				aux->empSched.hourSche = Temp->empSched.hourSche;

				delete reinterpret_cast<char*>(Temp);

				aux->next = NULL;
				aux = origen;
			}
			mechsArch.close();
			//aqui hay que reemplazar el global id con el ultimo id
			aux = origen;
			while (aux != NULL) {
				if (aux->next == NULL) {
					folioEmp = aux->idEmp;
					folioEmp++;
				}
				aux = aux->next;
			}
			aux = origen;
			MessageBoxA(NULL, "Cargado de servicios exitoso", "OK", MB_OK);
		}
	}
	~Mechs() {
		writeArchive();
		aux = lista;
		while (aux != NULL) {
			Nodo* borr = aux;
			aux = aux->next;
			delete borr;
		}

	}


}Mechanics;

#endif


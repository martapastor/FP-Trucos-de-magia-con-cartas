//Práctica 2 versión 3.0
//Trucos de magia con cartas
//Por Marta Pastor y Ángela Martínez

#include <iostream> //Biblioteca básica estándar
#include <string>	//Biblioteca para cadenas de caracteres
#include <fstream> //Biblioteca para flujo de datos entre archivos
#include <ctime> //Biblioteca para seguimiento del tiempo
#include <cstdlib> //Biblioteca para elección de datos aleatorios
#include <iomanip> //Biblioteca para ajustes estéticos en la consola
using namespace std;	  

typedef enum { Picas /*0*/, Treboles /*1*/, Diamantes /*2*/, Corazones /*3*/ } tPalo;
typedef enum { A /*0*/, n2 /*1*/, n3 /*2*/, n4 /*3*/, n5 /*4*/, n6 /*5*/, n7 /*6*/, n8 /*7*/, n9 /*8*/, n10 /*9*/, J /*10*/, Q /*11*/, K /*12*/ } tNumero;
const int CARTASxPALO = 13;

const char marcaFin = 'x';
const int MAX_CARTAS = 52;
typedef int tCarta;
typedef tCarta tMazo[MAX_CARTAS + 1];

void configConsola();
int menu();
tPalo palo (tCarta carta);
tNumero numero (tCarta carta);
void mostrar (tCarta carta);
void vacio (tMazo mazo);
int cuantas (const tMazo mazo);
void mostrar (const tMazo mazo);
bool abrir (string &nomb, ifstream &archivo);
bool cargar (tMazo mazo, string &nomb);
bool guardar (const tMazo mazo, string &nomb);
void barajar (tMazo mazo);
bool unir (tMazo mazo, const tMazo otroMazo);
bool partir (tMazo mazo, int cuantasCoger, tMazo otroMazo);
void cortar (tMazo mazo, int cuantas);
void repartirNegroRojo (const tMazo mazo, tMazo mazoNegro, tMazo mazoRojo);
void repartirBajaAlta (const tMazo mazo, tMazo mazoBajas, tMazo mazoAltas);
void repartirIntercalando (const tMazo mazo, int enCuantos, int queMazo, tMazo &mazoNuevo);
void repartirMontones (const tMazo mazo, int enCuantos, tMazo &monton);
void trucoTresMontones(tMazo mazo, string &nomb);

int main () {
	configConsola();
	srand (time(NULL)); //con escribirla aquí ya sirve la semilla para el PROGRAMA ENTERO

	ifstream archivo;
	string nomb;
	tMazo mazo;

	vacio (mazo);
	int opcionMenu;
	opcionMenu = menu ();

	while (opcionMenu != 0) {
		if (opcionMenu == 1) {
			cout << "CARGAR UN MAZO A PARTIR DE UN FICHERO" << endl << endl;
			bool cargarMazo = cargar (mazo, nomb); 
			if (cargarMazo == true) mostrar (mazo);
		}
		else if (opcionMenu == 2) {
			cout << "BARAJAR UN MAZO" << endl << endl;
			barajar (mazo);
			mostrar (mazo);
		}
		else if (opcionMenu == 3) {
			tMazo otroMazo;
			string otroNombre;
			cout << "AÑADIR OTRO MAZO" << endl << endl;
			bool cargarMazo = cargar (otroMazo, otroNombre); //Unir mazos, guardando primero las cartas del segundo mazo en el array otroMazo
			if (cargarMazo == true) {
				bool unirMazos = unir (mazo, otroMazo);
				if(unirMazos == true) mostrar (mazo);
			}
		}
		else if (opcionMenu == 4) {
			int cuantas;
			cout << "CORTAR EL MAZO" << endl << endl;
			cout << "Introduce la posición por la que quieres cortar el mazo: ";
			cin >> cuantas;
			cortar (mazo, cuantas);
			mostrar (mazo);
		}
		else if (opcionMenu == 5) {
			cout << "GUARDAR UN MAZO EN UN FICHERO" << endl << endl;
			bool guardarMazo = guardar (mazo, nomb);
		}
		else if (opcionMenu == 6) {
			cout << "SEPARAR EN NEGRAS Y ROJAS" << endl << endl;
			tMazo mazoNegro;
			tMazo mazoRojo;
			repartirNegroRojo (mazo, mazoNegro, mazoRojo);
			cout << "Cartas negras:" << endl;
			mostrar(mazoNegro);
			cout << "\nCartas rojas:" << endl;
			mostrar(mazoRojo);
		}
		else if (opcionMenu == 7) {
			cout << "SEPARAR EN BAJAS Y ALTAS" << endl << endl;
			tMazo mazoBajas;
			tMazo mazoAltas;
			repartirBajaAlta (mazo, mazoBajas, mazoAltas);
			cout << "Cartas bajas (A-7):" << endl;
			mostrar(mazoBajas);
			cout << "\nCartas altas (8-K):" << endl;
			mostrar(mazoAltas);
		}
		else if (opcionMenu == 8) {
			cout << "REPARTIR DE MANERA ALTERNA EN 3 MONTONES" << endl << endl;
			tMazo monton;
			repartirMontones (mazo, 3, monton);
		}
		else if (opcionMenu == 9) {
			cout << "REPARTIR DE MANERA ALTERNA EN 4 MONTONES" << endl;
			tMazo monton;
			repartirMontones (mazo, 4, monton);
		}
		else if (opcionMenu == 10) {
			cout << "TRUCO DE LOS TRES MONTONES" << endl;
			trucoTresMontones(mazo, nomb);
		}
		opcionMenu = menu ();
	}

	return 0;
}

int menu ()	{
	
	int opcion;

	cout << "\n1 - Cargar un mazo de cartas de un fichero" << endl;
	cout << "2 - Barajar el mazo de cartas" << endl;
	cout << "3 - Añadir un mazo" << endl;
	cout << "4 - Cortar el mazo" << endl;
	cout << "5 - Guardar el mazo de cartas en un fichero" << endl;
	cout << "6 - Separar en negras y rojas" << endl;
	cout << "7 - Separar en bajas (A-7) y altas (8-K)" << endl;
	cout << "8 - Repartir un mazo de manera alterna en 3 montones" << endl;
	cout << "9 - Repartir un mazo de manera alterna en 4 montones" << endl;
	cout << "10 - Truco de los tres montones" << endl;
	cout << "0 - Salir" << endl << endl;

	cout << "Opción: ";
	cin >> opcion;

	while (opcion < 0 || opcion > 10) {
		cout << "Error. Por favor, seleccione una opción válida: ";
		cin >> opcion;
	}

	return opcion;
}

tPalo palo (tCarta carta) {
	tPalo paloCarta;
	tCarta palo;
	palo = (carta - 1) / CARTASxPALO;

	if (palo == 0)
		paloCarta = Picas;
	else if (palo == 1)
		paloCarta = Treboles;
	else if (palo == 2)
		paloCarta = Diamantes;
	else if (palo == 3)
		paloCarta = Corazones;

	return paloCarta;
}

tNumero numero (tCarta carta) {
	tNumero numCarta;
    tCarta numero;
	numero = ((carta - 1) % CARTASxPALO);

	if (numero == 0)
		numCarta = A;
	else if (numero == 1)
		numCarta = n2;
	else if (numero == 2)
		numCarta = n3;
	else if (numero == 3)
		numCarta = n4;
	else if (numero == 4)
		numCarta = n5;
	else if (numero == 5)
		numCarta = n6;
	else if (numero == 6)
		numCarta = n7;
	else if (numero == 7)
		numCarta = n8;
	else if (numero == 8)
		numCarta = n9;
	else if (numero == 9)
		numCarta = n10;
	else if (numero == 10)
		numCarta = J;
	else if (numero == 11)
		numCarta = Q;
	else if (numero == 12)
		numCarta = K;

	return numCarta;
}

void mostrar (tCarta carta) {
	tNumero numCarta;
	tPalo paloCarta;
	string nombreNum;
	string nombrePalo;

	numCarta = numero (carta);
	if (numCarta == 0)
		nombreNum = "A";
	else if (1 <= numCarta && numCarta <= 8)
		nombreNum = numCarta + 49;
	else if (numCarta == 9)
		nombreNum = "10";
	else if (numCarta == 10)
		nombreNum = "J";
	else if (numCarta == 11)
		nombreNum = "Q";
	else if (numCarta == 12)
		nombreNum = "K";

	paloCarta = palo (carta);
	if (paloCarta == 0)
		nombrePalo = "picas";
	else if (paloCarta == 1)
		nombrePalo = "treboles";
	else if (paloCarta == 2)
		nombrePalo = "diamantes";
	else if (paloCarta == 3)
		nombrePalo = "corazones";

	cout << left << setw(5) << "\n" << nombreNum << " de " << nombrePalo << endl;

}

void vacio (tMazo mazo) {
	mazo[0] = 'x';
}

int cuantas (const tMazo mazo) {
	int cartasMazo = 0;
	int carta;
	carta = mazo[cartasMazo];

	while (carta != marcaFin && cartasMazo < MAX_CARTAS) {
		cartasMazo++;
		carta = mazo[cartasMazo];
	}

	return cartasMazo;
}

void mostrar (const tMazo mazo) {
	int i = cuantas (mazo);
	int cont = 0;
	tCarta carta = mazo[cont];

	while (cont < i && cont < MAX_CARTAS) {
		mostrar	(carta);
		cont++;
		carta = mazo[cont];
	}
}

bool abrir (string &nomb, ifstream &archivo) {

	bool archivoAbierto;

	cout << "Introduzca el archivo a partir del cual quiere cargar un mazo: ";
	cin >> nomb;

	archivo.open (nomb);

	if (!archivo.is_open()) {
		int i = 0;

		while (!archivo.is_open() && i != 3) {
			cout << "El archivo no ha podido abrirse." << endl;
			cout << "Por favor, introduzca el archivo a partir del cual quiere cargar un mazo: ";
			cin >> nomb;

			archivo.open (nomb);

			i++;
		}
		if (i == 3) {
			cout << "Error: no se ha encontrado el archivo especificado." << endl << endl;
			archivoAbierto = false;
		}
	}
	if (archivo.is_open())
		archivoAbierto = true;

	return archivoAbierto;
}

bool cargar (tMazo mazo, string &nomb) {
	
	bool archivoAbierto;
	ifstream archivo;
	archivoAbierto = abrir (nomb, archivo);

	if (archivo.is_open()) {
		tCarta carta;
		char palo;
		int numero;
		int i = 0;
		archivo >> palo;

		while (palo != marcaFin && i < MAX_CARTAS) {
			archivo >> numero;
		
			tPalo paloCarta;
			if (palo == 'p')
				paloCarta = Picas;
			else if (palo == 't')
				paloCarta = Treboles;
			else if (palo == 'd')
				paloCarta = Diamantes;
			else if (palo == 'c')
				paloCarta = Corazones;

			carta = (paloCarta * CARTASxPALO) + numero;

			mazo[i] = carta;
			archivo >> palo;
			i++;
		}
		mazo[i] = marcaFin;

		archivoAbierto = true;
	}

	else
		archivoAbierto = false;

	archivo.close();

	return archivoAbierto;
}

bool guardar (const tMazo mazo, string &nomb) {
	bool archivoAbierto;
    ofstream archivo; 
	string nombArchivo;
	cout << "El archivo abierto es " << nomb << endl;
	cout << "Introduzca el nombre del archivo que quiera abrir (Intro para el actual): ";
	cin >> nombArchivo;

	if (nombArchivo != "\n") 
		nomb =	nombArchivo;

	archivo.open(nomb);
	
	if (archivo.is_open()) {
	    int i = 0;
		tCarta carta;

		for (int i = 0; mazo[i] != marcaFin; i++) {
			carta = mazo[i];

			tPalo paloCarta = palo (carta);
			int numero = ((carta - 1) % CARTASxPALO) + 1;
			char nombrePalo;
			
			if (paloCarta == 0)
				nombrePalo = 'p';
			else if (paloCarta == 1)
				nombrePalo = 't';
			else if (paloCarta == 2)
				nombrePalo = 'd';
			else if (paloCarta == 3)
				nombrePalo = 'c';
		    
			archivo << nombrePalo << numero << endl;
		}
		archivo << 'x';

		archivoAbierto = true;
	}

	else
		archivoAbierto = false;

	archivo.close();

	return archivoAbierto;
}

void barajar (tMazo mazo) {	
	int intercambios = 3 * cuantas(mazo);
	int pos1, pos2;
	tMazo mazoAux;

	for (int i = 0; i <= intercambios; i++) {
		pos1 = rand() % cuantas(mazo);
		pos2 = rand() % cuantas(mazo);

		mazoAux[0] = mazo[pos1];
		mazo[pos1] = mazo[pos2];
		mazo[pos2] = mazoAux[0];
	}
}

bool unir (tMazo mazo, const tMazo otroMazo) {
	bool noMaxCartas;

    int cuantasMazo = cuantas(mazo);
	int cuantasOtroMazo = cuantas(otroMazo);
	int total = cuantasMazo + cuantasOtroMazo;

	if (total < 52) {
		int i = cuantasMazo; //Cuenta la posición de las cartas en el array mazo
		int j = 0; //Cuenta la posición de las cartas en el array otroMazo

		while (i < total && otroMazo[j] != marcaFin) {
			mazo[i] = otroMazo[j];
			i++;
			j++;
		}
		mazo[i] = marcaFin;
		noMaxCartas = true;
	}

	else { 
		cout << "Error: no ha podido añadirse el mazo seleccionado." << endl;
		noMaxCartas = false;
	}

	return noMaxCartas;
}

bool partir (tMazo mazo, int cuantasCoger, tMazo otroMazo) {
	bool sufCartas;
	int cuantasMazo = cuantas(mazo);

	if (cuantasMazo > cuantasCoger) {
		for (int i = 0; i <= cuantasCoger; i++) 
			otroMazo[i] = mazo[i]; 
		
		int i = 0;
		while (mazo[i + cuantasCoger] != marcaFin) {
			mazo[i] = mazo[i + cuantasCoger];
			i++;
		}
		sufCartas = true;
	}
	else sufCartas = false;

	return sufCartas;
}

void cortar (tMazo mazo, int cuantas) {
	tMazo mazoAux;
	bool puedePartirse = partir (mazo, cuantas, mazoAux);
	bool puedenUnirse = unir (mazo, mazoAux);
}	

void repartirNegroRojo (const tMazo mazo, tMazo mazoNegro, tMazo mazoRojo) {
	int i = 0, j = 0, k = 0;
	tCarta carta = mazo[i];
	tPalo paloCarta;

	while (carta != marcaFin) {
		paloCarta = palo(carta);
		if (paloCarta == Picas || paloCarta == Treboles) {
			mazoNegro[j] = carta;
			j++;
		}
		else if (paloCarta == Corazones || paloCarta == Diamantes) {
			mazoRojo[k] = carta;
			k++;
		}
		i++;
		carta = mazo[i];
	}
	mazoNegro[j] = marcaFin;
	mazoRojo[k] = marcaFin;
}

void repartirBajaAlta (const tMazo mazo, tMazo mazoBajas, tMazo mazoAltas) {
	int i = 0, j = 0, k = 0;
	tCarta carta = mazo[i];
	tNumero numCarta;

	while (carta != marcaFin) {
		numCarta = numero(carta);
		if (numCarta <= n7) {
			mazoBajas[j] = carta;
			j++;
		}
		else if (numCarta >= n8) {
			mazoAltas[k] = carta;
			k++;
		}
		i++;
		carta = mazo[i];
	}
	mazoBajas[j] = marcaFin;
	mazoAltas[k] = marcaFin;

}

void repartirIntercalando (const tMazo mazo, int enCuantos, int queMazo, tMazo &mazoNuevo) {
	int i = 0, j = 0;
	tCarta carta = mazo[i];
	
	while (carta != marcaFin) {
		if ((i % enCuantos) == (queMazo - 1)) {
			mazoNuevo[j] = carta;
			j++;
		}
		i++;
		carta = mazo[i];
	}
	mazoNuevo[j] = marcaFin;
}

void repartirMontones (const tMazo mazo, int enCuantos, tMazo &monton) {
	for (int i = 1; i <= enCuantos; i++) {
		repartirIntercalando (mazo, enCuantos, i, monton);
		cout << "\nMontón " << i << ":" << endl;
		mostrar (monton);
	}
}

void trucoTresMontones(tMazo mazo, string &nomb) {
	bool cargarMazo = cargar (mazo, nomb);
	int veintiunCartas = cuantas(mazo);
	int i = 0;
	while (veintiunCartas != 21 && i < 3) {
		cout << "Error: el mazo cargado no tiene 21 cartas." << endl;
		cargarMazo = cargar(mazo, nomb);
		veintiunCartas = cuantas(mazo);
		i++;
	}

	if (cargarMazo == true) {
		tMazo monton;
		int dondeEsta;
		int cont = 0;
		barajar (mazo);
		repartirMontones (mazo, 3, monton);	
		cout << "\nEscoja mentalmente una carta e indique en que mazo se encuentra (1-3): ";
		cin >> dondeEsta;

		while (cont < 3) {
			if (cont != 0) {
				repartirMontones (mazo, 3, monton);	
				cout << "\nIndique en qué mazo se encuentra la carta que eligió antes (1-3): ";
				cin >> dondeEsta;	
			}
			while (dondeEsta < 1 || dondeEsta > 3) {
				cout << "Error: la opción seleccionada no es válida.";
				cout << "\nIndique en que mazo se encuentra su carta (1-3): ";
				cin >> dondeEsta;
			}
			tMazo monton1, monton2, monton3;
			int queMazo = (rand() % 3) + 1;
			while (queMazo == dondeEsta) queMazo = rand() % 3;
			int mazoUno = queMazo;
			repartirIntercalando (mazo, 3, mazoUno, monton1);
			repartirIntercalando (mazo, 3, dondeEsta, monton2);

			queMazo = (rand() % 3) + 1;
			while (queMazo == mazoUno || queMazo == dondeEsta) queMazo = rand() % 3;
			int mazoTres = queMazo;
			repartirIntercalando (mazo, 3, mazoTres, monton3);
		
			unir (monton1, monton2);
			unir (monton1, monton3);

			vacio(mazo);
			unir(mazo, monton1);

			cont++;
		}
		tCarta carta = mazo[10];
		cout << "Su carta es:" << endl;
		mostrar(carta);
	}
	else
		cout << "Error: el truco no ha podido ejecutarse. El mazo no es válido." << endl;
}
/*
void trucoPosada(tMazo mazo, string &nomb) {
	bool cargarMazo = cargar (mazo, nomb);
	


}
*/

void configConsola() {
	system ("chcp 1252");
}
//Práctica 2 versión 1.0
//Trucos de magia con cartas
//Por Marta Pastor y Ángela Martínez

#include <iostream> //Biblioteca básica estándar
#include <string>	//Biblioteca para cadenas de caracteres
#include <fstream> //Biblioteca para flujo de datos entre archivos
#include <ctime> //Biblioteca para seguimiento del tiempo
#include <cstdlib> //Biblioteca para elección de datos aleatorios
#include <iomanip> //Biblioteca para ajustes estéticos en la consola
//#include <cmath> //Biblioteca para fórmulas y expresiones matemáticas
using namespace std;	  


typedef enum { Picas /*0*/, Treboles /*1*/, Diamantes /*2*/, Corazones /*3*/ } tPalo; //With this, we define the order of the suits
typedef enum { A /*0*/, n2 /*1*/, n3 /*2*/, n4 /*3*/, n5 /*4*/, n6 /*5*/, n7 /*6*/, n8 /*7*/, n9 /*8*/, n10 /*9*/, J /*10*/, Q /*11*/, K /*12*/ } tNumero; //With this, we define the order of each card; A->Position 0, 2->Pos.1 ... King->Pos.12
const int CARTASxPALO = 13; //Define the number of cards in each suit

const char marcaFin = 'x'; //To represent the end of a deck
const int MAX_CARTAS = 52;
typedef int tCarta;
typedef tCarta tMazo[MAX_CARTAS + 1]; //We declare an array of tCarta (which is an integer type) called tMazo of size MAX_CARTAS + 1. This is because we need one more space to end mark

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

int main () {
	configConsola(); //we set up the console
	srand (time(NULL)); //con escribirla aquí ya sirve la semilla para el PROGRAMA ENTERO

	ifstream archivo;
	string nomb;
	tMazo mazo;

	vacio (mazo); //We empty the deck each time we execute the program
	int opcionMenu;
	opcionMenu = menu (); //We make a call to the function that shows the menu
	//Depending the number returned in the menu function, we execute one or other function

	while (opcionMenu != 0) {
		if (opcionMenu == 1) {
			cout << "CARGAR UN MAZO A PARTIR DE UN FICHERO" << endl << endl;
			bool cargarMazo = cargar (mazo, nomb); 
			mostrar (mazo);
		}
		else if (opcionMenu == 2) {
			cout << "BARAJAR UN MAZO" << endl << endl;
			barajar (mazo);
			mostrar (mazo);
		}
		else if (opcionMenu == 3) {
			tMazo otroMazo; //We declare another deck, the one that we will add to the deck that we're using
			string otroNombre;
			cout << "AÑADIR OTRO MAZO" << endl << endl;
			bool cargarMazo = cargar (otroMazo, otroNombre); //Unir mazos, guardando primero las cartas del segundo mazo en el array otroMazo
			bool unirMazos = unir (mazo, otroMazo);
			mostrar (mazo);
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

		opcionMenu = menu ();
	}

	return 0;
}

//Function that displays the menu and return an integer representing the selected option
int menu ()	{
	
	int opcion;

	cout << "\n1 - Cargar un mazo de cartas de un fichero" << endl;
	cout << "2 - Barajar el mazo de cartas" << endl;
	cout << "3 - Añadir un mazo" << endl;
	cout << "4 - Cortar el mazo" << endl;
	cout << "5 - Guardar el mazo de cartas en un fichero" << endl;
	cout << "0 - Salir" << endl << endl;

	cout << "Opción: ";
	cin >> opcion;

	while (opcion < 0 || opcion > 5) {
		cout << "Error. Por favor, seleccione una opción válida: ";
		cin >> opcion;
	}

	return opcion;
}

//This function determines and return the suit of the given card
tPalo palo (tCarta carta) {
	tPalo paloCarta;
	tCarta palo;
	palo = (carta - 1) / CARTASxPALO;
	/*
		As when we're loading cards, we load each card with its original number (this means, for example, 2->2 instead of 2->1 as
		we mentioned above) to determine the suit of the card, first, we've to decrease 1 to the card and then divide it by the
		number of cards that are in each suit. Once we've done this, we can easily determine the suit of the card according to the
		type defined at the beginning.
	*/

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

//This function determines and return the number of the given card
tNumero numero (tCarta carta) {
	tNumero numCarta;
   	tCarta numero;
	numero = ((carta - 1) % CARTASxPALO);
	/*
		As when we're loading cards, we load each card with its original number (this means, for example, 2->2 instead of 2->1 as
		we mentioned above) to determine the number of the card, first, we've to decrease 1 to the card and then calculate the
		module of it. Once we've done this, we can easily determine the number of the card according to the defined order at the
		beginning.
	*/

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

//This is the function that is used to display the card to the player
void mostrar (tCarta carta) {
	tNumero numCarta;
	tPalo paloCarta;
	string nombreNum;
	string nombrePalo;

	numCarta = numero (carta);
	if (numCarta == 0)
		nombreNum = "A";
	else if (1 <= numCarta && numCarta <= 8)
		/*
			In these cards (2-9), we only have to display one digit. Because of this, we just add 49 to the number of the
			card and display that ASCII character. We add 49 because it's the code for 1 in ASCII and after it, the rest of
			the digits go in order. For example, if numCarta = 1, we should display a 2. So, we add to 49, 1 and we get 50
			and then nombreNum equals to 50 in ASCII code, that means, 2.
		*/
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

//With this function we create an empty deck. To do that, we only put in the first position of the deck an x, that is the mark that we
//use to indicate the end of a deck
void vacio (tMazo mazo) {
	mazo[0] = 'x';
}

//Function that returns the number of cards that are in a deck
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

//Function used to display the whole deck to the user
void mostrar (const tMazo mazo) {
	int i = cuantas (mazo);
	int cont = 0;
	tCarta carta;

	while (cont < i && cont < MAX_CARTAS) {
		carta = mazo[cont];
		mostrar	(carta);
		cont++;
	}
}

//This is the function used when we want to open a file
bool abrir (string &nomb, ifstream &archivo) {

	bool archivoAbierto;

	cout << "Introduzca el archivo a partir del cual quiere cargar un mazo: ";
	cin >> nomb;

	archivo.open (nomb);

	if (!archivo.is_open()) {
		int i = 0;

		/*
			If we couldn't open the file in our first attempt, we will have 3 more attemps to achieve it. If after that
			we cannot open it, we will return false and display an error message.
		*/
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
	else if (archivo.is_open())
		archivoAbierto = true;

	return archivoAbierto;
}

//Function used to load a deck from a file
bool cargar (tMazo mazo, string &nomb) {
	
	bool archivoAbierto;
	ifstream archivo;
	archivoAbierto = abrir (nomb, archivo);

	//If we've opened the file with the above function, we start reading it
	if (archivo.is_open()) {
		tCarta carta;
		char palo;
		int numero;
		int i = 0;
		archivo >> palo;

		//We keep reading the file until we find the end mark or the deck is full
		while (palo != 'x' && i < MAX_CARTAS) {
			archivo >> numero;
		
			//For every card that we read from the deck, we make the conversion and we place it in the deck
			tPalo paloCarta;
			if (palo == 'p')
				paloCarta = Picas;
			else if (palo == 't')
				paloCarta = Treboles;
			else if (palo == 'd')
				paloCarta = Diamantes;
			else if (palo == 'c')
				paloCarta = Corazones;

			//The card number we place in the deck will be determined by multiplying the position of the suit of the card
			//in the enum and the number of the card itself
			carta = (paloCarta * CARTASxPALO) + numero;

			mazo[i] = carta;
			archivo >> palo;
			i++;
		}
		
		//When we've completed the reading from the file, we put at the end of the deck an end mark
		mazo[i] = marcaFin;

		archivoAbierto = true;
	}

	else
		archivoAbierto = false;

	archivo.close();

	return archivoAbierto;
}

//Function used to save a deck into a file
bool guardar (const tMazo mazo, string &nomb) {
	bool archivoAbierto;
  	ofstream archivo; 
	string nombArchivo;
	cout << "El archivo abierto es " << nomb << endl;
	cout << "Introduzca el nombre del archivo que quiera abrir (Intro para el actual): ";
	cin >> nombArchivo;

	//This is used to prevent that we just press enter without entering any filename
	if (nombArchivo != "\n") 
		nomb =	nombArchivo;

	archivo.open(nomb);
	
	if (archivo.is_open()) {
	    int i = 0;
		tCarta carta;

		//For every card in the deck, we continue doing the conversion until we find the end mark
		for (int i = 0; mazo[i] != marcaFin; i++) {
			carta = mazo[i];

			tPalo paloCarta = palo (carta);
			/*
				To determine the number of the card, we've to substract 1 to the actual card number, then make the 
				module and finally add 1. This is because, if for example we've a king of clubs (carta 13), we make
				the module and get 0, but the number it's not 0, it should be 13. With this, we prevent that and get
				the expected number.
			*/
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
		//After doing the conversion of all the cards, we write in the file the final mark
		archivo << 'x';

		archivoAbierto = true;
	}

	else
		archivoAbierto = false;

	archivo.close();

	return archivoAbierto;
}

//Function used to shuffle the deck
void barajar (tMazo mazo) {	
	int intercambios = 3 * cuantas(mazo); //We determine the number of changes that are going to take place in the deck (3 times the
					      //number of cards in the deck)
	int pos1, pos2;
	tMazo mazoAux;

	for (int i = 0; i <= intercambios; i++) {
		/*
			For every change, we determine two random positions of the deck and then change the cards that are on those
			positions.
		*/
		pos1 = rand() % cuantas(mazo);
		pos2 = rand() % cuantas(mazo);

		mazoAux[0] = mazo[pos1];
		mazo[pos1] = mazo[pos2];
		mazo[pos2] = mazoAux[0];
	}
}

//Function used to join to our main deck another deck
bool unir (tMazo mazo, const tMazo otroMazo) {
	bool noMaxCartas;
  	int cuantasMazo = cuantas(mazo);
	int cuantasOtroMazo = cuantas(otroMazo);
	int total = cuantasMazo + cuantasOtroMazo;

	//If the total number of cards from the two decks is less than the maximum number of cards, we join both decks and return true	
	if (total < 52) {
		int i = cuantasMazo; //Cuenta la posición de las cartas en el array mazo
		int j = 0; //Cuenta la posición de las cartas en el array otroMazo

		/*
			To join both decks, we determine the number of cards that compose our main deck and we start adding cards from
			the auxiliary deck in that position (#cards in the main deck) until we find the end mark.
		*/
		while (i < total && otroMazo[j] != marcaFin) {
			mazo[i] = otroMazo[j];
			i++;
			j++;
		}
		mazo[i] = marcaFin;
		noMaxCartas = true;
	}

	else noMaxCartas = false;

	return noMaxCartas;
}

//Function used to divide the deck into two decks. We'll divide it by the position specified in cuantasCoger
bool partir (tMazo mazo, int cuantasCoger, tMazo otroMazo) {
	bool sufCartas;
	int cuantasMazo = cuantas(mazo);

	if (cuantasMazo > cuantasCoger) {
		/*
			With this for loop we put the first cuantasCoger cards in our new deck
		*/
		for (int i = 0; i <= cuantasCoger; i++) 
			otroMazo[i] = mazo[i]; 
		
		int i = 0;
		
		/*
			With this for loop we place the rest of the cards that we haven't put in our auxiliary deck in the first positions
			of our main deck
		*/
		while (mazo[i + cuantasCoger] != marcaFin) {
			mazo[i] = mazo[i + cuantasCoger];
			i++;
		}
		sufCartas = true;
	}
	else sufCartas = false;

	return sufCartas;
}

//Function used to cut the deck
void cortar (tMazo mazo, int cuantas) {
	tMazo mazoAux;
	//In these boolean variables we save if we can divide and join the decks
	bool puedePartirse = partir (mazo, cuantas, mazoAux); //We divide the original deck into two decks
	bool puedenUnirse = unir (mazo, mazoAux); //We join the two decks previously divided
}		

//Function used to set up the console
void configConsola() {
	system ("chcp 1252");
}

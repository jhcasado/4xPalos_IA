/* ==============================================================================
	
	4XPalos
	
	Jos� Angel Montelongo Reyes 
	Javier Honorio Casado Fern�ndez
	
	Universidad de Las Palmas de Gran Canaria	
	
   ============================================================================== */

#ifndef TableroH
#define TableroH


//---------------------------------------------------------------------------
/************************* DEFINICION DE MACROS *****************************/
//---------------------------------------------------------------------------
//M�ximo para las filas del tablero
#define MAX_FILAS 16


//M�ximo para las columnas del tablero
#define MAX_COLUMNAS 16


//Puntuacion m�xima para las casillas del tablero: 0..MAX_PESO
#define MAX_PESO 9


//N�mero m�ximo de jugadores permitidos: 1..MAX_JUGADORES
//Este objeto permite que jueguen m�s de 2 jugadores a la vez
//sobre un mismo tablero
#define MAX_JUGADORES 2


//Estados para los lugares(arriba,abajo,izquierda,derecha) de los palos
//LIBRE: el lugar est� libre y podr� ser ocupado por un palo
//OCUPADO: el lugar ya est� ocupado por otro palo
#define LIBRE false
#define OCUPADO true


//Define el lugar de la casilla en el que se quiera colocar un palo
//cuando se realiza un movimiento
#define ARRIBA     0
#define ABAJO      1
#define DERECHA    2
#define IZQUIERDA  3


//Indica el tipo de respuesta que quieres que te devuelva Game_Over (p);
//POR_PUNTOS: matem�ticamente ha ganado un jugador
//POR_FINAL: no quedan m�s palos que poner
#define POR_PUNTOS 0
#define POR_FINAL  1


//---------------------------------------------------------------------------
/*********************** DEFINICION DE ESTRUCTURAS **************************/
//---------------------------------------------------------------------------
//Estructura para poder realizar un movimiento con el m�todo Poner_Palo (mov);
//La idea es que cada jugador realice una funci�n que devuelva una
//variable con esta estructura para realizar su movimiento:
//  if (Poner_Palo (Jugador_A (TABLERO t))) return palo_puesto_correctamente;
struct TMovimiento
{
    int fila, columna; //fila: 0..(MAX_FILAS - 1); columna: (0..MAX_COLUMNAS - 1)
    int lugar; //{ARRIBA, ABAJO, DERECHA, IZQUIERDA}
};


//Esta estructura define las casillas que tiene el tablero.
//Si peso >= 0, el peso nos dice la puntuaci�n de la casilla
//Si peso < 0, nos dice que es una casilla rodeada de palos:
//      peso = -1, es una casilla negra
//      peso = (- MAX_JUGADORES - 1)..-2, es una casilla ocupada por un jugador:
//          -2 = jugador[0], -3 = jugador[1], -4 = jugador[2], etc.
struct TCasilla
{
    int peso; // (- MAX_JUGADORES - 1)..MAX_PESO
    bool arriba, abajo, izquierda, derecha; // {LIBRE, OCUPADO} = {false, true}
};


//---------------------------------------------------------------------------
/************************ DEFINICION DEL TABLERO ****************************/
//---------------------------------------------------------------------------
class TABLERO
{
//------------ ELEMENTOS PRIVADOS DEL TABLERO ----------------
    private:
    //Variables y m�todos privados:
        int filas, columnas, casillas_negras;
        int turno, jugador[MAX_JUGADORES], puntos_totales, puntos_para_ganar;
        TCasilla tablero_real[MAX_FILAS][MAX_COLUMNAS];

    //Genera un tablero nuevo y devuelve el total de puntos que se pueden
    //conseguir en �l
        int Generar_Tablero (int ncolumnas = MAX_COLUMNAS, int nfilas = MAX_FILAS, int nnegros = 0);

    //Vuelca el espacio de memoria del tablero real al virtual
        void Actualizar_Tablero_Virtual ();

    //Le pasa el turno al jugador siguiente
        void Jugador_Siguiente ();

    //Acaba la partida por haber obtenido, uno de los jugadores,
    //una puntuaci�n con la que matem�ticamente ganar�a la partida
        bool Game_Over_Por_Puntos ();

    //Acaba la partida por haber llegado hasta el final de �sta
        bool Game_Over_Por_Final ();


//------------ ELEMENTOS PUBLICOS DEL TABLERO ----------------
    public:
    //Este ser� el tablero que ver�n los jugadores pudiendo hacer
    //con �l lo que quieran
        TCasilla tablero_virtual[MAX_FILAS][MAX_COLUMNAS];

    //Constructor: Inicia un juego nuevo al crear el TABLERO
        TABLERO (int ncolumnas = MAX_COLUMNAS, int nfilas = MAX_FILAS, int nnegros = 0);

    //Destructor: No hace nada
        ~TABLERO ();

    //Inicia un juego nuevo
        void Juego_Nuevo (int ncolumnas = MAX_COLUMNAS, int nfilas = MAX_FILAS, int nnegros = 0);

    //Poner, si se puede, un palo en la posici�n indicada en por
    //estructura TMovimiento
        bool Poner_Palo (TMovimiento palo_en);

    //Indica si la partida ha acabado
    //tipo : {POR_PUNTOS = 0, POR_FINAL = 1}
        bool Game_Over (int tipo);

    //Indica el n�mero de filas del tablero
        int Filas ();

    //Indica el n�mero de columnas del tablero
        int Columnas ();

    //Indica el n�mero de casillas negras que contiene el tablero
        int Casillas_Negras ();

    //Indica el jugador que debe mover en ese instante
    //0 = jugador[0], 1 = jugador[1], 2 = jugador[2], etc.
        int Turno ();

    //Indica la puntuaci�n de uno de los jugadores
    //id = 0  jugador[0], id = 1  jugador[1], id = 2  jugador[2], etc.
        int Puntos_Jugador (int id);

    //Indica la puntuaci�n total que se puede obtener en el tablero
        int Puntos_Totales ();
};
//---------------------------------------------------------------------------
#endif
//FIN tablero.h

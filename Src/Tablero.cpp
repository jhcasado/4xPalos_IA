/* ==============================================================================
	
	4XPalos
	
	José Angel Montelongo Reyes 
	Javier Honorio Casado Fernández
	
	Universidad de Las Palmas de Gran Canaria	
	
   ============================================================================== */
 
#include <vcl.h>
#pragma hdrstop

#include <stdlib.h>
#include <stdio.h>
#include <mem.h>
#include "Tablero.h"


//---------------------------------------------------------------------------
/*********** IMPLEMENTACION DE LOS METODOS PRIVADOS DEL TABLERO *************/
//---------------------------------------------------------------------------
//Genera un tablero nuevo y devuelve el total de puntos que se pueden
//conseguir en él
int TABLERO::Generar_Tablero (int ncolumnas, int nfilas, int nnegros)
{
    int total = 0;

    //Genero un tablero nuevo sin casillas negras
    for (int i = 0; i < nfilas; i++)
        for (int j = 0; j < ncolumnas; j++)
        {
            //Calculo aleatoriamente su peso y lo sumo a los puntos
            tablero_real[i][j].peso = random (MAX_PESO + 1);
            total += tablero_real[i][j].peso;

            //Pongo palos en las casillas de los bordes
            if (i == 0)
                tablero_real[i][j].arriba = OCUPADO;
            else
                tablero_real[i][j].arriba = LIBRE;
            if (i == filas - 1)
                tablero_real[i][j].abajo = OCUPADO;
            else
                tablero_real[i][j].abajo = LIBRE;
            if (j == 0)
                tablero_real[i][j].izquierda = OCUPADO;
            else
                tablero_real[i][j].izquierda = LIBRE;
            if (j == columnas - 1)
                tablero_real[i][j].derecha = OCUPADO;
            else
                tablero_real[i][j].derecha = LIBRE;
        }

    //Le añado las casillas negras al tablero
    for (int c = 0; c < nnegros; c++)
    {
        int i, j;

        //Fila y columna de una casilla no negra
        do
        {
            i = random (nfilas);
            j = random (ncolumnas);
        } while (tablero_real[i][j].peso < 0);

        //Recupero su peso y la marco como casilla negra
        total -= tablero_real[i][j].peso;
        tablero_real[i][j].peso = -1;

        //Realizo los arreglos necesarios con las casillas adyacentes
        if (tablero_real[i][j].arriba == LIBRE)
        {
            tablero_real[i][j].arriba = OCUPADO;
            tablero_real[i-1][j].abajo = OCUPADO;
        }
        if (tablero_real[i][j].abajo == LIBRE)
        {
            tablero_real[i][j].abajo = OCUPADO;
            tablero_real[i+1][j].arriba = OCUPADO;
        }
        if (tablero_real[i][j].izquierda == LIBRE)
        {
            tablero_real[i][j].izquierda = OCUPADO;
            tablero_real[i][j-1].derecha = OCUPADO;
        }
        if (tablero_real[i][j].derecha == LIBRE)
        {
            tablero_real[i][j].derecha = OCUPADO;
            tablero_real[i][j+1].izquierda = OCUPADO;
        }
    }

    //Busco si al haber insertado las casillas negras se
    //han generado más casillas negras
    for (int i = 0; i < nfilas; i++)
        for (int j = 0; j < ncolumnas; j++)
        {
            //Estoy en una casilla negra
            if (tablero_real[i][j].peso < 0) continue;

            //Compruebo si está rodeada
            if (tablero_real[i][j].arriba == OCUPADO)
                if (tablero_real[i][j].abajo == OCUPADO)
                    if (tablero_real[i][j].derecha == OCUPADO)
                        if (tablero_real[i][j].izquierda == OCUPADO)
                            {
                                total -= tablero_real[i][j].peso;
                                tablero_real[i][j].peso = -1;
                                casillas_negras++;
                            }
        }

    //Actualizo el tablero virtual y retorno el número total de puntos
    Actualizar_Tablero_Virtual ();
    return total;
}


//Vuelca el espacio de memoria del tablero real al virtual
void TABLERO::Actualizar_Tablero_Virtual ()
{
    memcpy (tablero_virtual, tablero_real, sizeof(tablero_real));
}


//Le pasa el turno al jugador siguiente
void TABLERO::Jugador_Siguiente ()
{
    turno ++;
    if (turno == MAX_JUGADORES) turno = 0;
}


//Acaba la partida por haber obtenido, uno de los jugadores,
//una puntuación con la que matemáticamente ganaría la partida
bool TABLERO::Game_Over_Por_Puntos ()
{
    for (int c = 0; c < MAX_JUGADORES; c++)
        if (jugador[c] >= puntos_para_ganar) return true;

    return false;
}


//Acaba la partida por haber llegado hasta el final de ésta
bool TABLERO::Game_Over_Por_Final ()
{
    int total = 0;

    for (int c = 0; c < MAX_JUGADORES; c++)
        total += jugador[c];

    if (total != puntos_totales)
        return false;

    return true;
}


//---------------------------------------------------------------------------
/*********** IMPLEMENTACION DE LOS METODOS PUBLICOS DEL TABLERO *************/
//---------------------------------------------------------------------------
//Constructor: Inicia un juego nuevo al crear el TABLERO
TABLERO::TABLERO (int ncolumnas, int nfilas, int nnegros)
{
    randomize ();
    Juego_Nuevo (ncolumnas, nfilas, nnegros);
}


//Destructor: No hace nada
TABLERO::~TABLERO ()
{}


//Inicia un juego nuevo
void TABLERO::Juego_Nuevo (int ncolumnas, int nfilas, int nnegros)
{
    //Cálculos aleatorios para la generación un tablero distinto
    filas = nfilas;
    columnas = ncolumnas;
    casillas_negras = nnegros;

    //Genero el tablero
    puntos_totales = Generar_Tablero (ncolumnas, nfilas, nnegros);
    puntos_para_ganar = (puntos_totales / MAX_JUGADORES) + 1;

    //Inicializo el turno y las puntuaciones de los jugadores
    turno = 0;
    for (int c = 0; c < MAX_JUGADORES; c++)
        jugador[c] = 0;
}


//Poner, si se puede, un palo en la posición indicada en por
//estructura TMovimiento
bool TABLERO::Poner_Palo (TMovimiento palo_en)
{
    bool ha_puesto = false;

    //Compruebo si el movimiento pudiera ser erróneo
    if ((palo_en.fila < 0) || (palo_en.fila >= filas)) return false;
    if ((palo_en.columna < 0) || (palo_en.columna >= columnas))return false;
    if (tablero_real[palo_en.fila][palo_en.columna].peso < 0) return false;

    //Compruebo el lugar en el que se quiere insertar el palo
    switch (palo_en.lugar)
    {
        case ARRIBA:
            //Compruebo que la posición no esté ocupada de antemano
            if (tablero_real[palo_en.fila][palo_en.columna].arriba == OCUPADO)
                return false;

            //Pongo el palo en la casilla actual y en la adyacente
            tablero_real[palo_en.fila][palo_en.columna].arriba = OCUPADO;
            tablero_real[palo_en.fila - 1][palo_en.columna].abajo = OCUPADO;

            //Compruebo si se ha rodeado completamente la casilla adyacente
            if (tablero_real[palo_en.fila - 1][palo_en.columna].arriba == OCUPADO)
                if (tablero_real[palo_en.fila - 1][palo_en.columna].derecha == OCUPADO)
                    if (tablero_real[palo_en.fila - 1][palo_en.columna].izquierda == OCUPADO)
                    {
                        jugador[turno] += tablero_real[palo_en.fila - 1][palo_en.columna].peso;
                        tablero_real[palo_en.fila - 1][palo_en.columna].peso = (- (turno) - 2);
                        ha_puesto = true;
                    }
            break;

        case ABAJO:
            //Compruebo que la posición no esté ocupada de antemano
            if (tablero_real[palo_en.fila][palo_en.columna].abajo == OCUPADO)
                return false;

            //Pongo el palo en la casilla actual y en la adyacente
            tablero_real[palo_en.fila][palo_en.columna].abajo = OCUPADO;
            tablero_real[palo_en.fila + 1][palo_en.columna].arriba = OCUPADO;

            //Compruebo si se ha rodeado completamente la casilla adyacente
            if (tablero_real[palo_en.fila + 1][palo_en.columna].abajo == OCUPADO)
                if (tablero_real[palo_en.fila + 1][palo_en.columna].derecha == OCUPADO)
                    if (tablero_real[palo_en.fila + 1][palo_en.columna].izquierda == OCUPADO)
                    {
                        jugador[turno] += tablero_real[palo_en.fila + 1][palo_en.columna].peso;
                        tablero_real[palo_en.fila + 1][palo_en.columna].peso = (- (turno) - 2);
                        ha_puesto = true;
                    }
            break;

        case DERECHA:
            //Compruebo que la posición no esté ocupada de antemano
            if (tablero_real[palo_en.fila][palo_en.columna].derecha == OCUPADO)
                return false;

            //Pongo el palo en la casilla actual y en la adyacente
            tablero_real[palo_en.fila][palo_en.columna].derecha = OCUPADO;
            tablero_real[palo_en.fila][palo_en.columna + 1].izquierda = OCUPADO;

            //Compruebo si se ha rodeado completamente la casilla adyacente
            if (tablero_real[palo_en.fila][palo_en.columna + 1].arriba == OCUPADO)
                if (tablero_real[palo_en.fila][palo_en.columna + 1].abajo == OCUPADO)
                    if (tablero_real[palo_en.fila][palo_en.columna + 1].derecha == OCUPADO)
                    {
                        jugador[turno] += tablero_real[palo_en.fila][palo_en.columna + 1].peso;
                        tablero_real[palo_en.fila][palo_en.columna + 1].peso = (- (turno) - 2);
                        ha_puesto = true;
                    }
            break;

        case IZQUIERDA:
            //Compruebo que la posición no esté ocupada de antemano
            if (tablero_real[palo_en.fila][palo_en.columna].izquierda == OCUPADO)
                return false;

            //Pongo el palo en la casilla actual y en la adyacente
            tablero_real[palo_en.fila][palo_en.columna].izquierda = OCUPADO;
            tablero_real[palo_en.fila][palo_en.columna - 1].derecha = OCUPADO;

            //Compruebo si se ha rodeado completamente la casilla adyacente
            if (tablero_real[palo_en.fila][palo_en.columna - 1].arriba == OCUPADO)
                if (tablero_real[palo_en.fila][palo_en.columna - 1].abajo == OCUPADO)
                    if (tablero_real[palo_en.fila][palo_en.columna - 1].izquierda == OCUPADO)
                    {
                        jugador[turno] += tablero_real[palo_en.fila][palo_en.columna - 1].peso;
                        tablero_real[palo_en.fila][palo_en.columna - 1].peso = (- (turno) - 2);
                        ha_puesto = true;
                    }
            break;

        default:
            //Se ha introducido lugar no válido
            return false;
    }

    //Compruebo si se ha rodeado la casilla actual
    if (tablero_real[palo_en.fila][palo_en.columna].arriba == OCUPADO)
        if (tablero_real[palo_en.fila][palo_en.columna].abajo == OCUPADO)
            if (tablero_real[palo_en.fila][palo_en.columna].derecha == OCUPADO)
                if (tablero_real[palo_en.fila][palo_en.columna].izquierda == OCUPADO)
                    {
                        jugador[turno] += tablero_real[palo_en.fila][palo_en.columna].peso;
                        tablero_real[palo_en.fila][palo_en.columna].peso = (- (turno) - 2);
                        ha_puesto = true;
                    }

    //Actualizo el tablero
    Actualizar_Tablero_Virtual ();

    //Cambio el turno si no se ha rodeado ninguna casilla
    if (!ha_puesto)
        Jugador_Siguiente ();

    //Se ha puesto correctamente el palo
    return true;
}


//Indica si la partida ha acabado
//tipo : {POR_PUNTOS = 0, POR_FINAL = 1}
bool TABLERO::Game_Over (int tipo)
{
    switch (tipo)
    {
        case (POR_PUNTOS):
            return Game_Over_Por_Puntos ();

        case (POR_FINAL):
            return Game_Over_Por_Final ();

        default:
            return false;
    }
}


//Indica el número de filas del tablero
int TABLERO::Filas ()
{
    return filas;
}


//Indica el número de columnas del tablero
int TABLERO::Columnas ()
{
    return columnas;
}


//Indica el número de casillas negras que contiene el tablero
int TABLERO::Casillas_Negras ()
{
    return casillas_negras;
}


//Indica el jugador que debe mover en ese instante
//0 = jugador[0], 1 = jugador[1], 2 = jugador[2], etc.
int TABLERO::Turno ()
{
    return turno;
}


//Indica la puntuación de uno de los jugadores
//id = 0  jugador[0], id = 1  jugador[1], id = 2  jugador[2], etc.
//Devolverá -1 si el identificador es no válido
int TABLERO::Puntos_Jugador (int id)
{
    if ((id < 0) || (id >= MAX_JUGADORES)) return -1;
    return (jugador[id]);
}


//Indica la puntuación total que se puede obtener en el tablero
int TABLERO::Puntos_Totales ()
{
    return puntos_totales;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
//FIN tablero.cpp

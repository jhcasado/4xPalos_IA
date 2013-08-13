/* ==============================================================================
	
	4XPalos
	
	José Angel Montelongo Reyes 
	Javier Honorio Casado Fernández
	
	Universidad de Las Palmas de Gran Canaria	
	
   ============================================================================== */

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdlib.h>
#include "Jugador_2.h"
#include "Tablero.h"

int Sitios_Libres (TCasilla c)
//Devuelve el número de palos sin poner que tine la casilla dada
//{0..4}
{
    int aux = 0;

    if (c.arriba ==  LIBRE) aux ++;
    if (c.abajo == LIBRE) aux ++;
    if (c.izquierda == LIBRE) aux ++;
    if (c.derecha == LIBRE) aux ++;

    return aux;
}


int Peso_Camino (TABLERO *t, int f, int c)
//Dada una posición inicial en el tablero, recorre el camino
//y devuelve la suma de los pesos de las casillas que forman
//el camino
//Requiere explorado[][]
//Recursiva
{
    int suma = 0;

    if (t->tablero_virtual[f][c].peso < 0) return suma;

    suma = t->tablero_virtual[f][c].peso;
    t->tablero_virtual[f][c].peso = -1;

    if (Sitios_Libres(t->tablero_virtual[f][c]) > 2)
        return suma;

    if (t->tablero_virtual[f][c].arriba == LIBRE)
        suma += Peso_Camino (t, f - 1, c);

    if (t->tablero_virtual[f][c].abajo == LIBRE)
        suma += Peso_Camino (t, f + 1, c);

    if (t->tablero_virtual[f][c].derecha == LIBRE)
        suma += Peso_Camino (t, f, c + 1);

    if (t->tablero_virtual[f][c].izquierda == LIBRE)
        suma += Peso_Camino (t, f, c - 1);

    return suma;
}


bool Jugador_2(TABLERO *t, TMovimiento &palo)
//Movimiento para nuestro jugador
{
    int aux;
    bool salta = false;
    int casos_probados;
    bool encontrado = false;
    int peso = -1;

    randomize();

    //recorro el tablero
    for (int f = 0; f < t->Filas(); f++)
        for (int c = 0; c < t->Columnas(); c++)
        {
            //Si es una casilla ya marcada me la salto
            if (t->tablero_virtual[f][c].peso < 0) continue;

            //Consulto cuantas posiciones libres quedan
            aux = Sitios_Libres (t->tablero_virtual[f][c]);

            //Si a la casilla sólo le falta un palo
            //lo marco y salgo
            if (aux == 1)
            {
                palo.columna = c;
                palo.fila = f;
                aux = 0;
                do
                {
                palo.lugar = aux;
                aux ++;
                } while (!t->Poner_Palo(palo));
                return true;
            }

            //Si sólo le quedan 2 palos por poner me la salto
            //para no regalársela al contrario
            if (aux <= 2) continue;

            //Para intentar poner siempre en las de mayor peso
            //al final del tablero
            if (peso <= t->tablero_virtual[f][c].peso)
                salta = false;

            casos_probados = 0;

            //Busco un lugar en la casilla en la que pueda poner
            //el palo sin regalarle nada al contrario
            while ((salta == false) && (casos_probados < 4))
            {
                int lugar;
                bool caso[4] = {false, false, false, false};

                //Para no seguir un orden a la hora de poner palos
                do
                {
                    lugar = random(4);
                } while (caso[lugar]);

                //Realizo las comprobaciones oportunas con las casillas
                //adyacentes
                switch(lugar)
                {
                    case 0:
                        if (t->tablero_virtual[f][c].arriba ==  LIBRE)
                        {
                            aux = Sitios_Libres (t->tablero_virtual[f-1][c]);
                            if (aux > 2)
                            {
                                palo.lugar = ARRIBA;
                                salta = true;
                            }
                        }
                        break;

                    case 1:
                        if (t->tablero_virtual[f][c].abajo ==  LIBRE)
                        {
                            aux = Sitios_Libres (t->tablero_virtual[f+1][c]);
                            if (aux > 2)
                            {
                                palo.lugar = ABAJO;
                                salta = true;
                            }
                        }
                        break;

                    case 2:
                        if (t->tablero_virtual[f][c].izquierda ==  LIBRE)
                        {
                            aux = Sitios_Libres (t->tablero_virtual[f][c-1]);
                            if (aux > 2)
                            {
                                palo.lugar = IZQUIERDA;
                                salta = true;
                            }
                        }
                        break;

                    case 3:
                        if (t->tablero_virtual[f][c].derecha ==  LIBRE)
                        {
                            aux = Sitios_Libres (t->tablero_virtual[f][c+1]);
                            if (aux > 2)
                            {
                                palo.lugar = DERECHA;
                                salta = true;
                            }
                        }
                        break;
                }

                if (salta)
                {
                    palo.columna = c;
                    palo.fila = f;
                    peso = t->tablero_virtual[f][c].peso;
                    encontrado = true;
                }

                caso[lugar] = true;
                casos_probados ++;
            }
        }

    //Si ha habido suerte en el recorrido del tablero para encontrar
    //un sitio libre en el que no le regale nada al contrario
    if (encontrado) return t->Poner_Palo(palo);

    //Si no fue así y ya sólo quedan diferentes caminos para rellenar
    //buscaré el camino que contenga menos puntos para regalárselo al
    //contrario
    for (int f = 0; f < t->Filas(); f++)
        for (int c = 0; c < t->Columnas(); c++)
        {
            if (t->tablero_virtual[f][c].peso < 0) continue;

            aux = Peso_Camino (t, f, c);

            if ((peso > aux) || (peso == -1))
            {
                palo.fila = f;
                palo.columna = c;
                peso = aux;
            }
        }

    //Coloco un palo dentro del recorrido del camino que
    //voy a regalar
    aux = 0;
    do
    {
        palo.lugar = aux;
        aux ++;
    } while (!t->Poner_Palo(palo));

    return true;
}


//---------------------------------------------------------------------------
#pragma package(smart_init)

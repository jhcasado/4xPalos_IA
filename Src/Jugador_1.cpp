/* ==============================================================================
	
	4XPalos
	
	José Angel Montelongo Reyes 
	Javier Honorio Casado Fernández
	
	Universidad de Las Palmas de Gran Canaria	
	
   ============================================================================== */

#include "Jugador_2.h"
#include "Tablero.h"


bool CuadroOptimo(TABLERO *tablero, TCasilla sel){
//Comprueba si la casilla tiene un solo palo libre
    if ((sel.arriba+sel.abajo+sel.izquierda+sel.derecha)>=3)
        return true;
    else
        return false;
}

int Evaluacion (TABLERO *tablero){
//Función de Evaluación
//Suma los pesos de todas las casillas con un solo palo libre
//Devuelve la diferencia hasta los Puntos Totales del juego.
    int n_filas=tablero->Filas();
    int n_cols=tablero->Columnas();
    int sumaexpectativas=0;
    for (int i=0;i<n_filas;i++)
        for (int j=0;j<n_cols;j++){
            if (tablero->tablero_virtual[i][j].peso>0)
                if (CuadroOptimo(tablero, tablero->tablero_virtual[i][j]))
                    sumaexpectativas+=tablero->tablero_virtual[i][j].peso;
        }
    return tablero->Puntos_Totales()-sumaexpectativas;
}

int NumPalosLibres(TABLERO *tablero, TCasilla sel){
// Devuelve el número de palos libres
    int numOcupados=sel.arriba+sel.abajo+sel.derecha+sel.izquierda;
    return 4-numOcupados;
}


bool CasillaAdyacenteValida(TABLERO *tablero, int fila,int columna){
//Comprueba si los palos de la casilla no generan
//a su alrededor una casilla con un solo palo
    //comprobar límites
    if (fila>=0 && fila<16 && columna>=0 && columna<16)
        if (NumPalosLibres(tablero, tablero->tablero_virtual[fila][columna])>2)
            return true;
    return false;
}

int VerificarPaloLibre(TABLERO *tablero, int f,int c){
//Devuelve el lugar del palo libre que no genere
//casillas adyacentes con un solo palo
    if ((!tablero->tablero_virtual[f][c].arriba)&&(CasillaAdyacenteValida(tablero, f-1,c))) return ARRIBA;
    if ((!tablero->tablero_virtual[f][c].abajo)&&(CasillaAdyacenteValida(tablero, f+1,c))) return ABAJO;
    if ((!tablero->tablero_virtual[f][c].derecha)&&(CasillaAdyacenteValida(tablero, f,c+1))) return DERECHA;
    if ((!tablero->tablero_virtual[f][c].izquierda)&&(CasillaAdyacenteValida(tablero, f,c-1))) return IZQUIERDA;
    return -1;
}


int MarcarPaloLibre(TABLERO *tablero, int f,int c){
    if ((!tablero->tablero_virtual[f][c].arriba)&&(CasillaAdyacenteValida(tablero, f-1,c))) return ARRIBA;
    if ((!tablero->tablero_virtual[f][c].abajo)&&(CasillaAdyacenteValida(tablero, f+1,c))) return ABAJO;
    if ((!tablero->tablero_virtual[f][c].derecha)&&(CasillaAdyacenteValida(tablero, f,c+1))) return DERECHA;
    if ((!tablero->tablero_virtual[f][c].izquierda)&&(CasillaAdyacenteValida(tablero, f,c-1))) return IZQUIERDA;
    //en el caso de que ninguna de las casillas adyacentes tenga
    //un número de palos libres > 1 entonces selecciona cualquiera de los palos libres
    if (!tablero->tablero_virtual[f][c].arriba) return ARRIBA;
    if (!tablero->tablero_virtual[f][c].abajo) return ABAJO;
    if (!tablero->tablero_virtual[f][c].derecha) return DERECHA;
    if (!tablero->tablero_virtual[f][c].izquierda) return IZQUIERDA;
    return -1;
}

int PaloLibre(TABLERO *tablero, int f,int c){
// Devuelve el lugar en el que se situa el palo libre
    if (!tablero->tablero_virtual[f][c].arriba) return ARRIBA;
    if (!tablero->tablero_virtual[f][c].abajo) return ABAJO;
    if (!tablero->tablero_virtual[f][c].derecha) return DERECHA;
    if (!tablero->tablero_virtual[f][c].izquierda) return IZQUIERDA;
    return -1;
}


void MarcarTodosLosPalos(TABLERO *tablero, int fila,int col){
//Marca todos los palos de la casilla (fila,col)
        int lugar=PaloLibre(tablero, fila,col);
        while (lugar!=-1){
            switch (lugar) {
              case 0: tablero->tablero_virtual[fila][col].arriba=true;
                      tablero->tablero_virtual[fila-1][col].abajo=true;
                      break;
              case 1: tablero->tablero_virtual[fila][col].abajo=true;
                      tablero->tablero_virtual[fila+1][col].arriba=true;
                      break;
              case 2: tablero->tablero_virtual[fila][col].derecha=true;
                      tablero->tablero_virtual[fila][col+1].izquierda=true;
                      break;
              case 3: tablero->tablero_virtual[fila][col].izquierda=true;
                      tablero->tablero_virtual[fila][col-1].derecha=true;
                      break;
            }
            lugar=PaloLibre(tablero, fila,col);
        }
}

bool CasillaSoloPalo(TABLERO *tablero, int &fila,int &columna){
//Localiza la primera casilla con un solo palo libre
    int n_filas=tablero->Filas();
    int n_cols=tablero->Columnas();
    int i,j;
    bool encontrado=false;
    i=0;
    while (i<n_filas && !encontrado){
        j=0;
        while (j<n_cols && !encontrado){
            if ((tablero->tablero_virtual[i][j].peso>=0)
               && (NumPalosLibres(tablero, tablero->tablero_virtual[i][j])==1)){
                    encontrado=true;
                    fila=i; columna=j;
            }
            j++;
        }
        i++;
    }
    return encontrado;
}


bool MejorJugada(TABLERO *tablero, int &fila,int &col,int &lugar){
// Busca el cuadro de mejores expectativas
// Salida: fila , col, lugar (del palo) -- casilla seleccionada

    int sumaPesos;
    int f=0,c=0,l;
    int min=tablero->Puntos_Totales();

    for (int i=0;i<tablero->Filas();i++)
        for (int j=0;j<tablero->Columnas();j++){
          sumaPesos=0;
          //Localiza la casilla con dos palos libres
          if ((tablero->tablero_virtual[i][j].peso>-1)
             &&(NumPalosLibres(tablero, tablero->tablero_virtual[i][j])==2)){
                l=PaloLibre(tablero, i,j);
                MarcarTodosLosPalos(tablero, i,j);
                sumaPesos+=(tablero->tablero_virtual[i][j].peso);
                //Consigue todas las casillas que se puedan generar
                while (CasillaSoloPalo(tablero, f,c)){
                        MarcarTodosLosPalos(tablero, f,c);
                        sumaPesos+=(tablero->tablero_virtual[f][c].peso);
                }
                //Búsqueda de la jugada con menor peso
                if (sumaPesos<min){
                        min=sumaPesos;
                        fila=i;col=j;lugar=l;
                }
          }
        }
    if (min==(tablero->Puntos_Totales()))
        return false;
    else
        return true;
}

bool Jugador_1(TABLERO *tablero, TMovimiento &palo){
// A partir del estado del tablero devuelve un movimiento
// Entrada: tablero -- estado del tablero
// Salida: palo -- movimiento del jugador
//         true -- si movimiento es correcto
    int n_filas=tablero->Filas();
    int n_cols=tablero->Columnas();
    int i,j;
    int palosLibres;
    int fila,columna;
    int max=2;
    int lugar=-1;
    int maxfila=0;
    int maxcol=0;
    bool encontrado=false;
    //Recorre el tablero:
    // 1)Busca los cuadros con un solo palo libre
    // 2)Busca el cuadro con más palos libres y menor peso
    //   2.1.)Comprueba que no se producen cuadros con un palo libre adyacentes
    // 3)Busca el cuadro que genere la suma de cuadros consecutivos
    //   conseguidos de menor peso.
    i=0;
    while (i<n_filas && !encontrado){
        j=0;
        while (j<n_cols && !encontrado){
            if (tablero->tablero_virtual[i][j].peso>=0){
                palosLibres=NumPalosLibres(tablero, tablero->tablero_virtual[i][j]);
                if (palosLibres==1)
                {
                    //captura todos los cuadros de un palo libre
                    encontrado=true;
                    fila=i; columna=j;
                }else if ((palosLibres>max)&&(VerificarPaloLibre(tablero, i,j)!=-1)){
                     //guarda la posición de una casilla que no genere un cuadro
                     //de un solo palo libre
                     max=palosLibres; maxfila=i; maxcol=j;
                }
            }
            j++;
        }
        i++;
    }
    if (encontrado){
        palo.lugar=MarcarPaloLibre(tablero, fila,columna);
        palo.fila=fila; palo.columna=columna;
    }else{
        //Buscar mejor jugada
        if (max==2){
            if (!MejorJugada(tablero, maxfila,maxcol,lugar)){
                maxfila=0; maxcol=0;
            }
        }
        if (lugar==-1)
            palo.lugar=MarcarPaloLibre(tablero, maxfila,maxcol);
        else
            palo.lugar=lugar;
        palo.fila=maxfila; palo.columna=maxcol;

    }
    return tablero->Poner_Palo(palo);
}

// Fin del código para el "jugador Máquina"
//*********************************************************************/

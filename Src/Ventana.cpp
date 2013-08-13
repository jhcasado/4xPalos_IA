/* ==============================================================================
	
	4XPalos
	
	José Angel Montelongo Reyes 
	Javier Honorio Casado Fernández
	
	Universidad de Las Palmas de Gran Canaria	
	
   ============================================================================== */

#include <vcl.h>
#pragma hdrstop

#include "Ventana.h"
#include "Jugador_1.h"
#include "Jugador_2.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormTablero *FormTablero;
TABLERO *tablero = NULL;
int posX = 0;
int posY = 0;


__fastcall TFormTablero::TFormTablero(TComponent* Owner) : TForm(Owner)
{
}


// Funciones gáficas

void TFormTablero::Dibujar_Rectangulo(TPaintBox *PaintBox, int x1, int y1, int x2, int y2, unsigned long c)
// dibuhja un rectángulo en pantalla
// entrada : TPaintBox = control de destino
//           (x1, y1, x2, y2) = coordenadas del rectángulo
//           c = color
// salida : -
{
   PaintBox->Canvas->Pen->Mode = pmCopy;
   PaintBox->Canvas->Pen->Color = (TColor)c;
   PaintBox->Canvas->Brush->Color = (TColor)c;
   PaintBox->Canvas->Rectangle(x1, y1, x2, y2);
}


void TFormTablero::Dibujar_Casilla(TPaintBox *PaintBox, unsigned long cfondo,
                     unsigned long cpalo_arriba, unsigned long cpalo_abajo,
                     unsigned long cpalo_derecha, unsigned long cpalo_izquierda,
                     unsigned long ccontorno,
                     int x, int y, int peso)
// dibuja una casilla en pantalla
// entrada : TPaintBox = control de destino
//           cfondo = color del fondo
//           cpalo... = color del palo de esa posición
//           ccontorno = color del contorno
//           x = pos x de la casilla
//           y = pos y de la casilla
//           peso = peso de la pieza
// salida : -
{
    Dibujar_Rectangulo(PaintBox, x, y + 2, x + 2, y + 14, cpalo_izquierda);
    Dibujar_Rectangulo(PaintBox, x + 2, y + 2, x + 14, y + 14, cfondo);
    Dibujar_Rectangulo(PaintBox, x + 14, y + 2, x + 16, y + 14, cpalo_derecha);

    if (peso >= 0)
    {
        PaintBox->Canvas->Brush->Color = (TColor)cfondo;
        PaintBox->Canvas->Font->Color = (TColor)RGB(255, 255, 255);
        PaintBox->Canvas->TextOut(x + 4, y + 1, IntToStr(peso));
    }

    Dibujar_Rectangulo(PaintBox, x, y, x + 2, y + 2, ccontorno);
    Dibujar_Rectangulo(PaintBox, x + 2, y, x + 14, y + 2, cpalo_arriba);
    Dibujar_Rectangulo(PaintBox, x + 14, y, x + 16, y + 2, ccontorno);

    Dibujar_Rectangulo(PaintBox, x, y + 14, x + 2, y + 16, ccontorno);
    Dibujar_Rectangulo(PaintBox, x + 2, y + 14, x + 14, y + 16, cpalo_abajo);
    Dibujar_Rectangulo(PaintBox, x + 14, y + 14, x + 16, y + 16, ccontorno);
}


void TFormTablero::Dibujar_Tablero(int x1, int y1, int x2, int y2, TABLERO *t, TPaintBox *PaintBox)
// dibuja el tablero en pantalla
// entrada : t = variable que contiene el tablero
//           PaintBox = control de destino
// salida : -
{
    int peso;
    unsigned long carriba, cabajo, cizquierda, cderecha;

    for (int y = y1; y < y2; y ++)
    {
        for (int x = x1; x < x2; x ++)
        {
            peso = t->tablero_virtual [y][x].peso;
            carriba = RGB(64, 64, 64);
            cabajo = RGB(64, 64, 64);
            cizquierda = RGB(64, 64, 64);
            cderecha = RGB(64, 64, 64);

            if (t->tablero_virtual [y][x].arriba)
            {
                carriba = RGB(200, 127, 0);
            }

            if (t->tablero_virtual [y][x].abajo)
            {
                cabajo = RGB(200, 127, 0);
            }

            if (t->tablero_virtual [y][x].izquierda)
            {
                cizquierda = RGB(200, 127, 0);
            }

            if (t->tablero_virtual [y][x].derecha)
            {
                cderecha = RGB(200, 127, 0);
            }

            switch (peso)
            {
                case -1: // casilla negra
                    Dibujar_Casilla(PaintBox, RGB(0, 0, 0),
                                    carriba, cabajo, cderecha, cizquierda,
                                    RGB(64, 64, 64),
                                    x * 16, y * 16, peso);
                break;

                case -2: // jugador 1
                    Dibujar_Casilla(PaintBox, RGB(200, 0, 0),
                                    carriba, cabajo, cderecha, cizquierda,
                                    RGB(64, 64, 64),
                                    x * 16, y * 16, peso);
                break;

                case -3: // jugador 2
                    Dibujar_Casilla(PaintBox, RGB(0, 0, 200),
                                    carriba, cabajo, cderecha, cizquierda,
                                    RGB(64, 64, 64),
                                    x * 16, y * 16, peso);
                break;

                default: // casilla normal
                    Dibujar_Casilla(PaintBox, RGB(64, 64, 64),
                                    carriba, cabajo, cderecha, cizquierda,
                                    RGB(64, 64, 64),
                                    x * 16, y * 16, peso);
                break;
            }
        }
    }
}


void TFormTablero::Inicializar_Partida()
// inicializa una partida
// entrada : -
// salida : -
{
    AnsiString s_turnos [] = { "Rojo", "Azul" };

    if (tablero != NULL)
    {
        delete tablero;
    }

    // Cambia estos valores para cambiar el tamaño del tablero
    tablero = new TABLERO(16, 16, 16 + random(64));

    FormTablero->ComboBoxJugador1->ItemIndex = 0;
    FormTablero->ComboBoxJugador2->ItemIndex = 0;

    FormTablero->Label4->Caption = "Filas : " + IntToStr(tablero->Filas());
    FormTablero->Label5->Caption = "Columnas : " + IntToStr(tablero->Columnas());
    FormTablero->Label6->Caption = "Negras : " + IntToStr(tablero->Casillas_Negras());
    FormTablero->Label7->Caption = "Ptos Totales : " + IntToStr(tablero->Puntos_Totales());

    FormTablero->Label3->Caption = "Turno : " + s_turnos [tablero->Turno()];
    FormTablero->Label1->Caption = "Jugador Rojo : " + IntToStr(tablero->Puntos_Jugador(0));
    FormTablero->Label2->Caption = "Jugador Azul : " + IntToStr(tablero->Puntos_Jugador(1));

    FormTablero->Timer1->Enabled = true;
}


void TFormTablero::Mensaje_Final_Partida()
// muestra el mensaje de final de partida
// entrada : -
// salida : -
{
    char sr [100];

    sprintf(sr, "Resultado de la partida\n - Jugador Rojo : %d\n - Jugador Azul : %d\n",
            tablero->Puntos_Jugador(0), tablero->Puntos_Jugador(1));

    if(Application->MessageBox(sr, "Game Over", MB_OK))
    {
        Inicializar_Partida();
        Dibujar_Tablero(0, 0, tablero->Columnas(), tablero->Filas(), tablero, FormTablero->PaintBoxTablero);
    }
}


bool TFormTablero::Actualizar_Partida(TABLERO *t, TMovimiento palo)
// redibuja el tablero
// entrada : t = tablero
//           palo = lugar donde poner el palo
// salida : true si se ha terminado la partida
{
    int xo, yo, xd, yd;
    AnsiString s_turnos [] = { "Rojo", "Azul" };

    xo = palo.columna - 3;
    yo = palo.fila - 3;
    xd = palo.columna + 3;
    yd = palo.fila + 3;

    if (xo < 0) xo = 0;
    if (yo < 0) yo = 0;
    if (xd > t->Columnas()) xd = t->Columnas();
    if (yd > t->Filas()) yd = t->Filas();

    // dibujamos los 8 vecinos

    Dibujar_Tablero(xo, yo, xd, yd, t, FormTablero->PaintBoxTablero);

    // actualiza el marcador

    FormTablero->Label3->Caption = "Turno : " + s_turnos [tablero->Turno()];
    FormTablero->Label1->Caption = "Jugador Rojo : " + IntToStr(tablero->Puntos_Jugador(0));
    FormTablero->Label2->Caption = "Jugador Azul : " + IntToStr(tablero->Puntos_Jugador(1));

    if (t->Game_Over (POR_FINAL))
    {
        return true;
    }

    return false;
}


bool TFormTablero::Movimiento_Humano(TABLERO *t, int xr, int yr, int lugar, TMovimiento &palo)
// gestiona el movimiento de un humano
// entrada : t = tablero
//           xr = pos x del ratón
//           yr = pos y del ratón
//           palo = lugar donde vamos a poner el palo
// salida : true, pudo poner el palo
//          false, no pudo ponerlo
{
    bool b_poner_palo;

    if (((xr / 16) < t->Columnas()) &&
        ((yr / 16) < t->Filas()))
    {
        palo.columna = xr / 16;
        palo.fila = yr / 16;
        palo.lugar = lugar;
        b_poner_palo = t->Poner_Palo(palo);
    }

    return b_poner_palo;
}


bool TFormTablero::Movimiento_Maquina(TABLERO *t, TMovimiento &palo)
// gestiona el movimiento de la maquina de forma aleatoria
// entrada : t = tablero
//           palo = lugar donde vamos a poner el palo
// salida : true, pudo poner el palo
//          false, no pudo ponerlo
{
    bool b_poner_palo;

    randomize();

    do
    {
        palo.columna = random(t->Columnas());
        palo.fila = random(t->Filas());
        palo.lugar = random(4);
        b_poner_palo = t->Poner_Palo(palo);
    } while (!b_poner_palo);

    return b_poner_palo;
}

void TFormTablero::Colocar_Palo(int x, int y, int lugar)
{
    TMovimiento palo;

    if ((ComboBoxJugador1->ItemIndex == 0) && (ComboBoxJugador2->ItemIndex == 0)) // hombre vs hombre
    {
        if (Movimiento_Humano(tablero, x, y, lugar, palo))
        {
            if (Actualizar_Partida(tablero, palo))
            {
                Timer1->Enabled = false;
                Mensaje_Final_Partida();
            }
        }
    }

    if ((ComboBoxJugador1->ItemIndex == 0) && (ComboBoxJugador2->ItemIndex == 1)) // hombre vs maquina
    {
        if (tablero->Turno() == 0)
        {
            if (Movimiento_Humano(tablero, x, y, lugar, palo))
            {
                if (Actualizar_Partida(tablero, palo))
                {
                    Timer1->Enabled = false;
                    Mensaje_Final_Partida();
                }
            }
        }
    }

    if ((ComboBoxJugador1->ItemIndex == 1) && (ComboBoxJugador2->ItemIndex == 0)) // maquina vs hombre
    {
        if (tablero->Turno() == 1)
        {
            if (Movimiento_Humano(tablero, x, y, lugar, palo))
            {
                if (Actualizar_Partida(tablero, palo))
                {
                    Timer1->Enabled = false;
                    Mensaje_Final_Partida();
                }
            }
        }
    }
}

// gestion de la ventana

void __fastcall TFormTablero::FormCloseQuery(TObject *Sender, bool &CanClose)
// le preguntamos al usuario si realmente desea salir
{
    if(Application->MessageBox("¿Salir del programa?", "Salir", MB_YESNO) == ID_NO)
    {
        CanClose = False;
    }
}


void __fastcall TFormTablero::FormCreate(TObject *Sender)
// inicializamos las variables
{
    Inicializar_Partida();
}


void __fastcall TFormTablero::ButtonNuevaPartidaClick(TObject *Sender)
// pulsamos el botón de partida nueva
{
    if(Application->MessageBox("¿Abandonar la partida actual?", "Partida Nueva", MB_YESNO) == ID_YES)
    {
        Inicializar_Partida();
        Dibujar_Tablero(0, 0, tablero->Columnas(), tablero->Filas(), tablero, FormTablero->PaintBoxTablero);
    }
}


void __fastcall TFormTablero::ButtonSalirClick(TObject *Sender)
// salimos al pulsar el botón salir
{
    Close();
}


void __fastcall TFormTablero::PaintBoxTableroPaint(TObject *Sender)
// actualiza la pantalla
{
    Dibujar_Tablero(0, 0, tablero->Columnas(), tablero->Filas(), tablero, FormTablero->PaintBoxTablero);
}


void __fastcall TFormTablero::FormClose(TObject *Sender, TCloseAction &Action)
// al cerrar la ventana
{
    delete tablero;
}

void __fastcall TFormTablero::Timer1Timer(TObject *Sender)
// control del turno de la maquina
{
    TMovimiento palo;

    if ((ComboBoxJugador1->ItemIndex == 1) && (ComboBoxJugador2->ItemIndex == 1)) // máquina vs máquina
    {
        if (tablero->Turno() == 0)
        {
            if (Jugador_1(tablero, palo)) // aquí ponemos la primera función
            {
                if (Actualizar_Partida(tablero, palo))
                {
                    Timer1->Enabled = false;
                    Mensaje_Final_Partida();
                }
            }
        }

        if (tablero->Turno() == 1)
        {
            if (Jugador_2(tablero, palo)) // aquí ponemos la segunda función
            {
                if (Actualizar_Partida(tablero, palo))
                {
                    Timer1->Enabled = false;
                    Mensaje_Final_Partida();
                }
            }
        }
    }

    if ((ComboBoxJugador1->ItemIndex == 0) && (ComboBoxJugador2->ItemIndex == 1)) // hombre vs maquina
    {
        if (tablero->Turno() == 1)
        {
            if (Jugador_2(tablero, palo)) // aquí ponemos la segunda función
            {
                if (Actualizar_Partida(tablero, palo))
                {
                    Timer1->Enabled = false;
                    Mensaje_Final_Partida();
                }
            }
        }
    }

    if ((ComboBoxJugador1->ItemIndex == 1) && (ComboBoxJugador2->ItemIndex == 0)) // maquina vs hombre
    {
        if (tablero->Turno() == 0)
        {
            if (Jugador_1(tablero, palo)) // aquí ponemos la primera función
            {
                if (Actualizar_Partida(tablero, palo))
                {
                    Timer1->Enabled = false;
                    Mensaje_Final_Partida();
                }
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormTablero::Arriba1Click(TObject *Sender)
{
    Colocar_Palo(posX, posY, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTablero::Abajo1Click(TObject *Sender)
{
    Colocar_Palo(posX, posY, 1);
}
//---------------------------------------------------------------------------

void __fastcall TFormTablero::Derecha1Click(TObject *Sender)
{
    Colocar_Palo(posX, posY, 2);
}
//---------------------------------------------------------------------------

void __fastcall TFormTablero::Izquierda1Click(TObject *Sender)
{
    Colocar_Palo(posX, posY, 3);
}
//---------------------------------------------------------------------------

void __fastcall TFormTablero::PaintBoxTableroClick(TObject *Sender)
{
    PopupMenuPosicion->Popup(
        PanelTablero->Left + FormTablero->Left + posX + 6,
        PanelTablero->Top + FormTablero->Top + posY + 32);
}
//---------------------------------------------------------------------------


void __fastcall TFormTablero::PaintBoxTableroMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    posX = X;
    posY = Y;
}
//---------------------------------------------------------------------------


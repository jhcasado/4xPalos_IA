/* ==============================================================================
	
	4XPalos
	
	José Angel Montelongo Reyes 
	Javier Honorio Casado Fernández
	
	Universidad de Las Palmas de Gran Canaria	
	
   ============================================================================== */

#ifndef VentanaH
#define VentanaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

#include <stdlib.h>
#include <stdio.h>
#include "Tablero.h"
#include <Menus.hpp>

//---------------------------------------------------------------------------
class TFormTablero : public TForm
{
__published:	// IDE-managed Components
        TButton *ButtonNuevaPartida;
        TGroupBox *GroupBoxJugador2;
        TGroupBox *GroupBoxJugador1;
        TComboBox *ComboBoxJugador1;
        TComboBox *ComboBoxJugador2;
        TPanel *PanelTablero;
        TGroupBox *GroupBoxPuntos;
    TLabel *Label1;
    TLabel *Label2;
        TPaintBox *PaintBoxTablero;
        TButton *ButtonSalir;
        TGroupBox *GroupBoxInformacion;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TTimer *Timer1;
        TPopupMenu *PopupMenuPosicion;
        TMenuItem *Arriba1;
        TMenuItem *Abajo1;
        TMenuItem *Derecha1;
        TMenuItem *Izquierda1;
    void __fastcall ButtonSalirClick(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall ButtonNuevaPartidaClick(TObject *Sender);
    void __fastcall PaintBoxTableroPaint(TObject *Sender);
    
    
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Arriba1Click(TObject *Sender);
        void __fastcall Abajo1Click(TObject *Sender);
        void __fastcall Derecha1Click(TObject *Sender);
        void __fastcall Izquierda1Click(TObject *Sender);
        void __fastcall PaintBoxTableroClick(TObject *Sender);
        void __fastcall PaintBoxTableroMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
        void Dibujar_Rectangulo(TPaintBox *PaintBox, int x1, int y1, int x2, int y2, unsigned long c);
        void Dibujar_Casilla(TPaintBox *PaintBox, unsigned long cfondo,
                     unsigned long cpalo_arriba, unsigned long cpalo_abajo,
                     unsigned long cpalo_derecha, unsigned long cpalo_izquierda,
                     unsigned long ccontorno,
                     int x, int y, int peso);
        void Dibujar_Tablero(int x1, int y1, int x2, int y2, TABLERO *t, TPaintBox *PaintBox);
        void Inicializar_Partida();
        void Mensaje_Final_Partida();
        bool Actualizar_Partida(TABLERO *t, TMovimiento palo);
        bool Movimiento_Humano(TABLERO *t, int xr, int yr, int lugar, TMovimiento &palo);
        bool Movimiento_Maquina(TABLERO *t, TMovimiento &palo);
	void Colocar_Palo(int x, int y, int lugar);
public:		// User declarations
    __fastcall TFormTablero(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTablero *FormTablero;
//---------------------------------------------------------------------------


void Dibujar_Rectangulo(TPaintBox *PaintBox, int x1, int y1, int x2, int y2, unsigned long c);
// dibuja un rectángulo en pantalla

void Dibujar_Casilla(TPaintBox *PaintBox, unsigned long cfondo,
                     unsigned long cpalo_arriba, unsigned long cpalo_abajo,
                     unsigned long cpalo_derecha, unsigned long cpalo_izquierda,
                     unsigned long ccontorno,
                     int x, int y, int peso);
// dibuja una casilla en pantalla

void Dibujar_Tablero(TABLERO *t, TPaintBox *PaintBox);
// dibuja el tablero en pantalla

void Inicializar_Partida();
// inicializa una partida

void Mensaje_Final_Partida();
// muestra el mensaje de final de partida

bool Actualizar_Partida(TABLERO *t, TMovimiento palo);
// redibuja el tablero

bool Movimiento_Humano(TABLERO *t, int xr, int yr, TMovimiento &palo);
// gestiona el movimiento de un humano

bool Movimiento_Maquina(TABLERO *t, TMovimiento &palo);
// gestiona el movimiento de la maquina


#endif

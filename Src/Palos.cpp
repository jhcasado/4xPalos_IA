/* ==============================================================================
	
	4XPalos
	
	José Angel Montelongo Reyes 
	Javier Honorio Casado Fernández
	
	Universidad de Las Palmas de Gran Canaria	
	
   ============================================================================== */

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USEFORM("Ventana.cpp", FormTablero);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        Application->Initialize();
        Application->CreateForm(__classid(TFormTablero), &FormTablero);
                 Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------

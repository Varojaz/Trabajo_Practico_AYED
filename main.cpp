#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cmath> //solo lo incluí para utilizar la funcion modulo
#define FILAS 200
#define COLUMNAS 200
using namespace std;

/*HIPOTESIS IMPORTANTES.
1. Logicamente el archivo debe venir ordenado, si no no se podria saber en que orden se ejecutan las instrucciones.
2. Utilizamos un unico archivo donde se encuentra la totalidad de instrucciones a ejecutar, es decir, o se carga el archivo o se crea, no ambas.
*/


struct NodoSE
{
    int x, y;
    NodoSE* sgte = NULL;
};

struct Orden
{
    unsigned int espera = 0;
    bool soltarGranada1 = false;
    bool soltarGranada2 = false;
    bool ataqueKamikaze = false;
    bool aterrizaje = false;
    bool despegue = false;
    int siguientex =0;
    int siguientey= 0;
};

struct OrdenArchivo
{
    int x =0;
    int y =0;
    unsigned int espera = 0;
    bool soltarGranada1 = false;
    bool soltarGranada2 = false;
    bool ataqueKamikaze = false;
    bool aterrizaje = false;
    bool despegue = false;
    int siguientex =0;
    int siguientey= 0;
};

void inicializar(Orden tabla[][COLUMNAS]);
void MostrarMenu();
void Operacion(char opcion, Orden Tabla[][200], NodoSE*& lista);
void CargarAAenMemoria(Orden Tabla[][200], NodoSE*& lista);
void MostrarAtaqueCargado(NodoSE* lista, Orden Tabla[][COLUMNAS]);
bool CargarATabla (char FileName[], Orden Tabla[][COLUMNAS]);
bool Validacion (char FileName[], NodoSE*& lista);
void liberarLista(NodoSE*& lista);

void inicializar(Orden tabla[][COLUMNAS])
{
    for(int i = 0; i < FILAS; i++)
    {
        for(int j = 0; j < COLUMNAS; j++)
        {
            tabla[i][j] = Orden(); // Inicializa todos los campos por defecto
        }
    }
}

void MostrarMenu()
{
    cout<< "1. Cargar archivo de ataque en memoria."<<endl;
    cout<< "2. Mostrar ataque cargado."<<endl;
    return;
}

void Operacion(char opcion, Orden Tabla[][COLUMANAS], NodoSE*& lista)
{
    switch (opcion)
    {
    case '1':
        CargarAAenMemoria(Tabla, lista);
        break;
    case '2':
        MostrarAtaqueCargado(lista, Tabla);
        break;
    default:
        break;
    }
    return;
}

void CargarAAenMemoria(Orden Tabla[][COLUMNAS], NodoSE*& lista) //AA: archivo de ataque
{
    FILE* f;
    char FileName[100];
    cout<<endl<<"Ingrese ruta y nombre del archivo: "<<endl;
    cin>>FileName;
    f = fopen(FileName, "rb");
    if (f)
    {
        cout<< "Archivo encontrado."<<endl<<endl;
        fclose(f);
        if(Validacion(FileName, lista))
        {
            CargarATabla(FileName, Tabla);
            cout<< "ATAQUE CARGADO EXITOSAMENTE."<<endl;
        }
    }
    else 
        cout<< "Error al abrir el archivo / Archivo inexistente."<<endl;
    return;
}

bool CargarATabla (char FileName[], Orden Tabla[][COLUMNAS])
{
    FILE* f = fopen(FileName, "rb");
    OrdenArchivo buffer;
    inicializar(Tabla);
    while (fread(&buffer, sizeof(OrdenArchivo), 1, f))
    {
        Tabla[buffer.x][buffer.y].despegue = buffer.despegue;
        Tabla[buffer.x][buffer.y].aterrizaje = buffer.aterrizaje;
        Tabla[buffer.x][buffer.y].soltarGranada1 = buffer.soltarGranada1;
        Tabla[buffer.x][buffer.y].soltarGranada2 = buffer.soltarGranada2;
        Tabla[buffer.x][buffer.y].ataqueKamikaze = buffer.ataqueKamikaze;
        Tabla[buffer.x][buffer.y].espera = buffer.espera;
        Tabla[buffer.x][buffer.y].siguientex = buffer.siguientex;
        Tabla[buffer.x][buffer.y].siguientey = buffer.siguientey;
    }
    fclose(f);
    return true;
}

void MostrarAtaqueCargado(NodoSE* lista, Orden Tabla[][COLUMNAS]) //En la lista están cargadas en orden, HIPOTESIS FUNDAMENTAL que en el archivo ya vengan en orden.
{
    int mostrado = 0;
    if(!lista)
    {
        cout<< "No hay ataques cargados."<<endl;
        return;
    }
    cout<< "MOSTRANDO ATAQUE CARGADO: "<<endl;
    while(lista)
    {
        cout<< "CASILLERO ("<<lista->x<<" ; "<<lista->y<<")"<<endl;
        if(Tabla[lista->x][lista->y].despegue)
            cout<< "- Despegue."<<endl;
        if(Tabla[lista->x][lista->y].aterrizaje)
            cout<< "- Aterrizaje."<<endl;
        if(Tabla[lista->x][lista->y].soltarGranada1)
            cout<< "- Solatar granada 1."<<endl;
        if(Tabla[lista->x][lista->y].soltarGranada2)
            cout<< "- Solatar granada 2."<<endl;
        if(Tabla[lista->x][lista->y].ataqueKamikaze)
            cout<< "- Ataque kamikaze."<<endl;
        if(Tabla[lista->x][lista->y].espera)
            cout<< "- Esperar: "<<Tabla[lista->x][lista->y].espera<<"."<<endl;
        if(!Tabla[lista->x][lista->y].aterrizaje && !Tabla[lista->x][lista->y].ataqueKamikaze)
            cout<<"- Siguiente posicion: ("<<Tabla[lista->x][lista->y].siguientex<<" ; "<<Tabla[lista->x][lista->y].siguientey<<")"<<endl;

        mostrado ++;
        lista = lista->sgte;

        if(!(mostrado%3) && lista)
        {
            cout<< "Presione ENTER para ver mas."<<endl;
            cin.ignore();
            cin.get();
            cout<<endl;
        }
    }
    cout<< "FIN DEL ATAQUE"<<endl;
    return;
}

bool Validacion (char FileName[], NodoSE*& lista)
{
    NodoSE*aux = lista;
    liberarLista(lista);
    FILE* f = fopen(FileName, "rb"); //NO verifico denuevo porque ya lo hice en CargarAA
    OrdenArchivo buffer;
    bool valido = true;
    int despegue = 0, aterrizaje = 0, kamikaze = 0;
    while (fread(&buffer, sizeof(OrdenArchivo), 1, f) && valido == true)
        {
            if (buffer.x < 0 || buffer.x >= 200 || buffer.y < 0 || buffer.y >= COLUMNAS) //Valido si esta dentro de la tabla
            {
                cout << "ERROR: Coordenadas (" << buffer.x << "," << buffer.y << ") fuera de rango." << endl;
                fclose(f);
                return false;
            }
            if(buffer.despegue == true) //validando que exista despegue.
            {
                despegue ++;
                if(buffer.soltarGranada1 || buffer.soltarGranada2 || buffer.ataqueKamikaze || buffer.aterrizaje) //validando que en una misma orden no haya otra instruccion
                {
                    cout<< "ERROR: no se puede despegar y atacar en la misma instruccion."<<endl;
                    fclose(f);
                    return false;
                }
                if(despegue >1)
                {
                    cout<< "ERROR: no puede existir mas de un despegue."<<endl;
                    fclose(f);
                    return false;
                }
            }
            if(buffer.aterrizaje == true) //validando que exista aterrizaje
            {
                aterrizaje ++;
                if(aterrizaje >1)
                {
                    cout<< "ERROR: no puede existir mas de un aterrizaje."<<endl;
                    fclose(f);
                    return false;
                }
            }
            if(buffer.ataqueKamikaze == true) //validando que exista kamikaze
            {
                kamikaze ++;
                if(kamikaze >1)
                {
                    cout<< "ERROR: no puede existir mas de un kamikaze."<<endl;
                    fclose(f);
                    return false;
                }
            }
            if(kamikaze + aterrizaje > 1)
            {
                cout<< "ERROR: ataque y kamikaze son excluyentes,"<<endl;
                fclose(f);
                return false;
            }
            if (!buffer.aterrizaje && !buffer.ataqueKamikaze) //Porque la instruccion final tiene siguiente x,y = 0
            {
                if (abs(buffer.x - buffer.siguientex) > 1 || abs(buffer.y - buffer.siguientey) > 1)
                {
                    cout << "ERROR: Celdas no aledanias entre (" << buffer.x << "," << buffer.y 
                        << ") y (" << buffer.siguientex << "," << buffer.siguientey << ")." << endl;
                    fclose(f);
                    return false;
                }
            }
            aux = lista;
            NodoSE* ultimo = NULL;
            if (lista == NULL) //Lista para almacenar celdas pisadas.
            {
                lista = new NodoSE();
                lista->x = buffer.x;
                lista->y = buffer.y;
                lista->sgte = NULL;
            }
            else
            {
                while (aux) //verifico que no se pise dos veces una celda
                {
                    if( (aux->x == buffer.x) && (aux->y == buffer.y)) //mientras la recorro, si coinciden las celdas aborto
                    {
                        cout<< "ERROR: celda sobreescrita."<<endl;
                        fclose(f);
                        return false;
                    }
                    ultimo = aux;
                    aux = aux->sgte;
                }
                ultimo->sgte = new NodoSE(); //inserto al final para que me sea mas facil al MostrarAtaqueCarhado y no hacer una ListaDE.
                ultimo = ultimo->sgte;
                ultimo->x = buffer.x;
                ultimo->y = buffer.y;
                ultimo->sgte = NULL;
            }
        }
    if(!despegue) //valido que exista despegue
    {
        cout<<"ERROR: no existe instruccion de despegue."<<endl;
        fclose(f);
        return false;
    }
    if(!aterrizaje && !kamikaze) //valido que finalice el recorrido del dron.
    {
        cout<<"ERROR: no existe aterrizaje o kamikaze que finalice la operacion."<<endl;
        fclose(f);
        return false;
    }
    fclose(f);
    return true;
}

void liberarLista(NodoSE*& lista)
{
    while (lista)
    {
        NodoSE* aux = lista;
        lista = lista->sgte;
        delete aux;
    }
}


int main()
{

    NodoSE* lista = NULL;
    Orden Tabla [FILAS][COLUMNAS]; //En cada casillero puede haber una orden, hacer inicialización
    inicializar(Tabla);

    cout<< "=================================================="<<endl<<
    "DRONE: Offline System."<<endl<<"=================================================="<<endl;
    cout<<endl<<"INGRESE UNA OPCION: "<<endl<<endl;
    char opcion;
    MostrarMenu();
    do
    {
        cin>> opcion;
        Operacion(opcion, Tabla, lista);
    } while (opcion != 's');

    liberarLista(lista);
    
    return 0;
}

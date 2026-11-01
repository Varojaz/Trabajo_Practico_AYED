#include <iostream>
#include <stdio.h>
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
void Operacion(char opcion, Orden Tabla[][COLUMNAS], NodoSE*& lista);
void CargarAAenMemoria(Orden Tabla[][COLUMNAS], NodoSE*& lista);
void MostrarAtaqueCargado(NodoSE* lista, Orden Tabla[][COLUMNAS]);
bool CargarATabla (char FileName[], Orden Tabla[][COLUMNAS]);
bool Validacion (char FileName[], NodoSE*& lista);
void liberarLista(NodoSE*& lista);
void CrearArchivoDeAtaqueNuevo();
bool CorregirRegistroEnArchivo(const char FileName[], int xBuscado, int yBuscado);


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
    cout << "3. Crear archivo de ataque nuevo." << endl;
    cout << "4. Corregir un registro del archivo de ataque." << endl;
    return;
}

void Operacion(char opcion, Orden Tabla[][COLUMNAS], NodoSE*& lista)
{
    switch (opcion)
    {
    case '1':
        CargarAAenMemoria(Tabla, lista);
        break;
    case '2':
        MostrarAtaqueCargado(lista, Tabla);
        break;
    case '3':
        CrearArchivoDeAtaqueNuevo();
        break;
    case '4':
        char FileName[100];
        int x, y;

        cout << "Ingrese ruta y nombre del archivo a corregir: " << endl;
        cin >> FileName;

        cout << "Ingrese coordenada X del registro a corregir (0-199): ";
        cin >> x;
        cout << "Ingrese coordenada Y del registro a corregir (0-199): ";
        cin >> y;

        if (!CorregirRegistroEnArchivo(FileName, x, y))
        {
            cout << "No se pudo corregir el registro." << endl;
        }
        else
        {
            cout << "Registro corregido correctamente en el archivo." << endl;
        }
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
                cout<< "ERROR: aterrizaje y kamikaze son excluyentes,"<<endl;
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

//Operacion 3: Crear un archivo de ataque nuevo 
void CrearArchivoAtaque()
{
    char FileName[100];

    cout << "Ingrese ruta y nombre del archivo a crear: ";
    cin >> FileName;

    FILE* f = fopen(FileName, "wb");

    if (!f)
    {
        cout << "Error al crear el archivo." << endl;
        return;
    }

    OrdenArchivo buffer;
    char continuar;

    do
    {
        cout << "\n--- NUEVA INSTRUCCION ---" << endl;

        do
        {
            cout << "Fila (1-200): ";
            cin >> buffer.x;

            if (buffer.x < 1 || buffer.x > 200)
                cout << "ERROR: La fila debe estar entre 1 y 200." << endl;

        } while (buffer.x < 1 || buffer.x > 200);

        do
        {
            cout << "Columna (1-200): ";
            cin >> buffer.y;

            if (buffer.y < 1 || buffer.y > 200)
                cout << "ERROR: La columna debe estar entre 1 y 200." << endl;

        } while (buffer.y < 1 || buffer.y > 200);

        // Conversión a índices de matriz (0-199)
        buffer.x--;
        buffer.y--;

        cout << "Tiempo de espera: ";
        cin >> buffer.espera;

        cout << "Despegue (1=SI 0=NO): ";
        cin >> buffer.despegue;

        cout << "Aterrizaje (1=SI 0=NO): ";
        cin >> buffer.aterrizaje;

        cout << "Soltar granada 1 (1=SI 0=NO): ";
        cin >> buffer.soltarGranada1;

        cout << "Soltar granada 2 (1=SI 0=NO): ";
        cin >> buffer.soltarGranada2;

        cout << "Ataque Kamikaze (1=SI 0=NO): ";
        cin >> buffer.ataqueKamikaze;

        if (!buffer.aterrizaje && !buffer.ataqueKamikaze)
        {
            do
            {
                cout << "Siguiente fila (1-200): ";
                cin >> buffer.siguientex;

                if (buffer.siguientex < 1 || buffer.siguientex > 200)
                    cout << "ERROR: La siguiente fila debe estar entre 1 y 200." << endl;

            } while (buffer.siguientex < 1 || buffer.siguientex > 200);

            do
            {
                cout << "Siguiente columna (1-200): ";
                cin >> buffer.siguientey;

                if (buffer.siguientey < 1 || buffer.siguientey > 200)
                    cout << "ERROR: La siguiente columna debe estar entre 1 y 200." << endl;

            } while (buffer.siguientey < 1 || buffer.siguientey > 200);

            // Conversión a índices de matriz (0-199)
            buffer.siguientex--;
            buffer.siguientey--;
        }
        else
        {
            buffer.siguientex = 0;
            buffer.siguientey = 0;
        }

        fwrite(&buffer, sizeof(OrdenArchivo), 1, f);

        cout << "\nDesea ingresar otra instruccion? (S/N): ";
        cin >> continuar;

    } while (continuar == 'S' || continuar == 's');

    fclose(f);

    NodoSE* listaAux = NULL;

    if (Validacion(FileName, listaAux))
        cout << "\nArchivo creado y validado correctamente." << endl;
    else
        cout << "\nATENCION: El archivo fue creado pero contiene errores de validacion." << endl;

    liberarLista(listaAux);
}

//Operacion 4: Corregir un registro en archivo
bool CorregirRegistroEnArchivo(const char FileName[], int xBuscado, int yBuscado)
{
    FILE* f = fopen(FileName, "rb+");
    if (!f)
    {
        cout << "ERROR: No se pudo abrir el archivo." << endl;
        return false;
    }

    OrdenArchivo registro;
    OrdenArchivo registroOriginal;
    bool encontrado = false;
    long indice = 0;
    size_t leidos;

    leidos = fread(&registro, sizeof(OrdenArchivo), 1, f);
    while (leidos == 1 && !encontrado)
    {
        if (registro.x == xBuscado && registro.y == yBuscado)
        {
            encontrado = true;
        }
        else
        {
            indice++;
            leidos = fread(&registro, sizeof(OrdenArchivo), 1, f);
        }
    }

    if (!encontrado)
    {
        cout << "No se encontro un registro con coordenadas ("
             << xBuscado << "," << yBuscado << ") en el archivo." << endl;
        fclose(f);
        return false;
    }

    cout << "\nREGISTRO ENCONTRADO:\n";
    cout << "Coordenadas (x,y): (" << registro.x << "," << registro.y << ")\n";
    cout << "Espera: " << registro.espera << endl;
    cout << "Despegue: " << registro.despegue << endl;
    cout << "Aterrizaje: " << registro.aterrizaje << endl;
    cout << "Soltar granada 1: " << registro.soltarGranada1 << endl;
    cout << "Soltar granada 2: " << registro.soltarGranada2 << endl;
    cout << "Ataque kamikaze: " << registro.ataqueKamikaze << endl;
    cout << "Siguiente (x,y): (" << registro.siguientex << "," << registro.siguientey << ")\n";

    registroOriginal = registro;

    cout << "\n--- INGRESE LOS NUEVOS DATOS ---\n";

    cout << "Espera: ";
    cin >> registro.espera;

    cout << "Despegue (1=SI 0=NO): ";
    cin >> registro.despegue;

    cout << "Aterrizaje (1=SI 0=NO): ";
    cin >> registro.aterrizaje;

    cout << "Soltar granada 1 (1=SI 0=NO): ";
    cin >> registro.soltarGranada1;

    cout << "Soltar granada 2 (1=SI 0=NO): ";
    cin >> registro.soltarGranada2;

    cout << "Ataque kamikaze (1=SI 0=NO): ";
    cin >> registro.ataqueKamikaze;

    if (!registro.aterrizaje && !registro.ataqueKamikaze)
    {
        cout << "Siguiente x (0-199): ";
        cin >> registro.siguientex;
        cout << "Siguiente y (0-199): ";
        cin >> registro.siguientey;
    }
    else
    {
        registro.siguientex = 0;
        registro.siguientey = 0;
    }

    long offset = indice * (long)sizeof(OrdenArchivo);
    if (fseek(f, offset, SEEK_SET) != 0)
    {
        cout << "ERROR: no se pudo posicionar el puntero del archivo." << endl;
        fclose(f);
        return false;
    }

    if (fwrite(&registro, sizeof(OrdenArchivo), 1, f) != 1)
    {
        cout << "ERROR: no se pudo escribir el registro actualizado." << endl;
        fclose(f);
        return false;
    }

    fclose(f);

    NodoSE* listaAux = NULL;
    if (!Validacion((char*)FileName, listaAux))
    {
        cout << "La modificacion deja el archivo en un estado invalido.\n";
        cout << "Se restaurara el registro original.\n";

        f = fopen(FileName, "rb+");
        if (!f)
        {
            cout << "ERROR: no se pudo reabrir el archivo para restaurar." << endl;
            liberarLista(listaAux);
            return false;
        }

        if (fseek(f, offset, SEEK_SET) != 0)
        {
            cout << "ERROR: no se pudo reposicionar para restaurar." << endl;
            fclose(f);
            liberarLista(listaAux);
            return false;
        }

        if (fwrite(&registroOriginal, sizeof(OrdenArchivo), 1, f) != 1)
        {
            cout << "ERROR: no se pudo restaurar el registro original." << endl;
            fclose(f);
            liberarLista(listaAux);
            return false;
        }

        fclose(f);
        liberarLista(listaAux);
        return false;
    }

    liberarLista(listaAux);
    cout << "La modificacion es valida y el registro quedo actualizado.\n";
    return true;
}


// OPERACION 7: generar HTML.
void generarHTML(const char *nombreArchivo, Orden tabla[FILAS][COLUMNAS])
{
    FILE *archivo;
    int fila, columna;
    archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL)
    {
        printf("No se pudo crear el archivo.\n");
        return;
    }
    fprintf(archivo, "<html>\n");
    fprintf(archivo, "<head>\n");
    fprintf(archivo, "  <title>Ataque</title>\n");
    fprintf(archivo, "  <style>\n");
    fprintf(archivo, "    body { background-color: #FFFFFF; color: black; font-family: monospace; padding: 20px; }\n");
    fprintf(archivo, "    table { border-collapse: collapse; background-color: #FFFFFF; table-layout: fixed; width: %dpx; }\n", (COLUMNAS + 1) * 32);
    fprintf(archivo, "    td, th { width: 30px; height: 30px; text-align: center; font-size: 11px; border: 1px solid black; box-sizing: border-box; overflow: hidden; }\n");
    fprintf(archivo, "    th { background-color: #FFFFFF; color: black; font-weight: bold; }\n");
    fprintf(archivo, "    .encabezado-eje { background-color: #FFFFFF; color: black; font-weight: bold; }\n");
    fprintf(archivo, "    .c-despegue { background-color: #d97706; color: white; font-weight: bold; }\n");
    fprintf(archivo, "    .c-aterrizaje { background-color: #059669; color: white; font-weight: bold; }\n");
    fprintf(archivo, "    .c-granada1 { background-color: #dc2626; color: white; font-weight: bold; }\n");
    fprintf(archivo, "    .c-granada2 { background-color: #b91c1c; color: white; font-weight: bold; }\n");
    fprintf(archivo, "    .c-kamikaze { background-color: #7c3aed; color: white; font-weight: bold; }\n");
    fprintf(archivo, "    .c-espera { background-color: #4b5563; color: white; }\n");
    fprintf(archivo, "    .c-movimiento { background-color: #1e3a8a; color: white; }\n");
    fprintf(archivo, "  </style>\n");
    fprintf(archivo, "</head>\n");
    fprintf(archivo, "<body>\n");

    fprintf(archivo, "<table border=\"1\">\n");

    fprintf(archivo, "<tr>\n");
    fprintf(archivo, "  <th>X\\Y</th>\n");
    for (columna = 0; columna < COLUMNAS; columna++)
    {
        fprintf(archivo, "  <th>%d</th>\n", columna);
    }
    fprintf(archivo, "</tr>\n");

    for (fila = 0; fila < FILAS; fila++)
    {
        fprintf(archivo, "<tr>\n");
        fprintf(archivo, "  <td class=\"encabezado-eje\">%d</td>\n", fila);

        for (columna = 0; columna < COLUMNAS; columna++)
        {
            if (tabla[fila][columna].despegue)
                fprintf(archivo, "  <td class=\"c-despegue\">D</td>\n");
            else if (tabla[fila][columna].aterrizaje)
                fprintf(archivo, "  <td class=\"c-aterrizaje\">A</td>\n");
            else if (tabla[fila][columna].soltarGranada1)
                fprintf(archivo, "  <td class=\"c-granada1\">G1</td>\n");
            else if (tabla[fila][columna].soltarGranada2)
                fprintf(archivo, "  <td class=\"c-granada2\">G2</td>\n");
            else if (tabla[fila][columna].ataqueKamikaze)
                fprintf(archivo, "  <td class=\"c-kamikaze\">K</td>\n");
            else if (tabla[fila][columna].espera != 0)
                fprintf(archivo, "  <td class=\"c-espera\">E</td>\n");
            else if (tabla[fila][columna].siguientex >= 0 && tabla[fila][columna].siguientey >= 0)
                fprintf(archivo, "  <td class=\"c-movimiento\">M</td>\n");
            else
                fprintf(archivo, "  <td>&nbsp;</td>\n");
        }
        fprintf(archivo, "</tr>\n");
    }
    fprintf(archivo, "</table>\n");
    fprintf(archivo, "</body>\n");
    fprintf(archivo, "</html>\n");
    fclose(archivo);
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

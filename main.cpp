#include <iostream>
#include <limits>
#include <time.h>
#define MAX 8
#define CantBombas 2
#define MAX_ESPACIOS 62
#define MAX_JUGADORES 20
#define MAX_PARTIDAS 20
#define INGRESAR 1
#define SALIR 0
#define LISTADO_DE_JUGADORES 2
#define DAR_DE_BAJA 3
// #define DAR_DE_ALTA 5
#define MODIFICAR_USUARIO 4
#define JUGAR 5
#define LISTADO_POR_FECHAS 6
#define BUSCAR_PARTIDAS_POR_FECHAS 7
#define BUSCAR_PARTIDAS_POR_NICKNAME 8

// Códigos de escape ANSI para colores
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

using namespace std;

/*
//------------------//
      PROBLEMAS o FALTA
- falta que ordene por nickname.


//------------------//
*/

struct partida {
    int dia;
    int mes;
    int ano;
    string nickname;
    int resutado; //-1 = Perdio    0 = Abandono    1 = Gano
};

struct jugador{
    int ci;
    string nombre;
    string apellido;
    string nickname;
    string contrasena;
    int diaNac;
    int mesNac;
    int anoNac;
};

//-----------------------------------------------------------------------------------------------//
//------------------------------FUNCIONES PRIMERA PARTE------------------------------------------//
//-----------------------------------------------------------------------------------------------//
bool EsValido(int x, int y);
void llenarMatriz (char m[MAX][MAX],int x,int y);
void matriz_rellena (char m[MAX][MAX]);
void matriz_inicial(char m1[MAX][MAX]);
void mostrar_matriz(char m1[MAX][MAX]);
void Marcar_posicion(char m1[MAX][MAX], int x, int y);
void expansion (char m[MAX][MAX],char m1[MAX][MAX], int x, int y);
void the_champion (char m1[MAX][MAX],char m[MAX][MAX]);
//-----------------------------------------------------------------------------------------------//


//-----------------------------------------------------------------------------------------------//
//------------------------------FUNCIONES SEGUNDA PARTE------------------------------------------//
//-----------------------------------------------------------------------------------------------//
void menu();
jugador ingresar_jugador();
bool existe(int ci, jugador jugadores_activos[MAX_JUGADORES], int cantJugadoresActivos);
bool existe1(string nickname, jugador jugadores_activos[MAX_JUGADORES], int cantJugadoresActivos);
bool existeInactivos(string nickname, jugador jugadores_inactivos[MAX_JUGADORES], int cantJugadoresInactivos);
int posicion(string nickname, jugador arr[], int n);
bool formato_FechaNac (int dia,int mes,int ano);
bool formato_Fecha (int dia,int mes,int ano);
bool formato_cedula (int ci);
void esperar();
int getValidatedInt();
void ordenar(jugador jugadores_activos[MAX_JUGADORES], int cantJugadoresActivos);
void listarPartidasPorNickname(partida lista_partidas[MAX_PARTIDAS], int cantPartidas, string nickname);
//-----------------------------------------------------------------------------------------------//


jugador jugadores_activos[MAX_JUGADORES];
jugador jugadores_inactivos[MAX_JUGADORES];
int cantJugadoresActivos = 0;
int cantJugadoresInactivos = 0;

partida lista_partidas[MAX_PARTIDAS];
int cantPartidas = 0;
int cont1 = 0;
bool ingame = false; 

int main() {
    int diaa,mess,anoo;
    srand (time(0));
    char m[MAX][MAX];
    char m1[MAX][MAX];
    int op;
    int fila = 9,col = 9;
   //int cont = 1;
    int opcion;
    jugador j;
    int ci;
    string nickname;
    string nickname1;
    string nick;
    string nickname_modificador;
    int pos;
    string contra;
    bool existe2 = false;
    bool encontrado = false;
    bool encontrado1 = false;
    bool perder = false;
    //int totalGanadas = 0;
    do {
        menu();
        cin >> opcion;
        switch(opcion) {
            case INGRESAR:
                cout << "Ingrese nickname: ";
                cin >> nickname;
                if (existe1(nickname, jugadores_activos, cantJugadoresActivos)) {
                    cout << "Ya existe un jugador con este nickname." << endl;
                    esperar();
                    break;
                }else if (existeInactivos(nickname, jugadores_inactivos, cantJugadoresInactivos)) {
                    cout << "Ya existe un jugador con este nickname dado de baja, se recuperara el mismo." << endl;
                    pos = posicion(nickname, jugadores_inactivos, cantJugadoresInactivos);
                    if (pos != -1) {
                        cout << nickname << " se dio de alta." << endl;
                        jugadores_activos[cantJugadoresActivos] = jugadores_inactivos[pos];
                        for (int i = pos; i < cantJugadoresInactivos - 1; i++) {
                            jugadores_inactivos[i] = jugadores_inactivos[i + 1];
                        }
                        cantJugadoresActivos++;
                        cantJugadoresInactivos--;
                    } 
                    break;
                }
                while (true){
                    cout << "ingrese su cedula: ";
                    cin >> ci;
                    if(formato_cedula(ci)){
                        break;
                    }else{
                        cout << "Formato no valido...  "<<endl;
                    }
                }
                if (existe(ci, jugadores_activos, cantJugadoresActivos)) {
                    cout << "Ya existe un jugador con esta cedula." << endl;
                    esperar();
                    break;
                }

                else {
                    j = ingresar_jugador();
                    j.ci = ci;
                    j.nickname = nickname;
                    
                    formato_FechaNac (diaa,mess,anoo);
                    
                    jugadores_activos[cantJugadoresActivos] = j;
                    cantJugadoresActivos++;
                }
                break;
            case SALIR:
                system("clear");
                cout << "Gracias por jugar." << endl << endl;
                break;
            case LISTADO_DE_JUGADORES:
                system("clear");
                ordenar(jugadores_activos, cantJugadoresActivos);
                
                if (cantJugadoresActivos > 0) {
                    int player=1;
                    int totalGanadas=0;
                    for (int i = 0; i < cantJugadoresActivos; i++) {
                        
                        cout << "PERSONA " << player << ":" << endl;
                        cout << "Cedula: " << jugadores_activos[i].ci << endl;
                        cout << "Nombre: " << jugadores_activos[i].nombre << endl;
                        cout << "Apellido: " << jugadores_activos[i].apellido << endl;
                        cout << "Nickname: " << jugadores_activos[i].nickname << endl;
                        cout << endl;
                        for(int j=0;j < cantPartidas;j++){
                            if(lista_partidas[j].nickname == jugadores_activos[i].nickname && lista_partidas[j].resutado == 1){
                                totalGanadas++;    
                            }
                        }
                        cout << "cantidad ganadas: " << totalGanadas << endl;
                        player++;
                    }
                } else {
                    cout << "No hay jugadores ingresados." << endl;
                }
                break;
            case DAR_DE_BAJA:
                if (cantJugadoresActivos == 0) {
                    cout << "No hay jugadores registrados." << endl;
                    esperar();
                    break;
                }
                cout << "Nickname del jugador que quieres dar de baja." << endl;
                cin >> nickname1;
                pos = posicion(nickname1, jugadores_activos, cantJugadoresActivos);
                if (pos != -1) {
                    cout << nickname1 << " se dio de baja." << endl;
                    jugadores_inactivos[cantJugadoresInactivos] = jugadores_activos[pos];
                    for (int i = pos; i < cantJugadoresActivos - 1; i++) {
                        jugadores_activos[i] = jugadores_activos[i + 1];
                    }
                    cantJugadoresActivos--;
                    cantJugadoresInactivos++;
                } else {
                    cout << "Ese nickname no existe." << endl;
                }
                break;
            case MODIFICAR_USUARIO:
                if (cantJugadoresActivos <= 0) {
                    cout << "No hay jugadores ingresados." << endl;
                    esperar();
                    break;
                } else {
                    cout << "Ingrese el nickname para modificar los datos." << endl;
                    cin >> nickname_modificador;
                    for (int i = 0; i < cantJugadoresActivos; i++) {
                        if (jugadores_activos[i].nickname == nickname_modificador) {
                            j = ingresar_jugador();
                            j.nickname = jugadores_activos[i].nickname;
                            j.ci = jugadores_activos[i].ci;
                            jugadores_activos[i] = j;
                            existe2 = true;
                            break;
                        }
                    }
                    if (!existe2) {
                        cout << "No existe ese nickname." << endl;
                        esperar();
                    }
                }
            break;
            case JUGAR:
                ingame = true;
                perder = false;
                cout<<"ingrese nickname para jugar: ";
                cin>>nick;
                cout<<"ingrese contrasena para jugar: ";
                cin>>contra;
                    
                for(int i=0;i<cantJugadoresActivos;i++){
                    if(jugadores_activos[i].nickname == nick && jugadores_activos[i].contrasena == contra){
                        
                        encontrado = true;
                        
                        formato_Fecha (diaa,mess,anoo);
                        
                        lista_partidas[cantPartidas].nickname = nick;
                        
                        cantPartidas++;    
                        
                    }
                    
                    
                }
                if(encontrado == false){
                    cout<<"error al ingresar los datos."<<endl;
                    esperar();
                    break;
                }
                if (!ingame){
                    break;
                }
                if(encontrado == true){
                    matriz_inicial(m1);
                    do{
                                    
                        cout<< GREEN << "## BIENVENDIO AL BUSCAMINAS ##"<<endl<< RESET;
                        cout<<endl;
                        cout<<BLUE<<"1. (E:explorar)"<<RESET<<endl;
                        cout<<BLUE<<"2. (M:marcar)"<<RESET<<endl;
                        cout<<BLUE<<"3. rendirse"<<RESET<<endl;
                        cout<<BLUE<<"4. salir"<<RESET<<endl;
                        op = getValidatedInt();
                        
                        switch(op) {
                            
                            case 1: {
                                cout<<CYAN<<"ingrese la fila"<<RESET<<endl;
                                fila = getValidatedInt();
                                cout<<CYAN<<"ingrese la columna"<<RESET<<endl;
                                col = getValidatedInt();
                                
                                if(!EsValido(fila,col)){
                                    cout<<RED<<"jugada invalida..."<<RESET<<endl;
                                    mostrar_matriz(m1);
                                    break;
                                }
                                if(cont1 == 0){
                                    matriz_inicial(m1);
                                    llenarMatriz(m,fila,col);
                                    expansion(m,m1,fila,col);
                                    mostrar_matriz(m1);
                                    the_champion(m1,m); 
                                    cont1++;
                                }
                                else{
                                    if(m1[fila][col] == 'x'){
                                        cout<<RED<<"jugada invalida..."<<RESET<<endl;
                                        mostrar_matriz(m1);
                                        break;
                                    } 
                                    if(m[fila][col] == 'B'){
                                        cout<<RED<<"PERDISTE MARACA..."<<RESET<<endl;
                                        matriz_rellena(m);
                                        cout<<endl;
                                        cont1 = 0;
                                        lista_partidas[cantPartidas-1].resutado = -1;
                                        perder = true;
                                        esperar();
                                        break;
                                    }
                                    else{
                                        expansion(m,m1,fila,col);
                                        mostrar_matriz(m1);
                                        the_champion(m1,m);
                                    }
                                       
                                }
                                
                                break;
                            } 
                            case 2: {
                                cout<<CYAN<<"ingrese la fila"<<RESET<<endl;
                                cin>>fila;
                                cout<<CYAN<<"ingrese la columna"<<RESET<<endl;
                                cin>>col;
                                
                                if(!EsValido(fila,col)){
                                    cout<<RED<<"jugada invalida..."<<RESET<<endl;
                                    mostrar_matriz(m1);
                                    break;
                                }
                    
                                Marcar_posicion(m1,fila,col);
                                mostrar_matriz(m1);
                                break;
                            }
                            case 3: {
                                ingame = false;
                                cout<<"gracias por jugar."<<endl;
                                lista_partidas[cantPartidas-1].resutado = 0;
                                cont1 = 0;
                                esperar();
                                break;
                            }
                            case 4: {
                                cout<<"gracias por jugar."<<endl; // revisar esto 
                                cont1 = 0;
                                break;
                            }
                            default: {
                                cout<<RED<<"ingrese una opcion coherente animal..."<<RESET<<endl;
                                break;
                            }
                            
                        }    
                    }
                    while(op != 3 && op != 4 && !perder && ingame);
                }            
            break;
            case LISTADO_POR_FECHAS:
                if (cantPartidas > 0) {
                    int player=1;
                    for (int i = 0; i < cantPartidas; i++) {
                        cout << "PARTIDA " << player << ":" << endl;
                        cout << "Fecha: " << lista_partidas[i].dia << "-" << lista_partidas[i].mes << "-" << lista_partidas[i].ano << endl;
                        cout << "Nickname: " << lista_partidas[i].nickname << endl;
                        // int resutado; //-1 = Perdio    0 = Abandono    1 = Gano
                        if(lista_partidas[i].resutado == 1){
                            cout << "resultado: " << "Gano" << endl;
                        }
                        else if(lista_partidas[i].resutado == 0){
                            cout << "resultado: " << "Abandono" << endl;
                        }
                        else{
                            cout << "resultado: " << "Perdio" << endl;
                        }
                        cout << endl;
                        player++;
                    }
                    esperar();
                    
                }
                else {
                    cout << "No hay partidas." << endl;
                }
                break;
            case BUSCAR_PARTIDAS_POR_FECHAS:
            
                cout << "ingrese dia a buscar: ";
                cin >> diaa;
                cout << "ingrese mes a buscar: ";
                cin >> mess;
                cout << "ingrese ano a buscar: ";
                cin >> anoo;
                
                for(int i=0;i<cantPartidas;i++){
                    if(lista_partidas[i].dia == diaa && lista_partidas[i].mes == mess && lista_partidas[i].ano == anoo){
                        encontrado1 = true;
                        cout << "PARTIDA " << endl;
                        cout << "dia: " << lista_partidas[i].dia << endl;
                        cout << "mes: " << lista_partidas[i].mes << endl;
                        cout << "ano: " << lista_partidas[i].ano << endl;
                        cout << "Nickname: " << lista_partidas[i].nickname << endl;
                        cout << "Resultado: " << lista_partidas[i].resutado << endl;
                        cout << endl;
                        
                        
                    }
                    esperar();
                }
                if(!encontrado1){
                    cout<<"no existe una partida con esa fecha."<<endl;
                    esperar();
                }
                    
                break;
            case BUSCAR_PARTIDAS_POR_NICKNAME:
                cout << "Ingrese nickname a buscar: ";
                cin >> nickname;
                listarPartidasPorNickname(lista_partidas, cantPartidas, nickname);
                esperar();
                break;
            default:{
                system("clear");
                cout << "La opcion es incorrecta." << endl << endl;
                break;
            }
                
        }
    } while (opcion != SALIR);

    return 0;
}


//---------------------------------------------------------------------------------//
//------------------------FUNCIONES DE LA PRIMERA PARTE----------------------------//
//---------------------------------------------------------------------------------//


 bool EsValido(int x, int y) {  
        if(x > 0 && x <= MAX && y > 0 && y <= MAX)
            return true;
        else
            return false;
}

void llenarMatriz (char m[MAX][MAX],int x,int y){
    matriz_inicial(m);
    int pos1;
    int pos2;
    int pos3;
    int pos4;
    int contBombas = 0;
    int cont = 0;
    int numerosbomba = 0;
    char c;
    int auxbombas = 0;
    
    m[x][y] = '0';
    
    //UBICO LAS BOMBAS
    cout << "CONT BOMBAS: "<<contBombas<<endl;
    cout << "CANT BOMBAS: "<<CantBombas<<endl;
    while(contBombas < CantBombas){
        for (int h =x - 1; h <= x + 1; h++) {
            for (int t = y - 1; t <=y + 1; t++) {
                if (h != 0 || t != 0) {
                    m[h][t] = '.';
                }
            }
        }
        // Imprimo
        pos1 = rand () % MAX + 1;
        cout << "POS1: "<<pos1<<endl;
        pos2 = rand () % MAX + 1;
        cout << "POS2: "<<pos2<<endl;
        if(m[pos1][pos2] != 'B' && (x != pos1 && y != pos2) && m[pos1][pos2] != '.'){
            m[pos1][pos2] = 'B';
            contBombas++;
            cout << "COLOQUE UNA BOMBA" << endl;
            for(int i=1;i<=MAX;i++){
                for(int j=1;j<=MAX;j++){
                    if(m[i][j] != 'B'){                        
                        for(int h = i - 1;h <= i + 1;h++){      
                            for(int t = j - 1;t <= j + 1;t++){
                                if(m[h][t] == 'B' && EsValido(h,t)){  
                                    auxbombas++;
                                }                    
                            }
                        }
                        c = auxbombas + '0';
                        m[i][j] = c;
                        auxbombas = 0;
                    }
                }
            }
        }
        
    }
    
}

//MATRIZ CON LAS BOMBAS Y LOS NUMEROS 
void matriz_rellena (char m[MAX][MAX]){
     
     cout<<YELLOW<<"MATRIZ CARGADA"<<RESET<<endl;
     
    for (int i = 1; i <= MAX; i++) {
        for (int j = 1; j <= MAX; j++) {
            cout<<m[i][j] << "  ";
        }
        cout << endl;
    }
}

void matriz_inicial(char m1[MAX][MAX]){
    for(int i=1;i<=MAX;i++){
        for(int j=1;j<=MAX;j++){
            m1[i][j] = '?';
        }
    }
}    
      
void mostrar_matriz(char m1[MAX][MAX]){
    
    
    cout<<endl<<YELLOW<<"       MATRIZ JUEGO"<<RESET<<endl;
        cout<<" • ";
        for(int j = 1; j <= MAX; j++){
          cout<<RED<< j  <<"  "<<RESET;
        }
    
    cout<<endl;
    for(int i = 1; i <= MAX; i++){
            cout<<" ";
            cout<<RED<< i <<" "<<RESET;
              for(int j = 1; j <= MAX; j++){
                  cout<<m1[i][j]<< "  ";
              }
              cout<<endl;
            }
    cout<<endl;
} 

void Marcar_posicion(char m1[MAX][MAX], int x, int y) {
    
    if(m1[x][y] == '0'||m1[x][y] == '1'||m1[x][y] == '2'||m1[x][y] == '3'||m1[x][y] == '4'||m1[x][y] == '5'||m1[x][y] == '6'||m1[x][y] == '7'||m1[x][y] == '8'){
            cout<<RED<<"jugada invalidad..."<<RESET<<endl;
            return;
    }
    
    if (x > 0 && x <= MAX && y > 0 && y <= MAX) {
        if (m1[x][y] == '?')
            m1[x][y] = 'x';
        else
            m1[x][y] = '?';
        
    }
}

void expansion (char m[MAX][MAX],char m1[MAX][MAX], int x, int y){
    
    if(!EsValido(x,y))
        return;
        
    if (m1[x][y] != '?')
        return;

    m1[x][y] = m[x][y];
    
    if (m[x][y] == '0') {
        for (int h = - 1; h <=  1; h++) {
            for (int t = - 1; t <= 1; t++) {
                if (h != 0 || t != 0) {
                    expansion(m, m1, x + h, y + t);
                }
            }
        }
    }
}

void the_champion (char m1[MAX][MAX],char m[MAX][MAX]){
    
    
    int contador_espacios = 0;
    
    for(int i=1;i<=MAX;i++){
        for(int j=1;j<=MAX;j++){
            if(m1[i][j] == '0'||m1[i][j] == '1'||m1[i][j] == '2'||m1[i][j] == '3'||m1[i][j] == '4'||m1[i][j] == '5'||m1[i][j] == '6'||m1[i][j] == '7'||m1[i][j] == '8')
                contador_espacios++;
        }
    }
    
    if(contador_espacios == MAX_ESPACIOS ){
        cout<<GREEN<<"GANASTE, AL FINAL NO ERAS TAN MARACA, BIEN AHI"<<RESET<<endl;
        cout<<endl;
        cout<<YELLOW<<"    MATRIZ CARGADA"<<RESET<<endl;
        
        for (int i = 1; i <= MAX; i++) {
            for (int j = 1; j <= MAX; j++) {
                cout<<m[i][j] << "  ";
            }
            cout << endl;
        }
        lista_partidas[cantPartidas-1].resutado = 1;
        cont1 = 0;
        
        ingame = false;
        
    }
    
    
}

//---------------------------------------------------------------------------------//
//------------------------FUNCIONES DE LA SEGUNDA PARTE----------------------------//
//---------------------------------------------------------------------------------//

void menu() {
    esperar();
    system("clear");
    cout << "1. Ingresar jugador." << endl;
    cout << "2. Listado de jugadores." << endl;
    cout << "3. Dar de baja." << endl;
    cout << "4. Modificar usuario." << endl;
    cout << "5. Jugar." << endl;
    cout << "6. Listar partidas." << endl;
    cout << "7. Buscar partidas por fecha." << endl;
    cout << "8. Buscar partidas por nickname." << endl;
    cout << endl;
    cout << "0. Salir." << endl;
    cout << "Ingrese una opcion: ";
}

jugador ingresar_jugador() {
    jugador j;
    cout << "Ingrese su nombre: ";
    cin >> j.nombre;
    cout << "Ingrese su apellido: ";
    cin >> j.apellido;
    cout << "Ingrese su contrasena: ";
    cin >> j.contrasena;
    return j;
}

bool formato_FechaNac (int dia,int mes,int ano){
    int cont = 0;
     while (cont < 100){
        cout << "Ingrese dia de nacimiento: ";
        int fech;
        cin >> fech;
        if (fech<=31 && fech>=1){
            jugadores_activos[cantJugadoresActivos].diaNac = fech;
            cout << "Ingrese mes de nacimiento: ";
            cin>>fech;
            if(fech<=12 && fech>=1){
                jugadores_activos[cantJugadoresActivos].mesNac = fech; 
                                
                cout << "Digite de año nacimiento:  ";
                cin >> fech;
                if (fech<=2024){
                    jugadores_activos[cantJugadoresActivos].anoNac = fech;
                    return false;
                }
                else
                    cout << "Formato no valido...  "<<endl;
            }
            else
                cout << "Formato no valido...  "<<endl;
        }
        else
            cout << "Formato no valido...  "<<endl;
        }
        return true;
}

bool formato_Fecha (int dia,int mes,int ano){
    int cont = 0;
     while (cont < 100){
        cout << "Ingrese dia: ";
        cin >> dia;
        if (dia<=31 && dia>=1){
            lista_partidas[cantPartidas].dia = dia;
            cout << "Ingrese mes: ";
            cin>>mes;
            if(mes<=12 && mes>=1){
                lista_partidas[cantPartidas].mes = mes;
                cout << "Digite de año:  ";
                cin >> ano;
                if (ano<=2024){
                    lista_partidas[cantPartidas].ano = ano;
                    return false;
                }
                else
                    cout << "Formato no valido...  "<<endl;
            }
            else
                cout << "Formato no valido...  "<<endl;
        }
        else
            cout << "Formato no valido...  "<<endl;
    }
        return true;
}

bool formato_cedula (int ci){
  if(ci >= 10000000 && ci <= 99999999){
    return true;
  }else{
    return false;   
  }
}

bool existe(int ci, jugador jugadores_activos[MAX_JUGADORES], int cantJugadoresActivos) {
    for (int i = 0; i < cantJugadoresActivos; i++) {
        if (ci == jugadores_activos[i].ci) {
            return true;
        }
    }
    return false;
}

bool existe1(string nickname, jugador jugadores_activos[MAX_JUGADORES], int cantJugadoresActivos) {
    for (int i = 0; i < cantJugadoresActivos; i++) {
        if (nickname == jugadores_activos[i].nickname) {
            return true;
        }
    }
    return false;
}

bool existeInactivos(string nickname, jugador jugadores_inactivos[MAX_JUGADORES], int cantJugadoresInactivos) {
    for (int i = 0; i < cantJugadoresInactivos; i++) {
        if (nickname == jugadores_inactivos[i].nickname) {
            return true;
        }
    }
    return false;
}

int posicion(string nickname, jugador arr[], int n) {
    for (int i = 0; i < n; i++) {
        if (arr[i].nickname == nickname) {
            return i;
        }
    }
    return -1;
}

void esperar() {
    cout << "[ PRESIONE ENTER PARA CONTINUAR ]";
    // PAUSA EL SISTEMA HASTA QUE SE INGRESE ENTER
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    //system("clear");
}

int getValidatedInt() {
    int value;
    while (true) {
        std::cout << "Por favor ingrese un entero: ";
        std::cin >> value;

        // Verificar si la entrada es válida
        if (std::cin.fail()) {
            std::cin.clear(); // Limpia el estado de error de cin
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora los caracteres no válidos
            std::cout << "Entrada no válida. ";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora cualquier carácter adicional
            return value; // Entrada válida, retornar el valor
        }
    }
}

// Ordenar por nickname
void ordenar(jugador jugadores_activos[MAX_JUGADORES], int cantJugadoresActivos) {
    jugador aux;
    for (int i = 0; i < cantJugadoresActivos - 1; i++) {
        for (int j = 0; j < cantJugadoresActivos - i - 1; j++) {
            if (jugadores_activos[j].nickname > jugadores_activos[j + 1].nickname) {
                aux = jugadores_activos[j];
                jugadores_activos[j] = jugadores_activos[j + 1];
                jugadores_activos[j + 1] = aux;
            }
        }
    }
}

// Listar partidas por nickname
void listarPartidasPorNickname(partida lista_partidas[MAX_PARTIDAS], int cantPartidas, string nickname) {
    for (int i = 0; i < cantPartidas; i++) {
        if (lista_partidas[i].nickname == nickname) {
            cout << "PARTIDA " << i + 1 << ":" << endl;
            cout << "Fecha: " << lista_partidas[i].dia << "-" << lista_partidas[i].mes << "-" << lista_partidas[i].ano << endl;
            cout << "Nickname: " << lista_partidas[i].nickname << endl;
            if (lista_partidas[i].resutado == 1) {
                cout << "Resultado: Gano" << endl;
            } else if (lista_partidas[i].resutado == 0) {
                cout << "Resultado: Abandono" << endl;
            } else {
                cout << "Resultado: Perdio" << endl;
            }
            cout << endl;
        }
    }
}
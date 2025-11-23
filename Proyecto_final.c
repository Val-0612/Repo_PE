#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
// Colores
#define BLANCO "\033[0;37m"
#define AMARILLO "\033[0;33m"
#define ROJO "\033[0;31m"
#define AZUL "\033[0;34m"
#define Num_fichas 12

typedef struct {
    int fila;
    int columna;
    int activa;    // 1 = en juego, 0 = consumida
    int esDama;    // 1 = dama, 0 = peon
    int jugador;   // 0 = rojo, 1 = azul
} Ficha;
Ficha fichas[2][Num_fichas];  // fichas[0] = jugador 1, fichas[1] = jugador 2

int mostrar_menu(),buscarFichaEnPosicion(int fila, int col, int resultado[2]);
void inicializar_fichas(),mostrar_tablero();
/**
 * @brief Función para ingresar los nombres de los jugadores con validaciones
 * 
 * @param nombres Arreglo para almacenar los nombres de los jugadores
 * 
 * @retval void no retorna ningun valor
 */
void ingresarNombreJugadores(char nombres[2][150]);
/**
 * @brief Mueve una ficha en el tablero
 * 
 * @param jugadorActual Jugador que realiza el movimiento (0 o 1)
 * @param fichas Arreglo de fichas de ambos jugadores
 * @param resultado Arreglo para almacenar el resultado de la búsqueda de ficha
 * @param dest Arreglo para almacenar la posición de destino en caso de captura
 * 
 * @retval int Retorna 1 si se desea capturar una ficha, 0 si es un movimiento normal
 */
int moverFicha(int jugadorActual, Ficha fichas[2][Num_fichas], int resultado[2], int dest[2]);
int main(){
    char nombres[2][150];
    int opcion, jugadorActual=0, gameOver=0;
    int resultado[2]; //resultado[0]= jugador que pertenece, resultado[1]= indice de la ficha
    int coordsDest[2]; //coordsDest[0]= fila destino, coordsDest[1]= columna destino
    do{
        opcion= mostrar_menu();
        switch (opcion)
        {
        case 1:
            ingresarNombreJugadores(nombres);
            inicializar_fichas();
            do{
                if(jugadorActual==0){
                    printf("Turno del Jugador 1: %s\n", nombres[0]);
                    mostrar_tablero();
                    moverFicha(jugadorActual, fichas, resultado, coordsDest);
                    jugadorActual=1;
                }else if(jugadorActual==1){
                    printf("Turno del Jugador 2: %s\n", nombres[1]);
                    mostrar_tablero();
                    jugadorActual=0;
                }
            }while(gameOver==0);
            break;
        case 2: 
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            break;
        }
    }while(opcion!=4);
    return 0;
}

int mostrar_menu(){
    int opcion, revisarScanf;
    printf("\n====Damas Inglesas====\n");
    printf("[1] Jugar\n");
    printf("[2] Creditos\n");
    printf("[3] Historial de Victorias\n");
    printf("[4] Salir\n");
    do
    {
        printf("Seleccione una opcion: ");
        if(scanf("%d", &opcion)!=1){
            while(getchar()!='\n');
            printf("Opcion invalida\n");
        }
    } while (opcion<1||opcion>4);
    while(getchar()!='\n');
    return opcion;
}

void inicializar_fichas() {
    int idx = 0;
    // Fichas jugador 1 (rojo) - filas 0, 1, 2
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 8; j++) {
            if((i + j) % 2 == 1) {
                fichas[0][idx].fila = i;
                fichas[0][idx].columna = j;
                fichas[0][idx].activa = 1;
                fichas[0][idx].esDama = 0;
                fichas[0][idx].jugador = 0;
                idx++;
            }
        }
    }
    
    idx = 0;
    // Fichas jugador 2 (azul) - filas 5, 6, 7
    for(int i = 5; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if((i + j) % 2 == 1) {
                fichas[1][idx].fila = i;
                fichas[1][idx].columna = j;
                fichas[1][idx].activa = 1;
                fichas[1][idx].esDama = 0;
                fichas[1][idx].jugador = 1;
                idx++;
            }
        }
    }
}

//resultado[0]= jugador que pertenece, resultado[1]= indice de la ficha
int buscarFichaEnPosicion(int fila, int col, int resultado[2]) {
    int jugador, i;
    for(jugador = 0; jugador< 2; jugador++) {
        for(i = 0; i < 12; i++) {
            if(fichas[jugador][i].activa == 1 && fichas[jugador][i].fila == fila && fichas[jugador][i].columna == col) {
                resultado[0] = jugador;
                resultado[1] = i;
                return 1;
            }
        }
    }
    return 0;
}
void mostrar_tablero() {
    int i, j; //Variables para recorrer el tablero
    int resultado[2];
    int encontrada; // 1 si hay ficha en la posición, 0 si no
    int jugFicha, idxFicha; // Jugador e índice de la ficha encontrada
    printf("\n     0     1     2     3     4     5     6     7\n");
    for(int i = 0; i < 8; i++) {
        // Linea horizontal de arriba
        printf("   ");
        for(int j = 0; j < 8; j++) {
            if((i + j) % 2 == 1) {
                printf(" " AMARILLO "---" BLANCO "  ");
            } else {
                printf(" " BLANCO "---  ");
            }
        }
        printf("\n");
        
        // Paredes y fichas
        printf(" %d ", i);
        for(int j = 0; j < 8; j++) {
            if((i + j) % 2 == 1) {
                printf(AMARILLO "|" BLANCO);
            } else {
                printf("|");
            }
            
            encontrada = buscarFichaEnPosicion(i, j, resultado);

            if(encontrada == 1) {
                jugFicha = resultado[0];// Jugador de la ficha encontrada
                idxFicha = resultado[1];// Indice de la ficha encontrada
                
                if(fichas[jugFicha][idxFicha].jugador == 1) {
                    if(fichas[jugFicha][idxFicha].esDama == 1) {
                        printf(" " ROJO "D" BLANCO " ");
                    } else {
                        printf(" " ROJO "P" BLANCO " ");
                    }
                } else {
                    if(fichas[jugFicha][idxFicha].esDama == 1) {
                        printf(" " AZUL "D" BLANCO " ");
                    } else {
                        printf(" " AZUL "P" BLANCO " ");
                    }
                }
                printf(AMARILLO "|" BLANCO " ");
            } else {
                printf("   ");
                if((i + j) % 2 == 1) {
                    printf(AMARILLO "|" BLANCO " ");
                } else {
                    printf(BLANCO "| ");
                }
            }
        }
        printf("\n");
        
        // Linea horizontal de abajo
        printf("   ");
        for(int j = 0; j < 8; j++) {
            if((i + j) % 2 == 1) {
                printf(" " AMARILLO "---  " BLANCO);
            } else {
                printf(" " BLANCO "---  ");
            }
        }
        printf("\n");
    }
}

void ingresarNombreJugadores(char nombres[2][150]){
    int aprobarNombre=1;
    do{
        do{
            aprobarNombre=1; //Variable bandera para aprobar el nombre
            printf("Ingrese el nombre del Jugador 1: ");
            fgets(nombres[0], 150, stdin);
            //Valida de que el nombre no este vacio o inicie con espacio o salto de linea
            if(!(nombres[0][0]!='\n' && isspace(nombres[0][0])==0)){
                aprobarNombre=0;
            }
            //Valida de que el nombre solo contenga letras y espacios
            for(int i=strlen(nombres[0])-2; i>=0; i--){
                if(isalpha(nombres[0][i])==0 && nombres[0][i]!=' '){
                    aprobarNombre=0;
                    break;
                }
            }
            if(aprobarNombre==0){
                printf("Nombre invalido\n");
            }
        }while(aprobarNombre==0); //Repite hasta que el nombre sea valido
        //El mismo proceso para el jugador dos
        do{
            aprobarNombre=1;
            printf("Ingrese el nombre del Jugador 2: ");
            fgets(nombres[1], 150, stdin);
            if(!(nombres[1][0]!='\n' && isspace(nombres[1][0])==0)){
                aprobarNombre=0;
            }
            for(int i=strlen(nombres[1])-2; i>=0; i--){
                if(isalpha(nombres[1][i])==0 && nombres[1][i]!=' '){
                    aprobarNombre=0;
                    break;
                }
            }
            if(aprobarNombre==0){
                printf("Nombre invalido\n");
            }
        }while(aprobarNombre==0);
        if(strcmp(nombres[0], nombres[1])==0){
            printf("Los nombres no pueden ser iguales\n");
        }
    }while(strcmp(nombres[0], nombres[1])==0);
}
/**
 * @brief Mueve una ficha en el tablero
 * 
 * @param jugadorActual Jugador que realiza el movimiento (0 o 1)
 * @param fichas Arreglo de fichas de ambos jugadores
 * @param resultado Arreglo para almacenar el resultado de la búsqueda de ficha
 * @param dest Arreglo para almacenar la posición de destino en caso de captura
 * 
 * @retval int Retorna 1 si se desea capturar una ficha, 0 si es un movimiento normal
 */
int moverFicha(int jugadorActual, Ficha fichas[2][Num_fichas], int resultado[2], int dest[2]){
    int filaOrigen, colOrigen, filaDestino, colDestino, encontrada, repetirSolicitud, op;
    do{
        repetirSolicitud=0;
        do{
            printf("Ingrese la fila de la ficha que desea mover: ");
            if(scanf("%d", &filaOrigen)!=1){
                printf("Entrada invalida. Ingrese un numero entero para la fila: ");
                while(getchar()!='\n');
                filaOrigen=-1; //Asigna un valor invalido para que el ciclo continue
            }
        }while(filaOrigen<0 || filaOrigen>7);
        while(getchar()!='\n');
        do{
            printf("Ingrese la columna de la ficha que desea mover: ");
            if(scanf("%d", &colOrigen)!=1){
                printf("Entrada invalida. Ingrese un numero entero para la columna: ");
                while(getchar()!='\n');
                colOrigen=-1; //Asigna un valor invalido para que el ciclo continue
            }
        }while(colOrigen<0 || colOrigen>7);
        while(getchar()!='\n');
        encontrada=buscarFichaEnPosicion(filaOrigen, colOrigen, resultado);
        if(encontrada==1 && resultado[0]==jugadorActual){
            do{
                printf("Ingrese la fila de destino: ");
                if(scanf("%d", &filaDestino)!=1){
                    printf("Entrada invalida. Ingrese un numero entero para la fila de destino: ");
                    while(getchar()!='\n');
                    filaDestino=-1; //Asigna un valor invalido para que el ciclo continue
                }
            }while(filaDestino<0 || filaDestino>7);
            while(getchar()!='\n');
            do{
                printf("Ingrese la columna de destino: ");
                if(scanf("%d", &colDestino)!=1){
                    printf("Entrada invalida. Ingrese un numero entero para la columna de destino: ");
                    while(getchar()!='\n');
                    colDestino=-1; //Asigna un valor invalido para que el ciclo continue
                }
            }while(colDestino<0 || colDestino>7);
            while(getchar()!='\n');
            if(repetirSolicitud==1){
                continue;
            }
            //Verifica que el peon no se mueva para atras
            if(fichas[jugadorActual][resultado[1]].esDama==0){ 
                if((jugadorActual==0 && filaDestino<=filaOrigen) || (jugadorActual==1 && filaDestino>=filaOrigen)){
                    printf("Movimiento invalido segun las reglas del juego.\n");
                    repetirSolicitud=1;
                    continue;
                }
            }
            //Verifica que la casilla de destino sea amarilla
            if((filaDestino+colDestino)%2==0){
                printf("Movimiento invalido: solo se puede mover a casillas amarillas\n");
                repetirSolicitud=1;
                continue;
            }

            encontrada=buscarFichaEnPosicion(filaDestino, colDestino, resultado);
            if(encontrada==1){
                printf("La casilla seleccionada ya esta ocupada\n");
                repetirSolicitud=1;
                continue;
            }
            //Verifica si se esta intentando capturar una ficha
            if(abs(filaDestino-filaOrigen)==2 && abs(colDestino-colOrigen)==2){
                //Calcula la posicion despues del supuesto consumo
                buscarFichaEnPosicion(filaOrigen, colOrigen, resultado);
                //Guarda la posicion de la ficha antes del supuesto consumo
                dest[0]=(filaDestino);
                dest[1]=(colDestino);
                return 1; // Indica que se desea capturar una ficha
            }
            //Verifica que el movimiento sea solo de una casilla diagonalmente
            if(abs(filaDestino-filaOrigen)!=1 || abs(colDestino-colOrigen)!=1){
                printf("Movimiento invalido: solo se puede mover una casilla diagonalmente\n");
                repetirSolicitud=1;
                continue;
            }
            //Si pasa todas las validaciones, mueve la ficha
            buscarFichaEnPosicion(filaOrigen, colOrigen, resultado);
            fichas[jugadorActual][resultado[1]].fila=filaDestino;
            fichas[jugadorActual][resultado[1]].columna=colDestino;
            printf("Ficha movida a (%d, %d)\n", filaDestino, colDestino);
        }else{
            printf("Ficha no encontrada o no pertenece al jugador.\n");
            repetirSolicitud=1;
        }
    }while(repetirSolicitud==1);
    return 0;
}
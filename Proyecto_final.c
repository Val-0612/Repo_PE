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
    int primerConsumoRealizado; // 1 = si, 0 = no
} Ficha;

Ficha fichas[2][Num_fichas];  // fichas[0] = jugador 1, fichas[1] = jugador 2

/**
 * 
 * @brief Muestra el menú principal y obtiene la opción del usuario.
 * 
 * @retval opción: Número entero que representa la opción seleccionada por el usuario.
 * 
 */
int mostrar_menu();
/**
 * 
 * @brief Busca una ficha en una posición específica del tablero.
 * 
 * @param fila Fila del tablero.
 * @param col Columna del tablero.
 * @param resultado Arreglo donde se almacenan los resultados: resultado[0] = jugador, resultado[1] = índice de la ficha.
 * 
 * @retval 1 si se encuentra una ficha en la posición
 * @retval 0 si no se encuentra ninguna ficha en la posición
 */
int buscarFichaEnPosicion(int fila, int col, int resultado[2]);
/**
 * 
 * @brief Inicializa las fichas en sus posiciones iniciales en el tablero.
 * 
 * @retval void no retorna ningún valor.
 */
void inicializar_fichas();
/**
 * 
 * @brief Muestra el tablero de juego con las fichas en sus posiciones actuales.
 * 
 * @retval void no retorna ningún valor.
 */
void mostrar_tablero();
/**
 * 
 * @brief Solicita e ingresa los nombres de los dos jugadores.
 * 
 * @param nombres Arreglo donde se almacenan los nombres de los jugadores.
 * 
 * @retval void no retorna ningún valor.
 */
void ingresarNombreJugadores(char nombres[2][150]);
/**
 * 
 * @brief Maneja el movimiento de una ficha por parte del jugador actual.
 * 
 * @param jugadorActual Índice del jugador actual (0 o 1).
 * @param resultado Arreglo donde se almacenan los resultados: resultado[0] = jugador, resultado[1] = índice de la ficha.
 * @param dest Arreglo donde se almacenan las coordenadas de destino: dest[0] = fila destino, dest[1] = columna destino.
 * 
 * @retval 1 si se intenta capturar una ficha enemiga.
 * @retval 0 si se realiza un movimiento simple sin captura.
 */
int moverFicha(int jugadorActual, int resultado[2], int dest[2]);
/**
 * 
 * @brief Promueve una ficha a dama si alcanza la fila final del tablero.
 * 
 * @param jugadorActual Índice del jugador actual (0 o 1).
 * @param indiceFicha Índice de la ficha a promover.
 * 
 * @retval 1 si la ficha fue promovida a dama.
 * @retval 0 si la ficha no cumple con los requisitos para ser promovida.
 * @retval -1 si el índice de la ficha es inválido. (error)
 */
int promocionDama(int jugadorActual, int indiceFicha);
/**
 * 
 * @brief Consume una ficha enemiga al capturarla.
 * 
 * @param filaOrigen Fila de origen de la ficha que realiza la captura.
 * @param colOrigen Columna de origen de la ficha que realiza la captura.
 * @param filaDestino Fila de destino de la ficha que realiza la captura.
 * @param colDestino Columna de destino de la ficha que realiza la captura.
 * @param jugadorActual Índice del jugador actual (0 o 1).
 * 
 * @retval 1 si la ficha enemiga fue consumida exitosamente.
 * @retval 0 si no se pudo consumir la ficha enemiga.
 */
int consumirFicha(int filaOrigen, int colOrigen, int filaDestino, int colDestino, int jugadorActual);
/**
 * 
 * @brief Verifica si el juego ha terminado y determina el ganador o si hay empate.
 * 
 * @param jugadorActual Índice del jugador actual (0 o 1).
 * @param nombres Arreglo donde se almacenan los nombres de los jugadores.
 * 
 * @retval 1 si el juego ha terminado (hay un ganador o empate).
 * @retval 0 si el juego continúa.
 */
int verificarFinJuego(int jugadorActual, char nombres[2][150]);
/**
 * 
 * @brief Verifica si un jugador puede realizar algún movimiento válido.
 * 
 * @param jugador Índice del jugador (0 o 1).
 * @param notEndGame Indica si se está verificando durante el juego (1) o al final del juego (0).
 * @param ficha Índice de la ficha a verificar (solo si notEndGame es 1).
 * @param fila Fila de la ficha a verificar (solo si notEndGame es 1).
 * @param columna Columna de la ficha a verificar (solo si notEndGame es 1).
 * 
 * @retval 1 si el jugador puede moverse.
 * @retval 0 si el jugador no puede moverse.
 */
int puedeMoverse(int jugador, int notEndGame, int ficha, int fila, int columna);
/**
 * 
 * @brief Verifica si hay capturas disponibles para una ficha en una posición específica.
 * 
 * @param jugador Índice del jugador (0 o 1).
 * @param fila Fila de la ficha.
 * @param col Columna de la ficha.
 * 
 * @retval 1 si hay capturas disponibles.
 * @retval 0 si no hay capturas disponibles.
 */
int verificarCapturasDisponibles(int jugador, int fila, int col);
/**
 * 
 * @brief Procesa las capturas concatenadas para una ficha después de una captura inicial.
 * 
 * @param jugadorActual Índice del jugador actual (0 o 1).
 * @param resultado Arreglo donde se almacenan los resultados: resultado[0] = jugador, resultado[1] = índice de la ficha.
 * 
 * @retval void no retorna ningún valor.
 */
void procesarConsumosConcatenados(int jugadorActual, int resultado[2]);

int main(){
    char nombres[2][150], linea[200], nombreGanador[150], motivo[200];
    int opcion, jugadorActual=0, gameOver=0, fichasActivas;
    int resultado[2]; //resultado[0]= jugador que pertenece, resultado[1]= indice de la ficha
    int coordsDest[2]; //coordsDest[0]= fila destino, coordsDest[1]= columna destino
    int intentoCaptura;
    FILE *fCreditos;
    FILE *fHistorial;
    
    do{ //Bucle del menú principal, termina solo al seleccionar la opcion "Salir"
        opcion = mostrar_menu();
        switch (opcion)
        {
        case 1:
            ingresarNombreJugadores(nombres);
            inicializar_fichas();
            jugadorActual = 0; //Variable que indica el jugador actual (0 o 1)
            gameOver = 0; //Variable bandera que indica cuando termina el juego
            
            do{ //Bucle principal del juego, termina cuando gameOver=1
                if(jugadorActual==0){ //Turno del jugador 1
                    printf("\nTurno del Jugador 1: %s\n", nombres[0]);
                    mostrar_tablero();
                    intentoCaptura = moverFicha(jugadorActual, resultado, coordsDest); //Realiza el movimiento de la ficha
                    
                    if(intentoCaptura == 1){ //Se intenta realizar un movimiento de captura
                        //Obtiene las coordenadas de la ficha que se va a mover
                        int filaOrigen = fichas[jugadorActual][resultado[1]].fila;
                        int colOrigen = fichas[jugadorActual][resultado[1]].columna;
                        //Intenta consumir la ficha enemiga
                        if(consumirFicha(filaOrigen, colOrigen, coordsDest[0], coordsDest[1], jugadorActual)==1){
                            //Consumo exitoso, actualiza la posición de la ficha
                            fichas[jugadorActual][resultado[1]].fila = coordsDest[0];
                            fichas[jugadorActual][resultado[1]].columna = coordsDest[1];
                            printf("Ficha consumida y movida a (%d, %d)\n", coordsDest[0], coordsDest[1]);
                            promocionDama(jugadorActual, resultado[1]);
                            // Procesar consumos concatenados
                            fichas[jugadorActual][resultado[1]].primerConsumoRealizado = 1;
                            while(verificarCapturasDisponibles(jugadorActual, fichas[jugadorActual][resultado[1]].fila, fichas[jugadorActual][resultado[1]].columna)){
                                promocionDama(jugadorActual, resultado[1]);
                                procesarConsumosConcatenados(jugadorActual, resultado);
                                promocionDama(jugadorActual, resultado[1]);
                            }
                        }
                    }
                    //Reinicia el indicador de primer consumo
                    fichas[jugadorActual][resultado[1]].primerConsumoRealizado = 0;
                    promocionDama(jugadorActual, resultado[1]);
                    gameOver = verificarFinJuego(jugadorActual, nombres);
                    if(!gameOver){
                        jugadorActual=1;
                    }
                //Mismo proceso para el jugador 2
                }else if(jugadorActual==1){ //Turno del jugador 2
                    printf("\n=== Turno del Jugador 2: %s ===\n", nombres[1]);
                    mostrar_tablero();
                    intentoCaptura = moverFicha(jugadorActual, resultado, coordsDest);
                    
                    if(intentoCaptura == 1){
                        // Se intentó capturar una ficha
                        int filaOrigen = fichas[jugadorActual][resultado[1]].fila;
                        int colOrigen = fichas[jugadorActual][resultado[1]].columna;
                        if(consumirFicha(filaOrigen, colOrigen, coordsDest[0], coordsDest[1], jugadorActual)){
                            fichas[jugadorActual][resultado[1]].fila = coordsDest[0];
                            fichas[jugadorActual][resultado[1]].columna = coordsDest[1];
                            printf("Ficha consumida y movida a (%d, %d)\n", coordsDest[0], coordsDest[1]);
                            promocionDama(jugadorActual, resultado[1]);
                            // Procesar consumos concatenados
                            fichas[jugadorActual][resultado[1]].primerConsumoRealizado = 1;
                            while(verificarCapturasDisponibles(jugadorActual, fichas[jugadorActual][resultado[1]].fila, fichas[jugadorActual][resultado[1]].columna)){
                                promocionDama(jugadorActual, resultado[1]);
                                procesarConsumosConcatenados(jugadorActual, resultado);
                                promocionDama(jugadorActual, resultado[1]);
                            }
                            
                        }
                    }
                    fichas[jugadorActual][resultado[1]].primerConsumoRealizado = 0;
                    promocionDama(jugadorActual, resultado[1]);
                    gameOver = verificarFinJuego(jugadorActual, nombres);
                    if(!gameOver){
                        jugadorActual=0;
                    }
                }
            }while(gameOver==0);
            break;
            
        case 2: //Muestra los créditos del juego
            fCreditos=fopen("creditos.txt", "r");
            if(fCreditos==NULL){
                printf("No se pudo abrir el archivo de créditos.\n");
            }else{
                while(fgets(linea, sizeof(linea), fCreditos)!=NULL){
                    printf("%s", linea);
                }
                fclose(fCreditos);
            }
            break;
            
        case 3: //Muestra el historial de victorias
            FILE *fHistorial;
            fHistorial=fopen("historial.bin", "rb");
            if(fHistorial==NULL){
                printf("No se pudo abrir el archivo de historial.\n");
                return 1;
            }
            printf("\nHistorial de Victorias\n");
            //Lee y muestra cada registro del historial
            while(fread(&nombreGanador, sizeof(char), 150, fHistorial)!=0){
                fread(&motivo, sizeof(char), 200, fHistorial);
                fread(&fichasActivas, sizeof(int), 1, fHistorial);
                printf("Ganador: %s\nMotivo: %s\nFichas activas: %d\n\n", nombreGanador, motivo, fichasActivas);
            }
            fclose(fHistorial);
            break;
            
        case 4:
            printf("Gracias por jugar\n");
            break;
            
        default:
            break;
        }
    }while(opcion!=4);
    
    return 0;
}

int mostrar_menu(){
    int opcion;
    printf("[1] Jugar\n");
    printf("[2] Creditos\n");
    printf("[3] Historial de Victorias\n");
    printf("[4] Salir\n");
    do{ //Bucle para validar la opcion ingresada
        printf("Seleccione una opcion: ");
        if(scanf("%d", &opcion)!=1){ //Valida que la entrada sea un numero entero
            while(getchar()!='\n');
            printf("Opcion invalida\n");
        }
    } while (opcion<1||opcion>4);
    while(getchar()!='\n');
    return opcion;
}

void inicializar_fichas() {
    int idx = 0;
    // Fichas jugador 1 (azul) - filas 0, 1, 2
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
    // Fichas jugador 2 (rojo) - filas 5, 6, 7
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


int buscarFichaEnPosicion(int fila, int col, int resultado[2]) {
    int jugador, i;
    for(jugador = 0; jugador < 2; jugador++) { //Recorre ambos jugadores
        for(i = 0; i < Num_fichas; i++) { //Recorre las fichas de cada jugador
            if(fichas[jugador][i].activa == 1 && fichas[jugador][i].fila == fila && fichas[jugador][i].columna == col) {
                //Ficha encontrada en la posición especificada
                resultado[0] = jugador;
                resultado[1] = i;
                return 1;
            }
        }
    }
    return 0;
}

void mostrar_tablero() {
    int resultado[2];
    int encontrada;
    int jugFicha, idxFicha;
    
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
            encontrada = buscarFichaEnPosicion(i, j, resultado); //Busca si hay una ficha en la posición (i, j)
            //Si se encuentra una ficha, muestra su representación
            if(encontrada == 1) {
                jugFicha = resultado[0];
                idxFicha = resultado[1];
                
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
    int aprobarNombre=1; //Variable bandera que indica si el nombre es valido
    do{ //Bucle para validar que los nombres sean correctos y no iguales
        do{ //Bucle para validar el nombre del jugador 1
            aprobarNombre=1;
            printf("Ingrese el nombre del Jugador 1: ");
            fgets(nombres[0], 150, stdin);
            //Valida que el nombre no este vacio o solo tenga espacios
            if(!(nombres[0][0]!='\n' && isspace(nombres[0][0])==0)){
                aprobarNombre=0;
            }
            //Valida que el nombre solo contenga letras y espacios
            for(int i=strlen(nombres[0])-2; i>=0; i--){
                if(isalpha(nombres[0][i])==0 && nombres[0][i]!=' '){
                    aprobarNombre=0;
                    break;
                }
            }
            if(aprobarNombre==0){
                printf("Nombre invalido\n");
            }
        }while(aprobarNombre==0);
        
        //Mismo proceso para el jugador 2
        do{ //Bucle para validar el nombre del jugador 2
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


int moverFicha(int jugadorActual, int resultado[2], int dest[2]){
    int filaOrigen, colOrigen, filaDestino, colDestino, encontrada, repetirSolicitud, moverse, distanciaFila, distanciaCol, filaMedio, colMedio, dirFila;
    do{ //Bucle para validar la entrada de la ficha a mover
        repetirSolicitud=0;
        do{ //Bucle para validar la fila de origen
            printf("Ingrese la fila de la ficha que desea mover: ");
            if(scanf("%d", &filaOrigen)!=1){ //Valida que la entrada sea un numero entero
                printf("Entrada invalida. Ingrese un numero entero para la fila: ");
                while(getchar()!='\n');
                filaOrigen=-1;
            }
        }while(filaOrigen<0 || filaOrigen>7);
        while(getchar()!='\n');
        
        do{ //Bucle para validar la columna de origen
            printf("Ingrese la columna de la ficha que desea mover: ");
            if(scanf("%d", &colOrigen)!=1){
                printf("Entrada invalida. Ingrese un numero entero para la columna: ");
                while(getchar()!='\n');
                colOrigen=-1;
            }
        }while(colOrigen<0 || colOrigen>7);
        while(getchar()!='\n');
        encontrada=buscarFichaEnPosicion(filaOrigen, colOrigen, resultado); //Busca si hay una ficha en la posición de origen
        if(encontrada==1 && resultado[0]==jugadorActual){ //Verifica que la ficha encontrada pertenezca al jugador actual
            if(puedeMoverse(jugadorActual, 1, resultado[1], filaOrigen, colOrigen)==0){
                printf("La ficha seleccionada no tiene movimientos validos.\n");
                repetirSolicitud=1;
                continue;
            }
            do{ //Bucle para validar la fila de destino
                printf("Ingrese la fila de destino: ");
                if(scanf("%d", &filaDestino)!=1){
                    printf("Entrada invalida. Ingrese un numero entero para la fila de destino: ");
                    while(getchar()!='\n');
                    filaDestino=-1;
                }
            }while(filaDestino<0 || filaDestino>7);
            while(getchar()!='\n'); //Limpia el buffer de entrada
            
            do{ //Bucle para validar la columna de destino
                printf("Ingrese la columna de destino: ");
                if(scanf("%d", &colDestino)!=1){
                    printf("Entrada invalida. Ingrese un numero entero para la columna de destino: ");
                    while(getchar()!='\n');
                    colDestino=-1;
                }
            }while(colDestino<0 || colDestino>7);
            while(getchar()!='\n'); //Limpia el buffer de entrada
            
            if(repetirSolicitud==1){
                continue;
            }
            
            //Verifica que la casilla de destino sea amarilla
            if((filaDestino+colDestino)%2==0){
                printf("Movimiento invalido: solo se puede mover a casillas amarillas\n");
                repetirSolicitud=1;
                continue;
            }
            
            //Determina el tipo de movimiento por la distancia
            distanciaFila = abs(filaDestino-filaOrigen);
            distanciaCol = abs(colDestino-colOrigen);
            
            //CASO 1: Intento de captura (2 casillas)
            if(distanciaFila==2 && distanciaCol==2){
                //Verifica que la casilla de destino esté vacía
                int tempRes[2];
                encontrada = buscarFichaEnPosicion(filaDestino, colDestino, tempRes);
                if(encontrada==1){
                    printf("La casilla de destino esta ocupada\n");
                    repetirSolicitud=1;
                    continue;
                }
                
                //Calcula la posición intermedia
                filaMedio = (filaOrigen + filaDestino) / 2;
                colMedio = (colOrigen + colDestino) / 2;
                dirFila = (filaDestino - filaOrigen) / 2;
                //Si NO es dama y NO ha hecho su primer consumo, debe capturar hacia adelante
                if(!fichas[jugadorActual][resultado[1]].esDama && fichas[jugadorActual][resultado[1]].primerConsumoRealizado == 0){
                    if(jugadorActual == 0 && dirFila <= 0){
                        printf("El primer consumo debe ser hacia adelante.\n");
                        repetirSolicitud = 1;
                        continue;
                    }
                    if(jugadorActual == 1 && dirFila >= 0){
                        printf("El primer consumo debe ser hacia adelante.\n");
                        repetirSolicitud = 1;
                        continue;
                    }
                }

                // Verifica que haya una ficha enemiga en medio
                if(buscarFichaEnPosicion(filaMedio, colMedio, tempRes)!=0){
                    if(tempRes[0]!=jugadorActual){
                        // Es válido, hay ficha enemiga para capturar
                        buscarFichaEnPosicion(filaOrigen, colOrigen, resultado);
                        dest[0]=filaDestino;
                        dest[1]=colDestino;
                        return 1; // Indica que se desea capturar una ficha
                    }else{
                        printf("No puedes saltar sobre tus propias fichas\n");
                        repetirSolicitud=1;
                        continue;
                    }
                }else{
                    printf("No hay ficha enemiga para capturar en esa diagonal\n");
                    repetirSolicitud=1;
                    continue;
                }
            }else if(distanciaFila==1 && distanciaCol==1){
                // Verifica que el peon no se mueva para atras
                if(fichas[jugadorActual][resultado[1]].esDama==0){ 
                    if((jugadorActual==0 && filaDestino<=filaOrigen) || (jugadorActual==1 && filaDestino>=filaOrigen)){
                        printf("Movimiento invalido segun las reglas del juego.\n");
                        repetirSolicitud=1;
                        continue;
                    }
                }
                
                // Verifica que la casilla de destino esté vacía
                encontrada=buscarFichaEnPosicion(filaDestino, colDestino, resultado);
                if(encontrada==1){
                    printf("La casilla seleccionada ya esta ocupada\n");
                    repetirSolicitud=1;
                    continue;
                }
                // Si pasa todas las validaciones, mueve la ficha
                buscarFichaEnPosicion(filaOrigen, colOrigen, resultado);
                fichas[jugadorActual][resultado[1]].fila = filaDestino;
                fichas[jugadorActual][resultado[1]].columna = colDestino;
                printf("Ficha movida a (%d, %d)\n", 
                       fichas[jugadorActual][resultado[1]].fila, 
                       fichas[jugadorActual][resultado[1]].columna);
            }
            
            // CASO 3: Movimiento inválido
            else{
                printf("Movimiento invalido: solo puedes mover 1 casilla (movimiento simple) o 2 casillas (captura)\n");
                repetirSolicitud=1;
                continue;
            }
        }else{
            printf("Ficha no encontrada o no pertenece al jugador.\n");
            repetirSolicitud=1;
        }
    }while(repetirSolicitud==1);
    
    return 0;
}

int promocionDama(int jugadorActual, int indiceFicha){
    //Verifica que el índice de la ficha sea válido
    if(indiceFicha<0 || indiceFicha>=Num_fichas){
        return -1;
    }
    //Verifica si la ficha alcanza la fila final para ser promovida a dama
    if(jugadorActual==0 && fichas[jugadorActual][indiceFicha].fila==7 && fichas[jugadorActual][indiceFicha].esDama==0 && fichas[jugadorActual][indiceFicha].activa==1){
        fichas[jugadorActual][indiceFicha].esDama=1;
        printf("La ficha ha sido promovida a Dama\n");
        return 1;
    }else if(jugadorActual==1 && fichas[jugadorActual][indiceFicha].fila==0 && fichas[jugadorActual][indiceFicha].esDama==0 && fichas[jugadorActual][indiceFicha].activa==1){
        fichas[jugadorActual][indiceFicha].esDama=1;
        printf("La ficha ha sido promovida a Dama\n");
        return 1;
    }
    return 0;
}

int consumirFicha(int filaOrigen, int colOrigen, int filaDestino, int colDestino, int jugadorActual){
    // Calcula la posición de la ficha que está en medio
    int filaMedio = (filaOrigen + filaDestino) / 2;
    int colMedio = (colOrigen + colDestino) / 2;
    int resultado[2];
    
    // Busca si hay una ficha en la posición intermedia
    if(buscarFichaEnPosicion(filaMedio, colMedio, resultado)){
        // Verifica que la ficha encontrada sea del jugador contrario
        if(resultado[0] != jugadorActual){
            // Desactiva la ficha enemiga (la consume)
            fichas[resultado[0]][resultado[1]].activa = 0;
            printf("Ficha enemiga en (%d, %d) consumida!\n", filaMedio, colMedio);
            return 1; // Consumo exitoso
        } else {
            printf("No puedes consumir tus propias fichas.\n");
            return 0;
        }
    } else {
        printf("No hay ficha enemiga para consumir en esa diagonal.\n");
        return 0;
    }
}

int verificarCapturasDisponibles(int jugador, int fila, int col){
    int resultado[2], esDama=0;
    // Buscar si la ficha es dama
        if(buscarFichaEnPosicion(fila, col, resultado)){
            esDama = fichas[resultado[0]][resultado[1]].esDama;
        }
        
        // Direcciones posibles de captura diagonal
        int direcciones[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
        
        for(int d = 0; d < 4; d++){
            int dirFila = direcciones[d][0];
            int dirCol = direcciones[d][1];
            // Si es peón, solo puede capturar hacia adelante
            int filaCaptura = fila + dirFila;
            int colCaptura = col + dirCol;
            int filaDespuesCaptura = fila + 2 * dirFila;
            int colDespuesCaptura = col + 2 * dirCol;
            
            // Verifica que la posición después de la captura esté dentro del tablero
            if(filaDespuesCaptura >= 0 && filaDespuesCaptura < 8 && colDespuesCaptura >= 0 && colDespuesCaptura < 8){
                if((filaDespuesCaptura + colDespuesCaptura) % 2 == 1){ // Casilla amarilla
                    // Verifica si hay ficha enemiga en medio y casilla destino libre
                    if(buscarFichaEnPosicion(filaCaptura, colCaptura, resultado)){
                        if(resultado[0] != jugador){ // Es ficha enemiga
                            if(!buscarFichaEnPosicion(filaDespuesCaptura, colDespuesCaptura, resultado)){
                                return 1; // Hay una captura disponible
                            }
                        }
                    }
                }
            }
        }
    return 0; // No hay capturas disponibles
}

void procesarConsumosConcatenados(int jugadorActual, int resultado[2]){
    int filaActual = fichas[jugadorActual][resultado[1]].fila;
    int colActual = fichas[jugadorActual][resultado[1]].columna;
    int capturasContinuas = 1;
    char respuesta;
    
    while(capturasContinuas){
        // Verifica si hay más capturas disponibles desde la posición actual
        if(verificarCapturasDisponibles(jugadorActual, filaActual, colActual)){
            promocionDama(jugadorActual, resultado[1]); // Verifica promoción a dama antes de la captura adicional
            mostrar_tablero(); //Muestra el tablero actualizado
            printf("\n¡Puedes realizar otra captura!\n");
            printf("Posicion actual de tu ficha: (%d, %d)\n", filaActual, colActual);
            printf("¿Deseas continuar capturando? (s/n): ");
            scanf(" %c", &respuesta);
            while(getchar()!='\n'); //Limpia el buffer de entrada
            //Procesa la captura adicional si el jugador desea continuar
            if(respuesta == 's' || respuesta == 'S'){
                int filaDestino, colDestino;
                int intentoValido = 0;
                do{ //Bucle para validar el movimiento de captura adicional
                    do{ //Bucle para validar la fila de destino
                        printf("Ingrese la fila de destino para la siguiente captura: ");
                        if(scanf("%d", &filaDestino)!=1){
                            printf("Entrada invalida.\n");
                            while(getchar()!='\n');
                            filaDestino=-1;
                        }
                    }while(filaDestino<0 || filaDestino>7);
                    while(getchar()!='\n');
                    
                    do{ //Bucle para validar la columna de destino
                        printf("Ingrese la columna de destino para la siguiente captura: ");
                        if(scanf("%d", &colDestino)!=1){
                            printf("Entrada invalida.\n");
                            while(getchar()!='\n');
                            colDestino=-1;
                        }
                    }while(colDestino<0 || colDestino>7);
                    while(getchar()!='\n');
                    
                    // Valida que el movimiento sea de captura (2 casillas diagonales)
                    if(abs(filaDestino - filaActual) == 2 && abs(colDestino - colActual) == 2){
                        // Verifica que la casilla de destino sea amarilla
                        if((filaDestino + colDestino) % 2 == 1){
                            int resTemp[2];
                            // Verifica que la casilla de destino esté vacía
                            if(!buscarFichaEnPosicion(filaDestino, colDestino, resTemp)){
                                // Intenta consumir la ficha
                                if(consumirFicha(filaActual, colActual, filaDestino, colDestino, jugadorActual)){
                                    // Actualiza la posición de la ficha
                                    fichas[jugadorActual][resultado[1]].fila = filaDestino;
                                    fichas[jugadorActual][resultado[1]].columna = colDestino;
                                    filaActual = filaDestino;
                                    colActual = colDestino;
                                    printf("Captura adicional realizada! Ficha movida a (%d, %d)\n", filaActual, colActual);
                                    intentoValido = 1;
                                } else {
                                    printf("No se pudo realizar la captura. Intenta de nuevo.\n");
                                }
                            } else {
                                printf("La casilla de destino está ocupada. Intenta de nuevo.\n");
                            }
                        } else {
                            printf("Debes moverte a una casilla amarilla. Intenta de nuevo.\n");
                        }
                    } else {
                        printf("Para capturar debes moverte 2 casillas diagonalmente. Intenta de nuevo.\n");
                    }
                }while(!intentoValido);
                
            } else {
                printf("Finalizando capturas.\n");
                capturasContinuas = 0;
            }
        } else {
            // No hay más capturas disponibles
            capturasContinuas = 0;
        }
    }
    promocionDama(jugadorActual, resultado[1]);
}

int verificarFinJuego(int jugadorActual, char nombres[2][150]){
    FILE *fHistorial;
    int jugadorRival;
    int fichasActivas[2] = {0, 0};
    //Determina el jugador rival
    if(jugadorActual==0){
        jugadorRival=1;
    }else{
        jugadorRival=0;
    }

    // Cuenta las fichas activas de cada jugador
    for(int i = 0; i < Num_fichas; i++){
        if(fichas[0][i].activa == 1) fichasActivas[0]++;
        if(fichas[1][i].activa == 1) fichasActivas[1]++;
    }
    fHistorial=fopen("historial.bin", "ab");
    // Condición 1: El jugador rival no tiene fichas
    if(fichasActivas[jugadorRival] == 0){
        mostrar_tablero();
        printf("   JUEGO TERMINADO\n");
        printf("   Ganador: %s\n", nombres[jugadorActual]);
        printf("   Motivo: El oponente no tiene fichas\n");
        printf("========================================\n");
        fwrite(&nombres[jugadorActual], sizeof(char), 150, fHistorial);
        char motivo[] = "El oponente no tiene fichas";
        fwrite(motivo, sizeof(char), strlen(motivo), fHistorial);
        fclose(fHistorial);
        return 1;
    }
    
    // Condición 2: El jugador rival no puede moverse
    if(!puedeMoverse(jugadorRival, 0, 0, 0, 0)){
        mostrar_tablero();
        printf("   JUEGO TERMINADO\n");
        printf("   Ganador: %s\n", nombres[jugadorActual]);
        printf("   Motivo: El oponente no tiene movimientos válidos\n");
        printf("========================================\n");
        fwrite(&nombres[jugadorActual], sizeof(char), 150, fHistorial);
        char motivo[] = "El oponente no tiene movimientos válidos";
        fwrite(motivo, sizeof(char), strlen(motivo), fHistorial);
        fclose(fHistorial);
        return 1;
    }
    
    // Condición 3: Empate (Si ambos solo tienen una ficha)
    if(fichasActivas[0] == 1 && fichasActivas[1] == 1){
        mostrar_tablero();
        printf("   JUEGO TERMINADO\n");
        printf("   Resultado: Empate\n");
        printf("   Motivo: Ambos jugadores solo tienen una ficha restante\n");
        printf("========================================\n");
        return 1;
    }
    return 0; // El juego continúa
}

int puedeMoverse(int jugador, int notEndGame, int ficha, int fila, int columna){
    int resultado[2];
    if(notEndGame==0){
        // Revisa cada ficha activa del jugador
        for(int i = 0; i < Num_fichas; i++){
            if(fichas[jugador][i].activa == 0) continue;

            int fila = fichas[jugador][i].fila;
            int col = fichas[jugador][i].columna;
            int esDama = fichas[jugador][i].esDama;
            
            // Direcciones posibles de movimiento diagonal
            int direcciones[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
            
            for(int d = 0; d < 4; d++){
                int dirFila = direcciones[d][0];
                int dirCol = direcciones[d][1];
                
                // Si es peón, solo puede moverse hacia adelante
                if(!esDama){
                    if(jugador == 0 && dirFila < 0){
                        continue; // Jugador 0 solo hacia abajo
                    }
                    if(jugador == 1 && dirFila > 0){
                        continue; // Jugador 1 solo hacia arriba
                    }
                }
                
                // Verifica movimiento simple (1 casilla)
                int nuevaFila = fila + dirFila;
                int nuevaCol = col + dirCol;
                
                if(nuevaFila >= 0 && nuevaFila < 8 && nuevaCol >= 0 && nuevaCol < 8){
                    if((nuevaFila + nuevaCol) % 2 == 1){ // Casilla amarilla
                        if(!buscarFichaEnPosicion(nuevaFila, nuevaCol, resultado)){
                            return 1; // Hay un movimiento simple disponible
                        }
                    }
                }
                
                // Verifica movimiento de captura (2 casillas)
                int filaCaptura = fila + dirFila;
                int colCaptura = col + dirCol;
                int filaDespuesCaptura = fila + 2 * dirFila;
                int colDespuesCaptura = col + 2 * dirCol;
                // Verifica que la posición después de la captura esté dentro del tablero
                if(filaDespuesCaptura >= 0 && filaDespuesCaptura < 8 && colDespuesCaptura >= 0 && colDespuesCaptura < 8){
                    if((filaDespuesCaptura + colDespuesCaptura) % 2 == 1){ // Casilla amarilla
                        // Verifica si hay ficha enemiga en medio y casilla destino libre
                        if(buscarFichaEnPosicion(filaCaptura, colCaptura, resultado)){
                            if(resultado[0] != jugador){ // Es ficha enemiga
                                if(!buscarFichaEnPosicion(filaDespuesCaptura, colDespuesCaptura, resultado)){
                                    return 1; // Hay un movimiento de captura disponible
                                }
                            }
                        }
                    }
                }
            }
        }
    }else if(notEndGame==1){ // Verifica solo una ficha específica
        if(fichas[jugador][ficha].activa == 0){
            return 0;
        }
        int fila = fichas[jugador][ficha].fila;
        int col = fichas[jugador][ficha].columna;
        int esDama = fichas[jugador][ficha].esDama;
        
        // Direcciones posibles de movimiento diagonal
        int direcciones[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
        
        for(int d = 0; d < 4; d++){
            int dirFila = direcciones[d][0];
            int dirCol = direcciones[d][1];
            
            // Si es peón, solo puede moverse hacia adelante
            if(!esDama){
                if(jugador == 0 && dirFila < 0){
                    continue; // Jugador 0 solo hacia abajo
                }
                if(jugador == 1 && dirFila > 0){
                    continue; // Jugador 1 solo hacia arriba
                }
            }
            
            // Verifica movimiento simple (1 casilla)
            int nuevaFila = fila + dirFila;
            int nuevaCol = col + dirCol;
            
            if(nuevaFila >= 0 && nuevaFila < 8 && nuevaCol >= 0 && nuevaCol < 8){
                if((nuevaFila + nuevaCol) % 2 == 1){ // Casilla amarilla
                    if(!buscarFichaEnPosicion(nuevaFila, nuevaCol, resultado)){
                        return 1; // Hay un movimiento simple disponible
                    }
                }
            }
            
            // Verifica movimiento de captura (2 casillas)
            int filaCaptura = fila + dirFila;
            int colCaptura = col + dirCol;
            int filaDespuesCaptura = fila + 2 * dirFila;
            int colDespuesCaptura = col + 2 * dirCol;
            // Verifica que la posición después de la captura esté dentro del tablero
            if(filaDespuesCaptura >= 0 && filaDespuesCaptura < 8 && colDespuesCaptura >= 0 && colDespuesCaptura < 8){
                if((filaDespuesCaptura + colDespuesCaptura) % 2 == 1){ // Casilla amarilla
                    // Verifica si hay ficha enemiga en medio y casilla destino libre
                    if(buscarFichaEnPosicion(filaCaptura, colCaptura, resultado)){
                        if(resultado[0] != jugador){ // Es ficha enemiga
                            if(!buscarFichaEnPosicion(filaDespuesCaptura, colDespuesCaptura, resultado)){
                                return 1; // Hay un movimiento de captura disponible
                            }
                        }
                    }
                }
            }
        }
    }
    return 0; // No hay movimientos disponibles
}

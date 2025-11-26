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

// Declaraciones de funciones
int mostrar_menu();
int buscarFichaEnPosicion(int fila, int col, int resultado[2]);
void inicializar_fichas();
void mostrar_tablero();
void ingresarNombreJugadores(char nombres[2][150]);
int moverFicha(int jugadorActual, int resultado[2], int dest[2]);
int promocionDama(int jugadorActual, int indiceFicha);
int consumirFicha(int filaOrigen, int colOrigen, int filaDestino, int colDestino, int jugadorActual);
int verificarFinJuego(int jugadorActual, char nombres[2][150]);
int puedeMoverse(int jugador);
int verificarCapturasDisponibles(int jugador, int fila, int col);
void procesarConsumosConcatenados(int jugadorActual, int resultado[2]);

// ============== FUNCIÓN MAIN ==============
int main(){
    char nombres[2][150], linea[200];
    int opcion, jugadorActual=0, gameOver=0;
    int resultado[2]; //resultado[0]= jugador que pertenece, resultado[1]= indice de la ficha
    int coordsDest[2]; //coordsDest[0]= fila destino, coordsDest[1]= columna destino
    int intentoCaptura;
    FILE *fCreditos;
    
    do{
        opcion = mostrar_menu();
        switch (opcion)
        {
        case 1:
            ingresarNombreJugadores(nombres);
            inicializar_fichas();
            jugadorActual = 0;
            gameOver = 0;
            
            do{
                if(jugadorActual==0){
                    printf("\n=== Turno del Jugador 1: %s ===\n", nombres[0]);
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
                            
                            // Procesar consumos concatenados
                            procesarConsumosConcatenados(jugadorActual, resultado);
                        }
                    }
                    
                    promocionDama(jugadorActual, resultado[1]);
                    gameOver = verificarFinJuego(jugadorActual, nombres);
                    if(!gameOver) jugadorActual=1;
                    
                }else if(jugadorActual==1){
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
                            
                            // Procesar consumos concatenados
                            procesarConsumosConcatenados(jugadorActual, resultado);
                        }
                    }
                    
                    promocionDama(jugadorActual, resultado[1]);
                    gameOver = verificarFinJuego(jugadorActual, nombres);
                    if(!gameOver) jugadorActual=0;
                }
            }while(gameOver==0);
            
            printf("\nPresione Enter para volver al menu...");
            getchar();
            break;
            
        case 2: 
            fCreditos=fopen("creditos.txt", "r");
            if(fCreditos==NULL){
                printf("No se pudo abrir el archivo de créditos.\n");
            }else{
                while(fgets(linea, sizeof(linea), fCreditos)!=NULL){
                    printf("%s", linea);
                }
                fclose(fCreditos);
            }
            printf("\nPresione Enter para continuar...");
            getchar();
            break;
            
        case 3:
            printf("Funcion de historial no implementada aun.\n");
            printf("Presione Enter para continuar...");
            getchar();
            break;
            
        case 4:
            printf("Gracias por jugar!\n");
            break;
            
        default:
            break;
        }
    }while(opcion!=4);
    
    return 0;
}

// ============== MOSTRAR MENÚ ==============
int mostrar_menu(){
    int opcion;
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

// ============== INICIALIZAR FICHAS ==============
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

// ============== BUSCAR FICHA EN POSICIÓN ==============
int buscarFichaEnPosicion(int fila, int col, int resultado[2]) {
    int jugador, i;
    for(jugador = 0; jugador < 2; jugador++) {
        for(i = 0; i < Num_fichas; i++) {
            if(fichas[jugador][i].activa == 1 && 
               fichas[jugador][i].fila == fila && 
               fichas[jugador][i].columna == col) {
                resultado[0] = jugador;
                resultado[1] = i;
                return 1;
            }
        }
    }
    return 0;
}

// ============== MOSTRAR TABLERO ==============
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
            
            encontrada = buscarFichaEnPosicion(i, j, resultado);

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

// ============== INGRESAR NOMBRES ==============
void ingresarNombreJugadores(char nombres[2][150]){
    int aprobarNombre=1;
    do{
        do{
            aprobarNombre=1;
            printf("Ingrese el nombre del Jugador 1: ");
            fgets(nombres[0], 150, stdin);
            
            if(!(nombres[0][0]!='\n' && isspace(nombres[0][0])==0)){
                aprobarNombre=0;
            }
            
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

// ============== MOVER FICHA ==============
int moverFicha(int jugadorActual, int resultado[2], int dest[2]){
    int filaOrigen, colOrigen, filaDestino, colDestino, encontrada, repetirSolicitud;
    
    do{
        repetirSolicitud=0;
        
        do{
            printf("Ingrese la fila de la ficha que desea mover: ");
            if(scanf("%d", &filaOrigen)!=1){
                printf("Entrada invalida. Ingrese un numero entero para la fila: ");
                while(getchar()!='\n');
                filaOrigen=-1;
            }
        }while(filaOrigen<0 || filaOrigen>7);
        while(getchar()!='\n');
        
        do{
            printf("Ingrese la columna de la ficha que desea mover: ");
            if(scanf("%d", &colOrigen)!=1){
                printf("Entrada invalida. Ingrese un numero entero para la columna: ");
                while(getchar()!='\n');
                colOrigen=-1;
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
                    filaDestino=-1;
                }
            }while(filaDestino<0 || filaDestino>7);
            while(getchar()!='\n');
            
            do{
                printf("Ingrese la columna de destino: ");
                if(scanf("%d", &colDestino)!=1){
                    printf("Entrada invalida. Ingrese un numero entero para la columna de destino: ");
                    while(getchar()!='\n');
                    colDestino=-1;
                }
            }while(colDestino<0 || colDestino>7);
            while(getchar()!='\n');
            
            if(repetirSolicitud==1){
                continue;
            }
            
            // Verifica que la casilla de destino sea amarilla
            if((filaDestino+colDestino)%2==0){
                printf("Movimiento invalido: solo se puede mover a casillas amarillas\n");
                repetirSolicitud=1;
                continue;
            }
            
            // Determina el tipo de movimiento por la distancia
            int distanciaFila = abs(filaDestino - filaOrigen);
            int distanciaCol = abs(colDestino - colOrigen);
            
            // CASO 1: Intento de captura (2 casillas)
            if(distanciaFila == 2 && distanciaCol == 2){
                // Verifica que la casilla de destino esté vacía
                int tempRes[2];
                encontrada = buscarFichaEnPosicion(filaDestino, colDestino, tempRes);
                if(encontrada == 1){
                    printf("La casilla de destino esta ocupada\n");
                    repetirSolicitud=1;
                    continue;
                }
                
                // Calcula la posición intermedia
                int filaMedio = (filaOrigen + filaDestino) / 2;
                int colMedio = (colOrigen + colDestino) / 2;
                
                // Verifica que haya una ficha enemiga en medio
                if(buscarFichaEnPosicion(filaMedio, colMedio, tempRes)){
                    if(tempRes[0] != jugadorActual){
                        // Es válido, hay ficha enemiga para capturar
                        buscarFichaEnPosicion(filaOrigen, colOrigen, resultado);
                        dest[0] = filaDestino;
                        dest[1] = colDestino;
                        return 1; // Indica que se desea capturar una ficha
                    } else {
                        printf("No puedes saltar sobre tus propias fichas\n");
                        repetirSolicitud=1;
                        continue;
                    }
                } else {
                    printf("No hay ficha enemiga para capturar en esa diagonal\n");
                    repetirSolicitud=1;
                    continue;
                }
            }
            
            // CASO 2: Movimiento simple (1 casilla)
            else if(distanciaFila == 1 && distanciaCol == 1){
                // Verifica que el peon no se mueva para atras
                if(fichas[jugadorActual][resultado[1]].esDama == 0){ 
                    if((jugadorActual == 0 && filaDestino <= filaOrigen) || 
                       (jugadorActual == 1 && filaDestino >= filaOrigen)){
                        printf("Movimiento invalido segun las reglas del juego.\n");
                        repetirSolicitud=1;
                        continue;
                    }
                }
                
                // Verifica que la casilla de destino esté vacía
                encontrada = buscarFichaEnPosicion(filaDestino, colDestino, resultado);
                if(encontrada == 1){
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
            else {
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

// ============== PROMOCIÓN A DAMA ==============
int promocionDama(int jugadorActual, int indiceFicha){
    if(indiceFicha<0 || indiceFicha>=Num_fichas){
        return -1;
    }
    
    if(jugadorActual==0 && 
       fichas[jugadorActual][indiceFicha].fila==7 && 
       fichas[jugadorActual][indiceFicha].esDama==0 && 
       fichas[jugadorActual][indiceFicha].activa==1){
        fichas[jugadorActual][indiceFicha].esDama=1;
        printf("La ficha ha sido promovida a Dama\n");
        return 1;
    }else if(jugadorActual==1 && 
             fichas[jugadorActual][indiceFicha].fila==0 && 
             fichas[jugadorActual][indiceFicha].esDama==0 && 
             fichas[jugadorActual][indiceFicha].activa==1){
        fichas[jugadorActual][indiceFicha].esDama=1;
        printf("La ficha ha sido promovida a Dama\n");
        return 1;
    }
    return 0;
}

// ============== CONSUMIR FICHA ==============
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

// ============== VERIFICAR CAPTURAS DISPONIBLES ==============
int verificarCapturasDisponibles(int jugador, int fila, int col){
    int resultado[2];
    int esDama = 0;
    
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
        if(!esDama){
            if(jugador == 0 && dirFila < 0) continue; // Jugador 0 solo hacia abajo
            if(jugador == 1 && dirFila > 0) continue; // Jugador 1 solo hacia arriba
        }
        
        int filaCaptura = fila + dirFila;
        int colCaptura = col + dirCol;
        int filaDespuesCaptura = fila + 2 * dirFila;
        int colDespuesCaptura = col + 2 * dirCol;
        
        // Verifica que la posición después de la captura esté dentro del tablero
        if(filaDespuesCaptura >= 0 && filaDespuesCaptura < 8 && 
           colDespuesCaptura >= 0 && colDespuesCaptura < 8){
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

// ============== PROCESAR CONSUMOS CONCATENADOS ==============
void procesarConsumosConcatenados(int jugadorActual, int resultado[2]){
    int filaActual = fichas[jugadorActual][resultado[1]].fila;
    int colActual = fichas[jugadorActual][resultado[1]].columna;
    int capturasContinuas = 1;
    char respuesta;
    
    while(capturasContinuas){
        // Verifica si hay más capturas disponibles desde la posición actual
        if(verificarCapturasDisponibles(jugadorActual, filaActual, colActual)){
            mostrar_tablero();
            printf("\n¡Puedes realizar otra captura!\n");
            printf("Posicion actual de tu ficha: (%d, %d)\n", filaActual, colActual);
            printf("¿Deseas continuar capturando? (s/n): ");
            
            scanf(" %c", &respuesta);
            while(getchar()!='\n');
            
            if(respuesta == 's' || respuesta == 'S'){
                int filaDestino, colDestino;
                int intentoValido = 0;
                
                do{
                    do{
                        printf("Ingrese la fila de destino para la siguiente captura: ");
                        if(scanf("%d", &filaDestino)!=1){
                            printf("Entrada invalida.\n");
                            while(getchar()!='\n');
                            filaDestino=-1;
                        }
                    }while(filaDestino<0 || filaDestino>7);
                    while(getchar()!='\n');
                    
                    do{
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
}

// ============== VERIFICAR FIN DE JUEGO ==============
int verificarFinJuego(int jugadorActual, char nombres[2][150]){
    int jugadorRival = (jugadorActual == 0) ? 1 : 0;
    int fichasActivas[2] = {0, 0};
    
    // Cuenta las fichas activas de cada jugador
    for(int i = 0; i < Num_fichas; i++){
        if(fichas[0][i].activa == 1) fichasActivas[0]++;
        if(fichas[1][i].activa == 1) fichasActivas[1]++;
    }
    
    // Condición 1: El jugador rival no tiene fichas
    if(fichasActivas[jugadorRival] == 0){
        mostrar_tablero();
        printf("\n========================================\n");
        printf("   ¡JUEGO TERMINADO!\n");
        printf("   Ganador: %s\n", nombres[jugadorActual]);
        printf("   Motivo: El oponente no tiene fichas\n");
        printf("========================================\n");
        return 1;
    }
    
    // Condición 2: El jugador rival no puede moverse
    if(!puedeMoverse(jugadorRival)){
        mostrar_tablero();
        printf("\n========================================\n");
        printf("   ¡JUEGO TERMINADO!\n");
        printf("   Ganador: %s\n", nombres[jugadorActual]);
        printf("   Motivo: El oponente no tiene movimientos válidos\n");
        printf("========================================\n");
        return 1;
    }
    
    return 0; // El juego continúa
}

// ============== PUEDE MOVERSE ==============
int puedeMoverse(int jugador){
    int resultado[2];
    
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
                if(jugador == 0 && dirFila < 0) continue; // Jugador 0 solo hacia abajo
                if(jugador == 1 && dirFila > 0) continue; // Jugador 1 solo hacia arriba
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
            
            if(filaDespuesCaptura >= 0 && filaDespuesCaptura < 8 && 
               colDespuesCaptura >= 0 && colDespuesCaptura < 8){
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
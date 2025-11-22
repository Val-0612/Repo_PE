#include <stdio.h>
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
    int jugador;   // 1 = rojo, 2 = azul
} Ficha;
Ficha fichas[2][Num_fichas];  // fichas[0] = jugador 1, fichas[1] = jugador 2

int mostrar_menu(),buscarFichaEnPosicion(int fila, int col, int resultado[2]);
void inicializar_fichas(),mostrar_tablero();
int main(){
    int opcion;
    opcion= mostrar_menu();
    switch (opcion)
    {
    case 1:
        
        break;
    
    default:
        break;
    }
    return 0;
}

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
        scanf("%d", &opcion);
    } while (opcion<1||opcion>4);
    
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
                fichas[0][idx].jugador = 1;
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
                fichas[1][idx].jugador = 2;
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
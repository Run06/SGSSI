#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TAM_MAX_TEXTO 10000   // Tamaño máximo texto
// Buffers para texto original y descifrado
char texto_original[TAM_MAX_TEXTO];
char texto_descifrado[TAM_MAX_TEXTO];

int fichnumcarac(char c, FILE *fp);

int main() {
    char nombre[80],c;
    FILE *fp;
    
    int contadores[27] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    char alfabeto[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','Ñ'};
    
    char frecuenciasEsp[] = {'E','A','O','L','S','N','D','R','U','I','T','C','P','M','Y','Q','B','H','G','F','J','Ñ','Z','X','K','W','V'};
    //16.78, 11.96, 8.69, 8.37, 7.88, 7.01, 6.87, 4.94, 4.80, 4.15, 3.31, 2.92, 2.776, 2.12, 1.54, 1.53, 0.92, 0.89, 0.73, 0.52, 0.30, 0.29, 0.15, 0.06, 0.00, 0.00
    int cont = 0;

    printf ("¿Qué archivo deseas descifrar?\nNota: Ingresa el texto a desencriptar en un fichero.\n");
    scanf("%s",nombre);
    fp = fopen(nombre, "r");
    if (fp == NULL) {
        printf("No se puede abrir el archivo %s\n", nombre);
        return -1;
    } else {
        printf("El archivo a analizar es: %s\n", nombre);

        // Contar frecuencia de cada letra
        for (int i = 0; i < 26; i++) {
            contadores[i] = fichnumcarac('A' + i, fp);
            rewind(fp); // Regresar al inicio para cada letra
        }
        contadores[26] = fichnumcarac('Ñ', fp);
        rewind(fp);
        // Ñ no está en ASCII consecutivo, la ponemos aparte (posición 26)
    }
    
    /* Mostrar resultados
    printf("\nFrecuencia de letras:\n");
    for (int i = 0; i < 26; i++) {
        printf("%c: %d\n", 'A' + i, contadores[i]);
    }
    printf("Ñ: %d\n", contadores[26]);
    
    fclose(fp); */
    
    // Ordenar letras_detectadas por frecuencia descendente (bubble sort)
    for (int i = 0; i < 26; i++) {
        for (int j = i + 1; j < 27; j++) {
            if (contadores[j] > contadores[i]) {
                // Intercambiar frecuencias
                int temp_freq = contadores[i];
                contadores[i] = contadores[j];
                contadores[j] = temp_freq;

                // Intercambiar letras
                char temp_letra = alfabeto[i];
                alfabeto[i] = alfabeto[j];
                alfabeto[j] = temp_letra;
            }
        }
    }
    
    // Leer texto completo en buffer
    int idx = 0;
    while ((c = fgetc(fp)) != EOF && idx < TAM_MAX_TEXTO - 1) {
        texto_original[idx++] = c;
    }
    texto_original[idx] = '\0';

    fclose(fp);

    // Descifrar texto
    for (int i = 0; texto_original[i] != '\0'; i++) {
        char ch = texto_original[i];
        char ch_may = toupper((unsigned char)ch);
        int pos = -1;

        // Buscar letra en letras_detectadas
        for (int j = 0; j < 27; j++) {
            if (ch_may == alfabeto[j]) {
                pos = j;
                break;
            }
        }

        if (pos != -1) {
            // Reemplazar con letra según frecuencia del español
            char repl = frecuenciasEsp[pos];
            // Mantener mayúscula/minúscula
            texto_descifrado[i] = islower(ch) ? tolower(repl) : repl;
        } else {
            // No es letra (puntuación, espacios, números) => mantener
            texto_descifrado[i] = ch;
        }
    }
    texto_descifrado[idx] = '\0';

    printf("\nEl mensaje descifrado es:\n%s\n", texto_descifrado);
    
        char letra1, letra2;
    char opcion;

    do {
        printf("\n¿Deseas intercambiar dos letras en el mensaje descifrado?\n(0 para salir / intro para continuar): ");
        scanf(" %c", &opcion);

        if (opcion == '0') {
            break;
        }

        printf("Introduce la primera letra a intercambiar: ");
        scanf(" %c", &letra1);
        printf("Introduce la segunda letra a intercambiar: ");
        scanf(" %c", &letra2);

        // Convertir ambas letras a mayúscula para comparación uniforme
        char l1_may = toupper((unsigned char)letra1);
        char l2_may = toupper((unsigned char)letra2);

        for (int i = 0; texto_descifrado[i] != '\0'; i++) {
            char ch = texto_descifrado[i];
            char ch_may = toupper((unsigned char)ch);

            if (ch_may == l1_may) {
                texto_descifrado[i] = islower(ch) ? tolower(letra2) : toupper(letra2);
            } else if (ch_may == l2_may) {
                texto_descifrado[i] = islower(ch) ? tolower(letra1) : toupper(letra1);
            }
        }

        printf("\nTexto descifrado con las letras '%c' y '%c' intercambiadas:\n%s\n", letra1, letra2, texto_descifrado);

    } while (1); // Repite hasta que se elija salir


    return 0;
}

// Cuenta cuántas veces aparece un carácter (letra) en el archivo
int fichnumcarac(char c, FILE *fp) {
    char car;
    int cont = 0;

    while ((car = fgetc(fp)) != EOF) {
        // Convertir a mayúscula para comparar de forma consistente
        car = toupper((unsigned char)car);
        if (car == c) {
            cont++;
        }
    }
    
    return cont;
}

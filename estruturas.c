#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int ID;
    int NIF;
    char NOME[50];
    char MORADA[50];
    int CONTACTO;
} DONO;

typedef struct {
    int ID;
    char NOME[50];
    char ESPECIE[50];
    char RACA[50];
    char GENERO;
    float PESO;
    int ID_DONO;
} ANIMAL;

typedef struct {
    int ID;
    int NIF;
    char NOME[50];
    char ESPECIALIDADE[50];
    int CONTACTO;
    int HORAS_TRABALHADAS;
} VETERINARIO;

typedef struct {
    int ID;
    int TIPO_INTERVENCAO;
    int ID_VETERINARIO;
    int ID_ANIMAL;
    char DATA[20];
    float CUSTO;
} INTERVENCAO;

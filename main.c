#include <stdio.h>
#include <stdlib.h>
#include "estruturas.c"
#include <locale.h>
#include <time.h>

#define TAM_MAXLINHA 200
#define MAX_ANIMAIS 100
#define MAX_DONOS 100
#define MAX_INTERVENCOES 100
#define MAX_VETERINARIOS 2

float DETERMINARCUSTOTOTALINTERVENCOESDONO(INTERVENCAO listaIntervencoes[], int *num_intervencoes, ANIMAL listaAnimais[], int num_animais, int id_dono){
    int valor_total = 0;

    int* animais = DEVOLVEANIMAISDONOVETOR(listaAnimais, num_animais, id_dono);

    if(listaIntervencoes != NULL){
        for (int i = 0; i < num_intervencoes; i++)
        {
            for (int y = 0; y < sizeof(animais); y++)
            {
                if(listaIntervencoes[i].ID_ANIMAL == animais[y]){
                    valor_total += animais[y];
                }
            }
        }
    }

    return valor_total;
}

int* DEVOLVEANIMAISDONOVETOR(ANIMAL listaAnimais[], int num_animais, int id_dono){
    int* animaisDoDono = (int*)malloc(num_animais * sizeof(int));
    int aux = 0;

    if(listaAnimais != NULL){
        for (int i = 0; i < num_animais; i++)
        {
            if(listaAnimais[i].ID_DONO == id_dono){
                animaisDoDono[aux++] = listaAnimais[i].ID;
            }
        }
    }
    return animaisDoDono;
}

float DETERMINARCUSTOTOTALINTERVENCOESIDANIMAL(INTERVENCAO listaIntervencoes[], int *num_intervencoes, int id_animal){
    float valor_total = 0;

    if(listaIntervencoes != NULL){
        for (int i = 0; i < num_intervencoes; i++)
        {
            if(listaIntervencoes[i].ID_ANIMAL == id_animal){
                if(VERIFICARDATA(listaIntervencoes[i].DATA) == 1){
                    valor_total += listaIntervencoes[i].CUSTO;
                }
            }
        }
    }
    return valor_total;
}

int VERIFICARDATA(char *Data){
    time_t tempoAtual;
    struct tm *infoTempo;
    time(&tempoAtual);
    infoTempo = localtime(&tempoAtual);

    // Obter a data atual do sistema
    int diaAtual = infoTempo->tm_mday;
    int mesAtual = infoTempo->tm_mon + 1;
    int anoAtual = infoTempo->tm_year + 1900;

    int dia, mes, ano;
    sscanf(Data, "%d-%d-%d", &dia, &mes, &ano);

    if (ano < anoAtual || (ano == anoAtual && mes < mesAtual) || (ano == anoAtual && mes == mesAtual && dia < diaAtual)) {
        return 0;
    } else {
        return 1;
    }
}

void ELIMINARANIMAL(ANIMAL listaAnimais[], int *num_animais, int id_animal){
    if(listaAnimais != NULL && id_animal > 0){
        int posicao = DEVOLVEPOS(listaAnimais, *num_animais, id_animal);
    
        if(posicao != -1){
            for (int i = posicao; i < *num_animais - 1; i++)
            {
                listaAnimais[i] = listaAnimais[i + 1];
            }
            (*num_animais)--;

            FILE* ficheiro;
            ficheiro = fopen("animais.txt", "w");

            if(ficheiro != NULL){
                for (int y = 0; y < *num_animais; y++)
                {
                    fprintf(ficheiro, "\n%d\t %s\t %s\t %s\t %c\t %.2f\t %d", 
                    listaAnimais[y].ID, 
                    listaAnimais[y].NOME,
                    listaAnimais[y].ESPECIE,
                    listaAnimais[y].RACA, 
                    listaAnimais[y].GENERO, 
                    listaAnimais[y].PESO, 
                    listaAnimais[y].ID_DONO);
                }

                fclose(ficheiro);
                printf("\nAnimal removido com sucesso!\n");
                
            } else {
                printf("\nErro ao abrir ficheiro!\n");
                return;
            }
        } else {
            printf("Posição do animal não encontrado!");
        }
    }
}

int DEVOLVEPOS(ANIMAL listaAnimais[], int num_animais, int id_animal){
    if(listaAnimais != NULL && id_animal > 0){
        for (int i = 0; i < num_animais; i++)
        {
            if(listaAnimais[i].ID == id_animal){
                return i;
                break;
            }
        }
    }
    return -1;
}

void ADICIONARANIMAL(ANIMAL listaAnimais[], int *num_animais){
    ANIMAL novo_animal;
    FILE *ficheiro = fopen("animais.txt", "a");

    novo_animal.ID = DEVOLVEULTIMOID_ANIMAL(listaAnimais, num_animais) + 1;
    printf("Nome do animal: ");
    scanf("%s", novo_animal.NOME);
    printf("Espécie do animal: ");
    scanf("%s", novo_animal.ESPECIE);
    printf("Raça do animal: ");
    scanf("%s", novo_animal.RACA);
    printf("Gênero do animal (M/F): ");
    scanf(" %c", &novo_animal.GENERO);
    printf("Peso do animal: ");
    scanf("%f", &novo_animal.PESO);
    printf("ID do dono do animal: ");
    scanf("%d", &novo_animal.ID_DONO);

    listaAnimais[*num_animais] = novo_animal;
    (*num_animais)++;

    if(ficheiro == NULL) {
        printf("Erro ao abrir o ficheiro.");
        return;
    }

    if(fprintf(ficheiro, "\n%d\t %s\t %s\t %s\t %c\t %.2f\t %d", novo_animal.ID, novo_animal.NOME,novo_animal.ESPECIE, novo_animal.RACA, novo_animal.GENERO, novo_animal.PESO, novo_animal.ID_DONO)){
        printf("Animal adicionado com sucesso!");
    } else {
        printf("Erro ao adicionar o animal");
    }
    fclose(ficheiro);
}

void CARREGARDADOSINTERVENCOES(INTERVENCAO listaIntervencoes[], int *num_intervencoes){
    FILE *ficheiro;
    ficheiro = fopen("intervencoes.txt", "r");

    if(ficheiro == NULL){
        printf("Erro ao abrir o ficheiro.\n");
        return;
    }

    char linha[TAM_MAXLINHA];

    while(fgets(linha, TAM_MAXLINHA, ficheiro) != NULL){
        sscanf(linha, "%d %d %d %d %s %f",
        &listaIntervencoes[*num_intervencoes].ID,
        &listaIntervencoes[*num_intervencoes].TIPO_INTERVENCAO,
        &listaIntervencoes[*num_intervencoes].ID_VETERINARIO,
        &listaIntervencoes[*num_intervencoes].ID_ANIMAL,
        &listaIntervencoes[*num_intervencoes].DATA,
        &listaIntervencoes[*num_intervencoes].CUSTO);

        (*num_intervencoes)++;
    }

    fclose(ficheiro);
}

void AGENDARINTERVENCAO(VETERINARIO listaVeterinarios[], int num_veterinarios, INTERVENCAO listaIntervencoes[], int *num_intervencoes, ANIMAL listaAnimais[], int num_animais){
    FILE *ficheiro;
    ficheiro = fopen("intervencoes.txt", "a");
    INTERVENCAO nova_internvencao;

    int teste = OBTERIDULTIMAINTERVENCAO(listaIntervencoes, *num_intervencoes) + 1;

    nova_internvencao.ID = teste;
    int tipo_intervencao = 0, id_vet, id_ani;
    //1 - consulta \ 2 - vacinacao 3 - cirurgia
    printf("Tipo de intervenção\n1-Consulta\t2-Vacinacao\t3-Cirurgia\n");
    scanf("%d", &tipo_intervencao);

    if(tipo_intervencao < 1 || tipo_intervencao > 3){
        printf("\nErro! Tipo de intervenção inválida.\n");
        return;
    }

    nova_internvencao.TIPO_INTERVENCAO = tipo_intervencao;
    MOSTRAVETERINARIOS(listaVeterinarios, num_veterinarios);
    printf("\nDigite o id do veterinario: ");
    scanf("%d", &id_vet);

    if(VERIFICAR_ID_VETERINARIO(listaVeterinarios, num_veterinarios, id_vet) == 1){
        nova_internvencao.ID_VETERINARIO = id_vet;
    } else {
        printf("\nErro! ID de veterinario invalido.\n");
        return;
    }

    //POR RESOLVER

    MOSTRARDADOSANIMAIS(listaAnimais, num_animais);
    printf("\nDigite o id do animal: ");
    scanf("%d", &id_ani);

    if(VERIFICAR_ID_ANIMAL(listaAnimais, num_animais, id_ani) == 1){
        nova_internvencao.ID_ANIMAL = id_ani;
    } else {
        printf("\nErro! ID de animal invalido.\n");
        return;
    }

    printf("\nData (ex: 01-01-2023): ");
    scanf("%s", &nova_internvencao.DATA);

    printf("\nPreço (EX: 10.5): ");
    scanf("%f", &nova_internvencao.CUSTO);

    listaIntervencoes[*num_intervencoes] = nova_internvencao;
    (*num_intervencoes)++;

    if(ficheiro == NULL) {
        printf("Erro ao abrir ficheiro!\n");
        return;
    }

    if(fprintf(ficheiro, "\n%d\t %d\t %d\t %d\t %s\t %.2f", nova_internvencao.ID, nova_internvencao.TIPO_INTERVENCAO,nova_internvencao.ID_VETERINARIO, nova_internvencao.ID_ANIMAL, nova_internvencao.DATA, nova_internvencao.CUSTO)){
        printf("Intervencao marcada com sucesso!");
    } else {
        printf("Erro ao adicionar o intervencao");
    }
    fclose(ficheiro);
}

int VERIFICAR_ID_VETERINARIO(VETERINARIO listaVeterinarios[], int num_veterinarios, int id){
    int existe = 0;
    if(listaVeterinarios != NULL && num_veterinarios > 0){
        for (int i = 0; i < num_veterinarios; i++)
        {
            if(listaVeterinarios[i].ID == id);
            existe = 1;
        }
    }
    return existe;
}

int VERIFICAR_ID_ANIMAL(ANIMAL listaAnimal[], int num_animais, int id){
    int existe = 0;
    if(listaAnimal != NULL && num_animais > 0){
        for (int i = 0; i < num_animais; i++)
        {
            if(listaAnimal[i].ID == id);
            existe = 1;
        }
    }
    return existe;
}

void MOSTRAINTERVENCOES(INTERVENCAO listaIntervencoes[], int num_intervencoes){
    if(listaIntervencoes == NULL){
        printf("Erro ao listar veterinários\n");
    } else {
        printf("\nINTERVENCOES\n");
        printf("-------------------------------------------------------------\n");
        for (int i = 0; i < num_intervencoes; i++)
        {
            printf("%d\t%d\t%d\t%d\t%s\t%.2f\n",
            listaIntervencoes[i].ID,
            listaIntervencoes[i].TIPO_INTERVENCAO,
            listaIntervencoes[i].ID_VETERINARIO,
            listaIntervencoes[i].ID_ANIMAL,
            listaIntervencoes[i].DATA,
            listaIntervencoes[i].CUSTO);
        }
         printf("\n-------------------------------------------------------------\n");
    }
}

int OBTERIDULTIMAINTERVENCAO(INTERVENCAO listaIntervencoes[], int num_intervencoes){
    int ultimo_id = 0;
    if(listaIntervencoes != NULL && num_intervencoes > 0){
         for (int i = 0; i < num_intervencoes; i++)
        {
            ultimo_id = listaIntervencoes[i].ID;
        }
    }
    return ultimo_id;
}

void ADICIONARVETERINARIO(VETERINARIO listaVeterinarios[], int *num_veterinarios){
    int quantia = *num_veterinarios;

    if(quantia < 2) {
        VETERINARIO novo_veterinario;
        int NIF, CONTACTO, aux = 0;

        novo_veterinario.ID = DEVOLVEULTIMOID_ANIMAL(listaVeterinarios, quantia) + 1;
        printf("\nNIF do Veterinario: ");
        scanf("%d", &NIF);

        aux = VERIFICANIF(listaVeterinarios, quantia, NIF);
        if(aux == 1){
            printf("Erro! NIF em utilização.");
            return;
        }
        else {
            novo_veterinario.NIF = NIF;
        }

        printf("\nNome do Veterinario: ");
        scanf("%s", &novo_veterinario.NOME);

        printf("\nEspecialidade do Veterinario: ");
        scanf("%s", &novo_veterinario.ESPECIALIDADE);

        printf("\nContacto telefonico do Veterinario: ");
        scanf("%d", &novo_veterinario.CONTACTO);

        novo_veterinario.HORAS_TRABALHADAS = 0;

        listaVeterinarios[*num_veterinarios] = novo_veterinario;
        (*num_veterinarios)++;

        FILE *ficheiro = fopen("veterinarios.txt", "a");
        if(ficheiro == NULL) {
            printf("Erro ao abrir o ficheiro.");
            return;
        }


        if(fprintf(ficheiro, "\n%d\t %d\t %s\t %s\t %d\t %d", novo_veterinario.ID, novo_veterinario.NIF, novo_veterinario.NOME, novo_veterinario.ESPECIALIDADE, novo_veterinario.CONTACTO, novo_veterinario.HORAS_TRABALHADAS)){
            printf("Veterinario adicionado com sucesso!");
        } else {
            printf("Erro ao adicionar o animal");
            return;
        }
        fclose(ficheiro);
    } else {
        printf("Erro! Não é possível adicionar mais veterinários (Limite de %d alcançado!)", MAX_VETERINARIOS);
        return;
    }
}

int VERIFICANIF(VETERINARIO listaVeterinarios[], int num_veterinarios, int NIF){
    int usado = 0;
    if(listaVeterinarios != NULL && num_veterinarios > 0){
        for (int i = 0; i < num_veterinarios; i++)
        {
            if(listaVeterinarios[i].NIF == NIF){
                usado = 1;
            }
        }
    }
    return usado;
}

void CARREGARDADOSVETERINARIOS(VETERINARIO *veterinarios, int *num_veterinarios){
    FILE *ficheiro;
    ficheiro = fopen("veterinarios.txt", "r");

    if(ficheiro == NULL){
        printf("Erro ao abrir o ficheiro.\n");
        return;
    }

    char linha[TAM_MAXLINHA];

    while(fgets(linha, TAM_MAXLINHA, ficheiro) != NULL){
        sscanf(linha, "%d %d %s %s %d",
        &veterinarios[*num_veterinarios].ID,
        &veterinarios[*num_veterinarios].NIF,
        &veterinarios[*num_veterinarios].NOME,
        &veterinarios[*num_veterinarios].ESPECIALIDADE,
        &veterinarios[*num_veterinarios].CONTACTO,
        &veterinarios[*num_veterinarios].HORAS_TRABALHADAS);

        (*num_veterinarios)++;
    }

    fclose(ficheiro);
}

void MOSTRAVETERINARIOS(VETERINARIO veterinarios[], int num_veterinarios){
    if(veterinarios == NULL){
        printf("\nErro ao listar veterinarios\n");
    } else {
        printf("\nVETERINARIOS\n");
        printf("-------------------------------------------------------------\n");
        for (int i = 0; i < num_veterinarios; i++)
        {
            printf("\nVeterinario ID %d\n");
            printf("%d\t%d\t%s\t%s\t%d\t%d",
            veterinarios[i].ID,
            veterinarios[i].NIF,
            veterinarios[i].NOME,
            veterinarios[i].ESPECIALIDADE,
            veterinarios[i].CONTACTO,
            veterinarios[i].HORAS_TRABALHADAS);
        }
        printf("\n-------------------------------------------------------------\n");
    }
}

void CARREGARDADOSDONOS(DONO *donos, int *num_donos){
    FILE *ficheiro;
    ficheiro = fopen("donos.txt", "r");

    if(ficheiro == NULL){
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[TAM_MAXLINHA];

    while(fgets(linha, TAM_MAXLINHA, ficheiro) != NULL){
        sscanf(linha, "%d %d %s %s %d",
        &donos[*num_donos].ID,
        &donos[*num_donos].NIF,
        &donos[*num_donos].NOME,
        &donos[*num_donos].MORADA,
        &donos[*num_donos].CONTACTO);

        (*num_donos)++;
    }

    fclose(ficheiro);
}

void LISTAANIMAISDONOS(ANIMAL listaAnimais[], DONO listaDonos[], int num_animais, int num_donos){

    printf("\ANIMAIS E RESPETIVOS DONOS\n");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < num_donos; i++)
    {
        printf("ID Dono %d\n", listaDonos[i].ID);

        int quantiaAnimais = QUANTIAANIMAIS(listaAnimais, num_animais, listaDonos[i].ID);

        if(quantiaAnimais > 0){
            for (int x = 0; x < num_animais; x++)
            {

                if(listaAnimais[x].ID_DONO == listaDonos[i].ID){
                    
                    printf("%d\t %s\t %s\t %s\t %c\t %f\t %d\n", 
                    listaAnimais[x].ID, 
                    listaAnimais[x].NOME,
                    listaAnimais[x].ESPECIE,
                    listaAnimais[x].RACA,
                    listaAnimais[x].GENERO,
                    listaAnimais[x].PESO,
                    listaAnimais[x].ID_DONO);
                }
            } 
        } else {
            printf("Não tem qualquer animal registado.");
        }
    }
    printf("-------------------------------------------------------------\n");
}

int DEVOLVEULTIMOID_ANIMAL(ANIMAL listaAnimais[], int num_animais){
    int ultimoid = 0;

    if(listaAnimais != NULL && num_animais > 0){
        for (int i = 0; i < num_animais; i++)
        {
           ultimoid = listaAnimais[i].ID;
        }
    }
    return ultimoid;
}

int DEVOLVEULTIMOID_VETERINARIO(VETERINARIO listaVeterinarios[], int num_veterinarios){
    int ultimoid = 0;

    if(listaVeterinarios != NULL && num_veterinarios > 0){
        for (int i = 0; i < num_veterinarios; i++)
        {
           ultimoid = listaVeterinarios[i].ID;
        }
    }
    return ultimoid;
}

int QUANTIAANIMAIS(ANIMAL listaAnimais[], int num_animais, int id){
    int quantia = 0; 
    for (int i = 0; i < num_animais; i++)
    {
        if(listaAnimais[i].ID_DONO == id){
            quantia++;
        }
    }
    return quantia;
}

void CARREGARDADOSANIMAIS(ANIMAL *animais, int *num_animais) {
    FILE *ficheiro;
    ficheiro = fopen("animais.txt", "r");

    if (ficheiro == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[TAM_MAXLINHA];
    while (fgets(linha, TAM_MAXLINHA, ficheiro) != NULL) {
        sscanf(linha, "%d %s %s %s %c %f %d", 
        &animais[*num_animais].ID, 
        animais[*num_animais].NOME,
        animais[*num_animais].ESPECIE, 
        &animais[*num_animais].RACA, 
        &animais[*num_animais].GENERO, 
        &animais[*num_animais].PESO,
        &animais[*num_animais].ID_DONO);
        
        (*num_animais)++;
    }

    fclose(ficheiro);
}

void MOSTRARDADOSANIMAIS(ANIMAL animais[], int num_animais){
    printf("\ANIMAIS\n");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < num_animais; i++) {
        printf("%d\t %s\t %s\t %s\t %c\t %.2f\t %d\n",
               animais[i].ID, 
               animais[i].NOME, 
               animais[i].ESPECIE, 
               animais[i].RACA,
               animais[i].GENERO,
               animais[i].PESO, 
               animais[i].ID_DONO);
    }
    printf("-------------------------------------------------------------\n");
}

void MOSTRARDADOSDONOS(DONO donos[], int num_donos){
    printf("\DONOS\n");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < num_donos; i++) {
        printf("\n%d\t %s\t %s\t %d\n",
               donos[i].NIF, 
               donos[i].NOME, 
               donos[i].MORADA, 
               donos[i].CONTACTO);
    }
    printf("-------------------------------------------------------------\n");
}

int main() {
    system("cls");

    ANIMAL animais[MAX_ANIMAIS];
    DONO donos[MAX_DONOS];
    VETERINARIO veterinarios[MAX_VETERINARIOS];
    INTERVENCAO intervencoes[MAX_INTERVENCOES];

    int num_donos;
    int num_animais;
    int num_veterinarios;
    int num_intervencoes;

    int teste;
    
    CARREGARDADOSANIMAIS(animais, &num_animais);
    CARREGARDADOSVETERINARIOS(veterinarios, &num_veterinarios);
    CARREGARDADOSDONOS(donos, &num_donos);
    CARREGARDADOSINTERVENCOES(intervencoes, &num_intervencoes);
    /*

    MOSTRARDADOSANIMAIS(animais, num_animais);

    printf("\n");

    MOSTRARDADOSDONOS(donos, num_donos);


   LISTAANIMAISDONOS(animais, donos, num_animais, num_donos);

   MOSTRAVETERINARIOS(veterinarios, num_veterinarios);*/

   //MOSTRARDADOSANIMAIS(animais, num_animais);

   //ADICIONARANIMAL(animais, &num_animais);

   //MOSTRARDADOSANIMAIS(animais, num_animais);

   //ADICIONARVETERINARIO(veterinarios, &num_veterinarios);

   //MOSTRAVETERINARIOS(veterinarios, num_veterinarios);

   //MOSTRAINTERVENCOES(intervencoes, num_intervencoes);

   //AGENDARINTERVENCAO(veterinarios, &num_veterinarios, intervencoes, &num_intervencoes, animais, &num_animais);
   //MOSTRAINTERVENCOES(intervencoes, num_intervencoes);

/*
    MOSTRARDADOSANIMAIS(animais, num_animais);
    printf("\nDigite o id do animal que deseja remover: ");
    scanf("%d", &teste);

    ELIMINARANIMAL(animais, &num_animais, teste);

    printf("\n");

    MOSTRARDADOSANIMAIS(animais, num_animais);*/

    

    MOSTRAINTERVENCOES(intervencoes, num_intervencoes);
    /*
    printf("\nDigite o id do animal que deseja ver o custo de futuras intervencoes: ");
    scanf("%d", &teste);
    float valor_total = DETERMINARCUSTOTOTALINTERVENCOESIDANIMAL(intervencoes, num_intervencoes, teste);

    printf("\nValor total: %.2f euros", valor_total);*/

    printf("\nDigite o id do dono que deseja ver o custo de futuras intervencoes: ");
    scanf("%d", &teste);

    float valortotal = DETERMINARCUSTOTOTALINTERVENCOESDONO(intervencoes, num_intervencoes, animais, num_animais, teste);
    printf("\n%f", valortotal);

    return 0;
}

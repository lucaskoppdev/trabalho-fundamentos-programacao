#include <stdio.h>
#include <string.h>

#define MAX_DISCENTES 100
#define TOTAL_AULAS   30

/* ------------------------------------------------------------------ */
/*  Estrutura de dados                                                  */
/* ------------------------------------------------------------------ */
struct discente {
    int   matricula;
    char  nome[100];
    int   num_presencas;
    float notas[3];
};

/* ------------------------------------------------------------------ */
/*  Variáveis globais                                                   */
/* ------------------------------------------------------------------ */
struct discente lista[MAX_DISCENTES];
int num_discentes = 0;

/* ------------------------------------------------------------------ */
/*  Funções auxiliares                                                  */
/* ------------------------------------------------------------------ */

/* Retorna o índice do discente com a matrícula dada, ou -1 se não
   encontrado. */
int buscar_por_matricula(int matricula) {
    for (int i = 0; i < num_discentes; i++) {
        if (lista[i].matricula == matricula)
            return i;
    }
    return -1;
}

/* ------------------------------------------------------------------ */
/*  1. Listar discentes                                                 */
/* ------------------------------------------------------------------ */
void listar_discentes(void) {
    if (num_discentes == 0) {
        printf("\nNao existem discentes cadastrados.\n");
        return;
    }

    printf("\n%-10s %-30s %-12s %-8s %-8s %-8s\n",
           "Matricula", "Nome", "Presencas", "Nota 1", "Nota 2", "Nota 3");
    printf("%s\n", "--------------------------------------------------------------------------");

    for (int i = 0; i < num_discentes; i++) {
        printf("%-10d %-30s %-12d %-8.2f %-8.2f %-8.2f\n",
               lista[i].matricula,
               lista[i].nome,
               lista[i].num_presencas,
               lista[i].notas[0],
               lista[i].notas[1],
               lista[i].notas[2]);
    }
}

/* ------------------------------------------------------------------ */
/*  2. Cadastrar discente                                               */
/* ------------------------------------------------------------------ */
void cadastrar_discente(void) {
    if (num_discentes >= MAX_DISCENTES) {
        printf("\nLimite maximo de discentes atingido (%d).\n", MAX_DISCENTES);
        return;
    }

    int matricula;
    char nome[100];

    printf("\nNumero de matricula: ");
    scanf("%d", &matricula);
    getchar(); /* consome o '\n' residual */

    if (buscar_por_matricula(matricula) != -1) {
        printf("Erro: ja existe um discente com a matricula %d.\n", matricula);
        return;
    }

    printf("Nome: ");
    fgets(nome, sizeof(nome), stdin);
    /* Remove o '\n' inserido pelo fgets */
    nome[strcspn(nome, "\n")] = '\0';

    lista[num_discentes].matricula    = matricula;
    strcpy(lista[num_discentes].nome, nome);
    lista[num_discentes].num_presencas = 0;
    lista[num_discentes].notas[0]     = 0.0f;
    lista[num_discentes].notas[1]     = 0.0f;
    lista[num_discentes].notas[2]     = 0.0f;

    num_discentes++;

    printf("Discente \"%s\" cadastrado com sucesso!\n", nome);
}

/* ------------------------------------------------------------------ */
/*  3. Atualizar notas                                                  */
/* ------------------------------------------------------------------ */
void atualizar_notas(void) {
    int matricula;
    printf("\nNumero de matricula: ");
    scanf("%d", &matricula);

    int idx = buscar_por_matricula(matricula);
    if (idx == -1) {
        printf("Discente nao encontrado.\n");
        return;
    }

    printf("Digite as 3 notas de %s:\n", lista[idx].nome);
    for (int i = 0; i < 3; i++) {
        printf("  Nota %d: ", i + 1);
        scanf("%f", &lista[idx].notas[i]);
    }

    printf("Notas de \"%s\" atualizadas com sucesso!\n", lista[idx].nome);
}

/* ------------------------------------------------------------------ */
/*  4. Atualizar frequência                                             */
/* ------------------------------------------------------------------ */
void atualizar_frequencia(void) {
    int matricula;
    printf("\nNumero de matricula: ");
    scanf("%d", &matricula);

    int idx = buscar_por_matricula(matricula);
    if (idx == -1) {
        printf("Discente nao encontrado.\n");
        return;
    }

    lista[idx].num_presencas++;
    printf("Presenca registrada para \"%s\" (total: %d).\n",
           lista[idx].nome, lista[idx].num_presencas);
}

/* ------------------------------------------------------------------ */
/*  5. Remover discente                                                 */
/* ------------------------------------------------------------------ */
void remover_discente(void) {
    int matricula;
    printf("\nNumero de matricula: ");
    scanf("%d", &matricula);

    int idx = buscar_por_matricula(matricula);
    if (idx == -1) {
        /* Não faz nada conforme especificado */
        return;
    }

    /* Desloca os elementos seguintes uma posição para a esquerda */
    for (int i = idx; i < num_discentes - 1; i++) {
        lista[i] = lista[i + 1];
    }
    num_discentes--;

    printf("Discente removido com sucesso.\n");
}

/* ------------------------------------------------------------------ */
/*  6. Imprimir relatório                                               */
/* ------------------------------------------------------------------ */
void imprimir_relatorio(void) {
    int aprovados_nota  = 0;
    int reprovados_falta = 0;

    for (int i = 0; i < num_discentes; i++) {
        float media = (lista[i].notas[0] + lista[i].notas[1] + lista[i].notas[2]) / 3.0f;
        float frequencia = (lista[i].num_presencas / (float)TOTAL_AULAS) * 100.0f;

        if (media >= 7.0f)
            aprovados_nota++;

        if (frequencia < 75.0f)
            reprovados_falta++;
    }

    printf("\n=== Relatorio ===\n");
    printf("Total de discentes cadastrados : %d\n", num_discentes);
    printf("Aprovados por nota (media >= 7): %d\n", aprovados_nota);
    printf("Reprovados por falta (freq < 75%%): %d\n", reprovados_falta);
}

/* ------------------------------------------------------------------ */
/*  Menu principal                                                      */
/* ------------------------------------------------------------------ */
int main(void) {
    printf("=========================================\n");
    printf("   Bem-vindo ao Sistema de Gestao Escolar\n");
    printf("=========================================\n");

    int opcao;

    do {
        printf("\n--- Menu Principal ---\n");
        printf("1. Listar discentes\n");
        printf("2. Cadastrar discente\n");
        printf("3. Atualizar notas\n");
        printf("4. Atualizar frequencia\n");
        printf("5. Remover discente\n");
        printf("6. Imprimir relatorio\n");
        printf("7. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: listar_discentes();    break;
            case 2: cadastrar_discente();  break;
            case 3: atualizar_notas();     break;
            case 4: atualizar_frequencia(); break;
            case 5: remover_discente();    break;
            case 6: imprimir_relatorio();  break;
            case 7: break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 7);

    printf("\nAte logo!\n");
    return 0;
}
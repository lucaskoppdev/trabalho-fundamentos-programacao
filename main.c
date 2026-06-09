//importa as bibliotecas padroes necessarias para o funcionamento do codigo
//standard input output -> biblioteca para ler entradasa e mandar saidas
#include <stdio.h>
//biblioteca para manipular strings (cadeias de caracteres) de maneira mais dinamica
#include <string.h>

//MACROS -> pre-processador vai ler eles, e substituir automaticamente em tempo de pre-processamento
//Funcionam igual variaveis, mas a nivel de pre-processador
#define MAX_DISCENTES 100
#define TOTAL_AULAS 30

//a struct cria um tipo de dado novo, que armazena N valores num "mesmo endereco"
//funciona semelhante a um "formulario", uma unica variavel recebe K parametros
struct discente {
    int   matricula;
    char  nome[100];
    int   num_presencas;
    float notas[3];
};

//Aloca na memoria, N espacos do tipo discente
//nesse caso, o numero de espeacos alocados é o nosso macro de max discentes, ou seja 100
struct discente discentes[MAX_DISCENTES];

//variavel que vai armazenar o numero de discentes que foram cadastrados em tempo de execucao
//Ou seja, vai servir como variavel de controle enquanto o codigo roda
int num_discentes = 0;


//funcao para buscar os discentes que ja foram cadastrados, usando como referencial a matricula
//recebe como parametro a matricula do discente
//busca dentre os discentes ja cadastrados, e retorna as infos dele caso encontre
//do contrario, acusa que o discente nao existe e nao retorna nada
int buscar_por_matricula(int matricula_buscar) {
    for (int i = 0; i < num_discentes; i++) {
        if (discentes[i].matricula == matricula_buscar)
            return i;
    }
    return -1;
}

//========>FIM DA PARTE DO VITOR<==============
//========>COMECO DA PARTE DO LUCAS<=========

//lista todos os discentes que ja foram cadastrados
void listar_discentes(void) {
    if (num_discentes == 0) {
        printf("\nNao existem discentes cadastrados.\n");
        return;
    }
    //formata os dados em formato de tabela, alinhando a esquerda
    printf("\n%-10s %-30s %-12s %-8s %-8s %-8s\n",
           "Matricula", "Nome", "Presencas", "Nota 1", "Nota 2", "Nota 3");
    printf("%s\n", "--------------------------------------------------------------------------");

    for (int i = 0; i < num_discentes; i++) {
        printf("%-10d %-30s %-12d %-8.2f %-8.2f %-8.2f\n",
               discentes[i].matricula,
               discentes[i].nome,
               discentes[i].num_presencas,
               discentes[i].notas[0],
               discentes[i].notas[1],
               discentes[i].notas[2]);
    }
}


//funcao para cadastrar os discentes 
void cadastrar_discente(void) {
    //estrutura de controle para evitar memory leaks ou out of scope
    if (num_discentes >= MAX_DISCENTES) {
        printf("\nLimite maximo de discentes atingido (%d).\n", MAX_DISCENTES);
        return;
    }

    int matricula;
    char nome[100];

    printf("\nMatricula: ");
    scanf("%d", &matricula);
    
    //estrutura de controle para mitigar cadastro de matriculas != 0
    if (matricula <= 0) {
        printf("Erro: matricula deve ser um numero positivo.\n");
        return;
    }
    getchar(); // tira o '\n' residual

    //busca no banco se a matricula ja pertence a outra pessoa, mitigando duplicatas
    if (buscar_por_matricula(matricula) != -1) {
        printf("Erro: ja existe um discente com a matricula %d.\n", matricula);
        return;
    }

    //nome do discente
    printf("Nome: ");

    //pega o nome do usuario, limitando ao sizeof do nome
    fgets(nome, sizeof(nome), stdin);

    //substitui o \n
    nome[strcspn(nome, "\n")] = '\0';

    //cadastra o novo discente no "banco"
    discentes[num_discentes].matricula = matricula;
    //insere o nome dentro do struct
    strcpy(discentes[num_discentes].nome, nome);
    //numero de presencas
    discentes[num_discentes].num_presencas = 0;
    
    //add a nota formato em 1 casa decimal
    discentes[num_discentes].notas[0] = 0.0f;
    discentes[num_discentes].notas[1] = 0.0f;
    discentes[num_discentes].notas[2] = 0.0f;

    num_discentes++;

    printf("Discente \"%s\" cadastrado com sucesso!\n", nome);
}

//modificar os valores da nota de um aluno ja cadastrado
void atualizar_notas(void) {
    int matricula;
    printf("\nNumero de matricula: ");
    scanf("%d", &matricula);

    int idx = buscar_por_matricula(matricula);
    if (idx == -1) {
        printf("Discente nao encontrado.\n");
        return;
    }

    printf("Digite as 3 notas de %s:\n", discentes[idx].nome);
    for (int i = 0; i < 3; i++) {
        printf("  Nota %d: ", i + 1);
        scanf("%f", &discentes[idx].notas[i]);
    }

    printf("Notas de \"%s\" atualizadas com sucesso!\n", discentes[idx].nome);
}

//============>FIM DA PARTE DO LUCAS<=============
//============> COMECO DA PARTE DO AUGUSTO<=============

//atualizar a frequencia de um discente ja cadastrado
void atualizar_frequencia(void) {
    int matricula;
    printf("\nNumero de matricula: ");
    scanf("%d", &matricula);

    int idx = buscar_por_matricula(matricula);
    if (idx == -1) {
        printf("Discente nao encontrado.\n");
        return;
    }

    discentes[idx].num_presencas++;
    printf("Presenca registrada para \"%s\" (total: %d).\n",
           discentes[idx].nome, discentes[idx].num_presencas);
}


//remove um discente cadastrado do banco de dados
void remover_discente(void) {
    int matricula;
    printf("\nNumero de matricula: ");
    scanf("%d", &matricula);

    int idx = buscar_por_matricula(matricula);
    if (idx == -1) {

        return;
    }

    //reposicionando os elementos do vetor de discentes depois de remover um deles
    for (int i = idx; i < num_discentes - 1; i++) {
        discentes[i] = discentes[i + 1];
    }
    num_discentes--;

    printf("Discente removido com sucesso.\n");
}


//imprime o relatorio dos aprovados/reprovados por nota e falta
void imprimir_relatorio(void) {
    int aprovados_nota  = 0;
    int reprovados_falta = 0;

    for (int i = 0; i < num_discentes; i++) {
        float media = (discentes[i].notas[0] + discentes[i].notas[1] + discentes[i].notas[2]) / 3.0f;
        float frequencia = (discentes[i].num_presencas / (float)TOTAL_AULAS) * 100.0f;

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

//==================>FIM DA PARTE DO AUGUSTO<=================
///=================>ULTIMA PARTE DO VITOR<====================

//Loop Main do codigo, que vai efetivamente chamar as funcoes
int main(void) {

    //imrpime na tela um bem-vindo, na primeira interacao com o usuario
    printf("==========>Bem-vindo ao Sistema de Gestao Escolar<==========\n");

    //variavel para armazenar qual foi a escolha do usuario
    int opcao;

    //Do while para fazer o loop principal do codigo
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

        //switch case para chamar a funcao correta de acordo com a escolha do usuario
        //caso uma opcao invalida seja requisitada, executa novamente
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
#include <stdio.h>
#include <string.h>

#define VENDAS_DIA 50
#define MESES_ANO 12

// Estrutura para armazenar as informacoes de cada venda
typedef struct {
    char nome_cliente[100];
    char nome_produto[100];
    float preco;
    int quantidade;
    float valor_total;
    int num_devolucoes;
    int dia;
    int mes;
} Venda;

// Prototipos das funcoes para modularizacao
float calcularTotalItem(float preco, int quantidade);
void cadastrarVenda(Venda vendas[], int *total_cadastrado);
void registrarDevolucao(Venda vendas[], int total_cadastrado);
void relatorioDiario(Venda vendas[], int total_cadastrado);
void relatorioMensal(Venda vendas[], int total_cadastrado, float faturamento_mensal[]);
void ordenarMesesPorFaturamento(float faturamento[], int indices[]);
void relatorioAnual(Venda vendas[], int total_cadastrado, float faturamento_mensal[]);
void menuPrincipal();

// Funcao principal
int main() {
    Venda vendas[VENDAS_DIA];
    float faturamento_mensal[MESES_ANO] = {0.0};
    int total_cadastrado = 0;
    int opcao;

    do {
        menuPrincipal();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        switch(opcao) {
            case 1:
                cadastrarVenda(vendas, &total_cadastrado);
                break;
            case 2:
                registrarDevolucao(vendas, total_cadastrado);
                break;
            case 3:
                relatorioDiario(vendas, total_cadastrado);
                break;
            case 4:
                relatorioMensal(vendas, total_cadastrado, faturamento_mensal);
                break;
            case 5:
                relatorioAnual(vendas, total_cadastrado, faturamento_mensal);
                break;
            case 6:
                printf("\nEcoStore: Obrigado por apoiar o consumo sustentavel!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while(opcao != 6);

    return 0;
}

// Exibe o menu na tela
void menuPrincipal() {
    printf("\n================ ECOSTORE ================\n");
    printf("1. Registrar Venda\n");
    printf("2. Registrar Devolucao\n");
    printf("3. Relatorio Diario\n");
    printf("4. Relatorio Mensal\n");
    printf("5. Relatorio Anual\n");
    printf("6. Encerrar Programa\n");
    printf("==========================================\n");
}

// Calcula o valor bruto da venda de um item
float calcularTotalItem(float preco, int quantidade) {
    return preco * quantidade;
}

// Cadastra uma nova venda respeitando o limite de 50 por dia
void cadastrarVenda(Venda vendas[], int *total_cadastrado) {
    if (*total_cadastrado >= VENDAS_DIA) {
        printf("\nLimite maximo de %d vendas para o dia de hoje atingido!\n", VENDAS_DIA);
        return;
    }

    Venda nova_venda;

    printf("\n--- Registrar Venda (%d/%d) ---\n", *total_cadastrado + 1, VENDAS_DIA);
    printf("Nome do Cliente: ");
    fgets(nova_venda.nome_cliente, sizeof(nova_venda.nome_cliente), stdin);
    nova_venda.nome_cliente[strcspn(nova_venda.nome_cliente, "\n")] = '\0'; // Remove o \n

    printf("Nome do Produto Sustentavel: ");
    fgets(nova_venda.nome_produto, sizeof(nova_venda.nome_produto), stdin);
    nova_venda.nome_produto[strcspn(nova_venda.nome_produto, "\n")] = '\0';

    printf("Preco Unitario (R$): ");
    scanf("%f", &nova_venda.preco);
    
    printf("Quantidade Comprada: ");
    scanf("%d", &nova_venda.quantidade);
    
    printf("Dia da venda (1-31): ");
    scanf("%d", &nova_venda.dia);
    
    printf("Mes da venda (1-12): ");
    scanf("%d", &nova_venda.mes);

    nova_venda.valor_total = calcularTotalItem(nova_venda.preco, nova_venda.quantidade);
    nova_venda.num_devolucoes = 0; // Inicializa sem devolucoes

    vendas[*total_cadastrado] = nova_venda;
    (*total_cadastrado)++;

    printf("\nVenda registrada com sucesso!\n");
}

// Registra a devolucao e aplica taxa se for reincidente
void registrarDevolucao(Venda vendas[], int total_cadastrado) {
    if (total_cadastrado == 0) {
        printf("\nNenhuma venda registrada no sistema ainda.\n");
        return;
    }

    char cliente_busca[100];
    char produto_busca[100];
    int encontrado = -1;

    printf("\n--- Registrar Devolucao ---\n");
    printf("Digite o nome do cliente: ");
    fgets(cliente_busca, sizeof(cliente_busca), stdin);
    cliente_busca[strcspn(cliente_busca, "\n")] = '\0';

    printf("Digite o nome do produto a ser devolvido: ");
    fgets(produto_busca, sizeof(produto_busca), stdin);
    produto_busca[strcspn(produto_busca, "\n")] = '\0';

    // Busca a venda correspondente no array do dia
    for (int i = 0; i < total_cadastrado; i++) {
        if (strcmp(vendas[i].nome_cliente, cliente_busca) == 0 && 
            strcmp(vendas[i].nome_produto, produto_busca) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado != -1) {
        vendas[encontrado].num_devolucoes++;
        printf("\nDevolucao detectada! Ocorrencia(s) deste item: %d\n", vendas[encontrado].num_devolucoes);
        
        if (vendas[encontrado].num_devolucoes >= 2) {
            printf("[ALERTA ODS 12 - Desperdicio de Transporte]\n");
            printf("Segunda devolucao deste mesmo item. Aplicada taxa de logistica reversa de R$ 20.00.\n");
            vendas[encontrado].valor_total += 20.00;
        } else {
            printf("Devolucao registrada sem custos adicionais de frete.\n");
        }
    } else {
        printf("\nVenda nao localizada com os dados informados.\n");
    }
}

// Gera o relatorio das vendas de um dia especifico escolhido pelo usuario
void relatorioDiario(Venda vendas[], int total_cadastrado) {
    if (total_cadastrado == 0) {
        printf("\n================ RELATORIO DIARIO ================\n");
        printf("Nenhuma venda cadastrada no sistema ainda.\n");
        printf("==================================================\n");
        return;
    }

    int dia_busca, mes_busca;
    int vendas_encontradas = 0;
    float total_dia = 0.0;

    // Solicita o dia e mes para filtrar o relatorio
    printf("\nDigite o dia para o relatorio (1-31): ");
    scanf("%d", &dia_busca);
    printf("Digite o mes para o relatorio (1-12): ");
    scanf("%d", &mes_busca);

    printf("\n================ RELATORIO DIARIO: %02d/%02d ================\n", dia_busca, mes_busca);

    for (int i = 0; i < total_cadastrado; i++) {
        // Verifica se a venda corresponde ao dia e mes selecionados
        if (vendas[i].dia == dia_busca && vendas[i].mes == mes_busca) {
            printf("Cliente: %s | Produto: %s | Qtd: %d | Total: R$ %.2f (Devolucoes: %d)\n",
                   vendas[i].nome_cliente, vendas[i].nome_produto, 
                   vendas[i].quantidade, vendas[i].valor_total, vendas[i].num_devolucoes);
            total_dia += vendas[i].valor_total;
            vendas_encontradas++;
        }
    }

    if (vendas_encontradas == 0) {
        printf("Nenhuma venda realizada nesta data.\n");
    } else {
        printf("--------------------------------------------------\n");
        printf("VALOR TOTAL VENDIDO NO DIA: R$ %.2f\n", total_dia);
    }
    printf("==================================================\n");
}

// Consolida e exibe os dados de um mes especifico escolhido pelo usuario
void relatorioMensal(Venda vendas[], int total_cadastrado, float faturamento_mensal[]) {
    if (total_cadastrado == 0) {
        printf("\n================ RELATORIO MENSAL ================\n");
        printf("Nenhuma venda cadastrada no sistema ainda.\n");
        printf("==================================================\n");
        return;
    }

    int mes_busca;
    int qtd_vendas_mes = 0;
    int qtd_devolucoes_mes = 0;
    float total_mes = 0.0;
    char *nomes_meses[] = {"Janeiro", "Fevereiro", "Marco", "Abril", "Maio", "Junho", 
                           "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"};

    // Solicita o mes desejado
    printf("\nDigite o numero do mes para o relatorio (1-12): ");
    scanf("%d", &mes_busca);

    // Validacao simples do mes digitado
    if (mes_busca < 1 || mes_busca > 12) {
        printf("\nMes invalido! Retornando ao menu.\n");
        return;
    }

    // Varre o array acumulando os dados do mes escolhido
    for (int i = 0; i < total_cadastrado; i++) {
        if (vendas[i].mes == mes_busca) {
            total_mes += vendas[i].valor_total;
            qtd_vendas_mes += vendas[i].quantidade;
            qtd_devolucoes_mes += vendas[i].num_devolucoes;
        }
    }

    printf("\n================ RELATORIO MENSAL: %s ================\n", nomes_meses[mes_busca - 1]);
    printf("QUANTIDADE DE ITENS VENDIDOS: %d\n", qtd_vendas_mes);
    printf("TOTAL DE OCORRENCIAS DE DEVOLUCAO: %d\n", qtd_devolucoes_mes);
    printf("--------------------------------------------------\n");
    printf("FATURAMENTO TOTAL DO MES: R$ %.2f\n", total_mes);
    printf("==================================================\n");
}

// Bubble Sort para ordenar o faturamento de forma decrescente
void ordenarMesesPorFaturamento(float faturamento[], int indices[]) {
    // Inicializa o array auxiliar de indices (0 a 11)
    for (int i = 0; i < MESES_ANO; i++) {
        indices[i] = i;
    }

    // Algoritmo Bubble Sort aplicado em cima de uma copia ou arranjo dos dados
    for (int i = 0; i < MESES_ANO - 1; i++) {
        for (int j = 0; j < MESES_ANO - i - 1; j++) {
            // Condicao de ordenacao decrescente baseada no valor do faturamento
            if (faturamento[indices[j]] < faturamento[indices[j + 1]]) {
                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }
}

// Gera o relatorio anual exibindo faturamento ordenado de forma decrescente
void relatorioAnual(Venda vendas[], int total_cadastrado, float faturamento_mensal[]) {
    // Atualiza o faturamento consolidado primeiro
    for (int m = 0; m < MESES_ANO; m++) {
        faturamento_mensal[m] = 0.0;
    }
    float total_anual = 0.0;
    for (int i = 0; i < total_cadastrado; i++) {
        int m = vendas[i].mes - 1;
        if (m >= 0 && m < MESES_ANO) {
            faturamento_mensal[m] += vendas[i].valor_total;
            total_anual += vendas[i].valor_total;
        }
    }

    int indices_ordenados[MESES_ANO];
    ordenarMesesPorFaturamento(faturamento_mensal, indices_ordenados);

    char *nomes_meses[] = {"Janeiro", "Fevereiro", "Marco", "Abril", "Maio", "Junho", 
                           "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"};

    printf("\n================ RELATORIO ANUAL ================\n");
    printf("VALOR TOTAL CONSOLIDADO NO ANO: R$ %.2f\n", total_anual);
    printf("--------------------------------------------------\n");
    printf("Meses ordenados por maior faturamento (Decrescente):\n");
    
    for (int i = 0; i < MESES_ANO; i++) {
        int idx = indices_ordenados[i];
        printf("%d lugar: %s - Total: R$ %.2f\n", i + 1, nomes_meses[idx], faturamento_mensal[idx]);
    }
    printf("==================================================\n");
}
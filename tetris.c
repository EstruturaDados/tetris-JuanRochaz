#include <stdio.h>   // Biblioteca para entrada e saída de dados (printf, scanf)
#include <stdlib.h>  // Biblioteca para funções gerais (rand, srand)
#include <time.h>    // Biblioteca para tempo (time, para srand aleatório)

// Definição da struct Peca
// Esta estrutura representa uma peça do Tetris, com nome (caractere único, ex: 'I', 'O', 'T', 'L')
// e id (identificador numérico exclusivo e incremental).
typedef struct {
    char nome;  // Nome da peça (single char para simplicidade)
    int id;     // ID único da peça (gerado automaticamente)
} Peca;

// Constantes para a fila circular
#define TAMANHO_FILA 5  // Capacidade fixa da fila (exatamente 5 elementos)

// Variáveis globais para a fila circular
// Array fixo para armazenar as peças (usando struct e array conforme requisito).
Peca fila[TAMANHO_FILA];
int front = 0;       // Índice da frente da fila (próxima peça a ser removida)
int rear = -1;       // Índice do final da fila (próximo slot para inserir; -1 indica vazia inicialmente)
int contadorItens = 0;  // Contador de itens atuais na fila (facilita checagens de cheia/vazia)
int proximoId = 1;   // Contador global para IDs exclusivos das peças

// Protótipos das funções
// Essas funções gerenciam a fila circular, garantindo operações eficientes O(1) amortizado.
void inicializarFila();
Peca gerarPeca();  // Função obrigatória para criar peças automaticamente
void enfileirar(Peca novaPeca);  // Inserir no final (enqueue)
Peca desenfileirar();  // Remover da frente (dequeue/jogar)
void visualizarFila();  // Visualizar o estado atual
int isCheia();   // Verifica se a fila está cheia
int isVazia();   // Verifica se a fila está vazia

int main() {
    // Semente para geração aleatória de peças
    srand(time(NULL));

    // Inicialização da fila com 5 peças geradas automaticamente
    // Isso preenche a fila circular no início do programa.
    inicializarFila();

    int opcao;  // Variável para capturar a escolha do menu

    // Mensagem inicial para orientar o usuário (usabilidade)
    printf("=== Sistema de Fila Circular - Pecas de Tetris Stack (Nivel Novato) ===\n");
    printf("Fila inicializada com 5 pecas. Use o menu para interagir.\n");
    printf("A fila e circular: ao remover, o espaco e reaproveitado ao inserir.\n\n");

    // Laço principal do menu: while para manter o programa rodando até a saída
    // Usa switch para opções claras, facilitando a manutenção.
    while (1) {
        printf("Menu:\n");
        printf("1 - Visualizar Fila Atual\n");
        printf("2 - Jogar Peca (Remover da Frente)\n");
        printf("3 - Inserir Nova Peca (Automaticamente no Final)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        
        // Leitura da opção com scanf (inteiro), com tratamento básico de erro
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');  // Limpa buffer
            printf("Opcao invalida! Tente novamente.\n\n");
            continue;
        }
        
        // Limpa o buffer após scanf
        while (getchar() != '\n');

        switch (opcao) {
            case 1:
                visualizarFila();
                break;
            case 2:
                if (!isVazia()) {
                    Peca jogada = desenfileirar();
                    printf("Peca jogada: %c (ID: %d)\n", jogada.nome, jogada.id);
                } else {
                    printf("Fila vazia! Nao ha pecas para jogar.\n");
                }
                visualizarFila();  // Exibe estado atualizado após ação (requisito)
                break;
            case 3:
                if (!isCheia()) {
                    Peca nova = gerarPeca();
                    enfileirar(nova);
                    printf("Nova peca inserida: %c (ID: %d)\n", nova.nome, nova.id);
                } else {
                    printf("Fila cheia! Remova uma peca primeiro.\n");
                }
                visualizarFila();  // Exibe estado atualizado após ação
                break;
            case 0:
                printf("Saindo do sistema. Game over!\n");
                return 0;  // Sai do programa
            default:
                printf("Opcao invalida! Escolha de 0 a 3.\n");
        }
        printf("\n");  // Separação para melhor usabilidade
    }

    return 0;
}

// Função para inicializar a fila com 5 peças geradas automaticamente
// Usa um laço para enfileirar 5 peças, preenchendo a fila circular inicialmente.
void inicializarFila() {
    printf("Inicializando fila com 5 pecas...\n");
    for (int i = 0; i < TAMANHO_FILA; i++) {
        Peca inicial = gerarPeca();
        enfileirar(inicial);
    }
    printf("Fila inicializada com sucesso!\n\n");
}

// Função obrigatória: gera uma nova peça automaticamente
// Escolhe aleatoriamente um nome entre 'I', 'O', 'T', 'L' e atribui um ID único incremental.
// Isso simula a geração de peças no Tetris.
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};  // Tipos de peças do Tetris
    nova.nome = tipos[rand() % 4];  // Aleatório entre 4 tipos
    nova.id = proximoId++;  // ID exclusivo e incremental
    return nova;
}

// Função para enfileirar (inserir no final da fila circular)
// Atualiza rear com modulo para circularidade e incrementa contador.
// Opera em O(1) tempo.
void enfileirar(Peca novaPeca) {
    if (isCheia()) {
        return;  // Não insere se cheia (checado no main)
    }
    rear = (rear + 1) % TAMANHO_FILA;  // Avança rear circularmente
    fila[rear] = novaPeca;  // Insere no novo rear
    contadorItens++;
}

// Função para desenfileirar (remover da frente da fila circular)
// Retorna a peça removida, atualiza front com modulo e decrementa contador.
// Opera em O(1) tempo. Reaproveita espaço para futuras inserções.
Peca desenfileirar() {
    if (isVazia()) {
        Peca vazia = {' ', 0};  // Peça inválida para vazio
        return vazia;
    }
    Peca removida = fila[front];  // Pega a peça da frente
    front = (front + 1) % TAMANHO_FILA;  // Avança front circularmente
    contadorItens--;
    return removida;
}

// Função para visualizar o estado atual da fila
// Percorre da front até rear, considerando a circularidade (se front > rear, wrap around).
// Usa um laço for com modulo para exibir em ordem FIFO. Desempenho O(n).
void visualizarFila() {
    if (isVazia()) {
        printf("Fila vazia!\n");
        return;
    }
    
    printf("=== Estado Atual da Fila (%d pecas) ===\n", contadorItens);
    int i = front;
    int count = 0;
    while (count < contadorItens) {
        printf("Posicao %d: Peca %c (ID: %d)\n", count + 1, fila[i].nome, fila[i].id);
        i = (i + 1) % TAMANHO_FILA;  // Avança circularmente
        count++;
    }
    printf("Frente: %d, Final: %d\n", front, rear);  // Debug: índices atuais
    printf("============================\n");
}

// Função auxiliar: verifica se a fila está cheia
// Cheia quando (rear + 1) % TAMANHO == front E contador == TAMANHO
int isCheia() {
    return (contadorItens == TAMANHO_FILA);
}

// Função auxiliar: verifica se a fila está vazia
// Vazia quando contador == 0 (ou rear +1 == front, mas contador é mais direto)
int isVazia() {
    return (contadorItens == 0);
}

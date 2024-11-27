
#include <stdio.h>
#include <stdlib.h> // Biblioteca p/ gerenciar a memória
#include <time.h> // Biblioteca p/ aprimorar o rand
#include <locale.h> // Biblioteca p/ corrigir acentuação
#include <string.h>

void salvarvitoria(char* nomeVencedor); //chamando a função antes da função "jogar" para n "indicar" erro de compilação.

// Estrutura para armazenar dados do jogador
typedef struct {
    char nome[50];
    int vitorias;
} Jogador;

// Função para criar um tabuleiro 3x3 dinamicamente
char** criarTabuleiro() {
    char** tabuleiro = (char**)malloc(3 * sizeof(char*)); // Correção: malloc para ponteiros de char
    
	for (int i = 0; i < 3; i++) {
        tabuleiro[i] = (char*)malloc(3 * sizeof(char));
        for (int j = 0; j < 3; j++) {
            tabuleiro[i][j] = ' '; // Inicializa com espaços vazios 
        }
    }
    return tabuleiro;
}

// Libera a memória alocada para o tabuleiro
void liberarTabuleiro(char** tabuleiro) {
    for (int i = 0; i < 3; i++) {
        free(tabuleiro[i]);
    }
    free(tabuleiro);
}

// Exibe o tabuleiro
void exibirTabuleiro(char** tabuleiro) {
    printf("\n"); // espaçamento p/ melhor visualização do tabuleiro
    
	for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf(" %c ", tabuleiro[i][j]);
            if (j < 2) printf("|");
        }
        printf("\n");
        if (i < 2) {
            for (int j = 0; j < 3; j++) {
                printf("---");
                if (j < 2) printf("+");
            }
            printf("\n");
        }
    }
    printf("\n"); // espaçamento p/ melhor visualização do tabuleiro
}

// Verifica se houve vitória
char verificarVitoria(char** tabuleiro) {
    // Verificar linhas e colunas
    for (int i = 0; i < 3; i++) {
        if (tabuleiro[i][0] != ' ' && tabuleiro[i][0] == tabuleiro[i][1] && tabuleiro[i][1] == tabuleiro[i][2])
            return tabuleiro[i][0];
        if (tabuleiro[0][i] != ' ' && tabuleiro[0][i] == tabuleiro[1][i] && tabuleiro[1][i] == tabuleiro[2][i])
            return tabuleiro[0][i];
    }
    // Verificar diagonais
    if (tabuleiro[0][0] != ' ' && tabuleiro[0][0] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][2])
        return tabuleiro[0][0];
    if (tabuleiro[0][2] != ' ' && tabuleiro[0][2] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][0])
        return tabuleiro[0][2];
    return ' '; // Sem vencedor
}

// Função para registrar a jogada de um jogador
int registrarJogada(char** tabuleiro, int jogador, char simbolo) {
    int linha, coluna;
    printf("Jogador %d (%c), insira linha e coluna (1-3):\n ", jogador, simbolo);
    scanf("%d %d", &linha, &coluna);
    if (linha < 1 || linha > 3 || coluna < 1 || coluna > 3 || tabuleiro[linha - 1][coluna - 1] != ' ') {
        printf("Jogada inválida! Tente novamente.\n"); // 
        return 0;
    }
    tabuleiro[linha - 1][coluna - 1] = simbolo; // Marca a jogada
    printf("Sua jogada: ");
    return 1;
}

// Função para o computador realizar uma jogada
void jogadaComputador(char** tabuleiro, char simbolo) {
    int linha, coluna;
    do {
        linha = rand() % 3;
        coluna = rand() % 3;
    } while (tabuleiro[linha][coluna] != ' ');
    tabuleiro[linha][coluna] = simbolo; // Marca a jogada
    printf("Jogada CPU: ");
}

// Função para jogar o jogo
void jogar(int contraComputador) {
    char** tabuleiro = criarTabuleiro();
    int jogadas = 0, jogador = 1;
    char vencedor = ' ';
    char simboloJogadores[2] = {'X', 'O'};
    char nomeJogadores[2][50] = {"Jogador 1", "Jogador 2"};
    // Lógica para determinar a "duração" e o vencedor do jogo
    if(!contraComputador) { //salvar o nome dos jogadores p/ ranking
        printf("Nome do jogador 1 (X): ");
        scanf(" %[^\n]", nomeJogadores[0]);
        printf("Nome do jogador 2 (O): ");
        scanf(" %[^\n]", nomeJogadores[1]);
    }
    
    while (jogadas < 9 && vencedor == ' ') {
        exibirTabuleiro(tabuleiro);
        if (contraComputador && jogador == 2) {
            jogadaComputador(tabuleiro, simboloJogadores[jogador - 1]);
        } else {
            if (!registrarJogada(tabuleiro, jogador, simboloJogadores[jogador - 1])) continue;
        }
        jogadas++;
        vencedor = verificarVitoria(tabuleiro);
        jogador = (jogador == 1) ? 2 : 1;
    }

    exibirTabuleiro(tabuleiro);
    if (vencedor != ' ') {
        printf("Parabéns! Jogador %s venceu!\n", nomeJogadores[jogador == 1 ? 1: 0]);
        if(!contraComputador) {
            salvarvitoria(nomeJogadores[jogador == 1 ? 1 : 0]);
        }
        // Salvar no ranking
    } else {
        printf("Empate! Ninguém venceu desta vez.\n");
    }

    liberarTabuleiro(tabuleiro);
}
void salvarvitoria(char* nomeVencedor) {
    FILE* arquivo = fopen("ranking.txt", "rb+");
    if(!arquivo) { //criar novo arquivo ranking caso não exista
        
        arquivo = fopen("ranking.txt", "wb");
        if (!arquivo) {
            printf("Erro ao abrir o arquivo de ranking. \n");
             return;
        }
       
    }
    Jogador jogador;
    int encontrado = 0;

    while (fread(&jogador, sizeof(Jogador), 1, arquivo)) {
        if(strcmp(jogador.nome, nomeVencedor) == 0) {
            jogador.vitorias++;
            fseek(arquivo, -sizeof(Jogador), SEEK_CUR);
            fwrite(&jogador, sizeof(Jogador), 1, arquivo);
            encontrado = 1;
            break;
        }

    }
    // adicionar novo jogador caso não encontrado
    if (!encontrado) {
        Jogador novoJogador;
        strcpy(novoJogador.nome, nomeVencedor);
        novoJogador.vitorias = 1;
        fwrite(&novoJogador, sizeof(Jogador), 1, arquivo);
    }
    fclose(arquivo);
}

// Função para exibir o ranking
void exibirRanking() {
    FILE* arquivo = fopen("ranking.txt", "rb");
    if (!arquivo) {
        printf("Nenhum ranking encontrado.\n");
        return;
    }
    Jogador jogadores[100]; // limite de até 100 jogadores
    int count = 0;

    while (fread(&jogadores[count], sizeof(Jogador), 1, arquivo)) {
        count++;
    }
    fclose(arquivo);
        //ordenar jogadores por número de vitórias
    for(int i = 0; i < count - 1; i++) {
        for(int j = i + 1; j < count; j++) {
            if (jogadores[i].vitorias < jogadores[j].vitorias) {
                Jogador temp = jogadores[i];
                jogadores [i] = jogadores[j];
                jogadores[j] = temp;
            }
        }
    }
    printf("Ranking: \n");
    for (int i = 0; i < count; i++) {
        printf("%s - %d vitórias\n", jogadores[i].nome, jogadores[i].vitorias);
    }

    
}

// Menu principal
void menu() {
    
    int opcao;
    do {
        printf("\nMenu Principal:\n");
        printf("1. Jogar\n");
        printf("2. Ver Ranking\n");
        printf("3. Créditos\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                int modo;
                printf("1. Jogar contra outro jogador\n");
                printf("2. Jogar contra o computador\n");
                printf("Escolha o modo: ");
                scanf("%d", &modo);
                jogar(modo == 2);
                break;
            }
            case 2:
                exibirRanking();
                break;
            case 3:
                printf("Créditos: Desenvolvido por Arthur/Renan/Pietro/Gustavo/Guilherme\n");
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 4);
}

// Função principal
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL)); // Inicializa a semente para jogadas aleatórias do computador
    menu();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LARGURA 50
#define ALTURA 14

char telaJogo[ALTURA][LARGURA + 1];
char labirinto[ALTURA][LARGURA + 1] = 
{
    "##################################################",
    "#                                                #",
    "#                                                #",
    "#                                                #",
    "#                                                #",
    "#                                                #",
    "#                                                #",
    "#                                                #",
    "#                                                #",
    "#                                                #",
    "#                                                #",
    "#                                                #",
    "#                                                #",
    "##################################################"
}; // Leve alteração pra n dar voltas na tela

// Coordenadas da fruta
int frutaX, frutaY;

// Estrutura para a cobra (lista encadeada)
struct cobra {
    int x, y;  // Coordenadas da cabeça da cobra
    struct cobra *proximo;
};

// Função para limpar a tela e mostrar a tela inicial
void telaInicial() {
    system("clear || cls"); // Limpa a tela 

    printf("##################################################\n");
    printf("#                                                #\n");
    printf("#   OBS.: Clique enter ao inserir a direção      #\n");
    printf("#                                                #\n");
    printf("#                                                #\n");
    printf("#                                                #\n");
    printf("#                 COBRINHA ---(0-0)              #\n");
    printf("#                                                #\n");
    printf("#                                                #\n");
    printf("#                                                #\n");
    printf("#      >🍎<   Coma as frutas para crescer!       #\n");
    printf("#                                                #\n");
    printf("#        Pressione ENTER para começar...         #\n");
    printf("#                                                #\n");
    printf("##################################################\n");

    getchar(); // Inicia ao clicar ENTER
}

// Função para desenhar o labirinto
void desenharLabirinto() {
    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < LARGURA; j++) {
            telaJogo[i][j] = labirinto[i][j]; // Copia o labirinto na tela
        }
        telaJogo[i][LARGURA] = '\0'; // Finaliza string da linha
    }
    telaJogo[frutaY][frutaX] = '*'; // Garante que a fruta apareça
}

// Verifica se a posição está livre (não é parede ou parte da cobra)
int posicaoLivre(int x, int y, struct cobra *cobra) {
    if (labirinto[y][x] == '#') {
        return 0;
    }
    struct cobra *temp = cobra;
    while (temp != NULL) {
        if (temp->x == x && temp->y == y) {
            return 0;
        }
        temp = temp->proximo;
    }
    return 1;
}// 0 tem, 1 não tem

// Coloca a fruta aleatoriamente no labirinto
void colocarFruta(struct cobra *cobra) { 
    int posicaoValida = 0;
    while (!posicaoValida) {
        frutaX = rand() % (LARGURA - 2) + 1; // Não aparecerá nas bordas
        frutaY = rand() % (ALTURA - 2) + 1;
        posicaoValida = posicaoLivre(frutaX, frutaY, cobra);
    }
}

// Desenha a cobra na tela
void desenharCobra(struct cobra *c) {
    while (c != NULL) {
        telaJogo[c->y][c->x] = 'O';
        c = c->proximo;
    }// Percorre a cobra e adiciona na posição correspondente
}

// Imprime a tela do jogo
void imprimirTela() {
    system("clear || cls"); // Limpa a tela a cada frame
    for (int i = 0; i < ALTURA; i++) {
        printf("%s\n", telaJogo[i]);
    }
}

// Inicializa o jogo
void inicializarJogo(struct cobra **cobra) {
    desenharLabirinto();
    *cobra = (struct cobra*)malloc(sizeof(struct cobra));
    (*cobra)->x = LARGURA / 2;
    (*cobra)->y = ALTURA / 2; // Aloca memória, põe o primeiro frame, põe a fruta e a cobra
    (*cobra)->proximo = NULL;
    colocarFruta(*cobra);
}

// Move a cobra na direção escolhida e cresce se necessário
int moverCobra(struct cobra **cobra, char direcao, int *crescer) {
    struct cobra *novoSegmento = (struct cobra*)malloc(sizeof(struct cobra)); // Adiciona a cabeça e tira o rabo se ela não tiver crescido
    novoSegmento->proximo = *cobra;

    switch (direcao) {
        case 'w': novoSegmento->x = (*cobra)->x; novoSegmento->y = (*cobra)->y - 1; break;
        case 's': novoSegmento->x = (*cobra)->x; novoSegmento->y = (*cobra)->y + 1; break;
        case 'a': novoSegmento->x = (*cobra)->x - 1; novoSegmento->y = (*cobra)->y; break;
        case 'd': novoSegmento->x = (*cobra)->x + 1; novoSegmento->y = (*cobra)->y; break;
        default: free(novoSegmento); return 0;
    }
    *cobra = novoSegmento;

    // Se não precisar crescer, remover a cauda
    if (!(*crescer)) {
        struct cobra *temp = *cobra;
        while (temp->proximo && temp->proximo->proximo) {
            temp = temp->proximo;
        }
        if (temp->proximo) {
            free(temp->proximo);
            temp->proximo = NULL;
        }
    } else {
        *crescer = 0;
    }
    return 1;
}

// Verifica se a cobra colidiu com parede ou ela mesma
int verificarColisao(struct cobra *cobra) {
    if (cobra->x <= 0 || cobra->x >= LARGURA - 1 || cobra->y <= 0 || cobra->y >= ALTURA - 1) {
        return 1; // Morreu a pobre coitada
    }
    struct cobra *temp = cobra->proximo;
    while (temp != NULL) {
        if (temp->x == cobra->x && temp->y == cobra->y) {
            return 1;
        }
        temp = temp->proximo;
    }
    return 0;
}

// Verifica se a cobra comeu a fruta
int verificarComerFruta(struct cobra *cobra) {
    if (cobra->x == frutaX && cobra->y == frutaY) {
        colocarFruta(cobra);
        return 1; // Barriga cheia
    }
    return 0;
}

// Libera memória alocada pela cobra
void liberarCobra(struct cobra *cobra) {
    struct cobra *temp;
    while (cobra != NULL) {
        temp = cobra;
        cobra = cobra->proximo;
        free(temp);
    } // Libera cada nó
}

// Função principal
int main() {
    struct cobra *cobra = NULL;
    char direcao;
    int crescer = 0;

    srand(time(NULL));

    telaInicial(); // Chama a tela inicial antes de iniciar o jogo

    inicializarJogo(&cobra);

    while (1) {
        desenharLabirinto();
        desenharCobra(cobra);

        if (verificarComerFruta(cobra)) {
            crescer = 1;
        }
        imprimirTela();

        if (verificarColisao(cobra)) {
            printf("Game Over! A cobra colidiu.\n");
            break;
        }

        printf("OBS.: Clique ENTER ao inserir a direção \n \nDigite a direção (w: cima, s: baixo, a: esquerda, d: direita):");
        scanf(" %c", &direcao); // Lê o comando e mostra as instruções
        if (!moverCobra(&cobra, direcao, &crescer)) {
            printf("Direção inválida! Tente novamente.\n");
        }
    }
    liberarCobra(cobra);
    return 0;
}

/* Tema 5: Jogo de Batalha Naval - Níveis Novato, Aventureiro e Mestre
   Descrição: Programa em C que posiciona navios em um tabuleiro 10x10,
              adiciona navios diagonais e sobrepõe matrizes de habilidades
              (cone, cruz, octaedro) marcando as áreas afetadas. */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define SIZE 10        // Tamanho do tabuleiro 10x10
#define SHIP_VAL 3     // Valor usado para marcar navios no tabuleiro
#define EFFECT_VAL 5   // Valor usado para marcar áreas afetadas por habilidades
#define EMPTY_VAL 0    // Água

// Assinaturas de funções 
void init_board(int board[SIZE][SIZE]);
void print_board(int board[SIZE][SIZE]);
bool can_place_ship(int board[SIZE][SIZE], int row, int col, int length, char orientation);
void place_ship(int board[SIZE][SIZE], int row, int col, int length, char orientation);
bool check_overlap_area(int board[SIZE][SIZE], int start_row, int start_col, int mat_rows, int mat_cols, int mat[mat_rows][mat_cols], int center_row, int center_col);
void overlay_skill(int board[SIZE][SIZE], int mat_rows, int mat_cols, int mat[mat_rows][mat_cols], int center_row, int center_col);
void build_cone(int mat_rows, int mat_cols, int mat[mat_rows][mat_cols]);
void build_cross(int mat_rows, int mat_cols, int mat[mat_rows][mat_cols]);
void build_octahedron(int mat_rows, int mat_cols, int mat[mat_rows][mat_cols]);

// Função principal 
int main(void) {
    int board[SIZE][SIZE];

    /* NÍVEL NOVATO
       Objetivo: Criar tabuleiro 10x10, posicionar 2 navios (1 horizontal, 1 vertical) tamanho 3.
       As coordenadas estão fixas no código (0-index): linha, coluna. */

    printf("=== BATALHA NAVAL: NÍVEL NOVATO ===\n\n");

    init_board(board); // inicializa com zeros

    // Definições (escolhidas manualmente, garantidas dentro do tabuleiro)
    // Observação: coordenadas são 0-indexadas (linha 0..9, coluna 0..9)
    int ship_len = 3;

    // Navio 1: horizontal, inicia em (linha=2, col=1) e ocupa (2,1),(2,2),(2,3)
    int nov_row1 = 2, nov_col1 = 1;
    char nov_orient1 = 'H'; // 'H' = horizontal

    // Navio 2: vertical, inicia em (linha=5, col=7) e ocupa (5,7),(6,7),(7,7)
    int nov_row2 = 5, nov_col2 = 7;
    char nov_orient2 = 'V'; // 'V' = vertical

    // Validação simples antes de posicionar
    if (!can_place_ship(board, nov_row1, nov_col1, ship_len, nov_orient1)) {
        printf("Erro: navio horizontal (novato) não cabe nas coordenadas fornecidas.\n");
        return 1;
    }
    place_ship(board, nov_row1, nov_col1, ship_len, nov_orient1);

    if (!can_place_ship(board, nov_row2, nov_col2, ship_len, nov_orient2)) {
        printf("Erro: navio vertical (novato) não cabe nas coordenadas fornecidas.\n");
        return 1;
    }
    // Verifica sobreposição simples: can_place_ship já checa limites, aqui checamos se há SHIP_VAL antes
    // (a função can_place_ship, nesta implementação, não checa sobreposição, então checamos manualmente)
    for (int i = 0; i < ship_len; i++) {
        int r = nov_row2 + (nov_orient2 == 'V' ? i : 0);
        int c = nov_col2 + (nov_orient2 == 'H' ? i : 0);
        if (board[r][c] == SHIP_VAL) {
            printf("Erro: sobreposição detectada ao posicionar navio vertical (novato).\n");
            return 1;
        }
    }
    place_ship(board, nov_row2, nov_col2, ship_len, nov_orient2);

    printf("Tabuleiro após posicionamento (Nível Novato):\n");
    print_board(board);

    /* --- NÍVEL AVENTUREIRO ---
       Objetivo: Adicionar mais 2 navios (total 4), sendo agora 2 horizontais/verticais
       e 2 posicionados em diagonal (tamanho 3). Garantir limites e evitar sobreposição. */
    printf("\n=== NÍVEL AVENTUREIRO ===\n\n");

    // Vamos posicionar dois navios adicionais:
    // Navio 3: horizontal, inicia em (linha=0, col=6) -> ocupa (0,6),(0,7),(0,8)
    int adv_row3 = 0, adv_col3 = 6;
    char adv_orient3 = 'H';

    // Navio 4: diagonal (descendo para a direita), inicia em (linha=6, col=0) -> ocupa (6,0),(7,1),(8,2)
    int adv_row4 = 6, adv_col4 = 0;
    char adv_orient4 = 'D'; // 'D' = diagonal down-right (linha+1, col+1)

    // Validações e verificações de sobreposição
    if (!can_place_ship(board, adv_row3, adv_col3, ship_len, adv_orient3)) {
        printf("Erro: navio horizontal (aventureiro) não cabe nas coordenadas fornecidas.\n");
        return 1;
    }
    // checar sobreposição para adv3
    bool overlap = false;
    for (int i = 0; i < ship_len; i++) {
        int r = adv_row3 + (adv_orient3 == 'V' ? i : 0);
        int c = adv_col3 + (adv_orient3 == 'H' ? i : 0);
        if (board[r][c] == SHIP_VAL) { overlap = true; break; }
    }
    if (overlap) {
        printf("Erro: sobreposição detectada ao posicionar navio horizontal (aventureiro).\n");
        return 1;
    }
    place_ship(board, adv_row3, adv_col3, ship_len, adv_orient3);

    // Para navio diagonal, implementamos validação manual: linha+length-1 < SIZE e col+length-1 < SIZE
    if (adv_orient4 == 'D') {
        if (adv_row4 + ship_len - 1 >= SIZE || adv_col4 + ship_len - 1 >= SIZE) {
            printf("Erro: navio diagonal (aventureiro) está fora dos limites.\n");
            return 1;
        }
        // checar sobreposição
        overlap = false;
        for (int i = 0; i < ship_len; i++) {
            int r = adv_row4 + i;
            int c = adv_col4 + i;
            if (board[r][c] == SHIP_VAL) { overlap = true; break; }
        }
        if (overlap) {
            printf("Erro: sobreposição detectada ao posicionar navio diagonal (aventureiro).\n");
            return 1;
        }
        // posiciona diagonal (down-right)
        for (int i = 0; i < ship_len; i++) {
            board[adv_row4 + i][adv_col4 + i] = SHIP_VAL;
        }
    } else {
        // caso de outras diagonais (poderíamos implementar 'U' = up-right), mas não usado aqui
    }

    printf("Tabuleiro após posicionamento (Nível Aventureiro):\n");
    print_board(board);

    /* --- NÍVEL MESTRE ---
       Objetivo: Criar matrizes de habilidades (cone, cruz, octaedro) e sobrepor no tabuleiro.
       As matrizes têm 5x5 neste exemplo (poderia ser 7x7).
       As áreas marcadas na matriz de habilidade com 1 serão aplicadas ao tabuleiro
       e o valor EFFECT_VAL (5) será escrito nas posições afetadas (sobrescrevendo). */
    printf("\n=== NÍVEL MESTRE ===\n\n");

    // Definimos matrizes 5x5 para as habilidades
    const int MAT_SIZE = 5;
    int mat_cone[MAT_SIZE][MAT_SIZE];
    int mat_cross[MAT_SIZE][MAT_SIZE];
    int mat_octa[MAT_SIZE][MAT_SIZE];

    // Construir as matrizes usando funções que usam loops e condicionais
    build_cone(MAT_SIZE, MAT_SIZE, mat_cone);
    build_cross(MAT_SIZE, MAT_SIZE, mat_cross);
    build_octahedron(MAT_SIZE, MAT_SIZE, mat_octa);

    // Definir pontos de origem (centro) no tabuleiro para cada habilidade (coordenadas 0-index)
    // Atenção: centro da matriz de habilidade será alinhado com o ponto de origem no tabuleiro
    int center_cone_row = 1, center_cone_col = 4;  // exemplo: cone apontando para baixo, perto do topo
    int center_cross_row = 7, center_cross_col = 5; // exemplo: cruz no meio-inferior
    int center_octa_row = 4, center_octa_col = 2;  // exemplo: octaedro no meio do tabuleiro

    // Sobrepor as habilidades no tabuleiro (com verificações de limites)
    overlay_skill(board, MAT_SIZE, MAT_SIZE, mat_cone, center_cone_row, center_cone_col);
    overlay_skill(board, MAT_SIZE, MAT_SIZE, mat_cross, center_cross_row, center_cross_col);
    overlay_skill(board, MAT_SIZE, MAT_SIZE, mat_octa, center_octa_row, center_octa_col);

    printf("Tabuleiro final com áreas de efeito (Nível Mestre):\n");
    printf("Legenda: 0=água, 3=navio, 5=área afetada pela habilidade\n\n");
    print_board(board);

    printf("\nExecução finalizada. Bom estudo e mãos à obra! Salve Maria\n");

    return 0;
}

// Funções auxiliares 

// Inicializa o tabuleiro com valores EMPTY_VAL (0) 
void init_board(int board[SIZE][SIZE]) {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            board[r][c] = EMPTY_VAL;
        }
    }
}

// Imprime o tabuleiro de forma organizada
void print_board(int board[SIZE][SIZE]) {
    // imprimir índice de colunas no topo (opcional, para leitura)
    printf("   ");
    for (int c = 0; c < SIZE; c++) {
        printf("%d ", c);
    }
    printf("\n");

    // linha separadora
    printf("  ---------------------\n");

    for (int r = 0; r < SIZE; r++) {
        // imprime índice da linha
        printf("%d| ", r);
        for (int c = 0; c < SIZE; c++) {
            // imprime valor e um espaço separador para legibilidade
            printf("%d ", board[r][c]);
        }
        printf("\n");
    }
}

/* Verifica se um navio pode ser colocado sem sair dos limites.
   Para orientações:
   'H' = horizontal (coluna aumenta)
   'V' = vertical   (linha aumenta)
   'D' = diagonal down-right (linha+1, col+1)
   NOTA: Esta função checa apenas limites (não sobreposição). */
bool can_place_ship(int board[SIZE][SIZE], int row, int col, int length, char orientation) {
    if (row < 0 || col < 0 || row >= SIZE || col >= SIZE) return false;

    if (orientation == 'H') {
        if (col + length - 1 >= SIZE) return false;
    } else if (orientation == 'V') {
        if (row + length - 1 >= SIZE) return false;
    } else if (orientation == 'D') { // diagonal down-right
        if (row + length - 1 >= SIZE || col + length - 1 >= SIZE) return false;
    } else {
        // orientação desconhecida
        return false;
    }
    return true;
}

/* Posiciona um navio (assume que can_place_ship foi chamado antes).
   Esta função escreve SHIP_VAL nas posições correspondentes.
   Para diagonais simples (D) escreve na diagonal down-right. */
void place_ship(int board[SIZE][SIZE], int row, int col, int length, char orientation) {
    if (orientation == 'H') {
        for (int i = 0; i < length; i++) {
            board[row][col + i] = SHIP_VAL;
        }
    } else if (orientation == 'V') {
        for (int i = 0; i < length; i++) {
            board[row + i][col] = SHIP_VAL;
        }
    } else if (orientation == 'D') {
        for (int i = 0; i < length; i++) {
            board[row + i][col + i] = SHIP_VAL;
        }
    }
}

/* Checa se a sobreposição da matriz de habilidade com centro em (center_row, center_col)
   teria colisão com alguma restrição — opcional, usamos apenas para ilustração.
   (Não usada diretamente na lógica principal, mas mantida como utilidade). */
bool check_overlap_area(int board[SIZE][SIZE], int start_row, int start_col, int mat_rows, int mat_cols, int mat[mat_rows][mat_cols], int center_row, int center_col) {
    int half_r = mat_rows / 2;
    int half_c = mat_cols / 2;
    for (int i = 0; i < mat_rows; i++) {
        for (int j = 0; j < mat_cols; j++) {
            if (mat[i][j] == 1) {
                int board_r = center_row + (i - half_r);
                int board_c = center_col + (j - half_c);
                if (board_r < 0 || board_r >= SIZE || board_c < 0 || board_c >= SIZE) {
                    return true; // "overlap" here significa fora do tabuleiro
                }
            }
        }
    }
    return false;
}

/* Sobrepõe a matriz de habilidade ao tabuleiro, centrando-a em (center_row, center_col).
   Se a célula da matriz de habilidade for 1 e a posição do tabuleiro estiver dentro dos limites,
   escrevemos EFFECT_VAL (sobrescreve o que houver). */
void overlay_skill(int board[SIZE][SIZE], int mat_rows, int mat_cols, int mat[mat_rows][mat_cols], int center_row, int center_col) {
    int half_r = mat_rows / 2;
    int half_c = mat_cols / 2;

    for (int i = 0; i < mat_rows; i++) {
        for (int j = 0; j < mat_cols; j++) {
            if (mat[i][j] == 1) {
                int board_r = center_row + (i - half_r);
                int board_c = center_col + (j - half_c);
                // Verifica limites do tabuleiro antes de aplicar
                if (board_r >= 0 && board_r < SIZE && board_c >= 0 && board_c < SIZE) {
                    // DECISÃO DE DESIGN: sobrescrever com EFFECT_VAL para visualizar claramente a área.
                    board[board_r][board_c] = EFFECT_VAL;
                }
            }
        }
    }
}

/* Constrói uma matriz "cone" 5x5 apontando para baixo.
   Lógica: linha 0 tem 1 célula central, linha 1 tem 3 células (central ±1), linha 2 tem 5 células, etc.
   Aqui usaremos um formato simétrico limitado a MAT_SIZE linhas. */
void build_cone(int mat_rows, int mat_cols, int mat[mat_rows][mat_cols]) {
    // Inicializa tudo com 0
    for (int i = 0; i < mat_rows; i++)
        for (int j = 0; j < mat_cols; j++)
            mat[i][j] = 0;

    int center = mat_cols / 2;
    // Exemplo para mat_rows == 5:
    // i=0: apenas center -> largura 1
    // i=1: center-1..center+1 -> largura 3
    // i=2: center-2..center+2 -> largura 5 (chega ao limite)
    // i>=3: mantém largura máxima (poderíamos restringir a forma)
    for (int i = 0; i < mat_rows; i++) {
        int half_width = i; // expande 0,1,2,...
        if (half_width > center) half_width = center; // não sair do limite
        for (int j = center - half_width; j <= center + half_width; j++) {
            if (j >= 0 && j < mat_cols) mat[i][j] = 1;
        }
    }
}

/* Constrói uma matriz "cruz" 5x5 com ponto de origem no centro.
   Exemplo simples: marca a linha central inteira e a coluna central inteira. */
void build_cross(int mat_rows, int mat_cols, int mat[mat_rows][mat_cols]) {
    // inicializa com zeros
    for (int i = 0; i < mat_rows; i++)
        for (int j = 0; j < mat_cols; j++)
            mat[i][j] = 0;

    int center_r = mat_rows / 2;
    int center_c = mat_cols / 2;

    // marca coluna central
    for (int i = 0; i < mat_rows; i++) mat[i][center_c] = 1;
    // marca linha central
    for (int j = 0; j < mat_cols; j++) mat[center_r][j] = 1;
}

/* Constrói uma matriz "octaedro" (vista frontal -> losango/diamante).
   Em 5x5 fica:
     0 0 1 0 0
     0 1 1 1 0
     1 1 1 1 1
     0 1 1 1 0
     0 0 1 0 0
   Ou reduzido conforme tamanho. */
void build_octahedron(int mat_rows, int mat_cols, int mat[mat_rows][mat_cols]) {
    // inicializa com zeros
    for (int i = 0; i < mat_rows; i++)
        for (int j = 0; j < mat_cols; j++)
            mat[i][j] = 0;

    int center_r = mat_rows / 2;
    int center_c = mat_cols / 2;

    // distância de Manhattan <= center (cria um losango)
    for (int i = 0; i < mat_rows; i++) {
        for (int j = 0; j < mat_cols; j++) {
            int manhattan = abs(i - center_r) + abs(j - center_c);
            if (manhattan <= center_r) mat[i][j] = 1;
        }
    }
}

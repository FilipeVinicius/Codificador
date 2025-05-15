#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h> //!

#define MAX_LINHAS 1024
#define MAX_COLUNAS 768

int linhas, colunas;
int matriz[MAX_LINHAS][MAX_COLUNAS]; // definindo pra n der b.o

//* Assinatura das funções (Logo abaixo na parte do corpo das funções está a explicação delas e dos seus respectivos parâmetros)
bool verificarMatrizHomogenea(int linhasTot, int colunasTot, int matriz[MAX_LINHAS][MAX_COLUNAS], int linhaInicio, int linhaFim, int colunaInicio, int colunaFim);
void cortarMatriz(int linhasTot, int colunasTot, int matriz[MAX_LINHAS][MAX_COLUNAS], int linhaInicio, int linhaFim, int colunaInicio, int colunaFim);
bool eh_arquivo_pbm(char *arquivo); // Função para verificar se o arquivo é PBM
int ler_pbm(const char *nome_arquivo, int matriz[MAX_LINHAS][MAX_COLUNAS], int *linhas, int *colunas);

void exibirInstrucoes();

int main()
{
    char nome_arquivo[255];
    char nomeComando[20]; // Para armazenar o comando

    //* CLI
    printf("Digite o comando [-? (para help) | -m (para digitar manualmente)| -f ARQ (para entrada de arquivo)]\n");
    scanf("%s", &nomeComando);

    //* Caso o usuário deseje ajuda
    if (strcmp(nomeComando, "-?") == 0 || strcmp(nomeComando, "--help") == 0)
    {
        exibirInstrucoes();
    } 

    //* Caso o usuário queira inputar o arquivo
    if (strcmp(nomeComando, "-f") == 0 || strcmp(nomeComando, "--file") == 0)
    {
        printf("Digite o nome do arquivo: ");
        scanf("%s", &nome_arquivo);
        if (strlen(nome_arquivo) > 0)
        {
            if (!eh_arquivo_pbm(nome_arquivo))
            {
                printf("O arquivo '%s' NÃO é um PBM válido.\n", nome_arquivo);
                return 1;
            }

            if (!ler_pbm(nome_arquivo, matriz, &linhas, &colunas))
            {
                printf("Erro ao ler o conteúdo do arquivo PBM.\n");
                return 1;
            }

            printf("Compressão da imagem:\n");
            cortarMatriz(linhas, colunas, matriz, 0, linhas, 0, colunas);
            printf("\n");
        }
        else
        {
            printf("Nome do arquivo inválido.\n");
        }
    
    }
    //* Caso o usuário queira digitar manualmente
    if (strcmp(nomeComando, "-m") == 0 || strcmp(nomeComando, "--manual") == 0) {


        printf("Digite a largura (linha) da imagem: ");
        scanf("%d", &linhas);

        printf("Digite a altura (coluna) da imagem: ");
        scanf("%d", &colunas);

        int matriz[linhas][colunas];

        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                printf("Digite a dimensão matriz[%d][%d]: ", i, j);
                scanf("%d", &matriz[i][j]);
            }
        }

        cortarMatriz(linhas, colunas, matriz, 0, linhas, 0, colunas);
    }

    return 0;
}

// printf("Digite o nome do arquivo: ");
// scanf("%s", nome_arquivo);

// Matriz de teste
// int matriz1[3][5] = {
//{1, 1, 0, 1, 0},
//{1, 1, 0, 0, 1},
//{0, 0, 1, 1, 1}};

// Outra matriz de teste

// cortarMatriz(3, 5, matriz1, 0, 3, 0, 5); //* saída: XXPBPBXPBBPXBPP (exemplo do professor da matriz 3x5 que ele fez)
// printf("\n");

//* Função booleana que verifica se a matriz ou submatriz é toda branca (0) ou toda preta (1)
//* Consiste em 7 parâmetros
/**
 * Primeiro Parâmetro: recebe a quantidade de linhas totais da matriz
 * Segundo: recebe a quantidade de colunas totais da matriz
 * Terceiro: recebe a matriz na qual iremos fazer a verificação e os cortes
 * Quarto: recebe o índice em que a linha da matriz começa
 * Quinto: recebe o índice em que a linha da matriz acaba
 * Sexto: recebe o índice em que a coluna da matriz inicia
 * Sétimo: recebe o índice em que a coluna da matriz acaba
 */
bool verificarMatrizHomogenea(int linhasTot, int colunasTot, int matriz[MAX_LINHAS][MAX_COLUNAS], int linhaInicio, int linhaFim, int colunaInicio, int colunaFim)
{
    int primeiroValorMatriz = matriz[linhaInicio][colunaInicio]; //* Armazena o primeiro valor da matriz como referência (0 ou 1)

    //* Percorre todos os elementos da matriz e verifica se alguma delas é diferente do
    //* valor armazenado dentro do primeiroValorMatriz
    for (int indiceLinha = linhaInicio; indiceLinha < linhaFim; indiceLinha++) // Percorre as linhas da matriz
    {
        for (int indiceColuna = colunaInicio; indiceColuna < colunaFim; indiceColuna++) // Percorre as colunas da matriz
        {
            if (matriz[indiceLinha][indiceColuna] != primeiroValorMatriz) // Se o elemento atual da matriz for diferente do primeiroValorMatriz, retorna falso
            {
                return false; // Indica que a matriz não é homogênea
            }
        }
    }
    return true; // Caso nenhuma das condições seja atendida, quer dizer que a matriz é homogênea
}

//* Função que faz os cortes da matriz/submatriz não homogênea
//* Contém 7 parâmetros que seguem a mesma lógica dos parâmetros da função verificarMatrizHomogenea
void cortarMatriz(int linhasTot, int colunasTot, int matriz[MAX_LINHAS][MAX_COLUNAS], int linhaInicio, int linhaFim, int colunaInicio, int colunaFim)
{

    if (linhaInicio >= linhaFim || colunaInicio >= colunaFim)
        return;

    //* Se a matriz/submatriz for homogênea (somente 0 ou somente 1), então vai printar o "B" ou o "P"
    if (verificarMatrizHomogenea(linhasTot, colunasTot, matriz, linhaInicio, linhaFim, colunaInicio, colunaFim) == true)
    {

        int primeiroElementoMatriz = matriz[linhaInicio][colunaInicio]; // Pegar o primeiro elemento da matriz/submatriz como referência

        //* Condição que verifica se a matriz/submatriz é composta por 0 ou 1
        if (primeiroElementoMatriz == 0)
        { //* Se esse primeiro elemento for 0, quer dizer que ela matriz/submatriz é composta por 0, então printa B, se nao, printa P
            printf("B");
        }
        else
        {
            printf("P");
        }
        return;
    }

    //* Matriz/Submatriz nao homogenea = deve ser dividida, essa divisão será representada por X
    printf("X");

    //* Calcular o "Meio" da linha e da coluna para cortar
    int linhaMeio = linhaInicio + (linhaFim - linhaInicio + 1) / 2;
    int colunaMeio = colunaInicio + (colunaFim - colunaInicio + 1) / 2;

    //* Dividir em 4 quadrantes a matriz e/ou a submatriz
    //* Condições if necessárias para evitar chamada recursiva desnecessária (ex: uma matriz 1x2 (nao tem como dividir mais nas linhas))
    if (linhaInicio < linhaMeio && colunaInicio < colunaMeio)
    {
        // Os parâmetros representam onde cada quadrante está localizado, por isso eles variam
        // Os 3 primeiros parâmetros representam a quantidade total de linhas, colunas e a matriz em si
        // Os outros 4 mostram onde cada quadrante está localizado de acordo com o local onde cada linha/coluna começa e termina
        cortarMatriz(linhasTot, colunasTot, matriz, linhaInicio, linhaMeio, colunaInicio, colunaMeio); // Quadrante 1 (superior esquerda)
    }

    if (linhaInicio < linhaMeio && colunaMeio < colunaFim)
    {
        cortarMatriz(linhasTot, colunasTot, matriz, linhaInicio, linhaMeio, colunaMeio, colunaFim); // Quadrante 2 (superior direita)
    }

    if (linhaMeio < linhaFim && colunaInicio < colunaMeio)
    {
        cortarMatriz(linhasTot, colunasTot, matriz, linhaMeio, linhaFim, colunaInicio, colunaMeio); // Quadrante 3 (inferior esquerda)
    }

    if (linhaMeio < linhaFim && colunaMeio < colunaFim)
    {
        cortarMatriz(linhasTot, colunasTot, matriz, linhaMeio, linhaFim, colunaMeio, colunaFim); // Quadrante 4 (inferior direita)
    }
}

bool eh_arquivo_pbm(char *arquivo)
{
    FILE *fp = fopen(arquivo, "r");
    if (!fp)
    {
        perror("Erro ao abrir o arquivo");
        return false;
    }

    char magic_number[3];
    fscanf(fp, "%2s", magic_number);
    fclose(fp);

    return strcmp(magic_number, "P1") == 0;
}

int ler_pbm(const char *nome_arquivo,
            int matriz[MAX_LINHAS][MAX_COLUNAS],
            int *linhas, int *colunas)
{
    FILE *fp = fopen(nome_arquivo, "r");
    if (!fp)
    {
        perror("Erro ao abrir");
        return 0;
    }

    char magic[3];
    fscanf(fp, "%2s", magic);
    if (strcmp(magic, "P1") != 0)
    {
        fclose(fp);
        return 0;
    }

    // Descarta até o fim da linha do cabeçalho
    int c;
    do
    {
        c = fgetc(fp);
    } while (c != '\n' && c != EOF);

    // Pula espaços e comentários antes das dimensões
    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '#')
        {
            while ((c = fgetc(fp)) != '\n' && c != EOF)
                ;
            continue;
        }
        if (isspace(c))
            continue;
        ungetc(c, fp);
        break;
    }

    // Lê dimensões
    if (fscanf(fp, "%d %d", colunas, linhas) != 2)
    {
        fclose(fp);
        return 0;
    }

    // Lê pixels
    for (int i = 0; i < *linhas; i++)
        for (int j = 0; j < *colunas; j++)
            if (fscanf(fp, "%d", &matriz[i][j]) != 1)
            {
                fclose(fp);
                return 0;
            }

    fclose(fp);

    // DEBUG: verifique se a matriz está correta
    //printf("DEBUG: matriz %d x %d lida:\n", *linhas, *colunas);

    return 1;
}

void exibirInstrucoes()
{
    printf("Uso: codificador [-? | -m | -f ARQUIVO]\n");
    printf("Codifica imagens binárias dadas em arquivos PBM ou por dados informados manualmente.\n");
    printf("\nArgumentos:\n");
    printf("-?, --help   : apresenta essa orientação na tela.\n");
    printf("-m, --manual : ativa o modo de entrada manual.\n");
    printf("-f, --file   : lê imagem de um arquivo PBM.\n");
}

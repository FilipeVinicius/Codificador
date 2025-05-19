#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h> // Manipulação de caractere

#define MAX_LINHAS 1024
#define MAX_COLUNAS 768

int linhas, colunas;
int matriz[MAX_LINHAS][MAX_COLUNAS];

//* Assinatura das funções (Logo abaixo na parte do corpo das funções está a explicação delas e dos seus respectivos parâmetros)
bool verificarMatrizHomogenea(int matriz[MAX_LINHAS][MAX_COLUNAS], int linhaInicio, int linhaFim, int colunaInicio, int colunaFim);
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
    scanf("%s", nomeComando);

    //* Caso o usuário deseje ajuda
    if (strcmp(nomeComando, "-?") == 0 || strcmp(nomeComando, "--help") == 0)
    {
        exibirInstrucoes();
    }

    //* Caso o usuário queira inputar o arquivo
    if (strcmp(nomeComando, "-f") == 0 || strcmp(nomeComando, "--file") == 0)
    {
        printf("Digite o nome do arquivo: ");
        scanf("%s", nome_arquivo);
        if (strlen(nome_arquivo) > 0)
        {
            //* Se não for um arquivo pbm válido
            if (!eh_arquivo_pbm(nome_arquivo))
            {
                printf("O arquivo '%s' NÃO é um PBM válido.\n", nome_arquivo);
                return 1;
            }

            //* Se ocorrer algum erro na leitura do arquivo PBM
            if (!ler_pbm(nome_arquivo, matriz, &linhas, &colunas))
            {
                printf("Erro ao ler o conteúdo do arquivo PBM.\n");
                return 1;
            }

            //* Impressão do código da imagem
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
    if (strcmp(nomeComando, "-m") == 0 || strcmp(nomeComando, "--manual") == 0)
    {

        printf("Digite a largura (linha) da imagem: ");
        scanf("%d", &linhas);

        printf("Digite a altura (coluna) da imagem: ");
        scanf("%d", &colunas);

        for (int i = 0; i < linhas; i++)
        {
            for (int j = 0; j < colunas; j++)
            {
                printf("Digite a dimensão matriz[%d][%d]: ", i, j);
                scanf("%d", &matriz[i][j]);
            }
        }

        cortarMatriz(linhas, colunas, matriz, 0, linhas, 0, colunas);
    }

    return 0;
}

//* Função booleana que verifica se a matriz ou submatriz é toda branca (0) ou toda preta (1)
//* Consiste em 5 parâmetros
/**
 ** 1: recebe a matriz na qual iremos fazer a verificação e os cortes
 ** 2: recebe o índice em que a linha da matriz começa
 ** 3: recebe o índice em que a linha da matriz acaba
 ** 4: recebe o índice em que a coluna da matriz inicia
 ** 5: recebe o índice em que a coluna da matriz acaba
 */
bool verificarMatrizHomogenea(int matriz[MAX_LINHAS][MAX_COLUNAS], int linhaInicio, int linhaFim, int colunaInicio, int colunaFim)
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
    return true; //* Caso nenhuma das condições seja atendida, quer dizer que a matriz é homogênea
}

//* Função que faz os cortes da matriz/submatriz não homogênea
//* Contém 7 parâmetros que seguem a mesma lógica dos parâmetros da função verificarMatrizHomogenea
void cortarMatriz(int linhasTot, int colunasTot, int matriz[MAX_LINHAS][MAX_COLUNAS], int linhaInicio, int linhaFim, int colunaInicio, int colunaFim)
{

    //* Essa função recursiva possui 2 casos bases

    //* O primeiro caso base da função recursiva, caso não tenha mais nenhuma linha/coluna a ser considerada
    if (linhaInicio >= linhaFim || colunaInicio >= colunaFim)
        return;

    //* O segundo caso base da função recursiva
    //* Se a matriz/submatriz for homogênea (somente 0 ou somente 1), então vai printar o "B" ou o "P"
    if (verificarMatrizHomogenea(matriz, linhaInicio, linhaFim, colunaInicio, colunaFim) == true)
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
    /* 
        * Os parâmetros representam onde cada quadrante está localizado, por isso eles variam
         *Os 3 primeiros parâmetros representam a quantidade total de linhas, colunas e a matriz em si
        * Os outros 4 mostram onde cada quadrante está localizado de acordo com o local onde cada linha/coluna começa e termina
        */ 
    if (linhaInicio < linhaMeio && colunaInicio < colunaMeio)
    {

        cortarMatriz(linhasTot, colunasTot, matriz, linhaInicio, linhaMeio, colunaInicio, colunaMeio); //* Quadrante 1 (superior esquerda)
    }

    if (linhaInicio < linhaMeio && colunaMeio < colunaFim)
    {
        cortarMatriz(linhasTot, colunasTot, matriz, linhaInicio, linhaMeio, colunaMeio, colunaFim); //* Quadrante 2 (superior direita)
    }

    if (linhaMeio < linhaFim && colunaInicio < colunaMeio)
    {
        cortarMatriz(linhasTot, colunasTot, matriz, linhaMeio, linhaFim, colunaInicio, colunaMeio); //* Quadrante 3 (inferior esquerda)
    }

    if (linhaMeio < linhaFim && colunaMeio < colunaFim)
    {
        cortarMatriz(linhasTot, colunasTot, matriz, linhaMeio, linhaFim, colunaMeio, colunaFim); //* Quadrante 4 (inferior direita)
    }
}

/**
 * *Verifica se o arquivo dado é um PBM no formato “P1” (ASCII).
 * *retorna true se o magic number for “P1”, false caso contrário ou em erro de leitura.
 */
bool eh_arquivo_pbm(char *arquivo)
{
    //* Tenta abrir o arquivo em modo leitura de texto
    FILE *fp = fopen(arquivo, "r");
    if (!fp)
    {
        // Caso falhe, imprime mensagem de erro no stderr e retorna false
        perror("Erro ao abrir o arquivo");
        return false;
    }

    char magic_number[3];   //* Buffer para armazenar o “magic number” (2 caracteres + '\0')
    fscanf(fp, "%2s", magic_number); //* Lê os dois primeiros caracteres do arquivo (ex.: "P1", "P4" etc.)
    fclose(fp); //* Fecha o arquivo logo após ler o cabeçalho

    //* Compara o magic number lido com "P1" e retorna resultado booleano
    return strcmp(magic_number, "P1") == 0;
}

int ler_pbm(const char *nome_arquivo,
            int matriz[MAX_LINHAS][MAX_COLUNAS],
            int *linhas, int *colunas)
{
    //* Tenta abrir o arquivo em modo leitura. Em caso de falha, emite mensagem de erro
    FILE *fp = fopen(nome_arquivo, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo");
        return 0;
    }

    char linha[256];

    //* Lê a primeira linha do arquivo, que deve conter o "magic number" P1
    //*    Se fgets retornar NULL, então houve erro
    if (!fgets(linha, sizeof(linha), fp)) {
        fclose(fp);
        return 0;
    }

    //* Verifica se o magic number é realmente "P1"
    //* se não for P1, então não é PBM no formato ASCII P1
    if (strncmp(linha, "P1", 2) != 0) {
        fclose(fp);
        return 0;
    }


    //* Pular comentários
    do {
        if (!fgets(linha, sizeof(linha), fp)) {
            fclose(fp);
            return 0;
        }
    //* repete enquanto a linha começar com '#'
    } while (linha[0] == '#'); 

    //* Extrai os dois inteiros da string: colunas (largura) & linhas (altura) 
    //* O retorno de sscanf (ler uma string já existente na memória (linha)) é a quantidade de valores lidos com sucesso
    //* Se não for exatamente 2, o formato está incorreto
    if (sscanf(linha, "%d %d", colunas, linhas) != 2) {
        fclose(fp);
        return 0;  // erro: não conseguiu ler duas dimensões válidas
    }

    //* Lê cada pixel binário (0 ou 1) e preenche a matriz
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            int pixel;  // valor temporário do pixel lido
            if (fscanf(fp, "%d", &pixel) != 1) {
                fclose(fp);
                return 0;  // erro: não conseguiu ler o pixel
            }
            matriz[i][j] = pixel; // armazena o pixel na matriz
        }
    }

    //* Fecha o arquivo e sinaliza sucesso
    fclose(fp);
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

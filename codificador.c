#include <stdio.h>
#include <stdbool.h>

#define MAX_LINHAS 1024
#define MAX_COLUNAS 768

//* Assinatura das funções (Logo abaixo na parte do corpo das funções está a explicação delas e dos seus respectivos parâmetros)
bool verificarMatrizHomogenea(int linhasTot, int colunasTot, int matriz[linhasTot][colunasTot], int linhaInicio, int linhaFim, int colunaInicio, int colunaFim);
void cortarMatriz(int linhasTot, int colunasTot, int matriz[linhasTot][colunasTot], int linhaInicio, int linhaFim, int colunaInicio, int colunaFim);

int main()
{
    // Matriz de teste
    int matriz1[3][5] = {
        {1, 1, 0, 1, 0},
        {1, 1, 0, 0, 1},
        {0, 0, 1, 1, 1}};

    // Outra matriz de teste
    int matriz2[2][2] = {
        {0, 0},
        {0, 1}};

    cortarMatriz(3, 5, matriz1, 0, 3, 0, 5); //* saída: XXPBPBXPBBPXBPP (exemplo do professor da matriz 3x5 que ele fez)
    printf("\n");
    cortarMatriz(2, 2, matriz2, 0, 2, 0, 2); //* saída: XBBBP (exemplo do professor)

    return 0;
}

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
bool verificarMatrizHomogenea(int linhasTot, int colunasTot, int matriz[linhasTot][colunasTot], int linhaInicio, int linhaFim, int colunaInicio, int colunaFim)
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
void cortarMatriz(int linhasTot, int colunasTot, int matriz[linhasTot][colunasTot], int linhaInicio, int linhaFim, int colunaInicio, int colunaFim)
{
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

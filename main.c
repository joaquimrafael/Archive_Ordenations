//bibliotecas utilizadas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// criação de struct para armazenamento das variaveis
typedef struct {
  float nota;
  char semestre[2];
  char nome[100];
  char disciplina[100];
  char periodo[2];
  char turma[2];
} Aluno;

// declaração de uma struct global para evitar o armazenamento na memória heap
 Aluno estudantes[200000];
Aluno L[200000], M[200000];
// declaração das funções
int leEntrada(FILE *entrada, Aluno estudantes[]);
void criaEntrada(FILE *entrada, Aluno estudantes[], int tamanho);
void BubbleSort(int min, int max, Aluno alunos[], long int *comparacoes);
void printarMenu();
void criaEntrada(FILE *entrada, Aluno estudantes[], int tamanho);
void merge(Aluno alunos[], int p, int q, int r, long int *comparacoes);
void mergeSort(Aluno alunos[], int l, int r, long int *comparacoes);

int main(void) {
  setlocale(LC_ALL, "Portuguese");
  // abertura dos arquivos
  FILE *entrada;
  FILE *saida;
  
  entrada = fopen("entrada.csv", "r+");
  saida = fopen("saida.csv", "w");
  
 
  // Verificando escolha de gerar ou não entrada
  int escolha;
  int count;
  do {
    printf("Ler entrada em entrada.csv (1) ou gerar uma nova entrada aleatória "
           "(2)? ");
    scanf("%d", &escolha);
  } while (escolha <= 0 || escolha > 2);
  switch (escolha) {
    //decide se vai abrir arquivo existente ou gerar um novo aleatório
  case 1:
    //le arquivo existente
    count = leEntrada(entrada, estudantes);
    break;
  case 2:
    //Criando arquivo, caso não exista
    entrada = fopen("entrada.csv", "w");
    if(entrada==NULL){
      printf("Erro ao criar o arquivo!");
      return(1);
    }
    //Pergunta o tamanho de entrada
    printf("Digite o tamanho de entrada desejado: ");
    scanf("%d", &escolha);
    count = escolha;
    criaEntrada(entrada, estudantes, escolha);
    break;
  }

  printarMenu(); // Chama o menu para permitir que o usuário escolha a opção de
                 // ordenação.
  scanf("%d", &escolha);

  // variaveis para armazenas comparações e tempo de execução
  long int comparacoesBubble = 0;
  long int comparacoesMerge = 0;
  clock_t comecoBubble;
  clock_t fimBubble;
  clock_t comecoMerge;
  clock_t fimMerge;
  switch(escolha) {

  case 1:
    //escolha de ordenação por BubbleSort
    comecoBubble = clock();
    BubbleSort(0, count, estudantes, &comparacoesBubble);
    fimBubble = clock();
    printf("\nAlgoritmo: Bubble Sort O(n^2) \n");
    printf("Tamanho Entrada: %d\n", count);
    printf("Tempo execução: %lf segundos\n", ((double)(fimBubble-comecoBubble)/ CLOCKS_PER_SEC));
    printf("Comparações (passos): %ld\n", comparacoesBubble);
    break;

  case 2:
    //escolha de ordenação por MergeSort
    comecoMerge = clock();
    mergeSort(estudantes, 0, count, &comparacoesMerge);
    fimMerge = clock();
    printf("\nAlgoritmo: Merge Sort O(nlogn) \n");
    printf("Tamanho Entrada: %d\n", count);
    printf("Tempo execução: %lf segundos\n", ((double)(fimMerge-comecoMerge)/ CLOCKS_PER_SEC));
    printf("Comparações (passos): %ld\n", comparacoesMerge);
    break;

  case 3:
    //Saída
    printf("\nSaindo...\n");
    return 0;

  default:
    //Caso inválido
    printf("Opção inválida!\n");
    return 1;
  }

  int i;
  for (i = 0; i < count; i++) {
    // escrita dos dados ordenados no arquivo de saida
    fprintf(saida, "%s,%s,%s,%s,%s,%.2f\n", estudantes[i].semestre,
            estudantes[i].turma, estudantes[i].periodo, estudantes[i].nome,
            estudantes[i].disciplina, estudantes[i].nota);
  }

  // fechando arquivos e finalizando o programa
  fclose(entrada);
  fclose(saida);
  system("pause");
  return 0;
}

int leEntrada(FILE *entrada, Aluno estudantes[]) {
  int count = 0;
  // leitura dos dados do arquivo
  while (fscanf(entrada, "%3[^,],%2[^,],%2[^,],%99[^,],%99[^,],%f\n",
                estudantes[count].semestre, estudantes[count].turma,
                estudantes[count].periodo, estudantes[count].nome,
                estudantes[count].disciplina, &estudantes[count].nota) != EOF) {
    count++;
  }
  return (count);
}

void criaEntrada(FILE *entrada, Aluno estudantes[], int tamanho) {
  // Criando combinções de possíveis entradas
  char semestre[][2] = {"1", "2", "3", "4", "5", "6", "7", "8"};
  char turma[][2] = {"A", "B"};
  char periodo[][2] = {"M", "N"};
  char nome[][50] = {"João",    "Maria",  "Maria",    "José",
                     "Camila",  "Celine", "Anderson", "Cássio",
                     "Beatriz", "Flávio", "Nadson",   "Amaury"};
  int ano[] = {2011, 2012, 2013, 2014, 2015, 2016,
               2017, 2018, 2019, 2020, 2021, 2020};
  char disciplinas[][20] = {"matemática", "português", "geografia"};
  float nota;

  for (int i = 0; i < tamanho; i++) {
    strcpy(estudantes[i].semestre, semestre[rand() % 8]);
    strcpy(estudantes[i].turma, turma[rand() % 2]);
    strcpy(estudantes[i].periodo, periodo[rand() % 2]);
    char aux[50];
    strcpy(aux, nome[rand() % 12]);
    strcpy(estudantes[i].nome, aux);
    strcpy(estudantes[i].disciplina, disciplinas[rand() % 3]);
    estudantes[i].nota = rand() % 11;
  }
  // Escrevendo no arquivo
  for (int i = 0; i < tamanho; i++) {
    fprintf(entrada, "%s,%s,%s,%s,%s,%.2f\n", estudantes[i].semestre,
            estudantes[i].turma, estudantes[i].periodo, estudantes[i].nome,
            estudantes[i].disciplina, estudantes[i].nota);
  }
}

// função para BubbleSort O(n^2) (compara os elementos adjacentes e leva o maior para o final))
void BubbleSort(int min, int max, Aluno alunos[], long int *comparacoes) {
  int i, j;
  //compara todos os elementos do vetor um por um percorrendo-o todo
  for (i = min; i < max - 1; i++) {
    for (j = min; j < max - i - 1; j++) {
      if (alunos[j].nota <= alunos[j + 1].nota) {
        Aluno temp = alunos[j];
        alunos[j] = alunos[j + 1];
        alunos[j + 1] = temp;
        *comparacoes = *comparacoes+1;
      }
    }
  }
}

// função para MergeSort O(nlogn) (separa em 2 partes, ordena e junta)
void mergeSort(Aluno alunos[], int l, int r, long int *comparacoes) {
  if (l < r) {
    int q;
    q = (l + r) / 2;
    //separa em 2 partes
    mergeSort(alunos, l, q, comparacoes);
    mergeSort(alunos, q + 1, r, comparacoes);
    //junta as duas partes
    merge(alunos, l, q, r, comparacoes);
  }
}
// função de junção dos subvetores ja ordenados
void merge(Aluno alunos[], int p, int q, int r, long int *comparacoes) {
  //define o tamanho dos dois vetores auxiliares
  int n1 = q - p + 1;
  int n2 = r - q;
  //cria vetores auxiliares
  
  //percorre o vetor principal adicionando em cada um dos auxiliares uma metade ordenada
  for (int i = 0; i < n1; i++)
    L[i] = alunos[p + i];
  for (int j = 0; j < n2; j++)
    M[j] = alunos[q + 1 + j];
  //define os índices dos vetores auxiliares
  int i, j, k;
  i = 0;
  j = 0;
  k = p;
  //percorre o vetor principal e adiciona em cada posição o menor valor
  while (i < n1 && j < n2) {
    if (L[i].nota >= M[j].nota) {
      alunos[k] = L[i];
      i++;
    } else {
      alunos[k] = M[j];
      j++;
    }
    *comparacoes = *comparacoes + 1;
    k++;
  }
  //percorre o vetor auxiliar que ainda possui elementos não adicionados e adiciona no final os     //elementos
  
  while (i < n1) {
    alunos[k] = L[i];
    i++;
    k++;
  }

  while (j < n2) {
    alunos[k] = M[j];
    j++;
    k++;
  }
}

// Menu de escolhas
void printarMenu() {
  printf("--Programa de Organização de turmas!--\n");
  printf("Escolha a opção e digite o número correspondente:\n");
  printf("1. Ordenação por nota Bubble Sort\n");
  printf("2. Ordenar por nota Merge Sort\n");
  printf("3. Sair\n");
}

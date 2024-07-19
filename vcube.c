// Autor: Matheus Yuji Morishita
  
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "smpl.h"
#include "cisj.h"

// Vamos definir os EVENTOS 
#define test 1
#define fault 2
#define recovery 3

#define RAIZ 0

// Vamos definir o descritor do processo

typedef struct{ 
  int id;            // identificador de facility do SMPL
  int *state;        // vetor que mantem o estado de cada processo
  int *cluster;      // vetor com os clusters de um processo
  // outras variaveis locais dos processos sao declaradas aqui!
} TipoProcesso;

TipoProcesso *processo;

void minimun_spanning_tree(int raiz, int tam_cluster, int N, int *estado) {
    // Verifica se a raiz está correta antes de imprimir
    if (estado[raiz] == 0) {
        printf("Processo %d está falho. Não pode ser considerado.\n", raiz);
        return;
    }

    // Imprime a raiz da árvore
    printf("Raiz: %d\n", raiz);

    // Percorre cada dimensão do vCube
    for (int s = 1; s <= tam_cluster; s++) {
        int primeiro_correto = -1;

        // Obtém o conjunto de nós do cluster na dimensão s
        node_set *nodo = cis(raiz, s);

        // Procura pelo primeiro processo correto no cluster
        for (int i = 0; i < nodo->size; i++) {
            if (estado[nodo->nodes[i]] == 1) {  // Verifica se o processo está correto
                primeiro_correto = nodo->nodes[i];
                break;
            }
        }

        // Se encontrar um processo correto
        if (primeiro_correto != -1) {
            // Imprime o filho correto
            printf("Filho: %d\n", primeiro_correto);

            if (s == 1) {
                // Se s é 1, significa que o filho é uma folha
                printf("Processo %d é uma folha.\n", primeiro_correto);
            } else {
                // Caso contrário, continua a construir a árvore recursivamente
                minimun_spanning_tree(primeiro_correto, s - 1, N, estado);
            }
        } else {
            printf("Nenhum processo correto encontrado na dimensão %d para o processo %d.\n", s, raiz);
        }

        // Libera a memória alocada para o conjunto de nós
        free(nodo->nodes);
        free(nodo);
    }
}

int main (int argc, char *argv[]) {
   
  static int N,   // nºmero de processos
            token,  // indica o processo que esta executando
            event, r, i, k,
            tam_cluster, first,
            MaxTempoSimulac = 150;
           
  static char fa_name[5];

  if (argc != 2) {
    puts("Uso correto: tempo <numero de processos>");
    exit(1);
  }

  N = atoi(argv[1]);

  smpl(0, "Um Exemplo de Simulacao");
  reset();
  stream(1);

  // inicializar processos

  tam_cluster = (int)log2(N); // calcula o tamanho do cluster baseado no numero de processos

  processo = (TipoProcesso *) malloc(sizeof(TipoProcesso)*N);

  int *estados = (int*) malloc(N*sizeof(int));

  for (i=0; i<N; i++) {
    memset(fa_name, '\0', 5);
    sprintf(fa_name, "%d", i);
    processo[i].id = facility(fa_name,1);
    processo[i].state = (int*) malloc(N*sizeof(int)); // aloca espaco para o vetor state de cada processo
    processo[i].cluster = (int*) malloc(tam_cluster*sizeof(int)); 
    estados[i] = 1;
    for (k = 0; k < N; k++) {
      processo[i].state[k] = -1;
    }  
    processo[i].state[i] = 0; // inizializa em 0 para o proprio processo

    // preenche o vetor cluster com o primeiro processo de cada cluster
    node_set *nodo;
    for (k = 0; k < tam_cluster; k++) {
      nodo = cis(i, k+1);
      processo[i].cluster[k] = nodo->nodes[0];
      free(nodo->nodes);
      free(nodo);
    }
  }
  // vamos fazer o escalonamento inicial de eventos

  // nossos processos vÃ£o executar testes em intervalos de testes
  // o intervalo de testes vai ser de 30 unidades de tempo
  // a simulaÃ§Ã£o comeÃ§a no tempo 0 (zero) e vamos escalonar o primeiro teste de todos os    
  //       processos para o tempo 30.0

  for (i=0; i<N; i++) {
    schedule(test, 30.0, i); 
  }

  // Teste 0
  //schedule(fault, 31.0, 1);
  //schedule(recovery, 61.0, 1);

  // Teste 1
  // Raiz = 0
  // tam_cluster = 2 e nenhum processo falho

  // Teste 2
  // Raiz = 0
  // tam_cluster = 3 e nenhum processo falho

  // Teste 3
  // Raiz = 0
  // tam_cluster = 3 e 1,2,4 e 7 processos falhos
  //schedule(fault, 31.0, 1);
  //schedule(fault, 31.0, 2);
  //schedule(fault, 31.0, 4);
  //schedule(fault, 31.0, 7);

  // Teste 4
  // Raiz = 7
  // tam_cluster = 3 e 0 processo falho
  //schedule(fault, 31.0, 0);

  // Teste 5 
  // Raiz = 0
  // tam_cluster = 4 e nenhum processo falho

  // agora vem o loop principal do simulador

  puts("===============================================================");
  puts("           Sistemas Distribuidos Prof. Elias");
  puts("          LOG do Trabalho Pratico 1, VCube");
  printf("   Este programa foi executado para: N=%d processos.\n", N); 
  printf("           Tempo Total de Simulacao = %d\n", MaxTempoSimulac);
  puts("===============================================================");

  while(time() < MaxTempoSimulac) {
    cause(&event, &token);
      switch(event) {
        case test: 
          if (status(processo[token].id) !=0) break; // se o processo esta falho, nao testa!
      
          // loop para percorrer os clusters do processo
          for (i = 0; i < tam_cluster; i++) {
            first = processo[token].cluster[i]; // o primeiro processo de cada cluster

            // testa se o primeiro processo do cluster esta correto
            if (status(processo[first].id) == 0) { 
              //printf("Processo %d testou o Processo %d CORRETO no tempo %4.1f\n", token, first, time());
              
              if (processo[token].state[first] % 2 == 1)
                processo[token].state[first]++;

              // recebe a informacao de qualquer novidade
              for (k = 0; k < N; k++) {         
                if ((processo[first].state[k] > processo[token].state[k])) {         
                  processo[token].state[k] = processo[first].state[k];
                  //printf("Processo %d recebeu INFORMACAO do Processo %d no tempo %4.1f\n", token, k, time());
                }
              }
            } else {
              //printf("Processo %d testou o Processo %d FALHO no tempo %4.1f\n", token, first, time());
              if (processo[token].state[first] % 2 == 0)
                processo[token].state[first]++;
            }
          } 

          printf("Vetor STATE do Processo %d: [", token);
          for (int k = 0; k < N; k++) {
            if (processo[token].state[k] % 2 == 0)
              printf (" CORRETO");
            else if (processo[token].state[k] % 2 == 1)
              printf (" FALHO");
            else if (processo[token].state[k] == -1)
              printf (" UNKNOWN");
          }
          printf (" ]\n");
          puts("");
          schedule(test, 30.0, token);
          break;
        case fault:
          r = request(processo[token].id, token, 0);
          estados[token] = 0;
          printf("Socooorro!!! Sou o processo %d  e estou falhando no tempo %4.1f\n", token, time());
          break;
        case recovery:
          release(processo[token].id, token);
          estados[token] = 1;
          printf("Viva!!! Sou o processo %d e acabo de recuperar no tempo %4.1f\n", token, time());
          schedule(test, 1.0, token);
          break;
      } // switch
  } // while
  minimun_spanning_tree(RAIZ, tam_cluster, N, estados);
} // tempo.c
# Minimun Spanning Tree

## Introdução
O objetivo deste trabalho é implementar o algoritmo de construção da árvore geradora mínima para um vCube e demonstrar a eficácia da solução. 
A árvore geradora mínima deve conectar todos os processos do vCube, levando em consideração os processos falhos.

## Desenvolvimento
Foi adicionada uma nova função `minimun_spanning_tree()` para construir a árvore sobre o vCube. Esta função itera sobre cada dimensão do vCube. Para cada dimensão s, ela obtém o conjunto de nós do cluster correspondente usando a função `cis(raiz, s)`. O conjunto de nós (nodo) é uma estrutura que contém todos os processos na dimensão s do vCube. A função busca pelo primeiro processo correto no conjunto de nós e, ao encontrar um processo correto, armazena seu identificador em primeiro_correto e interrompe a busca. Se um processo correto for encontrado, o código imprime o identificador do filho. Se s for 1, isso indica que o processo filho é uma folha da árvore, e a construção da árvore para esse ramo é concluída. Caso contrário, a função é chamada recursivamente para continuar a construção da árvore a partir do filho correto.

## Testes
Os testes foram conduzidos em cinco cenários distintos, e os resultados estão detalhados no diretório `test`. Para melhorar a legibilidade dos logs, algumas informações do vCube foram omitidas.
1. Raiz: 0; Dimensões do vCube: 2; nenhum processo falho;
2. Raiz: 0; Dimensões do vCube: 3; nenhum processo falho;
3. Raiz: 0; Dimensões do vCube: 3; Lista de processos falhos: 1, 2, 4, 7;
4. Raiz: 7; Dimensões do vCube: 3; Lista de processos falhos: 0;
5. Raiz: 0; Dimensões do vCube: 4; nenhum processo falho.
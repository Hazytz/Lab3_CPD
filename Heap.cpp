#include<iostream>
#include<tuple>
#include<string>
#include<vector>
#include<unordered_map>
#include<bits/stdc++.h>
#include<map>
#include<chrono>

using namespace std;

// Definições e variáveis globais necessárias
typedef int element_t;                                                          // tipo do elemento
typedef vector<element_t> array_t;                                              // tipo do array
typedef tuple<int, int, double> loginfo_t;                                      // armazena informações de desempenho <trocas, comparações, tempo em ms>

typedef std::mt19937 MyRNG;
MyRNG rng;
uint32_t seed_val;


void selectionsort(array_t&, loginfo_t&);
void heapsort(array_t&, loginfo_t&);
void buildheap(array_t&, loginfo_t&);
int filho_e(const array_t&, element_t);
int filho_d(const array_t&, element_t);
int pai(const array_t&, element_t);
void heapify(array_t&, element_t, int, loginfo_t&);
int heap_max(const array_t&, loginfo_t&);
int extract_max(array_t&, loginfo_t&);
void heap_insert(array_t&, element_t, loginfo_t&);
void swap(element_t* n1, element_t* n2);
void merge(const array_t&, const array_t&, array_t&, loginfo_t&);
void two_way_merge(const vector<array_t>, array_t&, loginfo_t&);
void multi_way_merge(const vector<array_t>, array_t&, loginfo_t&);
void mergesort(array_t&, loginfo_t&);

/*int main(void)
{
    rng.seed(seed_val);                                                         // inicializa semente de gera��o de n�meros aleat�rios
    uniform_int_distribution<> distrib(0, INT_MAX);                             // cria gerador com distribui��o uniforme entre 0 e max_INT
    loginfo_t loginfo;                                                          // armazena contadores de compara��es e trocas (ver typedef acima)

    int x, y, maxt;

    array_t array1;
    array_t array2;


    for(auto r = 1; r < 4; r++)
    {
        switch(r)
        {
        case 1:
            maxt = 100;
            cout << endl << "                   Tamanho 100: ";
            cout << endl;
            break;
        case 2:
            maxt = 1000;
            cout << endl << "                   Tamanho 1000: ";
            cout << endl;
            break;
        case 3:
            maxt = 10000;
            cout << endl << "                   Tamanho 10000: ";
            cout << endl;
            break;
        }
        for(x = 1; x < 4; x++)
        {
            switch(x)
            {
            case 1:
                for(auto i=0; i<maxt; i++)
                    array1.push_back(i);
                cout << endl << "          Random: ";
                cout << endl;
                break;
            case 2:
                for(auto i=0; i<maxt; i++)
                    array1.push_back(maxt-i);
                cout << endl << "          Decrescente: ";
                cout << endl;                               // gera n�meros em ordem decrescente
                break;
            case 3:
                for(auto i=0; i<maxt; i++)
                    array1.push_back(i);
                cout << endl << "          Crescente: ";
                cout << endl;                                       // gera n�meros em ordem crescente
                break;
            }
            //cout << endl;
            //cout << "Array desordenado: ";
            //for(auto i=0; i<maxt; i++)
            //    cout << array[i] << " ";
            cout << endl;
            for(y = 1; y < 2; y++)
            {
                for(auto i=0; i<maxt; i++)
                    array2.push_back(array1[i]);
                auto start = std::chrono::steady_clock::now();
                switch(y)
                {
                case 1:
                    heapsort(array2, loginfo);
                    cout << endl << "                Heapsort: ";
                    cout << endl;
                    cout << endl << "Array ordenado: ";
                    for(auto i=0; i<maxt; i++)
                        cout << array2[i] << " ";
                    extract_max(array2,loginfo);
                    cout << endl << "                Heapsort apos extract max: ";
                    cout << endl;
                    cout << endl << "Array ordenado: ";
                    for(auto i=0; i<maxt; i++)
                        cout << array2[i] << " ";
                    break;
                }
                auto finish = std::chrono::steady_clock::now();

                std::chrono::duration<double> elapsed_seconds = finish - start;

                extract_max(array2,loginfo);

                //cout << endl << "Array ordenado: ";
                //for(auto i=0; i<maxt; i++)
                //    cout << array2[i] << " ";

                cout << endl;
                cout << "Quantidade de trocas: " << get<0>(loginfo) << endl;
                cout << "Quantidade de comparacoes: " << get<1>(loginfo) << endl;
                cout << "Tempo total: " << elapsed_seconds.count() << "s" << endl;

            }
        }
    }

    return 0;
}*/

int main(void){
  loginfo_t loginfo;
  int qtd;
  array_t array1 = {1,2,3,4,5,6,7,8};

  buildheap(array1, loginfo);

  for(auto e:array1)
     cout << e << " ";
  cout << endl;

  extract_max(array1, loginfo);

  for(auto e:array1)
     cout << e << " ";
  cout << endl;

  heap_insert(array1, 8, loginfo);

  for(auto e:array1)
     cout << e << " ";
  cout << endl;

  array_t array2 = {9,8,7,6,5,4,3,1};
  heapsort(array2, loginfo);

  for(auto e:array2)
     cout << e << " ";
  cout << endl;

  return 0;
}


// ################################################
// Algoritmos de ordenação por Seleção
// ################################################

// Seleção direta
void selectionsort(array_t& array, loginfo_t& loginfo){
   int trocas = 0;
   int comparacoes = 0;
   element_t menorchave;
   int qtd_elementos = array.size()-1;

   for(auto i=0; i<qtd_elementos;i++){
      menorchave = i;
      for(auto j=i; j<qtd_elementos+1; j++){
         comparacoes = comparacoes + 1;
         if(array[j] < array[menorchave]){
            menorchave = j;
         }
      }
      if(menorchave != i){
         swap(array[i], array[menorchave]);
         trocas = trocas + 1;
      }
   }
   get<0>(loginfo)= trocas;
   get<1>(loginfo)= comparacoes;
}

// Heapsort e funções auxiliares
void heapsort(array_t& array, loginfo_t& loginfo){
   int trocas = 0;
   int comparacoes = 0;
   int heap_size = array.size();
   int qtd_elementos = heap_size-1;

   buildheap(array, loginfo);

   for(auto i = qtd_elementos; i>0; i--){
      trocas++;
      swap(array[i], array[0]);
      heap_size--;
      heapify(array, 0, heap_size, loginfo);
   }
   get<0>(loginfo)= trocas;
   get<1>(loginfo)= comparacoes;
}

// usada no heapsort
void buildheap(array_t& array, loginfo_t& loginfo){
   int ultimo_pai = (array.size()/2)-1;
   for(auto i=ultimo_pai; i>=0; i--)
      heapify(array, i, array.size(), loginfo);
}

int filho_e(const array_t& array, element_t elemento){
   return elemento*2+1;
}

int filho_d(const array_t& array, element_t elemento){
   return elemento*2+2;
}

int pai(const array_t& array, element_t elemento){
   return ((elemento-1)/2);
}

// ################################################
// Implementação dos seus algoritmos:

// heapify: verifica se o elemento na posição passada é um heap e se não for transforma-o em um
// parâmetros: array, índice do elemento a heapificar, tamanho do heap, dicionário de logs
void heapify(array_t& array, element_t elemento, int heap_size, loginfo_t& loginfo){
    int e = filho_e(array,elemento);
    int d = filho_d(array,elemento);
    int temp, trocas;
    int maior = elemento;
    if((e < heap_size) && (array[e] > array[maior]))
        maior = e;
    if((d < heap_size) && (array[d] > array[maior]))
        maior = d;
    int comparacoes = comparacoes + 2;;
    if(maior != elemento)
    {
        comparacoes++;
        temp = array[elemento];
        array[elemento] = array[maior];
        array[maior] = temp;
        trocas++;
        heapify(array, maior, heap_size , loginfo);
        get<0>(loginfo)= trocas;
        get<1>(loginfo)= comparacoes;
    }

}

int heap_max(array_t& heap, loginfo_t& loginfo){
    return heap[0];
}

int extract_max(array_t& heap, loginfo_t& loginfo){
    int max_h = heap[0];
    heap[0] = heap[heap.size()-1];
    heap.pop_back();
    heapify(heap, 0, heap.size(), loginfo);
   return max_h;
}

void heap_insert(array_t& heap, element_t elemento, loginfo_t& loginfo){
   heap.push_back(elemento);
   buildheap(heap,loginfo);
}
// ################################################
// Algoritmos de intercalação
// ################################################
void merge(const array_t& array1, const array_t& array2, array_t& array_final, loginfo_t& loginfo){
    int i = 0, j = 0, trocas = 0, comparacoes = 0;
    int qtd_a1 = array1.size();
    int qtd_a2 = array2.size();

    bool elementos = true;
    array_final.clear();
    while(i<qtd_a1 && j<qtd_a2){
       if(array1[i] <= array2[j])
          array_final.push_back(array1[i++]);
       else
          array_final.push_back(array2[j++]);
    }

    if(j<qtd_a2 && i>=qtd_a1)                                                   // array 1 terminou
       for(auto e=j;e<qtd_a2;e++)
          array_final.push_back(array2[e]);

    if(i<qtd_a1 && j>=qtd_a2)                                                   // array 2 terminou
       for(auto e=i;e<qtd_a1;e++)
          array_final.push_back(array1[e]);

    // TODO: atualizar loginfo
}

// Recebe uma lista de arrays e intercala-os 2 a 2
// retorna um array com o resultado da intercalação
void two_way_merge(const vector<array_t> arrays, array_t& array_final, loginfo_t& loginfo){
    array_final.clear();

    // a implementar pois o seguinte só concatena
    for(auto array:arrays)
       for(int e:array){
          array_final.push_back(e);
       }
    // TODO: atualizar loginfo
}

void multi_way_merge(const vector<array_t> arrays, array_t& array_final, loginfo_t& loginfo){
    array_final.clear();

    // a implementar pois o seguinte só concatena
    for(auto array:arrays)
       for(int e:array){
          array_final.push_back(e);
       }
    // TODO: atualizar loginfo
}

void mergesort(array_t& array, loginfo_t& loginfo){
    // a implementar
}

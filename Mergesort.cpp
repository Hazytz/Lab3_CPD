#include<iostream>
#include<tuple>
#include<string>
#include<vector>
#include<unordered_map>
#include<bits/stdc++.h>
#include<map>

using namespace std;

// Defini��es e vari�veis globais necess�rias
typedef int element_t;                                                          // tipo do elemento
typedef vector<element_t> array_t;                                              // tipo do array
typedef tuple<int, int, double> loginfo_t;                                      // armazena informa��es de desempenho <trocas, compara��es, tempo em ms>

struct nodo {
    array_t array;
    struct nodo* n[10];
}; typedef nodo nodo_t;

void buildheap_min(array_t&, loginfo_t&);
int filho_e(const array_t&, element_t);
int filho_d(const array_t&, element_t);
int pai(const array_t&, element_t);
void heapify_min(array_t&, element_t, int, loginfo_t&);
int heap_first(const array_t&, loginfo_t&);
int extract_first(array_t&, loginfo_t&);
void heap_insert_min(array_t&, element_t, loginfo_t&);


void merge(const array_t&, const array_t&, array_t&, loginfo_t&);
void two_way_merge(const vector<array_t>, array_t&, loginfo_t&);
void multi_way_merge(const vector<array_t>, array_t&, loginfo_t&);
void mergesort(array_t&, loginfo_t&);
void swap(element_t* n1, element_t* n2);

void RADIXsortLSD(array_t& array, loginfo_t& loginfo);
void RLSDcountSort(array_t& array, int e, loginfo_t& loginfo);
nodo_t* n_nodo(void);
void RADIXsortMSD(nodo_t* nodo, int e, array_t& s_array, loginfo_t loginfo);
int get_e(array_t array);
void coutingSort(array_t& array, array_t& array_final, loginfo_t loginfo);
int get_maxarr(array_t array);

int main(void){
  loginfo_t loginfo;
  int qtd;
  array_t array1 = {2,2,4,6,7,8,15,22};
  array_t array2 = {1,2,3,4,5,6,7,8,25,27,35,44};
  array_t array5 = {7,3,4,1,8,9,22,33,77,11,32,45,25,74,23,51,18,46};
  array_t array3, array4, s_array;
  nodo_t *nodo = n_nodo();


  for(auto e:array1)
     cout << e << " ";
  cout << endl;

  for(auto e:array2)
     cout << e << " ";
  cout << endl;

  merge(array1, array2, array3, loginfo);

  for(auto e:array3)
     cout << e << " ";
  cout << endl;

  vector<array_t> arrays = { array1, array2, array3};

  two_way_merge(arrays, array4, loginfo);
  for(auto e:array4)
     cout << e << " ";
  cout << endl;

  multi_way_merge(arrays, array4, loginfo);
  for(auto e:array4)
     cout << e << " ";
  cout << endl;

  coutingSort(array5, s_array, loginfo);


  for(auto e:s_array)
     cout << e << " ";
  cout << endl;


  return 0;
}

// ################################################
// Algoritmos de intercala��o
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
       comparacoes++;
       trocas++;
    }

    if(j<qtd_a2 && i>=qtd_a1)                                                   // array 1 terminou
    {
       for(auto e=j;e<qtd_a2;e++)
       {
          trocas++;
          array_final.push_back(array2[e]);
       }
    }
    comparacoes++;
    if(i<qtd_a1 && j>=qtd_a2)
    {
       for(auto e=i;e<qtd_a1;e++)
       {
           trocas++;
          array_final.push_back(array1[e]);
       }
    }
    comparacoes++;
    get<0>(loginfo)=trocas;
    get<1>(loginfo)=comparacoes;

}

// Recebe uma lista de arrays e intercala-os 2 a 2
// retorna um array com o resultado da intercala��o
void two_way_merge(const vector<array_t> arrays, array_t& array_final, loginfo_t& loginfo){
    array_final.clear();
    array_t array_intermed1, array_intermed2;
    int i = 0;
    bool s = false;
    for(i = 0; i < arrays.size(); i++)
    {
        if(s == false)
        {
            merge(arrays[i], array_intermed1, array_intermed2, loginfo);
            s = true;
        }
        else
        {
            merge(arrays[i], array_intermed2, array_intermed1, loginfo);
            s = false;
        }
    }

    if(s == false)
        for(int e:array_intermed1)
        {
            array_final.push_back(e);
        }
    else
        for(int e:array_intermed2)
        {
            array_final.push_back(e);
        }

}

void multi_way_merge(const vector<array_t> arrays, array_t& array_final, loginfo_t& loginfo){
    array_final.clear();
    int trocas = 0, comparacoes = 0;

    array_t aux;

    for(auto array:arrays)
        for(int e:array)
        {
            heap_insert_min(aux, e, loginfo);
        }
    for(int e:aux)
    {
        array_final.push_back(extract_first(aux,loginfo));
    }

    get<0>(loginfo)=trocas;
    get<1>(loginfo)=comparacoes;
}

void mergesort(array_t& array, loginfo_t& loginfo){
    array_t metade1, metade2;
    for(auto i = 0;i < array.size();i++)
    {
        if(i <=  (array.size()-1)/2)
            metade1.push_back(array[i]);
        else
            metade2.push_back(array[i]);
    }
    if(metade1.size() > 1)
        mergesort(metade1, loginfo);
    if(metade2.size() > 1)
        mergesort(metade2, loginfo);
    array.clear();
    merge(metade1,metade2,array,loginfo);
}


void buildheap_min(array_t& array, loginfo_t& loginfo){
   int ultimo_pai = (array.size()/2)-1;
   for(auto i=ultimo_pai; i>=0; i--)
      heapify_min(array, i, array.size(), loginfo);
}

void heapify_min(array_t& array, element_t elemento, int heap_size, loginfo_t& loginfo){
    int e = filho_e(array,elemento);
    int d = filho_d(array,elemento);
    int temp, trocas;
    int menor = elemento;
    if((e < heap_size) && (array[e] < array[menor]))
        menor = e;
    if((d < heap_size) && (array[d] < array[menor]))
        menor = d;
    int comparacoes = comparacoes + 2;;
    if(menor != elemento)
    {
        comparacoes++;
        temp = array[elemento];
        array[elemento] = array[menor];
        array[menor] = temp;
        trocas++;
        heapify_min(array, menor, heap_size , loginfo);
        get<0>(loginfo)= trocas;
        get<1>(loginfo)= comparacoes;
    }

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

int heap_first(array_t& heap, loginfo_t& loginfo){
    return heap[0];
}

int extract_first(array_t& heap, loginfo_t& loginfo){
    int max_h = heap[0];
    heap[0] = heap[heap.size()-1];
    heap.pop_back();
    heapify_min(heap, 0, heap.size(), loginfo);
   return max_h;
}

void heap_insert_min(array_t& heap, element_t elemento, loginfo_t& loginfo){
   heap.push_back(elemento);
   buildheap_min(heap,loginfo);
}

void RLSDcountSort(array_t& array, int e, loginfo_t& loginfo)
{
    int out[array.size()];
    int i, c[10] = { 0 };
    for (i = 0; i < array.size(); i++)
        c[(array[i] / e) % 10]++;

    for (i = 1; i < 10; i++)
        c[i] += c[i - 1];
    for (i = array.size() - 1; i >= 0; i--)
    {
        out[c[(array[i] / e) % 10] - 1] = array[i];
        c[(array[i] / e) % 10]--;
    }
    for (i = 0; i < array.size(); i++)
        array[i] = out[i];
}


void RADIXsortLSD(array_t& array, loginfo_t& loginfo)
{
    int m = array[0];
    for(auto i = 1; i < array.size(); i++)
        if (array[i] > m)
            m = array[i];
    for (int e = 1; m / e > 0; e *= 10)
        RLSDcountSort(array, e, loginfo);

}

nodo_t* n_nodo(void)
{
    nodo_t* temp = new nodo_t;

    for (int i = 0; i < 10; i++) {
        temp->n[i] = NULL;
    }
    return temp;
}

void RADIXsortMSD(nodo_t* nodo, int e, array_t& s_array, loginfo_t loginfo)
{
    if (e <= 0)
    {
        return;
    }
    int j;
    for (int i = 0; i < nodo->array.size(); i++)
    {
        j = (nodo->array[i] / e) % 10;
        if (nodo->n[j] == NULL)
            nodo->n[j] = n_nodo();
        nodo->n[j]->array.push_back(nodo->array[i]);
    }
    for (int i = 0; i < 10; i++)
    {
        if (nodo->n[i] != NULL)
        {
            if (nodo->n[i]->array.size()> 1)
            {
                RADIXsortMSD(nodo->n[i], e/10, s_array,loginfo);
            }
            else
            {
                s_array.push_back(nodo->n[i]->array[0]);
            }
        }
    }
}


int get_e(array_t array)
{
    int m = array[0];

    for (int i = 1; i < array.size(); i++) {
        if (array[i] > m) {
            m = array[i];
        }
    }
    int e = 1;
    while (m > 10) {
        m /= 10;
        e *= 10;
    }
    return e;
}

void coutingSort(array_t& array, array_t& array_final, loginfo_t loginfo)
{
    int m = get_maxarr(array);
    int c[m];
    for(auto i = 0; i <= m; i++)
        c[i]=0;
    for(auto i = 0; i < array.size(); i++)
    {
        c[array[i]] = c[array[i]] +1;
        array_final.push_back(0);
    }
    for(auto i = 1; i <= m; i++)
        c[i] = c[i] + c[i-1];
    for(auto i = array.size()-1; i >= 0; i--)
    {
        //cout << array_final[c[array[i]]-1] << " ";
        //cout << endl;
        array_final[c[array[i]]-1] = array[i];
        cout << array_final[c[array[i]]-1] << " ";
        cout << endl;
        c[array[i]]--;
    }
    return;
}

int get_maxarr(array_t array)
{
    int m = array[0];
    for (int i = 1; i < array.size(); i++)
        if (array[i] > m)
            m = array[i];
    return m;
}

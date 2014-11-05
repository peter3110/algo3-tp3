
#include <cstdio>
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <vector>
#define INF 100000007
using namespace std;

/* Funciones Auxiliares */
void read(int &n, int &m, int &k, vector< pair< pair<int, int>, double> > &G);
vector<int> greedy(int n, int m, int k, vector< pair< pair<int, int>, double> > &G);

/* Estructuras auxiliares */
struct nodoaux{
	int valor;
	vector<pair<int,int> > adyacentes;		// adyacentes.second tiene la longitud del eje que une nodo.valor con adyacentes.first
};

/* main */ 
int main() {
	
  /* para testear */
  freopen("input3.in", "r", stdin);
  
  /* leo la entrada */	
  int n, m, k;
  vector< pair< pair<int, int>, double> > G;
  read(n,m,k,G);
  
  /* resuelvo con la función greedy */
  vector<int> conjuntos = greedy(n,m,k,G);
  
  /* escribo el resulado en pantalla */
  for(int i=1; i<=n; i++) {cout << conjuntos[i] << " ";}
  cout << endl;
  
  return 0;	
}


/* Funciones Auxiliares */

void read(int &n, int &m, int &k, vector< pair< pair<int, int>, double> > &G) 
{
  cin >> n >> m >> k;
  int u, v;
  double w;
  for(int i=0; i<m; i++) 
  {
    cin >> u >> v >> w;
    G.push_back(make_pair(make_pair(u,v),w));
  }
}

vector<nodoaux> generar_grafo_nodos(vector< pair< pair<int, int>, double > > &G, int n) {
  vector<nodoaux> G2;
  for(int i=0; i<=n; i++) {
    nodoaux v;
    v.valor = i;
    G2.push_back(v);
  }
  for(int i=0; i<(int)G.size(); i++) {
    G2[G[i].first.first].adyacentes.push_back(make_pair(G[i].first.second,G[i].second));
    G2[G[i].first.second].adyacentes.push_back(make_pair(G[i].first.first,G[i].second));
  }
  return G2;
}

pair<double, double> calcular_hazard(int nodo, int k, vector<int> &conjuntos, vector<int> &tam_conjuntos, vector< pair< pair<int, int>, double> > &G)
{ 
  if(tam_conjuntos[k] == 0) { return make_pair(0,0); }
  double hazard = 0;
  for(int i=0; i<(int)G.size(); i++) {
      if(G[i].first.first == nodo && (conjuntos[k] & (1 << G[i].first.second)) != 0) { 
		  hazard += G[i].second;
	  }
	  if(G[i].first.second == nodo && (conjuntos[k] & (1 << G[i].first.first)) != 0) { 
		  hazard += G[i].second; 
	  }
  }
  double hazard2 = hazard/(tam_conjuntos[k]+1);
  
  //return make_pair(hazard,hazard);
  return make_pair(hazard2, hazard);
}

pair<double, double> calcular_hazard2(int nodo, int k, vector<int> &conjuntos, vector<int> &tam_conjuntos, vector<nodoaux> &G2)
{ /* para mejorar la complejidad y calcular hazard de forma lineal en |V(G)| en lugar de cuadrática */
  if(tam_conjuntos[k] == 0) { return make_pair(0,0); }
  double hazard = 0;
  
  /* busco el nodo "nodo" */
  nodoaux nodo_auxiliar;
  for(int i=1; i<(int)G2.size(); i++) {
	  if(G2[i].valor == nodo) {nodo_auxiliar = G2[i];}
  }
  /* devuelvo la suma de los hazards que genera */
  for(int i=0; i<(int)nodo_auxiliar.adyacentes.size(); i++) {
      if(((1 << nodo_auxiliar.adyacentes[i].first) & conjuntos[k]) != 0) {
	      hazard += nodo_auxiliar.adyacentes[i].second;
	  }
  }
  
  double hazard2 = hazard/(tam_conjuntos[k]+1);
  
  //return make_pair(hazard,hazard);
  return make_pair(hazard2, hazard);
}

int seleccionar_conjunto(int nodo, int k, vector<int> &conjuntos, double &peso_total,
						 vector<int> &tam_conjuntos, int n, vector< pair< pair<int, int>, double> > &G) 
{
  int indice_conj = -1;
  double valor_min = INF, peso_total_aux;
  pair<double, double> temp;
  vector<nodoaux> G2 = generar_grafo_nodos(G, n);
  
  /* me fijo a qué conjunto 'k' me conviene agregar el nodo 'nodo' para minimizar w_k/|S_k| */
  for(int j=0; j<k; j++) {
      // temp = calcular_hazard(nodo, j, conjuntos, tam_conjuntos, G);
      temp = calcular_hazard2(nodo,j,conjuntos,tam_conjuntos,G2);
      cout << "Hazard de agregar " << nodo << " al conjunto " << j+1 << ": " << temp.first << endl;
      if(temp.first < valor_min) {
	      indice_conj = j;
	      valor_min = temp.first;
	      peso_total_aux = temp.second;
	  }
  }
  
  /* actualizo conjuntos y tam_conjuntos y peso_total */
  peso_total = peso_total + peso_total_aux;
  conjuntos[indice_conj] += (1<<nodo);
  tam_conjuntos[indice_conj]++;
  cout << "Agrego nodo " << nodo << " al conjunto " << indice_conj+1 << endl;
  cout << "El conjunto " << indice_conj << " tiene los elementos: ";
  for(int i=1; i<=n; i++) { if((conjuntos[indice_conj] & (1<<i)) != 0) {cout << i << " ";}}cout << endl;
  
  /* chequeo errores */
  assert(indice_conj != -1);
  
  return indice_conj;
}

vector<int> greedy(int n, int m, int k, vector< pair< pair<int, int>, double> > &G)
{
  vector<int> res(n+1,0);	// res[0] tiene basura
  
  vector<int> conjuntos(k,0);		// guardo qué elementos tiene cada conjunto
  vector<int> tam_conjuntos(k,0);	// guardo tamaño de cada conjunto
  double peso_total = 0;
	
  /* agrego nodo i al cjto. k tal que minimizamos w_k/|S_k| */
  int prox_conj = 0;
  for(int i=0; i<n; i++) {
	prox_conj = seleccionar_conjunto(i+1, k, conjuntos, peso_total, tam_conjuntos, n, G);
	res[i+1] = prox_conj+1;
  }
  cout << "Peso total: " << peso_total << endl;
  return res;	
}

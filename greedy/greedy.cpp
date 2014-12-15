
#include <cstdio>
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <vector>
#define INF 100000007
using namespace std;


/* Estructuras auxiliares */
struct nodoaux{
	int valor;
	vector<pair<int,double> > adyacentes;		// adyacentes.second tiene la longitud del eje que une nodo.valor con adyacentes.first
	double suma_total_aristas;
	double max_arista_incidente;
};


/* Funciones Auxiliares */
void read(int &n, int &m, int &k, vector< pair< pair<int, int>, double> > &G);
vector<int> greedy(int n, int m, int k, vector< pair< pair<int, int>, double> > &G);
bool cmp(nodoaux a, nodoaux b);

/* main */ 
int main() {
	
  /* para testear */
  freopen("input1.in", "r", stdin);
  
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
    v.suma_total_aristas = 0;
    v.max_arista_incidente = 0;
    G2.push_back(v);
  }
  for(int i=0; i<(int)G.size(); i++) {
    G2[G[i].first.first].adyacentes.push_back(make_pair(G[i].first.second,G[i].second));
    G2[G[i].first.first].suma_total_aristas += G[i].second;
    G2[G[i].first.first].max_arista_incidente = max(G2[G[i].first.first].max_arista_incidente, G[i].second);
    
    G2[G[i].first.second].adyacentes.push_back(make_pair(G[i].first.first,G[i].second));
    G2[G[i].first.second].suma_total_aristas += G[i].second;
    G2[G[i].first.second].max_arista_incidente = max(G2[G[i].first.second].max_arista_incidente, G[i].second);
  }
  return G2;
}



pair<double, double> calcular_hazard(int nodo, int k, vector< vector <int> > &conjuntos, vector<int> &tam_conjuntos, vector<nodoaux> &G2)
{ 
	
  //~ if(G2[nodo+1].valor == 2) {cout << " << endl;}
	
  /* calculo hazard de forma lineal en |V(G)| */
  //~ if(tam_conjuntos[k] == 0) { return make_pair(0,0); }
  double hazard = 0;
  
  /* busco el nodo "nodo" */
  nodoaux nodo_auxiliar;
  for(int i=1; i<(int)G2.size(); i++) {
	  if(G2[i].valor == nodo) {nodo_auxiliar = G2[i]; }
  }
  
  /* devuelvo la suma de los hazards que genera */
  for(int i=0; i<(int)nodo_auxiliar.adyacentes.size(); i++) {
	  //~ cout << nodo_auxiliar.adyacentes[i].first << " ";
      if(conjuntos[k][nodo_auxiliar.adyacentes[i].first-1] == 1) {
	      hazard += nodo_auxiliar.adyacentes[i].second;
	  }
  }
  //~ cout << endl;
  
  return make_pair(hazard,hazard);
}

vector< vector<int> > actualizar_conjuntos(int nodo, int k, vector< vector<int> > &conjuntos, double &peso_total,
						 vector<int> &tam_conjuntos, int n, vector<nodoaux> &G2) 
{
  int indice_conj = -1;
  double valor_min = INF, peso_total_aux;
  pair<double, double> temp;
  
  
  /* me fijo a qué conjunto 'k' me conviene agregar el nodo 'nodo' para minimizar w_k */
  for(int j=0; j<k; j++) {
      temp = calcular_hazard(nodo,j,conjuntos,tam_conjuntos,G2);
      
      if(temp.first < valor_min) {
	      indice_conj = j;
	      valor_min = temp.first;
	      peso_total_aux = temp.second;
	  }
  }
  
  /* actualizo conjuntos, tam_conjuntos y peso_total */
  peso_total = peso_total + peso_total_aux;
  conjuntos[indice_conj][nodo-1] = 1;
  
  //~ cout << "Agrego " << nodo << " a " << indice_conj << endl;
  
  /* escribo conjuntos */
  //~ for(int i=0; i<(int)conjuntos.size(); i++) {
	  //~ for(int j=0; j<(int)conjuntos[0].size(); j++) {
		  //~ cout << conjuntos[i][j] << " " ;  
	  //~ }  
	  //~ cout << endl;
  //~ }
  //~ cout << endl;
  
  tam_conjuntos[indice_conj]++;
 
  /* chequeo errores */
  assert(indice_conj != -1);
  
  return conjuntos;
}

vector<int> greedy(int n, int m, int k, vector< pair< pair<int, int>, double> > &G)
{
  vector<int> res(n+1,0);	// res[0] tiene basura
  
  vector< vector<int> > conjuntos(k, std::vector<int>(n,0));		// guardo qué elementos tiene cada conjunto
  vector<int> tam_conjuntos(k,0);	// guardo tamaño de cada conjunto
  double peso_total = 0;
	
  /* agrego nodo i al cjto. k tal que minimizamos w_k */
  vector< vector<int> > conj_nuevo;
  
  /* genero grafo y ordeno segun aristas */  
  vector<nodoaux> G2 = generar_grafo_nodos(G, n);
  sort(G2.begin() + 1, G2.end(), cmp);
  
  for(int i=1; i<=n; i++) {
	conj_nuevo = actualizar_conjuntos(G2[i].valor, k, conjuntos, peso_total, tam_conjuntos, n, G2);
  }
  
  for(int i=0; i<(int)conjuntos.size(); i++) {
	  for(int j=0; j<(int)conjuntos[0].size(); j++) {
		  if(conjuntos[i][j] == 1) {
			  res[j+1] = i+1;
		  }  
	  }  
  }
  
  cout << "Peso total: " << peso_total << endl;
  return res;	
}

bool cmp(nodoaux a, nodoaux b)
{  /* Ordeno de menor a mayor */
  double temp1 = a.max_arista_incidente;
  double temp2 = b.max_arista_incidente;
  
  if(temp1 == temp2) {
	  return (a.suma_total_aristas >= b.suma_total_aristas);
  }
  return (temp1 > temp2);
}

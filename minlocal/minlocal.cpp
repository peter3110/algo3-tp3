
#include <cstdio>
#include <stdlib.h>
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
};

/* Funciones Auxiliares */
void read(int &n, int &m, int &k, vector< pair< pair<int, int>, double> > &G);
pair<vector<int>, double> minlocal(int n, int m, int k, vector< pair< pair<int, int>, double> > &G, int sc_iniciales, int vecindad);
pair<vector<int>, double> solucion_inicial_aleatoria(int n, int k);
pair<vector<int>, double> hallar_minimo(int n, int m, int k, vector< pair< pair<int, int>, double> > &G, pair<vector<int>, double> &v, int vecindad);
vector<nodoaux> generar_grafo_nodos(vector< pair< pair<int, int>, double > > &G, int n);

/* main */ 
int main() {
	
  /* para testear */
  freopen("input.in", "r", stdin);
  
  /* leo la entrada */	
  int n, m, k;
  vector< pair< pair<int, int>, double> > G;
  read(n,m,k,G);
  
  /* Determino con que tipo de vecindad resolver */
  int vecindad = 1;
  
  /* resuelvo con la función minlocal */
  pair<vector<int>, double> ans = minlocal(n,m,k,G,1,vecindad);
  vector<int> conjuntos = ans.first;
  double costo = ans.second;
  
  /* escribo el resulado en pantalla */
  cout << "Utilizando la vecindad tipo " << vecindad << ": " << endl;
  cout << "Costo final: " << costo << endl;
  cout << "Distribucion de nodos: ";
  for(int i=1; i<=n; i++) {cout << conjuntos[i] << " ";}
  cout << endl;
  
  return 0;	
}


/* Funciones Auxiliares */

int procesar_vecindad2(int n, int m, int k, vector<nodoaux> &G, pair<vector<int>, double> &v) 
{
  /* para cada par de nodos, me fijo si intercambiándolos de conjunto mejoro el costo. En ese caso, los 
   * intercambio de cjto. y devuelvo 1. Sino, los dejo como están y devuelvo 0. */
  
  
  return 0;
}

double calcular_costo( vector<nodoaux> &G, int n, pair<vector<int>, double> &v, int nodo_i )
{
  double res = 0.0;
  for(int j=0; j<(int)G[nodo_i].adyacentes.size(); j++) {
	   if(v.first[G[nodo_i].valor] == v.first[G[nodo_i].adyacentes[j].first]) {		// estan en el mismo conj.
		  res += G[nodo_i].adyacentes[j].second;
	   } 
  }
  return res;
}

int procesar_vecindad1( int n, int m, int k, vector<nodoaux> &G, pair<vector<int>, double> &v ) 
{
  /* para cada nodo, me fijo si cambiándolo de conjunto mejoro 
   * el costo. En ese caso, lo cambio de cjto. y devuelvo 1. Si no hay ningún nodo que cumpla esto, devuelvo 0. */
  int res = 0;
  for(int i=1; i<=n; i++) {
	  for(int j=1; j<=k; j++) {
		   int meguardo = v.first[i];
		   double costo_de_nodo_viejo = calcular_costo(G, n, v, i);	  // O(n)
		   
		   v.first[i] = j;											  // cambio de conjunto al nodo i para ver que pasa
		   double costo_de_nodo_nuevo = calcular_costo(G, n, v, i);	  // O(n)
		    
		   if(costo_de_nodo_nuevo < costo_de_nodo_viejo) { 
			   //~ cout << "Costo nodo viejo: " << costo_de_nodo_viejo << endl;
			   //~ cout << "Costo nodo nuevo: " << costo_de_nodo_nuevo << endl;
			   res = 1; 
			   v.second = v.second - costo_de_nodo_viejo + costo_de_nodo_nuevo;   // actualizo costo
			   //~ cout << "Costo nuevo: " << v.second << endl;
			   //~ cout << costo_de_nodo_nuevo << " es mejor que " << costo_de_nodo_viejo << endl;
			   //~ cout << "Nodo " << i << ": " << meguardo << " -> " << j << endl;
			   //~ for(int i=1; i<(int)v.first.size(); i++) {cout << v.first[i] << " ";}cout << endl;
		   
		   }			                      // no hago nada xq ya modifique el cjto del nodo i. 
		   else {
			   v.first[i] = meguardo;											  // actualizo conjuntos
		  }
	  }  
  }
  
  return res;
}

pair<vector<int>, double> hallar_minimo(int n, int m, int k, vector<nodoaux> &G2, pair<vector<int>, double> &v, int vecindad) 
{
  int hay_vecindad_mejor = 1;
  while(hay_vecindad_mejor) {
	  if(vecindad == 1) {
		 hay_vecindad_mejor = procesar_vecindad1(n,m,k,G2,v);
	  } else if (vecindad == 2) {
	     hay_vecindad_mejor = procesar_vecindad2(n,m,k,G2,v);  
	  }
  }
  
  return v;
}

double calcular_costo_conj_inicial(vector<int> &conjuntos, vector<nodoaux> &G2) {		// O(n*n)
  double res = 0;
  for(int i=1; i<(int)conjuntos.size(); i++) {
	  for(int j=0; j<(int)G2[i].adyacentes.size(); j++) {
	        if(conjuntos[G2[i].valor] == conjuntos[G2[i].adyacentes[j].first]) {
			    res += G2[i].adyacentes[j].second;
			}
	  }  
  }
  
  return res/2.0;	
}

pair<vector<int>, double> solucion_inicial_aleatoria(int n, int k, vector<nodoaux> &G2) 
{
  srand(time(NULL));
  vector<int> conjuntos;
  for(int i=0; i<=n; i++) {
	  int temp = rand() % k + 1;
      conjuntos.push_back(temp);
  }
  double costo = calcular_costo_conj_inicial(conjuntos, G2);
  cout << "Costo inicial: " << costo << endl;
  return make_pair(conjuntos,costo);
}

pair<vector<int>, double> minlocal(int n, int m, int k, vector< pair< pair<int, int>, double> > &G, int sc_iniciales, int vecindad) 
{
  vector<nodoaux> G2 = generar_grafo_nodos(G,n);
  pair<vector<int>, int> res;
  for(int sc=0; sc<sc_iniciales; sc++) {
	  pair<vector<int>, double> v = solucion_inicial_aleatoria(n,k,G2);
	  
	  for(int i=1; i<(int)v.first.size(); i++) {cout << v.first[i] << " ";}cout << endl;
	  
	  res = v;
	  if(hallar_minimo(n,m,k,G2,v,vecindad).second < res.second) {		 // hallar_minimo modifica v por referencia
		  res = v;
	  }  
  }
  
  return res;
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

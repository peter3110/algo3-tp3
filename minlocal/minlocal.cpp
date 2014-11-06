
#include <cstdio>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <vector>
#define INF 100000007
using namespace std;

/* Funciones Auxiliares */
void read(int &n, int &m, int &k, vector< pair< pair<int, int>, double> > &G);
pair<vector<int>, double> minlocal(int n, int m, int k, vector< pair< pair<int, int>, double> > &G, int sc_iniciales);
pair<vector<int>, double> solucion_inicial_aleatoria(int n, int k);
pair<vector<int>, double> hallar_minimo(int n, int m, int k, vector< pair< pair<int, int>, double> > &G, pair<vector<int>, double> &v);


/* Estructuras auxiliares */
struct nodoaux{
	int valor;
	vector<pair<int,int> > adyacentes;		// adyacentes.second tiene la longitud del eje que une nodo.valor con adyacentes.first
};

/* main */ 
int main() {
	
  /* para testear */
  freopen("input.in", "r", stdin);
  
  /* leo la entrada */	
  int n, m, k;
  vector< pair< pair<int, int>, double> > G;
  read(n,m,k,G);
  
  /* resuelvo con la función minlocal */
  pair<vector<int>, double> ans = solucion_inicial_aleatoria(n,k);//minlocal(n,m,k,G,n);
  vector<int> conjuntos = ans.first;
  double costo = ans.second;
  
  /* escribo el resulado en pantalla */
  cout << "Costo total: " << costo << endl;
  for(int i=1; i<=n; i++) {cout << conjuntos[i] << " ";}
  cout << endl;
  
  return 0;	
}


/* Funciones Auxiliares */

pair<vector<int>, double> hallar_minimo(int n, int m, int k, vector< pair< pair<int, int>, double> > &G, pair<vector<int>, double> &v) 
{
  
  
  return v;
}

pair<vector<int>, double> solucion_inicial_aleatoria(int n, int k) 
{
  double costo = INF;
  vector<int> conjuntos;
  for(int i=0; i<=n; i++) {
	  int temp = rand() % k + 1;
      conjuntos.push_back(temp);
  }
  return make_pair(conjuntos,costo);
}

pair<vector<int>, double> minlocal(int n, int m, int k, vector< pair< pair<int, int>, double> > &G, int sc_iniciales) 
{
  pair<vector<int>, int> res; res.second = INF;
  for(int sc=0; sc<sc_iniciales; sc++) {
	  pair<vector<int>, double> v = solucion_inicial_aleatoria(n,k);
	  if(hallar_minimo(n,m,k,G,v).second < res.second) {		 // hallar_minimo modifica v por referencia
		  res = v;
	  }  
  }
  
  return res;
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

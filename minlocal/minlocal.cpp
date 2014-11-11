
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
vector<int> cantidadPorConjunto(vector<int> &v, int k);
double calcular_costo_conj_inicial(vector<int> &conjuntos, vector<nodoaux> &G2, int vecindad, vector<int> &cant_por_conj);
int procesar_vecindad( int n, int m, int k, vector<nodoaux> &G, pair<vector<int>, double> &v , int vecindad, vector<int> &cant_por_conj);
double calcular_costo1( vector<nodoaux> &G, int n, pair<vector<int>, double> &v, int nodo_i );
double calcular_costo2( vector<nodoaux> &G, int n, pair<vector<int>, double> &v, int nodo_i );
double calcular_costo3( vector<nodoaux> &G, int n, pair<vector<int>, double> &v, int nodo_i );

/* main */ 
int main() {
	
  /* para testear */
  freopen("input.in", "r", stdin);
  
  /* leo la entrada */	
  int n, m, k;
  vector< pair< pair<int, int>, double> > G;
  read(n,m,k,G);
  
  /* Determino con que tipo de vecindad resolver */
  int vecindad = 3;
  
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

double calcular_costo1( vector<nodoaux> &G, int n, pair<vector<int>, double> &v, int nodo_i )		// solo considero la suma
{
  double res = 0.0;
  for(int j=0; j<(int)G[nodo_i].adyacentes.size(); j++) {
	   if(v.first[G[nodo_i].valor] == v.first[G[nodo_i].adyacentes[j].first]) {		// estan en el mismo conj.
		  res += G[nodo_i].adyacentes[j].second;
	   } 
  }
  return res;
}

double calcular_costo2( vector<nodoaux> &G, int n, pair<vector<int>, double> &v, int nodo_i, vector<int> &cant_por_conj )		// considero suma/tamaño(conj)
{
  double res = 0.0;
  int conjunto_del_nodo = v.first[G[nodo_i].valor];
  for(int j=0; j<(int)G[nodo_i].adyacentes.size(); j++) {
	   if(v.first[G[nodo_i].valor] == v.first[G[nodo_i].adyacentes[j].first]) {		// estan en el mismo conj.
		  res += G[nodo_i].adyacentes[j].second;
	   } 
  }
  return res/(double)cant_por_conj[conjunto_del_nodo];
}

double calcular_costo3( vector<nodoaux> &G, int n, pair<vector<int>, double> &v, int nodo_i )		// considero suma/(cantNodosAdyacentes)
{
  double res = 0.0;
  for(int j=0; j<(int)G[nodo_i].adyacentes.size(); j++) {
	   if(v.first[G[nodo_i].valor] == v.first[G[nodo_i].adyacentes[j].first]) {		// estan en el mismo conj.
		  res += (G[nodo_i].adyacentes[j].second/(double)G[nodo_i].adyacentes.size());
	   } 
  }
  return res;
}


int procesar_vecindad( int n, int m, int k, vector<nodoaux> &G, pair<vector<int>, double> &v , int vecindad, vector<int> &cant_por_conj) 
{
  /* para cada nodo, me fijo si cambiándolo de conjunto mejoro 
   * el costo. En ese caso, lo cambio de cjto. y devuelvo 1. Si no hay ningún nodo que cumpla esto, devuelvo 0. */
  int res = 0;
  for(int i=1; i<=n; i++) {
	  for(int j=1; j<=k; j++) {
		   int meguardo = v.first[i];
		   double costo_de_nodo_viejo, costo_de_nodo_nuevo;
		   
		   if(vecindad == 1) {
			   costo_de_nodo_viejo = calcular_costo1(G, n, v, i);	  // O(n), cambio de conjunto al nodo i para ver que pasa
			   v.first[i] = j;
			   cant_por_conj[j]++; cant_por_conj[meguardo]--;
			   costo_de_nodo_nuevo = calcular_costo1(G, n, v, i);	  // O(n)
			   
		   } else if(vecindad == 2) {
			   costo_de_nodo_viejo = calcular_costo2(G, n, v, i, cant_por_conj);	  // O(n), cambio de conjunto al nodo i para ver que pasa
			   v.first[i] = j;
			   cant_por_conj[j]++; cant_por_conj[meguardo]--;
			   costo_de_nodo_nuevo = calcular_costo2(G, n, v, i, cant_por_conj);	  // O(n)
			   
		   } else if(vecindad == 3) {
			   costo_de_nodo_viejo = calcular_costo3(G, n, v, i);	  // O(n), cambio de conjunto al nodo i para ver que pasa
			   v.first[i] = j;
			   cant_por_conj[j]++; cant_por_conj[meguardo]--;
			   costo_de_nodo_nuevo = calcular_costo3(G, n, v, i);	  // O(n)
		   }
		    
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
			   v.first[i] = meguardo;
			   cant_por_conj[j]--;							// dejo todo como estaba
			   cant_por_conj[meguardo]++;
		  }
	  }  
  }
  
  return res;
}

pair<vector<int>, double> hallar_minimo(int n, int m, int k, vector<nodoaux> &G2, pair<vector<int>, double> &v, int vecindad, vector<int> &cant_por_conj) 
{
  int hay_vecindad_mejor = 1;
  while(hay_vecindad_mejor) {
      hay_vecindad_mejor = procesar_vecindad(n,m,k,G2,v,vecindad,cant_por_conj);
  }
  
  return v;
}

double calcular_costo_conj_inicial(vector<int> &conjuntos, vector<nodoaux> &G2, int vecindad, vector<int> &cant_por_conj) {		// O(n*n)
  double res1 = 0, res2 = 0, res3 = 0, res4 = 0;
  for(int i=1; i<(int)conjuntos.size(); i++) {
	  int conjActual = conjuntos[G2[i].valor];
	  for(int j=0; j<(int)G2[i].adyacentes.size(); j++) {
	        if(conjuntos[G2[i].valor] == conjuntos[G2[i].adyacentes[j].first]) {
			    res1 += G2[i].adyacentes[j].second;
				res2 += (G2[i].adyacentes[j].second/(double)cant_por_conj[conjActual]); 
				res3 += (G2[i].adyacentes[j].second/(double)G2[i].adyacentes.size());
			}
	  } 
  }
  
  if(vecindad == 1) {return res1/2.0; }
  else if(vecindad == 2) {return res2/2.0;}
  else if(vecindad == 3) {return res3/2.0;}
  else return res4;
}

vector<int> cantidadPorConjunto(vector<int> &v, int k)
{
  vector<int> res(k+1,0);
  for(int i=1; i<(int)v.size(); i++) {
	  res[v[i]]++;
  }
  return res;	
}

vector<int> solucion_inicial_aleatoria(int n, int k, vector<nodoaux> &G2, int vecindad) 
{
  srand(time(NULL));
  vector<int> conjuntos;
  for(int i=0; i<=n; i++) {
	  int temp = rand() % k + 1;
      conjuntos.push_back(temp);
  }
  return conjuntos;
}


pair<vector<int>, double> minlocal(int n, int m, int k, vector< pair< pair<int, int>, double> > &G, int sc_iniciales, int vecindad) 
{
  vector<nodoaux> G2 = generar_grafo_nodos(G,n);
  pair<vector<int>, int> res;
  for(int sc=0; sc<sc_iniciales; sc++) {
	  
	  //~ pair<vector<int>, double> v = solucion_inicial_aleatoria(n,k,G2,vecindad);
	  
	  vector<int> v1_conj         = solucion_inicial_aleatoria(n,k,G2,vecindad);
	  vector<int> cant_por_conj   = cantidadPorConjunto(v1_conj, k);
	  double v2_costo             = calcular_costo_conj_inicial(v1_conj,G2,vecindad,cant_por_conj);
	  cout << "Costo inicial: " << v2_costo << endl;
	  
	  pair<vector<int>, double> v = make_pair(v1_conj,v2_costo);
	  
	  /* Escribo el conjunto aleatorio inicial */
	  for(int i=1; i<(int)v.first.size(); i++) {cout << v.first[i] << " ";}cout << endl;
	  
	  res = v;
	  if(hallar_minimo(n,m,k,G2,v,vecindad,cant_por_conj).second < res.second) {		 // hallar_minimo modifica v por referencia
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

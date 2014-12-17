
#include <cstdio>
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <vector>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#define INF 100000007
#define MAX_ITER 25


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
vector<int> greedy(int n, int m, int k, vector< pair< pair<int, int>, double> > &G, int beta);
bool cmp(nodoaux a, nodoaux b);

pair<vector<int>, double> minlocal(int n, int m, int k, vector< pair< pair<int, int>, double> > &G, vector<int> &conjuntos, int vecindad) ;

/* main */
int main() {

  /* para testear */
  freopen("time_17", "r", stdin);
  int beta = 5;
  
  /* leo la entrada */
  int n, m, k;
  vector< pair< pair<int, int>, double> > G;
  read(n,m,k,G);
  int iter = 0;
  vector<int> conjuntos, mejorSolucion;
  double mejorPeso = INF;
  
  while(iter < MAX_ITER){
	  
	  /* resuelvo con la función greedy randomizada */
	  conjuntos = greedy(n,m,k,G,beta);
	  
	  /* Intento mejorar solucion de greedy con vecindad 1 */
	  pair<vector<int>, double> solucion_mejorada_vecindad1 = minlocal(n,m,k,G,conjuntos,1);
	  pair<vector<int>, double> solucion_mejorada_vecindad2 = minlocal(n,m,k,G,conjuntos,2);
	  
	  if(solucion_mejorada_vecindad1.second < solucion_mejorada_vecindad2.second) {
			mejorSolucion = solucion_mejorada_vecindad1.first;
			mejorPeso = solucion_mejorada_vecindad1.second;
			cout << "encuentro mejor solucion con vecindad 1" << endl;
	  } else {
			if(solucion_mejorada_vecindad2.second < solucion_mejorada_vecindad1.second) {cout << "encuentro mejor solucion con vecindad 2" << endl;}
			mejorSolucion = solucion_mejorada_vecindad2.first;
			mejorPeso = solucion_mejorada_vecindad1.second;
	  }
	  
	  ///if (LOCAL(CONJUNTOS) < peso(conjuntos)) { conjuntos = LOCAL(CONJUNTOS)}

	  iter++;
}

  /* escribo el resulado en pantalla */
  for(int i=1; i<=n; i++) {cout << mejorSolucion[i] << " ";} cout << endl;
  cout << "Peso Total: " << mejorPeso << endl;
  cout << endl;

  return 0;
}




// =====================================================================
/* Funciones Auxiliares de GREEDY */

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
						 vector<int> &tam_conjuntos, int n, vector<nodoaux> &G2, int beta)
{
  int indice_conj = -1;
  double valor_min = INF, peso_total_aux;
  pair<double, double> temp;

vector<pair<int, pair<double, double> > >RCL(beta, make_pair(-1, make_pair(valor_min,valor_min))); /// EN LA RCL ESTA (CONJ, (DOUBLE, DOUBLE) )


  /* me fijo a qué conjunto 'k' me conviene agregar el nodo 'nodo' para minimizar w_k */
  for(int j=0; j<k; j++) {
      temp = calcular_hazard(nodo,j,conjuntos,tam_conjuntos,G2);



      for (int ind=0; ind<(int)RCL.size();++ind){             /// me fijo si está entre los beta mejores

            if(temp.first < (RCL[ind].second).first){ /// temp.first < valor_min
              //  cout<<temp.first<<" "<<(RCL[ind].second).first<<endl;
                RCL[ind].first=j; ///ACTUALIZO CONJUNTO POSIBLE
                (RCL[ind].second).first=temp.first; ///ACTUALIZO VALOR
                (RCL[ind].second).second=temp.second; /// ACTUALIZO PESO
                break;
            }


        }



  }
  srand (time(NULL));
   int rnd_ind=rand() % RCL.size();
  while(RCL[rnd_ind].first <0){ rnd_ind=rand() % RCL.size(); ///garantizo indice valido TERMINA RANDOM

}

	      indice_conj = RCL[rnd_ind].first;

	      peso_total_aux =  (RCL[rnd_ind].second).second;

  /* actualizo conjuntos, tam_conjuntos y peso_total */
  peso_total = peso_total + peso_total_aux;
  conjuntos[indice_conj][nodo-1] = 1;

  //~ cout << "Agrego " << nodo << " a " << indice_conj << endl;

  /* escribo conjuntos */
  for(int i=0; i<(int)conjuntos.size(); i++) {
	  for(int j=0; j<(int)conjuntos[0].size(); j++) {
	//	  cout << conjuntos[i][j] << " " ;
	  }
	 // cout << endl;
  }
  //cout << endl;

  tam_conjuntos[indice_conj]++;

  /* chequeo errores */
  assert(indice_conj != -1);

  return conjuntos;
}

vector<int> greedy(int n, int m, int k, vector< pair< pair<int, int>, double> > &G, int beta)
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
	conj_nuevo = actualizar_conjuntos(G2[i].valor, k, conjuntos, peso_total, tam_conjuntos, n, G2, beta);
  }

  for(int j=0; j<(int)conjuntos.size(); j++) {
	  for(int i=0; i<(int)conjuntos[0].size(); i++) {
		  if(conjuntos[j][i] == 1) {
			  res[i+1] = j+1;
		  }
	  }
  }

  //~ cout << "Peso total: " << peso_total << endl;
  return res;
}

bool cmp(nodoaux a, nodoaux b)
{  /* Ordeno de menor a mayor */
  double temp1 = a.max_arista_incidente;
  double temp2 = b.max_arista_incidente;

  if(temp1 == temp2 && a.suma_total_aristas == b.suma_total_aristas) {
	  return a.adyacentes.size() > b.adyacentes.size();
  }

  if(temp1 == temp2) {
	  return (a.suma_total_aristas > b.suma_total_aristas);
  }
  return (temp1 > temp2);
}

// =====================================================================
/* Funciones Auxiliares de MINLOCAL */

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

int prox_conj(int conj, int k, int excepcion) { 
	int res = (conj+1)%k; 
	if (res == excepcion) {return res + 1;}
	return res;
}

int procesar_vecindad( int n, int m, int k, vector<nodoaux> &G, pair<vector<int>, double> &v , int vecindad, vector<int> &cant_por_conj) 
{
  int res = 0;
  if(vecindad == 1) {
	  /* para cada nodo, me fijo si cambiándolo de conjunto mejoro 
	   * el costo. En ese caso, lo cambio de cjto. y devuelvo 1. Si no hay ningún nodo que cumpla esto, devuelvo 0. */
	  for(int i=1; i<=n; i++) {
		  for(int j=1; j<=k; j++) {
			  int meguardo = v.first[i];
			  double costo_de_nodo_viejo, costo_de_nodo_nuevo;
			   
			  /* Siempre uso el costo1 */
			  costo_de_nodo_viejo = calcular_costo1(G, n, v, i);	  // O(n), cambio de conjunto al nodo i para ver que pasa
			  v.first[i] = j;
			  cant_por_conj[j]++; cant_por_conj[meguardo]--;
			  costo_de_nodo_nuevo = calcular_costo1(G, n, v, i);	  // O(n)
				
			   if(costo_de_nodo_nuevo < costo_de_nodo_viejo) { 
				   
				   res = 1; 
				   v.second = v.second - costo_de_nodo_viejo + costo_de_nodo_nuevo;   // actualizo costo

			   
			   }			                      // no hago nada xq ya modifique el cjto del nodo i. 
			   else {
				   v.first[i] = meguardo;
				   cant_por_conj[j]--;							// dejo todo como estaba
				   cant_por_conj[meguardo]++;
			  }
		  }  
	  }
  } else if (vecindad == 2) {
	 /* vecindad 2:  para cada nodo, me fijo si swappeandolo de conjunto con algún otro nodo mejoro el costo. En ese caso,
	  * los swappeo. También me fijo si agregando el nodo a un conjunto vacío se reduce el costo, y dependiendo de cuánto se 
	  * reduzca el costo, decido si swappeo los nodos o si agrego el nodo al conjunto vacío. */
	 for(int i=1; i<=n; i++) {
		 
		int nodo_modificado_1 = -1;
		int nodo_modificado_2 = -1;
		double costo_original = v.second;
		
		/* Calculo el mejor swap que puedo hacer */
		for(int j=1; j<=n && j!= i; j++) {
			double costo_de_nodo_viejo_1, costo_de_nodo_viejo_2; 
			double costo_de_nodo_nuevo_1, costo_de_nodo_nuevo_2;
			
			/* Siempre uso el costo1 */
			costo_de_nodo_viejo_1 = calcular_costo1(G,n,v,i);
			costo_de_nodo_viejo_2 = calcular_costo1(G,n,v,j);

			swap(v.first[i], v.first[j]);
			
			costo_de_nodo_nuevo_1 = calcular_costo1(G,n,v,i);
			costo_de_nodo_nuevo_2 = calcular_costo1(G,n,v,j);
			
			double costo_viejo = costo_de_nodo_viejo_1 + costo_de_nodo_viejo_2;
			double costo_nuevo = costo_de_nodo_nuevo_1 + costo_de_nodo_nuevo_2;
			
			if(costo_nuevo < costo_viejo) {
				res = 1;
				v.second = v.second - costo_viejo + costo_nuevo;
				nodo_modificado_1 = i;
				nodo_modificado_2 = j;
				break;
		    } else {
				swap(v.first[i], v.first[j]);
			}
	    }
	    
	    /* Calculo el mejor conjunto vacío al que puedo mandar algún nodo (si es que lo hay) - hay que verificar si se pudo
	     * mejorar con algún swappeo o no (ver nodo_modificado_1 o nodo_modificado_2) */
	    int conjunto_vacio = -1;
	    
		for(int t=1; t<=k; t++) {
			if(cant_por_conj[t] == 0) { conjunto_vacio = t; break; }
		}
		
	    
	    // Anulo el swappeo que había hecho. v.second tiene el costo que implicaría hacer el swappeo. costo_original tiene el costo sin swappeo
	    
	    int encontreMejorSeparado = 0;
	    pair<double, int> res;
	    res.first = v.second;				// valor inicial de res.first == costo haciendo el swap
	    
	    if(conjunto_vacio != -1) {
			
			if(nodo_modificado_1 != -1) { swap(v.first[nodo_modificado_1], v.first[nodo_modificado_2]); }
			
			for(int j=1; j<=n; j++) {									// me fijo qué nodo me conviene mandar al conjunto vacío
				
				double delta_costo_del_cambio = calcular_costo1(G,n,v,j);
				double costo_del_cambio = costo_original - delta_costo_del_cambio;
				
				if(costo_del_cambio < res.first) {			// si el costo separando un nodo es menor que haciendo el swappeo
															// y/o mejor que el ultimo nodo separado
						res.first = costo_del_cambio;
						res.second = j;
						encontreMejorSeparado = 1;
				}
			}
			
			if(encontreMejorSeparado == 0 && nodo_modificado_1 != -1) {
				swap(v.first[nodo_modificado_1], v.first[nodo_modificado_2]);	// si no encontre un nodo que convenga mandarlo al conjunto vacío antes
																			    // que hacer el swap Y habíamos hecho un swap
				//~ cout << "swapeo " << nodo_modificado_1 << " y " << nodo_modificado_2 << endl;
			} else {															// si sí encontré un nodo que convenga mandarlo al conj vacío..
				v.second = res.first;
				v.first[res.second] = conjunto_vacio;
				cant_por_conj[conjunto_vacio]++;
				//~ cout << "guardo " << v.second << endl;
			}
		}
		
		//~ if(nodo_modificado_1 != -1) {
			//~ cout << "swapeo " << nodo_modificado_1 << " y " << nodo_modificado_2 << endl;
		//~ }
	 }
	
  }
  return res;
}

pair<vector<int>, double> hallar_minimo(int n, int m, int k, vector<nodoaux> &G2, pair<vector<int>, double> &v, int vecindad, vector<int> &cant_por_conj) 
{
  int hay_vecindad_mejor = 1;
  int max_cant_iteraciones = MAX_ITER;
  int contador = 0;
  while(hay_vecindad_mejor && max_cant_iteraciones > 0) {
      hay_vecindad_mejor = procesar_vecindad(n,m,k,G2,v,vecindad,cant_por_conj);
      max_cant_iteraciones--;
      contador++;
  }
  //~ cout << "Cantidad de iteraciones: " << contador << endl;
  return v;
}

double calcular_costo_conj_inicial(vector<int> &conjuntos, vector<nodoaux> &G2) {		// O(n*n)
  double res1 = 0;
  for(int i=1; i<(int)conjuntos.size(); i++) {
	  for(int j=0; j<(int)G2[i].adyacentes.size(); j++) {
	        if(conjuntos[G2[i].valor] == conjuntos[G2[i].adyacentes[j].first]) {
			    res1 += G2[i].adyacentes[j].second;
			}
	  }
  } 
  
 return res1/2.0;
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


pair<vector<int>, double> minlocal(int n, int m, int k, vector< pair< pair<int, int>, double> > &G, vector<int> &conjuntos, int vecindad) 
{
	vector<nodoaux> G2 = generar_grafo_nodos(G,n);
	pair<vector<int>, int> res;
	  
	  vector<int> v1_conj         = conjuntos;
	  vector<int> cant_por_conj   = cantidadPorConjunto(v1_conj, k);
	  double v2_costo             = calcular_costo_conj_inicial(v1_conj,G2);
	  
	  //~ cout << "Costo inicial: " << v2_costo << endl;
	  
	  pair<vector<int>, double> v = make_pair(v1_conj,v2_costo);
	  
	  /* Escribo el conjunto aleatorio inicial */
	  
	  //~ for(int i=1; i<(int)v.first.size(); i++) {cout << v.first[i] << " ";}cout << endl;
	  
	  /* Resuelvo */
	  res = v;
	  if(hallar_minimo(n,m,k,G2,v,vecindad,cant_por_conj).second < res.second) {		 // hallar_minimo modifica v por referencia
		  res = v;
	  }  
  
  return res;
}



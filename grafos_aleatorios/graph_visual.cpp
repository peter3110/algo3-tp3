
#include <iostream>
#include <cstdio>
#include <assert.h>
#include <stdlib.h>
#define INF 100000007
using namespace std;

int proximo(int i, int cantElemsPorGrupo) {
  int res = i;
  while(!(res % cantElemsPorGrupo == 0)) {res++;}
  return res + 1;	
}

int proximo2(int i) {return i+1;}

int main(int argc, char ** argv)
{
  freopen(argv[1], "r", stdin);
  freopen(argv[2], "w", stdout);
  
  /* n = cantNodos, m = particiones, c = pesoMaximo 
   * Sólo voy a usar n */
  int n, m, c;
  cin >> n >> m >> c;
  
  /* Escribo para visualizar en .dot */
  int e1, e2, peso;
  cout << "graph {" << endl;
  while(cin >> e1 >> e2 >> peso) {
	  if(peso != 0) {
		cout << e1 << " -- " << e2 << ";" << endl;
	  }
  }
  cout << "}" << endl;

  return 0;
}

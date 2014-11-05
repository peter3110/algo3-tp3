
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
  freopen("input3.in", "w", stdout);
  int cantGrupos = atoi(argv[1]);
  int cantElemsPorGrupo = atoi(argv[2]);
  int cantNodos = atoi(argv[3]);
  int particiones = atoi(argv[4]);
  // int cantAristas = cantNodos*((cantGrupos-1)*cantElemsPorGrupo)/2;
  int cantAristas = cantNodos*(cantNodos-1)/2;
  
  cout << cantNodos << " " << cantAristas << " " << particiones << endl;	
  for(int i=1; i<=cantNodos; i++) {
	 for(int j=proximo2(i); j<=cantNodos; j++) {
	    if(i%2 == 0 && j%2 == 0) { cout << i << " " << j << " " << 101 << endl;}
        else if(i%2 == 1 && j%2 == 1) { cout << i << " " << j << " " << 101 << endl;}
        else { cout << i << " " << j << " " << 100 << endl;}
     }
  }

  return 0;
}

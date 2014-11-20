
#include <iostream>
#include <cstdio>
#include <assert.h>
#include <stdlib.h>
#include <string>
#define INF 100000007
using namespace std;

int proximo(int i, int cantElemsPorGrupo) {
  int res = i;
  while(!(res % cantElemsPorGrupo == 0)) {res++;}
  return res + 1;	
}

int main(int argc, char ** argv)
{
  freopen(argv[1], "w", stdout);
  int cantNodos = atoi(argv[2]);
  int particiones = atoi(argv[3]);			// busco k-PMP 
  int maxPeso = atoi(argv[4]);				// max peso de arista
  int threshold = atoi(argv[5]);			// cuantas aristas quiero? (aprox). Es un porcentaje
  
  int cantAristas = cantNodos*(cantNodos-1)/2;
  
  cout << cantNodos << " " << cantAristas << " " << particiones << endl;	
  for(int i=1; i<=cantNodos; i++) {
	 for(int j=i+1; j<=cantNodos; j++) {
		 
        int num = rand() % maxPeso + 1;
        //~ int num = 1;
        
        int decision = ((rand() % 100) < threshold);
        if(decision == true) {
			cout << i << " " << j << " " << num << endl; 
		} else {
			cout << i << " " << j << " " << 0 << endl; 
	    }
        
     }
  }

  return 0;
}

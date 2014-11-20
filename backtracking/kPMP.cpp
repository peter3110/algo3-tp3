#include<iostream>
#include <fstream>
#include<vector>


using namespace std;
struct nodo{
	int id;
	int part;
	};



void actualizarParticiones(vector<nodo>& nodos, vector<int>& particiones){

for (int i=0; i<(int)nodos.size();++i){
    if (nodos[i].id>=0){//si es un id valido
        particiones[i]=nodos[i].part;
    }
}

}

double sumaIntraParticion(vector<vector<pair<double,nodo> > >& graph, vector<nodo>& nodos){ // calcula cuanto hay que sumar o restar al peso de las k particiones si se node pasa a estar en part_new desde part_old
	   double res=0;
	   for (int i=0; i<(int)graph.size();++i){
        nodo nodo_i=nodos[i];
        if (nodo_i.id>=0) {///si el nodo tiene adjacentes --- estrategia2
        for(int j=0; j<(int)graph[i].size();++j){
            int nodo_adj_id=(graph[i][j].second).id;
            if(nodo_i.part == nodos[nodo_adj_id].part){
            res+=graph[i][j].first;
           // cout<<res<<endl;
            }
        }
        }

	   }

	   return res;
}


double kPMP(vector<vector<pair<double,nodo> > >& graph, int k, vector<nodo>::iterator node, vector<nodo>& nodos, double sumaParcial, vector<int>& particiones)
{
    /* base case: si estoy en el último nodo o si encontré un coloreo*/
    if (node == nodos.end() || sumaParcial==0)
        return sumaParcial;

    bool color=false; // flag para detectar un coloreo (peso=0)

	double rec; //variable para guardar lo que devuelve la recursión

    for (int c = 1; c <= k; c++){ //itera sobre las k posibles particiones

        (*node).part = c; //asigno la partición c

        double dif=sumaIntraParticion(graph, nodos); //calcula la suma del peso de las aristas inttraparticion


        if(dif<sumaParcial){ //SI ESTRICTAMENTE MEJORA EL MEJOR VALOR
            sumaParcial=dif;  // actualizo el mejor valor hasta el momento
            actualizarParticiones(nodos, particiones); //guardo estado de las particiones

         //   cout<<"nodo: "<<(*node).id<<" ahora en part: "<<c<<endl;
		//   cout<<"PMP mejorado a: "<<dif<<endl;
           rec=kPMP (graph, k, node+1,  nodos,sumaParcial, particiones); //llamo a la recusión para el nodo siguiente con los valores mejorados

           if (rec==0){
			   //~ cout<<"rama de 0"<<endl; 
			   color=true;break;
		   }///estrategia 1 - si encontre un peso 0, cierro todo.

           if (rec<sumaParcial){sumaParcial=rec;} //si la recursión resulta mejor que lo que tenía, actualizo solo el mejor valor porque el estado de las particiones ya se actualizó durante la recursión

        }else{ // SI NO PUDE MEJORAR

           rec=kPMP (graph, k, node+1,  nodos,sumaParcial, particiones); //hago la recursión sobre el siguiente nodo

           if (rec==0){
			   //~ cout<<"rama de 0"<<endl;
			   color=true;
			   break;
		   }///estrategia 1- si encontre un peso 0, cierro todo.

           if (rec<sumaParcial){sumaParcial=rec;} //si la recursión resulta mejor que lo que tenía, actualizo solo el mejor valor porque el estado de las particiones ya se actualizó durante la recursión

        }
       //particiones[(*node).id]=partAnterior;

        /* si asignar node a c no funcionó, lo devuelvo a la partición donde estaba */
        //   (*node).part=partAnterior;


        }



    if(color){return 0;} //cuando salgo de la recursión, si es por coloreo, devuelvo 0

    return sumaParcial; //si terminé de iterar sobre los k posibles conjuntos devuelvo a lo que llegué

}






int main(){

	int n,m,k;
	freopen("../grafos aleatorios/input.in", "r", stdin);
	cin>>n>>m>>k; //leo los 3 primeros parámetros


	int u,v;
	double w;
	double mejorHastaAhora=0;
	nodo temp;
	temp.id=-15;
	temp.part=1;
	//vector<bool> estan(n,false);
	vector<nodo> nodos(n,temp);//INCIALIZO LOS NODOS CON IDS INVALIDOS
	vector<vector<pair<double,nodo> > > graph(n);
	vector<int>particiones(n,1);
	//inicializo lista de listas que va a ser el grafo
	for (int i=0;i<m;++i){
			cin>>u>>v>>w;
			temp.id=v-1;
			if(nodos[v-1].id==-15){nodos[v-1]=temp;}
			graph[u-1].push_back(make_pair(w,temp)); //v es adyacente a u

			temp.id=u-1;
			if(nodos[u-1].id==-15){nodos[u-1]=temp;}
			//graph[v-1].push_back(make_pair(w,temp)); //NO GUARDO 2 VECES LA MISMA ADYACENCIA

			mejorHastaAhora+=w;
		}// mejor hasta ahora tiene la suma de todas las aristas (todos los nodos en la partición 1)
//	cout<<"SUMA DE LAS ARISTAS: "<<mejorHastaAhora<<" y hay :"<<nodos.size()<<endl;

    //    vector<nodo> nodos2=nodos;
    //vector<vector<pair<double,nodo> > > graph2=graph;
    //double mejorHastaAhora2=mejorHastaAhora;
	//double peso2=kPMP(graph2, k, nodos2.begin()+1, nodos2,mejorHastaAhora2);

	double peso=kPMP(graph, k, nodos.begin(), nodos,mejorHastaAhora, particiones);
	for (int i=0; i<(int)particiones.size();++i){cout<<particiones[i]<<" ";}
	cout<<endl;
	cout<<"Costo total: "<<peso<<endl;
	return 0;
}

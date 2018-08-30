#include <iostream>
#include <fstream>
#include<utility>
#include <vector>
#include <array>
#include <queue>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef pair<int, int> point;

//Lee imagen y la guarda en una matriz, guarda el número de filas en r y el de columnas en c.
void imageToMatrix(string filename, vector<vector<int> > &M, unsigned int &r, unsigned int &c){
    ifstream in_file;
    string trash;
    in_file.open(filename);
    in_file>>trash; //Palabra mágica de pgm.
    in_file >> c >> r;
    in_file>> trash; //Máximo valor dentro de la imagen.
    M.resize(r);
    for(int i=0; i<r; i++){
        M[i].resize(c);
        for(int j=0; j<c; j++){
            in_file >> M[i][j];
        }
    }
    in_file.close();
    in_file.clear();
}

//Algoritmo principal, inicia el flujo en el punto dado y continua hacia vecinos de su mismo color.
//Guarda un representante de cada componente blanca (255) junto con el tamaño de la componente.
void floodFill (point pos,  vector<vector<int> > &M, array<point, 8> &directions, vector<pair<int, point> > &ans, int &cont){
    int r=M.size(), c=M[0].size();
    int sz=directions.size();
    queue<point> bf;
    int color=M[pos.first][pos.second];
    cont++; //Lleva la cuenta de las componentes
    bf.push(pos);
    pair<int, point> auxp=make_pair(1, pos);
    while(!bf.empty()){
        pos=bf.front();
        bf.pop();
        if(!(pos.first<0 || pos.first>=r || pos.second<0 || pos.second>=c)){//Los valores están en la matrix
            //Revisa si la celda aún es del color de la celda que inició y si no se ha pasado por ella.
            if(M[pos.first][pos.second]==color){
                auxp.first++;
                M[pos.first][pos.second]=-cont;  //Cada punto blanco se identifica con su número de componente.
                for(int i=0; i<sz; i++){
                    point aux=pos;
                    aux.first+=directions[i].first;
                    aux.second+=directions[i].second;
                    bf.push(aux);//Se agrega a todos los vecinos a la cola.
                }
            }
        }
    }
    ans.push_back(auxp);
}

//Imprime en pgm dos componentes conexas dada una celda de cada una.
void printComp(string filename, vector<vector<int> > &M, point &pos,  point &pos2, unsigned int &r, unsigned int &c){
	int color=M[pos.first][pos.second];
    int color1=M[pos2.first][pos2.second];
	ofstream imFile;
	imFile.open(filename, ios::trunc);
	imFile << "P2" << "\n";
	imFile << c << " " << r << "\n";
    imFile << 255 << "\n";
	for(int i=0; i<r; i++){
		for(int j=0; j<c; j++){
			if(M[i][j]==color) imFile << 255 <<"\n";//Identifica si el punto pertenece a la componente.
            else{
                if(M[i][j]==color1) imFile << 120 << "\n";
			    else imFile << 0 << "\n";
            }
		}
	}
	imFile.close();
}


int main(){
    //Llena el arreglo de direcciones.
    array<point, 8> directions;
    for(int i=0; i<3; i++) directions[i].first=-1;
    directions[3].first=0;
    for(int i=4; i<7; i++) directions[i].first=1;
    directions[7].first=0;
    for(int i=2; i<5; i++) directions[i].second=1;
    directions[5].second=0;
    for(int i=6; i<8; i++) directions[i].second=-1;
    directions[0].second=-1;
    directions[1].second=0;
    //Itera sobre las 5 ímágenes.
    for(int k=1; k<=5; k++){
        vector<vector<int> > M;
        string finName="Img0";
        char c1=k+'0';
        finName.push_back(c1);
        finName+=".pgm";
        cout << finName << "\n";
        unsigned int c, r;
        imageToMatrix(finName, M, r, c);
        vector<pair<int, point> > ans;
        high_resolution_clock::time_point t1=high_resolution_clock::now();//Inicia tiempo.
        int cont=0;
        for(int i=0; i<r; i++){
            for(int j=0; j<c; j++){
                point pos(i, j);
                if(M[i][j]==255)floodFill(pos, M, directions, ans, cont);
            }
        }
        //Ordena las componentes con respecto a su tamaño.
        sort(ans.begin(), ans.end());
        high_resolution_clock::time_point t2=high_resolution_clock::now();//Finaliza tiempo
        duration<double> time_span=duration_cast<duration<double> >(t2-t1);
        cout << "La imagen "<< k << " tiene " << ans.size() << " elementos conexos\n";
        string foutName="Out0";
        foutName.push_back(c1);
        foutName+=".txt";
        ofstream out_file;
        out_file.open(foutName, ios::trunc);
        out_file << "Las dimensiones de la imagen son: " << c << " " << r << "\n";
        for(int i=0; i<ans.size(); i++) out_file << "Componente " << i+1 << " : " << ans[i].first << " pixeles\n";
        out_file.close();
        cout << "Tiempo de ejecucion: "<< time_span.count()<< " segundos\n";
       	foutName="Comps0";
       	foutName.push_back(c1);
       	foutName+=".pgm";
        //Imprime componentes de menor y mayor tamaño.
       	printComp(foutName, M, ans[0].second, ans.back().second, r, c);

    }
    return 0;
}

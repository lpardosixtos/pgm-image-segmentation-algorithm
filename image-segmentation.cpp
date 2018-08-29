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


void imageToMatrix(string filename, vector<vector<int> > &M, unsigned int &r, unsigned int &c){
    ifstream in_file;
    string trash;
    in_file.open(filename);
    in_file>>trash;
    in_file >> c >> r;
    in_file>> trash;
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

void floodFill (point pos,  vector<vector<int> > &M, array<point, 8> &directions, vector<pair<int, point> > &ans){
    int r=M.size(), c=M[0].size();
    int sz=directions.size();
    queue<point> bf;
    int color=M[pos.first][pos.second];
    bf.push(pos);
    pair<int, point> auxp=make_pair(1, pos);
    while(!bf.empty()){
        pos=bf.front();
        bf.pop();
        if(!(pos.first<0 || pos.first>=r || pos.second<0 || pos.second>=c)){
            if(M[pos.first][pos.second]==color){
                if(color){
                    auxp.first++;
                    M[pos.first][pos.second]=2;
                }
                else M[pos.first][pos.second]=1;
                for(int i=0; i<sz; i++){
                    point aux=pos;
                    aux.first+=directions[i].first;
                    aux.second+=directions[i].second;
                    bf.push(aux);
                }
            }
        }
    }
    if(color==255) ans.push_back(auxp);
}




int main(){
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
        high_resolution_clock::time_point t1=high_resolution_clock::now();
        for(int i=0; i<r; i++){
            for(int j=0; j<c; j++){
                point pos(i, j);
                if(M[i][j]==0 || M[i][j]==255)floodFill(pos, M, directions, ans);
            }
        }
        sort(ans.begin(), ans.end());
        high_resolution_clock::time_point t2=high_resolution_clock::now();
        duration<double> time_span=duration_cast<duration<double> >(t2-t1);
        cout << "La imagen tiene " << ans.size() << " elementos conexos\n";
        string foutName="Out0";
        foutName.push_back(c1);
        foutName+=".txt";
        ofstream out_file;
        out_file.open(foutName, ios::trunc);
        for(int i=0; i<ans.size(); i++) out_file << "Componente " << i << " : " << ans[i].first << " elementos\n";
        out_file.close();
        cout << "Tiempo de ejecucion: "<< time_span.count()<< " segundos\n";
    }
    return 0;
}

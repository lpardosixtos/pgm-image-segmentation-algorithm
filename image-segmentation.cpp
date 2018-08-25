#include <iostream>
#include <fstream>
#include<utility>
#include <vector>
#include <array>
#include <queue>
#include <algorithm>

using namespace std;

typedef pair<int, int> point;


void imageToMatrix(string filename, vector<vector<int> > &M, unsigned int &r, unsigned int &c){
    ifstream in_file;
    in_file.open(filename);
    in_file>>filename;
    in_file >> r >> c;
    in_file>> filename;
    M.resize(r);
    for(int i=0; i<r; i++){
        M[i].resize(c);
        for(int j=0; j<c; j++) in_file >> M[i][j];
    }
    in_file.close();
}

void floodFill (point pos,  vector<vector<int> > &M, array<point, 8> &directions, vector<pair<int, point> > &ans){
    int c=M.size(), r=M[0].size();
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

    string filename;
    cin >> filename;
    vector<vector<int> > M;
    unsigned int c, r;
    imageToMatrix(filename, M, r, c);
    vector<pair<int, point> > ans;
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
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            point pos(i, j);
            if(M[i][j]==0 || M[i][j]==255) floodFill(pos, M, directions, ans);
        }
    }
    sort(ans.begin(), ans.end());
    cout << "La imagen tiene " << ans.size() << " elementos conexos\n";
}

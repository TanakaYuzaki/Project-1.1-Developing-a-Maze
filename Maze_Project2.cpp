#include <iostream>
#include <vector>
#include <stack>
#include <utility>
#include <random>
#include <algorithm>

using namespace std;

int count_1(const vector<vector<char>>& matrix,int size){
    int count = 0;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(matrix[i][j] == '1'){
                count += 1;
            }
        }
    }
    return count;
}


vector<vector<char>> Random_Generated_Maze(int n){
    int size = 2*n+1;
    vector<pair<int,int>> Directions =  {{1,0},{-1,0},{0,1},{0,-1}};
    vector<vector<char>> matrix(size,vector<char>(size,'1'));
    pair<int,int> Start = {0,0};
    matrix[1][1] = '0';
    stack<pair<int,int>> Stack;

    Stack.push(Start);

    std::random_device rd;   // seed source
    std::mt19937 g(rd());    // Mersenne Twister engine

    while(!Stack.empty()){
        auto [cur_r, cur_c] = Stack.top();
        shuffle(Directions.begin(),Directions.end(),g);
        int count = 0;
        for(auto [dr,dc]: Directions){
            int new_r = cur_r + dr;
            int new_c = cur_c + dc;
            if(new_r >= 0 && new_c >= 0 && new_r < n && new_c < n && matrix[2*new_r+1][2*new_c+1] == '1'){
                matrix[2*new_r+1][2*new_c+1] = '0';
                matrix[2*cur_r+1+dr][2*cur_c+1+dc] = '0';
                Stack.push({new_r,new_c});
                break;
            }
            count += 1;

        }
        if(count == 4){
            Stack.pop();
        }

    }
    int wall_density = count_1(matrix, size);
    int path_density = size*size-wall_density;
    while(wall_density*1.0/(wall_density+path_density) > 0.5){
        for(int i = 1; i < size-1; i++){
            for(int j = 1; j < size-1; j++){
                std::random_device rd;       // seed
                std::mt19937 gen(rd());      // random engine
                std::uniform_real_distribution<> dist(0.0, 1.0);
                double probability = dist(gen);
                if((i%2 == 0 && j%2 == 1) || (i%2 == 1 && j%2 == 0)){
                    if(probability > 0.5){
                        matrix[i][j] = '0';
                    }
                }
            }
        }
        wall_density = count_1(matrix, size);
        path_density = size*size-wall_density;
    }
    return matrix;

}

vector<vector<char>> Turn_To_ASCII(vector<vector<char>> matrix,int size){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(i%2 == 0 && matrix[i][j] != '0'){
                matrix[i][j] = '-';
            }
            else if(i%2 == 1 && matrix[i][j] != '0'){
                matrix[i][j] = '|';
            }
            else if(((i%2 == 0 && j%2 == 1) || (i%2 == 1 && j%2 == 0)) && matrix[i][j] == '0'){
                matrix[i][j] = ' ';
            }

        }
    }
    return matrix;
}



int main(){
    vector<vector<char>> res = Random_Generated_Maze(5);
    vector<vector<char>> ascii = Turn_To_ASCII(res,11);
    for(int i = 0; i < 11; i++){
        for(int j = 0; j < 11; j++){
            cout<<res[i][j]<<" ";
        }
        cout<<endl;
    }

    for(int i = 0; i < 11; i++){
        for(int j = 0; j < 11; j++){
            cout<<ascii[i][j]<<" ";
        }
        cout<<endl;
    }
}
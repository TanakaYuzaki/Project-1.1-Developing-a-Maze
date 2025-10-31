#include <iostream>
#include <vector>
#include <stack>
#include <utility>
#include <random>
#include <algorithm>

using namespace std;


int count_frequency_path(const vector<vector<char>>& matrix,int size){
    int count = 0;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(matrix[i][j] == '0'){
                count += 1;
            }
        }
    }
    return count;
}


vector<vector<char>> random_generated_maze(int size){
    char wall = '1';
    char path = '0';
    int maze_size = 2*size+1; //Calculating the maze size for vector
    vector<pair<int,int>> directions =  {{1,0},{-1,0},{0,1},{0,-1}}; //Directions allowed to move
    vector<vector<char>> maze_map(maze_size,vector<char>(maze_size,'1')); //Create a 2D array with char '1'
    pair<int,int> starting_point = {0,0}; //Starting point
    maze_map[1][1] = path; //Marking the starting point in the map (2*0+1,2*0+1)
    stack<pair<int,int>> stack; //initiating the stack

    stack.push(starting_point); //adding the starting point in the stack

    std::random_device rd;   // seed source
    std::mt19937 g(rd());    // Mersenne Twister engine

    while(!stack.empty()){ //while the stack is not empty
        auto [current_row, current_column] = stack.top(); //using the last values in the stack
        shuffle(directions.begin(),directions.end(),g); //randomize the direction
        int visited_directions = 0;
        for(auto [direction_row,direction_column]: directions){ //looping through all 4 directions
            int new_row = current_row + direction_row; 
            int new_column = current_column + direction_column;
            if(new_row >= 0 && new_column >= 0 && new_row < size && new_column < size && maze_map[2*new_row+1][2*new_column+1] == wall){ //if the new position is within the map and the new position hasn't been visited
                maze_map[2*new_row+1][2*new_column+1] = path; //marking new position as visited
                maze_map[2*current_row+1+direction_row][2*current_column+1+direction_column] = path; //opening the path
                stack.push({new_row,new_column}); //appending the new stack at the end of the list
                break;
            }
            visited_directions += 1; 

        }
        if(visited_directions == 4){ //if all the directions are visited
            stack.pop();
        }

    }

    int path_density = count_frequency_path(maze_map,maze_size); //calculating the paths available
    int wall_density = maze_size*maze_size-path_density; //calculating the wall density by taking the complement

    while(wall_density*1.0/(wall_density+path_density) > 0.45){ //while the wall density is higher than a certain percentage
        for(int row = 1; row < maze_size-1; row++){
            for(int column = 1; column < maze_size-1; column++){ 
                std::random_device rd;       // seed
                std::mt19937 gen(rd());      // random engine
                std::uniform_real_distribution<> dist(0.0, 1.0);
                double probability = dist(gen); //generating random number
                if((row%2 == 0 && column%2 == 1) || (row%2 == 1 && column%2 == 0)){ //usually the paths are (odd,even) and (even,odd)
                    if(probability > 0.8){ //if the probability satisfy
                        maze_map[row][column] = path; //making the path
                    }
                }
            }
        }
        path_density = count_frequency_path(maze_map,maze_size); //recalculating the density
        wall_density = maze_size*maze_size-path_density;
    }
    return maze_map;

}

vector<vector<char>> Turn_To_ASCII(vector<vector<char>> matrix,int size){
    for(int row = 0; row < size; row++){
        for(int column = 0; column < size; column++){
            if(row%2 == 0 && matrix[row][column] != '0'){
                matrix[row][column] = '-';
            }
            else if(row%2 == 1 && matrix[row][column] != '0'){
                matrix[row][column] = '|';
            }
            else if(((row%2 == 0 && column%2 == 1) || (row%2 == 1 && column%2 == 0)) && matrix[row][column] == '0'){
                matrix[row][column] = ' ';
            }

        }
    }
    return matrix;
}



int main(){
    int n;
    cin>>n;
    vector<vector<char>> res = random_generated_maze(n);
    vector<vector<char>> ascii = Turn_To_ASCII(res,2*n+1);
    for(int i = 0; i < 2*n+1; i++){
        for(int j = 0; j < 2*n+1; j++){
            cout<<res[i][j]<<" ";
        }
        cout<<endl;
    }

    for(int i = 0; i < 2*n+1; i++){
        for(int j = 0; j < 2*n+1; j++){
            cout<<ascii[i][j]<<" ";
        }
        cout<<endl;
    }
}
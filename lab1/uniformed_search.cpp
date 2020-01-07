#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;

pii zero = {0, 0};

struct node{
    char value;
    bool visited;
    pii parent;
    int depth;
    node(){
        parent = make_pair(0,0);
        depth = 0;
        visited = false;
        value = -1;
    }
};


class Maze{
    node ** G; //Graph
    int M, N; // Dimensions


    node* get_node(pii pos){
        return &(G[pos.first][pos.second]);
    }


    vector<pii> moveGen(pii pos){
        vector<pii> neighbours; 
        int x, y;
        int rot_x[] = {-1, 0, 1, 0};
        int rot_y[] = {0, -1, 0, 1};
        for(int i=0; i<4; ++i){
            x = pos.first + rot_x[i]; 
            y = pos.second + rot_y[i];

            if(x >= 0 && x < M && y >= 0 && y < N){// if valid x, y co-ordinates
                if(G[x][y].value == ' ' || G[x][y].value == '*')
                    neighbours.push_back(make_pair(x, y));
            }
        }
        return neighbours;
    }


    bool goalTest(pii pos){
        return (G[pos.first][pos.second].value == '*');
    }


    pii bfs(){

        queue<pii> Q;

        // Push Source
        Q.push(make_pair(0,0));    

        while(!Q.empty()){

            // Pop source
            pii source = Q.front();
            Q.pop();

            // Mark source visited
            G[source.first][source.second].visited = true;

            // Goal test
            if(goalTest(source))
                return source;

            // Check neighbours
            vector<pii> neighbours = moveGen(source);
            for(pii n: neighbours){
                if(!G[n.first][n.second].visited){
                    G[n.first][n.second].parent = source;
                    Q.push(n);
                }
            }
        }
        return {-1, -1};        
    }

    pii dfs(int bound = -1){

        stack<pii> S;

        // Push Source
        S.push(make_pair(0,0));    

        while(!S.empty()){

            // Pop source
            pii source = S.top();
            S.pop();

            // Mark source visited
            get_node(source)->visited = true;

            // Goal test
            if(goalTest(source))
                return source;

            if(get_node(source)->depth == bound)
                continue;

            // Check neighbours
            vector<pii> neighbours = moveGen(source);
            for(pii n: neighbours){
                if(!G[n.first][n.second].visited){
                    G[n.first][n.second].parent = source;
                    S.push(n);
                }
            }
        }
        return {-1, -1};        
    }

    pii dfid(){
        int bound = -1;
        while(bound++){
            pii goal = dfs(bound);
            if(goalTest(goal))
                return goal;
        } 
    }


public:
    void printOutput(int algo){
        pii goal;
        switch(algo){
            case 0:{
                goal = bfs();
                break;
            }
            case 1:{
                goal = dfs();
                break;
            }
            case 2:{
                goal = dfid();
                break;
            }
            default: {
                cout << "Invalid input.\n";
                break;
            }          
        }

        // number of states
        int num_states = 0;
        for(int i=0; i<M; ++i)
            for(int j=0; j<N; ++j)
                num_states ++;
        cout << num_states << "\n" ;

        // length of path found
        int len = 0;
        pii pos = goal;
        while(pos != zero){
            pos = get_node(pos)->parent;
            get_node(pos)->value = '0'; // colour Graph
            len ++;
        }

        // maze
        for(int i=0; i<M; ++i){
            for(int j=0; j<N; ++j){
                cout << G[i][j].value;
                if(G[i][j].value == '0')
                    G[i][j].value = ' ';
            }
            cout << "\n";
        }
    }
};

int main(){
    Maze M;
    return 0;
}

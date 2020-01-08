#include<bits/stdc++.h>
using namespace std;

#define pii pair<int, int>

struct Node{
    char value;
    int visited;
    int color;
    int depth;
    pii parent;
    Node(){
        depth = 0;
        value = ' ';
        visited = 0;
        color = 0;
        parent = make_pair(-1, -1);
    }
};

class Graph{
    vector<vector<Node>> G;
    int row, col; 
    //solution vars
    int num_visited;
    int path_length;
public:
    Graph(string filename);
    
    pii bfs();
    pii dfs(int db=-1);
    pii dfid();

    //helpers
    void showGraph();
    void markGraph(pii index);
    vector<pii> moveGen(pii pos);
    bool goalTest(pii pos);
};


Graph :: Graph(string filename){
    ifstream fin(filename);
    string str;
    int algo;
    fin>>algo;
    row = 0;
    while(getline(fin, str))
        if(str.length()){
            col = str.length();
            G.push_back(vector<Node>(col));
            for(int j = 0; j<col; j++)
                G[row][j].value = str[j];
            row++;
        }
    fin.close();

    pii goal;
    if (algo==0) goal = bfs();
    else if (algo == 1) goal = dfs();
    else goal = dfid();
    
    markGraph(goal);
    
    //num states explored
    cout<<num_visited<<endl;
    //path_length
    cout<<path_length<<endl;
    showGraph();
}

/* helpers */
void Graph:: showGraph(){
    for(vector<Node> Grow : G){
        for(Node iter: Grow)
            cout<<iter.value;
        cout<<endl;
    }
}

void Graph:: markGraph(pii index){
    path_length = 0;
    while(index != make_pair(-1,-1)){
        path_length ++;
        G[index.first][index.second].value = '0';
        index = G[index.first][index.second].parent;
    }
}

vector<pii> Graph:: moveGen(pii pos){
    int rot_x[] = {-1, 0, 1, 0};
    int rot_y[] = {0, 1, 0, -1};
    vector<pii> res;
    for(int i=0; i<4; i++){
        int x = pos.first + rot_x[i];
        int y = pos.second + rot_y[i];
        if(x>=0 && x<row && y>=0 && y<col)
            res.push_back(make_pair(x, y));
    }
    return res;
}

bool Graph:: goalTest(pii pos){
    if(pos.first<row && pos.first >=0 && pos.second <col && pos.second >=0)
        return G[pos.first][pos.second].value == '*';
    return false;
}
/* end of helpers*/

pii Graph:: bfs(){
    queue<pii> Q;
    Q.push(make_pair(0,0));
    G[0][0].visited = true;
    num_visited = 0;
    while( !Q.empty() ){
        // pop and mark visited
        pii iter_index = Q.front();
        Q.pop();
        Node &iter = G[iter_index.first][iter_index.second];
        // cout<<iter_index.first<<" "<<iter_index.second<<endl;
        num_visited ++;

        // check if we reached goal!
        if(goalTest(iter_index)) return iter_index;
        
        //otherwise iter over neighbours
        vector<pii> iter_neighbours = moveGen(iter_index);
        for(pii neighbour : iter_neighbours){
            Node &neighbour_node = G[neighbour.first][neighbour.second];

            if(neighbour_node.visited==0 &&  //not visited
                (neighbour_node.value == ' ' || neighbour_node.value=='*') //free node
            ){
                neighbour_node.visited = true;
                neighbour_node.parent = iter_index;
                Q.push(neighbour);
            }
        }
    }
    return {-1,-1};
}

pii Graph:: dfs(int db){
    stack<pii> S;
    S.push(make_pair(0,0));
    G[0][0].visited = db;
    num_visited = 0;
    while( !S.empty() ){
        // pop and mark visited
        pii iter_index = S.top();
        S.pop();
        Node &iter = G[iter_index.first][iter_index.second];
        num_visited ++;

        // check if we reached goal!
        if(goalTest(iter_index)) return iter_index;

        //check if we reached depth bound
        //then this node has no neighbours 
        if(iter.depth == db) continue;

        //otherwise iter over neighbours
        vector<pii> iter_neighbours = moveGen(iter_index);
        for(pii neighbour : iter_neighbours){
            Node &neighbour_node = G[neighbour.first][neighbour.second];

            if(neighbour_node.visited != db &&  //not visited
                (neighbour_node.value == ' ' || neighbour_node.value=='*') //free node
            ){
                neighbour_node.visited = db;
                neighbour_node.depth = iter.depth + 1;
                neighbour_node.parent = iter_index;
                S.push(neighbour);
            }
        }
    }
    return {-1,-1};
}

pii Graph:: dfid(){
    pii goal = {-1, -1};
    int db = 0;
    while(! goalTest(goal)){
        goal = dfs(db++);
    }
    return goal;
}

int main(int argc, char** argv){
    if(argc != 2){
        cout<<"Usage ./run.sh <filename>"<<endl;
        return 1;
    }

    Graph G(argv[1]);
    return 0;
}
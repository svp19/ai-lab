#include <bits/stdc++.h>
using namespace std;
#define pii pair<int, int>

pii nil = {-1, -1};

struct Node{
    int path_cost;
    int color; // visited = 1, else 0 
    pii parent;
    
    bool operator <(const Node &n) const{
        return path_cost < n.path_cost; 
    }

    Node(){
        path_cost = 0;
        color = 0;
        parent = nil;
    }
};


class JobAllocation{
    vector<vector<int>> cost;
    vector<vector<Node>> G;
    int N;

    // cost:   Jobs ->
    // Person
    // |
    // |

    Node& getNode(pii pos){
        return G[pos.first][pos.second];
    }


    vector<pii> getPath(pii pos){
        vector<pii> path;
        for(pii node=pos; node!=nil; node=getNode(node).parent){
            path.push_back(node);
        }
        return path;
    }


    bool goalTest(vector<int> state){
        /* 
            Checks if every job is assigned to a unique person
            Parameters:
                state(vector<int>) -> index is person, value is job 
            Assure that state[i]!=state[j] for i=!j and state[i]!=-1
        */
        set<int> S;
        for(int job : state){
            if(job==-1) 
                return false;
            S.insert(job);
        }
        return (S.size()==N);
    }

    vector<pii> moveGen(pii pos){
        set<int> jobsTaken;
        for(pii parent: getPath(pos)){
            jobsTaken.insert(parent.second);
        }
        vector<pii> nextMoves;
        for(int i=0; i<N; ++i){
            if(jobsTaken.find(i) == jobsTaken.end())
                nextMoves.push_back(make_pair(pos.first+1, i));
        }
        return nextMoves;
    }
};


int main(){
    return 0;
}
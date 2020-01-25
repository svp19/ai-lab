#include "state.cpp"

pii nil = {-1, -1};

class JobAllocation{
    vector<vector<int>> cost;
    ss open, closed; 
    int N;

    int heuristic(State S);
    bool goalTest(State S, int constraint);
    State bestNeighbour(State S, int K);
    State bestFirstSearch(int constraint);
    State hillClimbing();

public:
    void input(char *filename){
        cout<<"opening "<<filename<<endl;
        int cell_cost;
        ifstream fin;
        fin.open(filename);
        
        fin >> N;
        // Input N x N cost matrix
        for(int i=0; i<N; ++i){
            vector<int> rowC;
            
            for(int j=0; j<N; ++j){
                fin >> cell_cost;
                rowC.push_back(cell_cost);
            }
            cost.push_back(rowC);
        }
        fin.close();
    }  


    void testPrint(){
        State sol = bestFirstSearch(10000); 
        // pii sol = beamSearch(N, 13);
        // printf("Total Cost: %d\n", getNode(sol).path_cost);
        // vector<int> sol = hillClimbing(); 
        sol.print();
        cout << "BFS Total cost = " << heuristic(sol) << "\n";
    }
};

int JobAllocation:: heuristic(State S){
    /*
        @params
            * State S, having the jobs allocated to people
        @return
            * integer representing the cost of this allocation
    */
    int sum = 0;
    for(int i=0; i<S.jobs.size(); i++)
        sum += cost[i][S.jobs[i]];
    return sum;
}

bool JobAllocation:: goalTest(State S, int constraint=INT_MAX){
    /* 
        *  Checks if the cost of the State is less than the constraint
        *   
        * @params:
        *          State S: having array of jobs assigned to ith person
        *          constraint(int): maximum bound constraint for path cost
        * @return:
        *          Boolean of whether state is equal to goal or not.
        */
    return (
        (S.jobs.size() == N) && 
        (heuristic(S) <= constraint)
    );
}


State JobAllocation:: bestNeighbour(State S, int K=2){
    /* 
        for a give node, this returns the best neighbour
        The best neighbour is the one with minimum cost
    */
    vector<State> neighbours = S.moveGen(closed, K);
    State bestNeb = neighbours[0];
    int minCost = INT_MAX;
    for(State T : neighbours){
        int cost = heuristic(T);
        if(cost < minCost){
            bestNeb = T;
            minCost = cost;
        }
    }
    return bestNeb;
}

State JobAllocation::bestFirstSearch(int constraint=INT_MAX){

    //declare empty priority queue
    priority_queue<State, vector<State>, function<bool(State, State)>> Q( [this](State l, State r) -> bool {// Lambda Comparator Constructor for function<>
            // Min Priority Queue based on score
            return (heuristic(l) > heuristic(r));
    });
    
    State S(N);
    S.print();
    // Push Source,
    Q.push(S);
    closed.insert(hash_value(S.jobs));
    int num = 0;
    while(!Q.empty()){
        S = Q.top();
        Q.pop();
        cout<<"At state "<<num<<": ";
        S.print();
        num++;
        if(goalTest(S, constraint)){
            cout << "Goal Found\n";
            return S;
        }
        // generate moves and update closed set too
        vector<State> neighbours = S.moveGen(closed, 2);
        printf("Size of closed is %ld\n", closed.size());

        for(State T : neighbours)
            Q.push(T);
    }
    cout<<"Could not find any goal\n";
    return S;
}


State JobAllocation::hillClimbing(){
    State S(N);
    int cost, newCost;
    newCost = heuristic(S);

    while(true){
        cost = newCost;
        S = bestNeighbour(S);
        newCost = heuristic(S);
        if(newCost >= cost)
            break;
    }
    return S;
}

int main(int argc, char** argv){
    JobAllocation solver;
    solver.input(argv[1]);
    solver.testPrint();
    return 0;
}


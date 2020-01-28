#include "state.cpp"
#include <chrono>
using namespace std::chrono; 


class TSP{
    edge_t edges;
    vp cities;
    ss open, closed; 
    int N;
    int constraint;
    bool isEuclidean;
    int num_states;
    int heuristic(State S);
    bool goalTest(State S);

/*
    Question 1:  Best First Search
    *******************************************************
    Use priority queue as the OPEN set
*/
    State simulatedAnnealing();

/*
    Question 2: Beam search
    *******************************************************
    consists of 2 functions
    * fuck , you know for what
    * geneticAlgorithm, the actual function that does the traversal
*/
    vector<State> fuck(State S, int K, int beamSize);
    State geneticAlgorithm(int beamSize);

/*
    Question 3: Hill Climbing
    *******************************************************
    consists of 2 functions
    * bestNeighbour , for generating neighbours
    * hillClimbing, calls bestNeighbour to go to the best node
*/
    State bestNeighbour(State S, int K);
    State hillClimbing(State node, int K);

/*
    Question 4: Variable NeighbourHood descent
    *******************************************************
    Only one function that does the job 
*/
    State variableNeighbourhoodDescent(int max_density);

public:
    void input(char *filename){
        // cout<<"Opening "<<filename<<endl;
        int cell_cost;
        ifstream fin;
        fin.open(filename);
        fin >> isEuclidean >> N;
        cities = vp (N);

        for(point &city : cities)
            cin>>city.x>>city.y;

        // Input N x N cost matrix
        for(int i=0; i<N; ++i){
            vector<double> rowC;
            
            for(int j=0; j<N; ++j){
                fin >> cell_cost;
                rowC.push_back(cell_cost);
            }
            edges.push_back(rowC);
        }
        fin.close();
    }  


    void testPrint(){
        // Get starting timepoint 
        auto start = high_resolution_clock::now(); 
        State sol = variableNeighbourhoodDescent(5);
        // State sol = simulatedAnnealing();
        // State sol = geneticAlgorithm(2);
        // State sol = hillClimbing(State(N));

        // Get ending timepoint 
        auto stop = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stop - start);

        cout<<"Solution\t: ";
        sol.print();
        printf("H. value\t: %d\n", heuristic(sol));
        printf("Visited\t\t: %d\n", num_states);
        printf("Time taken (micro s): %ld\n", duration.count());
    }
};


int TSP:: heuristic(State S){
    /*
        @params
            * State S, having the order to visit places
        @return
            * integer representing the cost of this path
    */
    int path_cost = 0;
    for(int i=1; i<S.places.size(); i++){
        int u = S.places[i-1];
        int v = S.places[i];
        path_cost += edges[u][v];
    }
    return path_cost;
}

bool TSP:: goalTest(State S){
    /* 
        *  Checks if the cost of the State is less than the constraint
        *   
        * @params:
        *          State S: having array of places assigned to ith person
        *          constraint(int): maximum bound constraint for path cost
        * @return:
        *          Boolean of whether state is equal to goal or not.
        */
    return false;
}

State TSP::simulatedAnnealing(){

    //declare empty priority queue
    priority_queue<State, vector<State>, function<bool(State, State)>> Q( [this](State l, State r) -> bool {// Lambda Comparator Constructor for function<>
            // Min Priority Queue based on score
            return (heuristic(l) > heuristic(r));
    });
    
    State S(N);
    // Push Source,
    Q.push(S);
    closed.insert(hash_value(S.places));
    while(!Q.empty()){
        S = Q.top();
        Q.pop();
        num_states++;

        if(goalTest(S)){
            cout << "Goal Found!!!!!!\n";
            return S;
        }

        // generate moves and update closed set too
        for(State T : S.moveGen(closed, 2))
            Q.push(T);

    }
    cout<<"Could not find any goal with constraint "<<constraint<<endl;
    return S;
}


vector<State> TSP::fuck(State S, int K, int beamSize){
    ss dummy;
    vector<State> beam;
    priority_queue<State, vector<State>, function<bool(State, State)>> Q( [this](State l, State r) -> bool {// Lambda Comparator Constructor for function<>
        // Min Priority Queue based on score
        return (heuristic(l) > heuristic(r));
    });
    for(State T: S.moveGen(dummy, K)) Q.push(T);

    int cost = INT_MAX, newCost;
    while(!Q.empty()){
        State T = Q.top();
        Q.pop();

        //continue if state has been visited
        string t_key = hash_value(T.places);
        if(closed.find(t_key)!=closed.end()) continue;

        //if not found in closed, add it to beam and closed
        beam.push_back(T);
        closed.insert(t_key);
        
        /*
            We would generally stop at beam.size() == beamSize
            But we keep on adding elements if elements past beamsize
            have the same heuristic value as the last element in the beam
        */
        newCost = heuristic(T);
        if(cost!=newCost &&  beam.size()>=beamSize) // hence this condition
            break;
    }
    return beam;
}

State TSP::geneticAlgorithm(int beamSize){
    //declare empty priority queue
    priority_queue<State, vector<State>, function<bool(State, State)>> Q( [this](State l, State r) -> bool {// Lambda Comparator Constructor for function<>
            // Min Priority Queue based on score
            return (heuristic(l) > heuristic(r));
    });
    
    State S(N);
    // Push Source,
    Q.push(S);
    closed.insert(hash_value(S.places));
    while(!Q.empty()){
        S = Q.top();
        Q.pop();
        num_states++;
        
        if(goalTest(S)){
            cout << "Goal Found!!!!!\n";
            return S;
        }

        // generate moves and update closed set too
        for(State T : fuck(S, 2, beamSize))
            Q.push(T);
    }
    cout<<"Could not find any goal with constraint "<<constraint<<endl;
    return S;
}


State TSP:: bestNeighbour(State S, int K=2){
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

State TSP::hillClimbing(State node, int K=2){
    State newNode = bestNeighbour(node, K);
    while(heuristic(node) >= heuristic(newNode)){
        num_states++;
        node = newNode;
        newNode = bestNeighbour(node);
    }
    return node;
}

State TSP:: variableNeighbourhoodDescent(int max_density){
    State node = State(N);
    for(int k = 2; k<=max_density && k<=N; k++){
        // printf("Density: %d\n", k);
        node = hillClimbing(node, k);
        // printf("Num visited: %d\n\n", num_states);
    }
    return node;
}

int main(int argc, char** argv){
    if(argc!=2){
        cout<<"Error: "<<argv[0]<<" <input_file_path>\n";
        return 0;
    }
    TSP solver;
    solver.input(argv[1]);
    solver.testPrint();
    return 0;
}


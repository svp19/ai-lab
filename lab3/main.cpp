#include "state.cpp"
#include <chrono>
using namespace std::chrono; 


class TSP{
    edge_t edges;
    vp cities;
    bool isEuclidean;
    ss open, closed; 
    int N;
    int num_states;
    int density;
    double iter_count;
    int heuristic(State S);
    bool goalTest(State S);
/*
    Question 1:  Best First Search
    *******************************************************
    Use priority queue as the OPEN set
*/
    bool validateMove(State &node, State &newNode, int k);
    State randomNeighbour(State S);
    State simulatedAnnealing(State Node);

/*
    Question 2: geneticAlgorithm
    *******************************************************
    consists of 2 functions
    * makeMove, you know for what, Daddy does Mommy
    * geneticAlgorithm, the actual function that does the traversal
*/
    vector<State> makeMove(State A, State B);
    State geneticAlgorithm(int beamSize);

/*
    Question 3: Hill Climbing
    *******************************************************
    consists of 2 functions
    * randNeighbour , for generating neighbours
    * hillClimbing, calls randNeighbour to go to the best node
*/
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


State TSP:: randomNeighbour(State S){
    /* 
        for a give node, this returns the best neighbour
        The random neighbour is the one with minimum cost
    */
    vector<State> neighbours = S.moveGen(closed, density);
    int rand_index = rand() % neighbours.size();
    return neighbours[rand_index];
}

bool TSP:: validateMove(State &node, State &newNode, int k){
    double temperature = iter_count/(k+1);
    double deltaH = heuristic(node) - heuristic(newNode);
    if(deltaH > 0)
        return true;
    double prob = 1;
    if(k != 0)
        prob = 1 - exp( deltaH / k * temperature );
    double p = double(rand())/double((RAND_MAX));
    return (p < prob);
}

State TSP::simulatedAnnealing(State node){
    for(int k=0; k < iter_count; k++){
        State newNode = randomNeighbour(node);
        num_states++;
        if(validateMove(node, newNode, k))
            node = newNode;
    }
    return node;
}


vector<State> TSP::makeMove(State A, State B){
    
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
        for(State T : makeMove(S, 2, beamSize))
            Q.push(T);
    }
    return S;
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


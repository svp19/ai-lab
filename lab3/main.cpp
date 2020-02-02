#include "state.cpp"
#include <chrono>
using namespace std::chrono; 


class TSP{
    edge_t edges;
    vp cities;
    string isEuclidean;
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
    * makeMove
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
        double cell_cost;
        ifstream fin;
        fin.open(filename);
        fin >> isEuclidean;
        fin >> N;
        cities = vp (N);

        iter_count = 100;
        for(point &city : cities)
            fin>>city.x>>city.y;

        // Input N x N cost matrix
        loop(i, N){
            vector<double> rowC;
            
            loop(j, N){
                fin >> cell_cost;
                rowC.push_back(cell_cost);
            }
            edges.push_back(rowC);
        }
        fin.close();
    }  


    void testPrint(string function_name){
        // Get starting timepoint 
        //init some vars
        auto start = high_resolution_clock::now(); 
        State sol = null_state;
        density = 2;
        if(function_name=="simann")
            sol = simulatedAnnealing(State(N));
        else
            cout<<"Wrong function!!\n";
        

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
    closed.clear();
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
    State bestNode = node;
    for(int k=0; k < iter_count; k++){
        State newNode = randomNeighbour(node);
        num_states++;
        if(validateMove(node, newNode, k))
            node = newNode;
        if(heuristic(node) < heuristic(bestNode))
            bestNode = node;
    }
    return bestNode;
}


vector<State> TSP::makeMove(State A, State B){
    
}

int main(int argc, char** argv){
    if(argc!=2){
        cout<<"Error: "<<argv[0]<<" <input_file_path>\n";
        return 0;
    }
    TSP solver;
    solver.input(argv[1]);
    solver.testPrint("simann");
    return 0;
}


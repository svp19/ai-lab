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
    double k_max; // max iteration count
    double heuristic(State S);
    bool goalTest(State S);
/*
    Question 1:  Simulated Annealing
    *******************************************************
    Use priority queue as the OPEN set
*/
    bool validateMove(State &node, State &newNode, int k);
    State randomNeighbour(State S);
    State simulatedAnnealing();

/*
    Question 2: geneticAlgorithm
    *******************************************************
    consists of 2 functions
    * makeChild
    * geneticAlgorithm, the actual function that does the traversal
*/
    State makeChild(State &A, State &B, string crossover);
    vs selectParents(vs &population);
    State geneticAlgorithm();

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

        k_max = 200;
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
            sol = simulatedAnnealing();
        else if(function_name=="genAlg")
            sol = geneticAlgorithm();
        else
            cout<<"Wrong function!!\n";
        

        // Get ending timepoint 
        auto stop = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stop - start);

        cout<<"Solution\t: ";
        sol.print();
        printf("H. value\t: %lf\n", heuristic(sol));
        printf("Visited\t\t: %d\n", num_states);
        printf("Time taken (micro s): %ld\n", duration.count());
    }
};


double TSP:: heuristic(State S){
    /*
        @params
            * State S, having the order to visit places
        @return
            * integer representing the cost of this path
    */
    double path_cost = 0;
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
     *  For a given node, this returns a random neighbour
     * 
     *  @Params:
     *      S(State ): current state
     *           
     *  @Returns:
     *      State: a random neighbour of S
     * 
     */
    closed.clear();
    vs neighbours = S.moveGen(closed, density);
    int rand_index = rand() % neighbours.size();
    return neighbours[rand_index];
}


bool TSP:: validateMove(State &node, State &newNode, int k){
    /*
     *  Calculates whether next possible state (newNode) is taken using probability 
     *      temperature = k_max/(k+1)
     *      P = 1 - exp([h(node) - h(newNode)]/ k * temperature)
     * 
     *  @Params: 
     *      node(State): current state
     *      newNode(State): next possible state
     *      k(int): iteration number
     
     * @Returns:
     *      Boolean: whether to consider newNode for next move
     * 
     */
    double temperature = k_max/(k+1);
    cout << "Temp: " << temperature << "\n";
    double deltaH = heuristic(node) - heuristic(newNode);

    if(deltaH > 0) // If newNode better, choose it
        return true;
    
    double prob = 1;
    double p = double(rand())/double((RAND_MAX));

    if(k != 0)
        prob = 1/(1 + exp( -deltaH / k * temperature ));
    cout << prob << "\n";

    return (p < prob);
}


State TSP::simulatedAnnealing(){
    State node(N);
    State bestNode = node;
    for(int k=0; k < k_max; k++){
        State newNode = randomNeighbour(node);
        num_states++;
        
        if(validateMove(node, newNode, k))
            node = newNode;
        
        if(heuristic(node) < heuristic(bestNode))
            bestNode = node;
    }
    return bestNode;
}


State TSP::makeChild(State &A, State &B, string crossover="pmx"){
    vi child(N, -1);
    int index = 0;
    if(crossover == "pmx"){
        // search for B[index] in A and copy it
        while(child[index]==-1){
            child[index] = A.places[index];
            index = search(A.places, B.places[index]);
        }

        loop(i, N)
            if(child[i]==-1)
                child[i] = B.places[i];
    } else if(crossover == "order"){
        int start = rand()%N; 
        int end = rand()%(N-start) + start;
        child = vi(A.places.begin() + start, A.places.begin() + end);
        loop(j, N){ // Add remaining cities in the order it occurs in B.
            if(find(child.begin(), child.end(), B.places[j]) == child.end()){
                child.push_back(B.places[j]);
            }
        }
    }
    return State(child);
}


vs TSP:: selectParents(vs &population){
    vd H;
    loop(i, population.size())
        H.push_back(heuristic(population[i]));

    int normalizer = accumulate(H.begin(), H.end(), 0);
    
    //normalize the probabilities
    loop(i, H.size())
        H[i] = H[i]/normalizer;
    
    //find the cummulative probabilities for random selection
    loop(i, H.size()-1)
        H[i+1] = H[i] + H[i+1];

    vs selected;
    loop(i, H.size()){
        //roll random number
        double p = double(rand())/double((RAND_MAX));
        //find the greatest lower bound of p in H
        int index = greatestLowerBound(H, p);
        //append that to the selected population
        selected.push_back(population[index]);
    }
    //Knuth shuffle
    for(int i=H.size()-1; i>0; i--){
        int r = rand()%i;
        swap(selected[i], selected[r]);
    }
    
    return selected;
}

State TSP:: geneticAlgorithm(){
    /*
        hyper_parameters
        * k_max : # of iterations  
        * P : population size
        * k : # of organisms to be replaced by children
    */
    k_max = 100;
    int P = 5000;
    int k = 25 * P/100;
    double mutation_prob = 0.005;
    vs population;

    loop(i, P){
        while(true){
            State node(N);
            //Knuth shuffle
            for(int j=N-1; j>0; j--){
                int r = rand()%j;
                swap(node.places[j], node.places[r]);
            }
            string key = hash_value(node.places);
            if(closed.find(key) != closed.end())
                continue;
            closed.insert(key);
            population.push_back(node);
            break;
        }
    }
    sort(population.begin(), population.end(), [this](const State &a, const State &b){
        return heuristic(a) < heuristic(b);
    });

    loop(i, k_max){
        vs selected = selectParents(population);
        
        vs children;
        loop(j, P/2){
            children.push_back(makeChild(selected[j], selected[P/2+j], "order"));
            children.push_back(makeChild(selected[P/2+j], selected[j], "order"));
        }

        //mutate a small portion of the population
        loop(j, children.size()){
            double p = double(rand())/double((RAND_MAX));
            if(p < mutation_prob){
                int mutation_extent = rand()%N;
                loop(x, mutation_extent){
                    int a = rand() % N;
                    int b = rand() % N;
                    swap(children[j].places[a], children[j].places[b]);
                }
            }
        }


        sort(children.begin(), children.end(),[this](const State &a, const State &b){
            return heuristic(a) < heuristic(b);
        });
        
        //replace last k of population with first k children
        int k = P *15/100; 
        loop(j, k)
            population[P-1-j] = children[j];

        //sort the population according to heuristic
        sort(population.begin(), population.end(),[this](const State &a, const State &b){
            return heuristic(a) < heuristic(b);
        });
        double best = heuristic(population[0]);
        double worst = heuristic(population[P-1]);
        printf("Loop %d/%d (%lf, %lf)\n", i+1, (int)k_max, best, worst);
    }
    //return best among population
    return *population.begin();
}
int main(int argc, char** argv){
    if(argc!=3){
        cout<<"Error: "<<argv[0]<<" <input_file_path>\n";
        return 0;
    }
    TSP solver;
    solver.input(argv[1]);
    solver.testPrint(argv[2]);
        return 0;
}


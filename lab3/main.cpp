#include "tsp.cpp"
using namespace std;

// class TSP{
//     edge_t edges;
//     vp cities;
//     string isEuclidean;
//     ss open, closed; 
//     int N;
//     int num_states;
//     int density;
//     double k_max; // max iteration count
//     double heuristic(State S);
//     bool goalTest(State S);
// /*
//     Question 1:  Simulated Annealing
//     *******************************************************
//     Use priority queue as the OPEN set
// */
//     bool validateMove(State &node, State &newNode, int k);
//     State randomNeighbour(State S);
//     State simulatedAnnealing();

// /*
//     Question 2: geneticAlgorithm
//     *******************************************************
//     consists of 2 functions
//     * makeChild
//     * geneticAlgorithm, the actual function that does the traversal
// */
//     State makeChild(State &A, State &B, string crossover);
//     vs selectParents(vs &population);
//     State geneticAlgorithm();


// public:
//     void input(char *filename){
//         // cout<<"Opening "<<filename<<endl;
//         double cell_cost;
//         ifstream fin;
//         fin.open(filename);
//         fin >> isEuclidean;
//         fin >> N;
//         cities = vp (N);

//         k_max = 200;
//         for(point &city : cities)
//             fin>>city.x>>city.y;

//         // Input N x N cost matrix
//         loop(i, N){
//             vector<double> rowC;
//             loop(j, N){
//                 fin >> cell_cost;
//                 rowC.push_back(cell_cost);
//             }
//             edges.push_back(rowC);
//         }
//         fin.close();
//     }  


//     void testPrint(string function_name){
//         // Get starting timepoint 
//         //init some vars
//         auto start = high_resolution_clock::now(); 
//         State sol = null_state;
//         density = 2;
//         if(function_name=="simann")
//             sol = simulatedAnnealing();
//         else if(function_name=="genAlg")
//             sol = geneticAlgorithm();
//         else
//             cout<<"Wrong function!!\n";
        

//         // Get ending timepoint 
//         auto stop = high_resolution_clock::now(); 
//         auto duration = duration_cast<microseconds>(stop - start);

//         cout<<"Solution\t: ";
//         sol.print();
//         printf("H. value\t: %lf\n", heuristic(sol));
//         printf("Visited\t\t: %d\n", num_states);
//         printf("Time taken (micro s): %ld\n", duration.count());
//     }
// };




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
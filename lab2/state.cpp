#include <bits/stdc++.h>
using namespace std;
#define pii pair<int, int>
#define vi vector<int>
#define ss set<string>

string hash_value(vi &jobs){
    string s = "";
    for(int num: jobs)
        s = s + "," + to_string(num);
    return s;
}

class State{
public:
    vi jobs;
    int N;
    State(int n){
        N = n;
        for(int i=0; i<N; i++)
            jobs.push_back(i);
    }

    State(vi L){
        jobs = L;
        N = jobs.size();
    }

    vector<State> moveGen(ss &closed, int K){
        /*
            This function returns a list of states
            that has less or equal to K jobs perturbed.

            @params
                * set<string> closed : set of states already visited
                * int K : number of jobs to be perturbed

            @returns
                * vector<vector<int>> neighbours
        */
        
        vector<State> neighbours;

        // Generate bitmask with K 1's and N-K 0's
        string bitmask(N-K, 0);
        bitmask.resize(N, 1);

        // permute bitmask to obtain every combo
        do{
            vi shuffled;
            for (int i = 0; i < N; i++)
                if (bitmask[i])
                    shuffled.push_back(jobs[i]);
            do{
                vi move;
                for(int i = 0, j=0; i< N; i++){
                    if(bitmask[i])
                        move.push_back(shuffled[j++]);
                    else
                        move.push_back(jobs[i]);
                }
                    
                string key = hash_value(move);
                if(closed.find(key) == closed.end()){
                    neighbours.push_back(State(move));
                    closed.insert(key);
                }
            }while(next_permutation(shuffled.begin(), shuffled.end()));
        }while (next_permutation(bitmask.begin(), bitmask.end()));
        return neighbours;
    }


    void print(){
        for(int j : jobs)
            cout<<j<<" ";
        cout<<endl;
    }
};
#include <bits/stdc++.h>
using namespace std;
#define loop(i, n) for(int i=0; i<n; i++)

class test{
    public:
        int value;
        test(int t = 0){
            value = t;
        }
        int square(){
            return value*value;
        }
        int cube(){
            return value*value*value;
        }
};

bool comparator(const test &a, const test &b){
    return a.value*a.value < b.value*b.value;
}
int greatedLowerBound(vector<int> V, int p){
    int index = 0;
    int beg = 0, end =V.size()-1;
    while(beg<=end){
        int mid = (beg+end)/2;
        if(V[mid]==p){
            return mid;
        }
        else if(V[mid] < p){
            index = mid;
            beg = mid + 1;
        }
        else{
            end = mid - 1;
        }
    }
    return index;
}
int main(){
    vector<int> V = {1,20,35,44,73,90};
    int p = 90;
    cout<<greatedLowerBound(V, p)<<endl;
    // test M = *max_element(V.begin(), V.end(), comparator);
    // cout<<M.value<<endl;
    return 0;
}
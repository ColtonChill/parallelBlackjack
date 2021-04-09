#include <iostream>
#include <vector>


int main(){
    // input array
    int src1[] = { 1, 2, 3 };
    int src2[] = { 4, 5, 6 };
 
    std::vector<int> dest;
    dest.insert(dest.end(), std::begin(src1), std::end(src1));
    dest.insert(dest.end(), std::begin(src2), std::end(src2));
    
    for (int i: dest) {
        std::cout << i << " ";
    }
    std::cout<<std::endl;
 
    return 0;
}
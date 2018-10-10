#include <iostream>
#include <stdlib.h>
using namespace std;
int partition(int * arr, int start, int end) 
{ 
    int range = (end-start);
    if(end>start)
    {
        int index = rand()%range + start;
        int temp1 = arr[index];
        arr[index] = arr[end];
        arr[end] = temp1;
    }
    int pivot = arr[end];
    int i = start; 
    for (int j = start; j<=(end-1); j++) 
    { 
        if (arr[j] <= pivot) 
        { 
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++; 
        } 
    } 
    int temp = arr[i];
    arr[i] = arr[end];
    arr[end] = temp;
    return i; 
} 

int kth_smallest(int * arr, int start, int end, int n) 
{ 
    int range = end-start+1;
    if (n <= range) 
    { 
        int position = partition(arr, start, end); 
        if (position-start == n-1)
        {
            return arr[position];
        } 
        if (position-start > n-1)
        {
            return kth_smallest(arr, start, position-1, n);
        } 
        return kth_smallest(arr, position+1, end, n-position+start-1); 
    } 
    exit(1); 
} 

int main()
{
    int N,K;
    cin>>N>>K;
    int * arr = (int *)malloc(N*sizeof(int));
    for(int i=0; i<N; i++)
    {
        arr[i] = rand()%100000;
    }
    int val = kth_smallest(arr,0,(N-1),K);
    cout<<endl<<val<<endl;
    return 0;
}
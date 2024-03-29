#include<iostream>
#include<string>

using namespace std;

void print_suffix_array(struct suffix_struct *, int);
struct suffix_struct
{
    int orig_index;
    int rank;
    int next_rank;
};

struct suffix_struct * count_sort(struct suffix_struct * arr, int n, int digit_10, int sort_by)
{
    struct suffix_struct out[n];
    int * count_arr = (int *)calloc(10,sizeof(int));
    for(int i=0; i<n; i++)
    {
        int q = (sort_by==1)?arr[i].rank:arr[i].next_rank;
        q = q/digit_10;
        int d = q%10;
        count_arr[d]++;
    }
    for(int i=1; i<10; i++)
    {
        count_arr[i] = count_arr[i] + count_arr[i-1];
    }
    for(int i=(n-1); i>=0; i--)
    {
        int q = (sort_by==1)?arr[i].rank:arr[i].next_rank;
        q = q/digit_10;
        int d = q%10;
        int c = count_arr[d] - 1;
        out[c] = arr[i];
        count_arr[d]--;
    }
    for(int i=0; i<n; i++)
    {
        arr[i] = out[i];
    }
    return arr;
}

struct suffix_struct * radix_sort(struct suffix_struct * arr, int n)
{
    int max = -1;
    for(int i=0; i<n; i++)
    {
        if(arr[i].rank>max)
        {
            max = arr[i].rank;
        }
    }
    for(int digit_10=1; (max/digit_10)>0; digit_10=digit_10*10)
    {
        arr = count_sort(arr,n,digit_10,1);
    }
    int i=0;
    while(i<n)
    {
        int j = (i+1);
        while(j<n)
        {
            if(arr[j].rank!=arr[i].rank)
            {
                break;
            }
            j++;
        }
        if(j>(i+1))
        {
            int size = j-i;
            struct suffix_struct * temp_arr = (struct suffix_struct *)calloc(size,sizeof(struct suffix_struct));
            int index = 0;
            for(int k=i; k<j; k++)
            {
                temp_arr[index] = arr[k];
                index++;
            }
            int temp_max = -1;
            for(int temp_i=0; temp_i<size; temp_i++)
            {
                if(temp_arr[temp_i].next_rank>temp_max)
                {
                    temp_max = temp_arr[temp_i].next_rank;
                }
            }
            for(int temp_digit_10=1; (temp_max/temp_digit_10)>0; temp_digit_10=temp_digit_10*10)
            {
                temp_arr = count_sort(temp_arr,size,temp_digit_10,2);
            }
            index = 0;
            for(int k=i; k<j; k++)
            {
                arr[k] = temp_arr[index];
                index++;
            }
        }
        i=j;
    }
    return arr;
}

struct suffix_struct * construct_suffix_array(string str, struct suffix_struct * suffix_array)
{
    int n = str.length();
    for(int i=0; i<n; i++)
    {
        char c = str[i];
        suffix_array[i].orig_index = i;
        suffix_array[i].rank = (c - 'a' + 1);
        if(i!=(n-1))
        {
            suffix_array[i].next_rank = (str[i+1] - 'a' + 1);
        }
        else
        {
            suffix_array[i].next_rank = 0;
        }
    }
    radix_sort(suffix_array,n);
    int * orig_indices = (int *)calloc(n,sizeof(int));
    int limit = 2*n;
    for(int k=4; k<limit; k=k*2)
    {
        int distance = k/2;
        int curr_rank = 1;
        int prev_rank = suffix_array[0].rank;
        suffix_array[0].rank = 1;
        orig_indices[suffix_array[0].orig_index] = 0;

        for(int i=1; i<n; i++)
        {
            if(suffix_array[i].rank == prev_rank && suffix_array[i].next_rank == suffix_array[i-1].next_rank)
            {
                prev_rank = suffix_array[i].rank;
                suffix_array[i].rank = curr_rank;
            }
            else
            {
                prev_rank = suffix_array[i].rank;
                curr_rank++;
                suffix_array[i].rank = curr_rank;
            }
            int temp_index = suffix_array[i].orig_index; 
            orig_indices[temp_index] = i;
        }
        for(int i=0; i<n; i++)
        {
            int ni = suffix_array[i].orig_index + distance;
            if(ni<n)
            {
                int temp_index = orig_indices[ni];
                suffix_array[i].next_rank = suffix_array[temp_index].rank;
            }
            else
            {
                suffix_array[i].next_rank = 0;
            }
        }
        radix_sort(suffix_array,n);
    }
    return suffix_array;
}

int * construct_lcp(string str, struct suffix_struct * suffix_array)
{
    int n = str.length();
    int * lcp = (int *)calloc(n,sizeof(int));
    int * inverse_suffix = (int *)calloc(n,sizeof(int));
    for(int i=0; i<n; i++)
    {
        int curr_index = suffix_array[i].orig_index;
        inverse_suffix[curr_index] = i;
    }
    int k=0;
    for(int i=0; i<n; i++)
    {
        int limit = (n-1);
        if(inverse_suffix[i]==limit)
        {
            k=0;
            continue;
        }
        int next_index = inverse_suffix[i] + 1;
        int j=suffix_array[next_index].orig_index;
        int x = i+k;
        int y = j+k;
        while((x < n)&&(y < n) && (str[x]==str[y]))
        {
            k++;
            x = i+k;
            y = j+k;
        }
        int l_index = inverse_suffix[i];
        lcp[l_index] = k;
        if(k>0)
        {
            k--;
        }
    }
    return lcp;
}

void print_suffix_array(struct suffix_struct * sarr, int n)
{
    cout<<endl;
    for(int i=0; i<n; i++)
    {
        cout<<sarr[i].orig_index<<" "<<sarr[i].rank<<" "<<sarr[i].next_rank<<endl;
    }
    cout<<endl;
}

int main()
{
    string str;
    cin>>str;
    //str = str.append(str);
    int str_len = str.length();
    struct suffix_struct * suffix_array = (struct suffix_struct *)calloc(str_len,sizeof(struct suffix_struct));

    suffix_array = construct_suffix_array(str,suffix_array);
    int * lcp = construct_lcp(str,suffix_array);
    cout<<endl;
    for(int i=0; i<str_len; i++)
    {
        cout<<lcp[i]<<" ";
    }
    cout<<endl<<endl;
    string repeated_substring = "";
    int max_length = -1;
    for(int i=1; i<str_len; i++)
    {
        int len = lcp[i];
        if(len > max_length)
        {
            max_length = len;
        }
    }
    cout<<"\n\nLength of longest repeated substring : "<<max_length<<endl;
    // int ind = 0;
    // for(int i=0; i<str_len; i++)
    // {
    //     ind = suffix_array[i].orig_index;
    //     if(ind<str_len)
    //     {
    //         break;
    //     }
    // }
    // int count = str_len/2;
    // while(count--)
    // {
    //     int index = ind%str_len;
    //     cout<<str[index];
    //     ind++;
    // }    
    return 0;
}
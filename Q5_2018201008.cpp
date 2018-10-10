#include<iostream>
#include<time.h>

using namespace std;
struct char_node
{
    const char * string;
    struct char_node * left_child;
    struct char_node * right_child;
};
struct ret_struct
{
    char * full_string;
    int curr_index;
};

struct string_info
{
    char * string;
    int string_length;    
};

class stringBuilder
{
    private:
    struct char_node * root;
    public:
    stringBuilder()
    {
        root=NULL;
    }
    stringBuilder(const char * str)
    {
        if(str==NULL)
        {
            stringBuilder();
        }
        root = (struct char_node * )malloc(sizeof(struct char_node));
        root->string = str;
        root->left_child = NULL;
        root->right_child = NULL;
    }

    struct char_node * get_root()
    {
        return root;
    }
    stringBuilder append(stringBuilder sb)
    {
        struct char_node * new_root = (struct char_node *)malloc(sizeof(struct char_node));
        struct char_node * node_to_append = sb.get_root();
        new_root->string = NULL;
        new_root->left_child = root;
        new_root->right_child = node_to_append;
        root = new_root;
        return *this;
    }

    stringBuilder append(const char * str)
    {
        struct char_node * new_root = (struct char_node *)malloc(sizeof(struct char_node));
        struct char_node * new_leaf = (struct char_node *)malloc(sizeof(struct char_node));
        int i=0;
        int count = 0;

        new_leaf->string = str;
        new_leaf->left_child = NULL;
        new_leaf->right_child = NULL;
        new_root->string = NULL;
        new_root->left_child = root;
        new_root->right_child = new_leaf;
        root = new_root;
        return *this;
    }
    
    int get_string_length(struct char_node * curr_node)
    {
        if(curr_node==NULL)
        {
            return 0;
        }
        int count = 0;
        if(curr_node->string!=NULL)
        {
            int i = 0;
            while(curr_node->string[i]!='\0')
            {
                i++;
                count++;
            }
        }
        int left_c = get_string_length(curr_node->left_child);
        int right_c = get_string_length(curr_node->right_child);;
        int len = count + left_c + right_c;
        return len;
    }

    int length()
    {
        return get_string_length(root);
    }

    struct ret_struct get_string(struct char_node * curr_node, char * full_string, int curr_index)
    {
        struct ret_struct return_value;
        if(curr_node==NULL)
        {
            return_value.full_string = full_string;
            return_value.curr_index = curr_index;
        }
        if(curr_node->string!=NULL)
        {
            int i = 0;
            while(curr_node->string[i]!='\0')
            {
                full_string[curr_index] = curr_node->string[i];
                i++;
                curr_index++;
            }
            return_value.full_string = full_string;
            return_value.curr_index = curr_index; 
        }
        else
        {
            struct ret_struct temp_struct = get_string(curr_node->left_child, full_string,curr_index);
            return_value = get_string(curr_node->right_child, temp_struct.full_string, temp_struct.curr_index);
        }
        return return_value;
    }

    char * get_string()
    {
        int string_length = length();
        char * full_string = (char *)malloc((string_length+1)*sizeof(char));
        struct ret_struct s_struct = get_string(root,full_string,0);
        full_string = s_struct.full_string;
        int curr_index = s_struct.curr_index;
        full_string[curr_index] = '\0';
        return full_string;
    }

    struct string_info get_string_info()
    {
        int string_length = length();
        char * full_string = (char *)malloc((string_length+1)*sizeof(char));
        struct ret_struct s_struct = get_string(root,full_string,0);
        full_string = s_struct.full_string;
        int curr_index = s_struct.curr_index;
        full_string[curr_index] = '\0';
        struct string_info info;
        info.string = full_string;
        info.string_length = string_length;
        return info;
    }

    int find_substring(char * string, int sn, const char * pattern, int pn)
    {
        int * pat_array = (int *)malloc(pn*sizeof(int));
        pat_array[0] = 0;
        int j=0, i=1;
        while(i<pn)
        {
            if(pattern[i]==pattern[j])
            {
                pat_array[i] = (j+1);
                i++;
                j++;
            }
            else
            {
                if(j<=0)
                {
                    pat_array[i] = 0;
                    i++;
                }
                else
                {
                    j = pat_array[j-1];
                }
            }
        }

        i=0;
        j=0;
        while(i<sn && j<pn)
        {
            if(string[i]==pattern[j])
            {
                i++;
                j++;
            }
            else
            {
                if(j<=0)
                {
                    i++;
                }
                else
                {
                    j = pat_array[j-1];
                }
            }
        }
        if(j==pn)
        {
            return (i-j);
        }
        else
        {
            return -1;
        }
    }

    int find_substring(const char * pattern)
    {
        struct string_info info = get_string_info();
        char * str = info.string;
        int sn = info.string_length;
        int i=0;
        int pn = 0;
        while(pattern[i]!='\0')
        {
            pn++;
            i++;
        } 
        int index = find_substring(str,sn,pattern,pn);
        return index;
    }
     
    
};

stringBuilder stringInitialize(const char * str)
{
    stringBuilder sb = stringBuilder(str);
    return sb;
}
stringBuilder stringAppend(stringBuilder s1, stringBuilder s2)
{
    return s1.append(s2);
}

int findSubstring(stringBuilder s, const char * pattern)
{
    return s.find_substring(pattern);
}
int main()
{
    stringBuilder sb1 = stringInitialize("hello");
    stringBuilder sb2 = stringInitialize("world");
    sb1.append(sb2);
    cout<<endl<<endl<<sb1.find_substring("ellowp")<<endl<<endl;
    return 0;
}
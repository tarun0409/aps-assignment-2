#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <limits.h>
#include <type_traits>
#define MAP_SIZE 32768
using namespace std;

template <typename T> 
struct node
{
    int key;
    T value;
    node<T> * left_child;
    node<T> * right_child;
    node<T> * parent;
    int height;
    int balance_factor;
};

int operator==(string x, string y)
{
    if(!x.compare(y))
    {
        return 1;
    }
    return 0;
}

int operator>(string x, string y)
{
    if(x.compare(y)>0)
    {
        return 1;
    }
    return 0;
}

int operator<(string x, string y)
{
    if(x.compare(y)<0)
    {
        return 1;
    }
    return 0;
}

int pow(int a, int b)
{
    int result = 1;
    for(int i=0; i<b; i++)
    {
        result = result*a;
    }
    return result;
}

int get_hash_code(int x)
{
    return x;
}

int get_hash_code(string x)
{
	int hash = 0;
	for(int i=0; i<x.length(); i++)
	{
		hash += x[i] * pow(7,i);
	}
	return hash;

}

int length(int x)
{
    int d = 0;
    if(x<0)
    {
        x=x*-1;
    }
    while(x>0)
    {
        x=x/10;
        d++;
    }
    if(x==0)
    {
        d=1;
    }
    return d;
}

int length(string s)
{
    return s.length();
}

int operator%(string x, int mod)
{
    int hash_value = get_hash_code(x);
    return (hash_value%mod);
}

template <typename T> 
class AVLTree
{
    public:
    node<T> * root = NULL;

    node<T> * create_node(int key, T value)
    {
        node<T> * new_node = (node<T> *)malloc(sizeof(node<T>));
        new_node->key = key;
        new_node->value = value;
        new_node->left_child = NULL;
        new_node->right_child = NULL;
        new_node->parent = NULL;
        new_node->height = 1;
        new_node->balance_factor = 0;
        return new_node;
    }

    void update_height(node<T> * curr_node)
    {
        int max_h = -1;
        if(curr_node->left_child!=NULL)
        {
            if((curr_node->left_child->height) > max_h)
            {
                max_h = curr_node->left_child->height;
            }
        }
        if(curr_node->right_child!=NULL)
        {
            if((curr_node->right_child->height) > max_h)
            {
                max_h = curr_node->right_child->height;
            }
        }
        if(max_h>-1)
        {
            curr_node->height = (max_h+1);
        }
        else if(max_h==-1)
        {
            curr_node->height = 1;
        }
    }

    void update_balance_factor(node<T> * curr_node)
    {
        int l_h = 0;
        int r_h = 0;
        if(curr_node->left_child!=NULL)
        {
            l_h = curr_node->left_child->height;
        }
        if(curr_node->right_child!=NULL)
        {
            r_h = curr_node->right_child->height;
        }
        curr_node->balance_factor = (l_h-r_h);
    }

    void left_left_rotation(node<T> * curr_node)
    {
        node<T> * temp = curr_node ->right_child;
        curr_node->right_child = curr_node->parent;
        curr_node->parent->left_child = temp;
        if(temp!=NULL)
        {
            temp->parent = curr_node->parent;
        }
        if(curr_node->parent == root)
        {
            root = curr_node;
            curr_node->parent->parent = curr_node;
            curr_node->parent = NULL;
        }
        else
        {
            if((curr_node->parent->parent->key) > (curr_node->parent->key))
            {
                curr_node->parent->parent->left_child = curr_node;
            }
            else
            {
                curr_node->parent->parent->right_child = curr_node;
            }
            node<T> * temp_parent = curr_node->parent;
            curr_node->parent = curr_node->parent->parent;
            temp_parent->parent = curr_node;
        }
    }

    void right_right_rotation(node<T> * curr_node)
    {
        node<T> * temp = curr_node->left_child;
        curr_node->left_child = curr_node->parent;
        curr_node->parent->right_child = temp;
        if(temp!=NULL)
        {
            temp->parent = curr_node->parent;
        }
        if(curr_node->parent==root)
        {
            root = curr_node;
            curr_node->parent->parent = curr_node;
            curr_node->parent = NULL;
        }
        else
        {
            if((curr_node->parent->parent->key) > (curr_node->parent->key))
            {
                curr_node->parent->parent->left_child = curr_node;
            }
            else
            {
                curr_node->parent->parent->right_child = curr_node;
            }
            node<T> * temp_parent = curr_node->parent;
            curr_node->parent = curr_node->parent->parent;
            temp_parent->parent = curr_node;
        }
    }
    void left_right_rotation(node<T> * curr_node)
    {
        node<T> * temp1 = curr_node->left_child;
        curr_node->left_child = curr_node->parent;
        curr_node->parent->right_child = temp1;
        if(temp1!=NULL)
        {
            temp1->parent = curr_node->parent;
        }
        curr_node->parent->parent->left_child = curr_node;
        node<T> * temp2 = curr_node->parent;
        curr_node->parent = curr_node->parent->parent;
        temp2->parent = curr_node;
    }

    void right_left_rotation(node<T> * curr_node)
    {
        node<T> * temp1 = curr_node->right_child;
        curr_node->right_child = curr_node->parent;
        curr_node->parent->left_child = temp1;
        if(temp1!=NULL)
        {
            temp1->parent = curr_node->parent;
        }
        curr_node->parent->parent->right_child = curr_node;
        node<T> * temp2 = curr_node->parent;
        curr_node->parent = curr_node->parent->parent;
        temp2->parent  = curr_node;
    }

    int check_and_balance_tree(node<T> * curr_node)
    {
        int change = 0;
        if(curr_node==NULL)
        {
            return change;
        }
        if(curr_node->balance_factor>1)
        {
            if(curr_node->left_child->balance_factor > 0)
            {
                node<T> * temp_curr = curr_node->left_child;
                left_left_rotation(curr_node->left_child);
                change = 1;
                if(temp_curr->left_child!=NULL)
                {
                    update_height(temp_curr->left_child);
                    update_balance_factor(temp_curr->left_child);
                }
                if(temp_curr->right_child!=NULL)
                {
                    update_height(temp_curr->right_child);
                    update_balance_factor(temp_curr->right_child);
                }
                update_height(temp_curr);
                update_balance_factor(temp_curr);
            }
            else if(curr_node->left_child->balance_factor<0)
            {
                left_right_rotation(curr_node->left_child->right_child);
                node<T> * temp_curr = curr_node->left_child;
                left_left_rotation(curr_node->left_child);
                change = 1;
                if(temp_curr->left_child!=NULL)
                {
                    update_height(temp_curr->left_child);
                    update_balance_factor(temp_curr->left_child);
                }
                if(temp_curr->right_child!=NULL)
                {
                    update_height(temp_curr->right_child);
                    update_balance_factor(temp_curr->right_child);
                }
                update_height(temp_curr);
                update_balance_factor(temp_curr);
            }
        }
        else if(curr_node->balance_factor<-1)
        {
            if(curr_node->right_child->balance_factor < 0)
            {
                node<T> * temp_curr = curr_node->right_child;
                right_right_rotation(curr_node->right_child);
                change = 1;
                if(temp_curr->left_child!=NULL)
                {
                    update_height(temp_curr->left_child);
                    update_balance_factor(temp_curr->left_child);
                }
                if(temp_curr->right_child!=NULL)
                {
                    update_height(temp_curr->right_child);
                    update_balance_factor(temp_curr->right_child);
                }
                update_height(temp_curr);
                update_balance_factor(temp_curr);
            }
            else if(curr_node->right_child->balance_factor>0)
            {
                right_left_rotation(curr_node->right_child->left_child);
                node<T> * temp_curr = curr_node->right_child;
                right_right_rotation(curr_node->right_child);
                change = 1;
                if(temp_curr->left_child!=NULL)
                {
                    update_height(temp_curr->left_child);
                    update_balance_factor(temp_curr->left_child);
                }
                if(temp_curr->right_child!=NULL)
                {
                    update_height(temp_curr->right_child);
                    update_balance_factor(temp_curr->right_child);
                }
                update_height(temp_curr);
                update_balance_factor(temp_curr);
            }
        }
        return change;
    }

    node<T> * find_predecessor(node<T> * curr_node)
    {
        if(curr_node==NULL)
        {
            return NULL;
        }
        if(curr_node->left_child == NULL)
        {
            return curr_node;
        }
        node<T> * temp = curr_node->left_child;
        while(temp->right_child!=NULL)
        {
            temp = temp->right_child;
        }
        return temp;
    }

    int insert_node(node<T> * curr_node, node<T> * new_node)
    {
        int status;
        if(root==NULL)
        {
            root = new_node;
            return 1;
        }
        if((new_node->key) == (curr_node->key))
        {
            return 0;
        }
        if((new_node->key) < (curr_node->key))
        {
            if(curr_node->left_child==NULL)
            {
                curr_node->left_child = new_node;
                new_node->parent = curr_node;
                update_height(curr_node);
                update_balance_factor(curr_node);
                return 1;
            }
            else
            {
                status = insert_node((curr_node->left_child) , new_node);
                if(status)
                {
                    update_height(curr_node);
                    update_balance_factor(curr_node);
                }
            }
        }
        else if((new_node->key) > (curr_node->key))
        {
            if(curr_node->right_child==NULL)
            {
                curr_node->right_child = new_node;
                new_node->parent = curr_node;
                update_height(curr_node);
                update_balance_factor(curr_node);
                return 1;
            }
            else
            {
                status =  insert_node((curr_node->right_child), new_node);
                if(status)
                {
                    update_height(curr_node);
                    update_balance_factor(curr_node);
                    int change = check_and_balance_tree(curr_node);
                }
            }
        }
        return status;
    }

    int insert(int new_key, T value)
    {
        node<T> * new_node = create_node(new_key, value);
        return insert_node(root,new_node);
    }

    node<T> * find_node(node<T> * curr_node, int target)
    {
        node<T> * dest = NULL;
        if(root==NULL)
        {
            return NULL;
        }
        //cout<<"\n"<<curr_node->key<<" "<<target<<"\n";
        if((curr_node->key) == target)
        {
            return curr_node;
        }
        else if(((curr_node->key) > target) && curr_node->left_child!=NULL)
        {
            dest = find_node(curr_node->left_child, target);
        }
        else if(((curr_node->key) < target) && curr_node->right_child!=NULL)
        {
            dest = find_node(curr_node->right_child, target);
        }
        return dest;
    }

    node<T> * find(int target)
    {
        return find_node(root,target);
    }

    int remove(int key)
    {
        node<T> * curr_node = find(key);
        if(curr_node==NULL)
        {
            return 0;
        }
        if(curr_node->left_child==NULL)
        {
            if(curr_node==root)
            {
                root = curr_node->right_child;
                if(curr_node->right_child!=NULL)
                {
                    curr_node->right_child->parent = NULL;
                }
            }
            else
            {
                if(curr_node->parent->left_child == curr_node)
                {
                    curr_node->parent->left_child = curr_node->right_child;
                }
                else if(curr_node->parent->right_child == curr_node)
                {
                    curr_node->parent->right_child = curr_node->right_child;
                }
                if(curr_node->right_child!=NULL)
                {
                    curr_node->right_child->parent = curr_node->parent;
                }
            }
            curr_node = curr_node->parent;
        }
        else
        {
            node<T> * pred = find_predecessor(curr_node);
            curr_node->key = pred->key;

            if(pred->parent->left_child == pred)
            {
                pred->parent->left_child = pred->left_child;
            }
            else if(pred->parent->right_child == pred)
            {
                pred->parent->right_child = pred->left_child;
            }
            if(pred->left_child!=NULL)
            {
                pred->left_child->parent = pred->parent;
            }
            curr_node = pred->parent;
        }

        while(curr_node!=NULL)
        {
            update_height(curr_node);
            update_balance_factor(curr_node);
            check_and_balance_tree(curr_node);
            curr_node = curr_node->parent;
        }
        return 1;
    }

    void print_tree(node<T> * curr_node)
    {
        if(curr_node==NULL)
        {
            return;
        }
        print_tree(curr_node->left_child);
        cout<<curr_node->key<<" --> "<<curr_node->value<<endl;
        print_tree(curr_node->right_child);
    }

    void print_tree()
    {
        if(root==NULL)
        {
            cout<<"\nempty tree\n";
            return;
        }
        print_tree(root);
    }

};
template <typename T1, typename T2> 
class Map
{
    public:
    AVLTree<T2> * map;
    Map()
    {
        map = (AVLTree<T2> *)malloc(MAP_SIZE*sizeof(AVLTree<T2>));
    }
    int get_index(T1 key)
    {
        return (key%MAP_SIZE);
    }
    int insert(T1 key, T2 value)
    {
        if(length(key)>10 || length(key)<=0)
        {
            return 0;
        }
        int index = get_index(key);
        int in_key = get_hash_code(key);
        if(index<0)
        {
            index = index*-1;
        }
        node<T2> * result = map[index].find(in_key);
        if(result==NULL)
        {
            map[index].insert(in_key,value);
            return 1;
        }
        else
        {
            result->value = value;
            return 1;
        }
        return 0;
    }
    T2 get(T1 key)
    {
        int index = get_index(key);
        int g_key = get_hash_code(key);
        if(index<0)
        {
            index = index*-1;
        }
        node<T2> * result = map[index].find(g_key);
        if(result==NULL)
        {
            T2 x;
            return x;
        }
        return result->value;
    }
    int remove(T1 key)
    {
        int index = get_index(key);
        int r_key = get_hash_code(key);
        if(index<0)
        {
            index = index*-1;
        }
        node<T2> * result = map[index].find(r_key);
        if(result==NULL)
        {
            return 0;
        }
        return map[index].remove(r_key);
    }
    void print_map()
    {
        for(int i=0; i<MAP_SIZE; i++)
        {
            cout<<"\nindex : "<<i<<"\n";
            map[i].print_tree();
            cout<<endl;
        }
    }
    
};

int main()
{
    clock_t t; 
    t = clock();
    Map<int,string> m;
    for(int i=-1000; i<=10000000; i++)
    {
        string s = "s";
        int x = i;
        x = x<0?(-1*x):x;
        s = s.append(to_string(x));
        m.insert(i,s);
    }
    t = clock() - t;
    cout<<endl<<endl<<t<<endl<<endl;
    return 0;
}

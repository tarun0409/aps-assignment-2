#include <iostream>
#define ORDER 5

using namespace std;
struct block
{
  int * keys;
  struct block ** children;
  int filled;
  struct block * parent;
};

class BTree
{
  public:
  struct block * root;
  int order;
  int no_of_keys;
  BTree(int p)
  {
    root = NULL;
    order = p;
    no_of_keys = (p-1);
  }
  struct block * create_block()
  {
    struct block * new_node = (struct block *)calloc(order,sizeof(struct block));
    new_node->keys = (int *)calloc(no_of_keys,sizeof(int));
    new_node->children = (struct block **)calloc(order,sizeof(struct block *));
    new_node->filled = 0;
    new_node->parent = NULL;

  }

  void handle_overflow(int new_value, struct block * curr_block, struct block * left_block, struct block * right_block)
  {
    if(curr_block==NULL)
    {
      struct block * new_root = create_block();
      new_root->keys[0] = new_value;
      new_root->children[0] = left_block;
      left_block->parent = new_root;
      new_root->children[1] = right_block;
      right_block->parent = new_root;
      root = new_root;
      return;
    }
    int i=0;
    for( ; i<no_of_keys; i++)
    {
      if(curr_block->keys[i]>=new_value)
      {
        break;
      }
    }
    if((curr_block->filled)<no_of_keys)
    {
      int j = curr_block->filled - 1;
      for( ; j>=i; j--)
      {
        curr_block->keys[j+1] = curr_block->keys[j];
        curr_block->children[j+2] = curr_block->children[j+1];
      }
      curr_block->keys[i] = new_value;
      curr_block->children[i] = left_block;
      left_block->parent = curr_block;
      curr_block->children[i+1] = right_block;
      right_block->parent = curr_block;
      curr_block->filled++;
    }
    else
    {
      int * temp_key_arr = (int *)calloc((no_of_keys+1),sizeof(int));
      struct block ** temp_block_arr = (struct block **)calloc((order+1),sizeof(struct block *));
      int j=0;
      while(j<i)
      {
        temp_key_arr[j] = curr_block->keys[j];
        temp_block_arr[j] = curr_block->children[j];
        j++;
      }
      int k=j;
      temp_key_arr[j] = new_value;
      temp_block_arr[j] = left_block;
      temp_block_arr[j+1] = right_block;
      j++;
      while(j<(no_of_keys+1) && k<no_of_keys)
      {
        temp_key_arr[j] = curr_block->keys[k];
        temp_block_arr[j+1] = curr_block->children[k+1];
        j++;
        k++;
      }
      int mid = (no_of_keys+1)/2;
      struct block * new_left_block = create_block();
      struct block * new_right_block = create_block();
      j=0;
      while(j<mid)
      {
        new_left_block->keys[j] = temp_key_arr[j];
        new_left_block->filled++;
        j++;
      }
      int next_new_value = temp_key_arr[j];
      j++;
      k=0;
      while(j<(no_of_keys+1) && k<no_of_keys)
      {
        new_right_block->keys[k] = temp_key_arr[j];
        new_right_block->filled++;
        j++;
        k++;
      }
      j=0;
      while(j<=mid)
      {
        new_left_block->children[j] = temp_block_arr[j];
        new_left_block->children[j]->parent = new_left_block;
        j++;
      }
      k=0;
      while(j<(order+1) && k<order)
      {
        new_right_block->children[k] = temp_block_arr[j];
        new_right_block->children[k]->parent = new_right_block;
        j++;
        k++;
      }
      handle_overflow(next_new_value,curr_block->parent,new_left_block,new_right_block);
    }
  }

  void insert(int value, struct block * curr_block)
  {
    int i=0;
    for( ; i<no_of_keys; i++)
    {
      if(curr_block->keys[i]>=value)
      {
        break;
      }
    }
    if(curr_block->children[i]!=NULL)
    {
      insert(value,curr_block->children[i]);
    }
    else
    {
      if(curr_block->filled < no_of_keys)
      {
        int j = curr_block->filled - 1;
        for( ; j>=i; j--)
        {
          curr_block[j+1] = curr_block[j];
        }
        curr_block->keys[i] = value;
        curr_block->filled++;
      }
      else
      {
        //overflow condition
        int * key_values = (int *)calloc(order, sizeof(int));
        int j=0;
        while(j<i)
        {
          key_values[j] = curr_block->keys[j];
          j++;
        }
        int k = j;
        key_values[j] = value;
        j++;
        while(j<order && k<no_of_keys)
        {
          key_values[j] = curr_block->keys[k];
          j++;
          k++;
        }
        int mid = order/2;
        j=0;
        struct block * left_block = create_block();
        struct block * right_block = create_block();
        while(j<mid)
        {
          left_block->keys[j] = key_values[j];
          left_block->filled++; 
          j++;
        }
        int new_value = key_values[j];
        j++;
        k=0;
        while(j<order && k<no_of_keys)
        {
          right_block->keys[k] = key_values[j];
          right_block->filled++;
          j++;
          k++;
        }
        handle_overflow(new_value,curr_block->parent,left_block,right_block);
      }
    }
  }
  void insert(int value)
  {
    if(root==NULL)
    {
      struct block * new_block = create_block();
      root = new_block;
      new_block->keys[0] = value;
      new_block->filled++;
      return;
    }
    insert(value, root);
  }
  void print_tree(struct block * curr_node)
  {
    cout<<"( ";
    for(int i=0; i<(curr_node->filled); i++)
    {
      if(curr_node->children[i]!=NULL)
      {
        print_tree(curr_node->children[i]);
      }
      cout<<curr_node->keys[i]<<" ";
    }
    int i = curr_node->filled;
    if(curr_node->children[i]!=NULL)
    {
      print_tree(curr_node->children[i]);
    }
    cout<<" )";
  }
  void print_tree()
  {
    cout<<endl;
    struct block * temp_root = root;
    print_tree(temp_root);
    cout<<endl;
  }
};


int main()
{
  BTree b = BTree(ORDER);
  b.insert(1);
  b.insert(2);
  b.insert(3);
  b.print_tree();
  return 0;
}
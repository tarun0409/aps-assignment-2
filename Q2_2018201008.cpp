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
  int min_no_of_keys;
  BTree(int p)
  {
    root = NULL;
    order = p;
    no_of_keys = (p-1);
    min_no_of_keys = (p%2 == 0)?((p/2)-1):(p/2);
  }
  struct block * create_block()
  {
    struct block * new_node = (struct block *)calloc(order,sizeof(struct block));
    new_node->keys = (int *)calloc(no_of_keys,sizeof(int));
    new_node->children = (struct block **)calloc(order,sizeof(struct block *));
    new_node->filled = 0;
    new_node->parent = NULL;

  }
  int is_leaf(struct block * curr_block)
  {
    int leaf = 1;
    for(int i=0; i<order; i++)
    {
      if(curr_block->children[i]!=NULL)
      {
        leaf = 0;
        break;
      }
    }
    return leaf;
  }
  struct block * search(int value, struct block * curr_block)
  {
    int found = 0;
    int i = 0;
    for( ; i<curr_block->filled; i++)
    {
      if(curr_block->keys[i] == value)
      {
        found = 1;
        break;
      }
      if(curr_block->keys[i] > value)
      {
        break;
      }
    }
    if(found)
    {
      return curr_block;
    }
    if((i<(curr_block->filled)) || (curr_block->children[i]!=NULL))
    {
      return search(value, curr_block->children[i]);
    }
    return NULL;
  }
  struct block * search(int value)
  {
    return search(value,root);
  }
  struct block * find_predecessor_block(int value, struct block * curr_block)
  {
    int i = 0;
    for( ; i<curr_block->filled; i++)
    {
      if(curr_block->keys[i]==value)
      {
        break;
      }
    }
    struct block * temp_curr_block = curr_block->children[i];
    while(!is_leaf(temp_curr_block))
    {
      int last_index = temp_curr_block->filled;
      if(temp_curr_block->children[last_index]!=NULL)
      {
        temp_curr_block = temp_curr_block->children[last_index];
      }
      else
      {
        temp_curr_block = temp_curr_block->children[last_index-1];
      }
    }
    return temp_curr_block;
  }
  void handle_underflow(struct block * curr_block)
  {
    if(curr_block==root)
    {
      if(curr_block->filled <=0)
      {
        root=NULL;
      } 
      return;
    }
    struct block * parent = curr_block->parent;
    int c = 0;
    for( ; c<order; c++)
    {
      if(parent->children[c]==curr_block)
      {
        break;
      }
    }
    struct block * left_sibling = NULL;
    struct block * right_sibling = NULL;
    if(c>0)
    {
      left_sibling = parent->children[c-1];
    }
    if(c<no_of_keys)
    {
      right_sibling = parent->children[c+1];
    }
    if(left_sibling!=NULL && (left_sibling->filled > min_no_of_keys))
    {
      int left_element = left_sibling->keys[(left_sibling->filled - 1)];
      left_sibling->filled--;
      int end_index = curr_block->filled - 1;
      while(end_index>=0)
      {
        curr_block->keys[end_index+1] = curr_block->keys[end_index];
        end_index--;
      }
      curr_block->keys[0] = parent->keys[c-1];
      curr_block->filled++;
      parent->keys[c-1] = left_element;
    }
    else if(right_sibling!=NULL && (right_sibling->filled > min_no_of_keys))
    {
      int right_element = right_sibling->keys[0];
      int start_index = 0;
      while(start_index < right_sibling->filled)
      {
        right_sibling->keys[start_index] = right_sibling->keys[start_index+1];
        start_index++;
      }
      right_sibling->filled--;
      curr_block->keys[curr_block->filled] = parent->keys[c];
      parent->keys[c] = right_element;
    }
    else
    {
      if(left_sibling!=NULL)
      {        
        int parent_element = parent->keys[c-1];
        struct block * new_block = create_block();
        int i=0;
        while(i<left_sibling->filled)
        {
          new_block->keys[i] = left_sibling->keys[i];
          new_block->filled++;
          i++;
        }
        new_block->keys[i]=parent_element;
        new_block->filled++;
        i++;
        int j=0;
        while(j<curr_block->filled)
        {
          new_block->keys[i] = curr_block->keys[j];
          new_block->filled++;
          i++;
          j++;
        }
        i=0;
        if(!is_leaf(left_sibling))
        {
          while(left_sibling->children[i]!=NULL && i<order)
          {
            new_block->children[i] = left_sibling->children[i];
            left_sibling->children[i]->parent = new_block;
            i++;
          }
          j=0;
          while(curr_block->children[j]!=NULL && j<order)
          {
            new_block->children[i] = curr_block->children[j];
            curr_block->children[j]->parent = new_block;
            i++;
            j++;
          }
        }
        parent->children[c-1] = new_block;
        new_block->parent = curr_block->parent;
        int k=(c-1);
        while(k<parent->filled)
        {
          parent->keys[k] = parent->keys[k+1];
          k++;
        }
        parent->filled--;
        if(parent->filled<=0)
        {
          root = new_block;
          return;
        }
        k=c;
        int last_k = k;
        while(k<(order-1))
        {
          parent->children[k] = parent->children[k+1];
          last_k = k;
          k++;
        }
        last_k++;
        parent->children[last_k] = NULL;
        if(parent->filled < min_no_of_keys)
        {
          handle_underflow(parent);
        }
      }
      else if(right_sibling!=NULL)
      {
        int parent_element = parent->keys[c];
        struct block * new_block = create_block();
        int i=0;
        while(i<curr_block->filled)
        {
          new_block->keys[i] = curr_block->keys[i];
          new_block->filled++;
          i++;
        }
        new_block->keys[i] = parent_element;
        new_block->filled++;
        i++;
        int j=0;
        while(j<right_sibling->filled)
        {
          new_block->keys[i] = right_sibling->keys[j];
          new_block->filled++;
          i++;
          j++;
        }
        i=0;
        if(!is_leaf(right_sibling))
        {
          while(curr_block->children[i]!=NULL && i<order)
          {
            new_block->children[i] = curr_block->children[i];
            curr_block->children[i]->parent = new_block;
            i++;
          }
          j=0;
          while(right_sibling->children[j]!=NULL && j<order)
          {
            new_block->children[i] = right_sibling->children[j];
            right_sibling->children[j]->parent = new_block;
            i++;
            j++;
          }
        }
        parent->children[c] = new_block;
        new_block->parent = curr_block->parent;
        int k=c;
        while(k<parent->filled)
        {
          parent->keys[k] = parent->keys[k+1];
          k++;
        }
        parent->filled--;
        if(parent->filled<=0)
        {
          root = new_block;
          return;
        }
        k=c+1;
        int last_k = k;
        while(k<(order-1))
        {
          parent->children[k] = parent->children[k+1];
          last_k = k;
          k++;
        }
        last_k++;
        parent->children[last_k] = NULL;
        if(parent->filled < min_no_of_keys)
        {
          handle_underflow(parent);
        }
      } 
    }
  }
  void remove(int value, struct block * del_block)
  {
    int i=0;
    for( ; i<del_block->filled; i++)
    {
      if(del_block->keys[i]==value)
      {
        break;
      }
    }
    if(is_leaf(del_block))
    {
      if(i==(del_block->filled - 1))
      {
        del_block->filled--;
      }
      else
      {
        while(i<(del_block->filled-1))
        {
          del_block->keys[i] = del_block->keys[i+1];
          i++;
        }
        del_block->filled--;
      }
      if(del_block->filled < min_no_of_keys)
      {
        handle_underflow(del_block);
      }
    }
    else
    {
      struct block * predecessor_block = find_predecessor_block(value,del_block);
      int pred_index = predecessor_block->filled - 1;
      int temp_value = del_block->keys[i];
      del_block->keys[i] = predecessor_block->keys[pred_index];
      predecessor_block->keys[pred_index] = temp_value;
      remove(value,predecessor_block);
    }
  }
  void remove(int value)
  {
    struct block * delete_block = search(value);
    if(delete_block!=NULL)
    {
      remove(value,delete_block);
    }

  }
  void handle_overflow(int new_value, struct block * curr_block, struct block * left_block, struct block * right_block)
  {
    if(curr_block==NULL)
    {
      struct block * new_root = create_block();
      new_root->keys[0] = new_value;
      new_root->filled++;
      new_root->children[0] = left_block;
      left_block->parent = new_root;
      new_root->children[1] = right_block;
      right_block->parent = new_root;
      root = new_root;
      return;
    }
    int i=0;
    for( ; i<curr_block->filled; i++)
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
    for( ; i<curr_block->filled; i++)
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
          curr_block->keys[j+1] = curr_block->keys[j];
        }
        curr_block->keys[i] = value;
        curr_block->filled++;
      }
      else
      {
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
    if(root==NULL)
    {
      cout<<"\ntree is empty\n";
      return;
    }
    print_tree(temp_root);
    cout<<endl;
  }
};
int main()
{
  BTree b = BTree(ORDER);
  for(int i=1; i<=53; i++)
  {
    b.insert(i);
  }
  b.print_tree();
  for(int i=53; i>=1; i--)
  {
    b.remove(i);
    b.print_tree();
  }
  return 0;
}
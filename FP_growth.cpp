#include<bits/stdc++.h>
#include<fstream>
using namespace std;
#define int long long

struct node
{
    map<int,node*> pointer;
    int freq;
    int item;
    int parent;
};
map<int,int> m0;

bool compareInterval(int i1, int i2) 
{ 
    return m0[i1] > m0[i2]; 
} 
  
void print_Tree(node *cur)
{
    for(auto i: cur->pointer)
    {
        cout <<cur->item<<" "<<i.second->item<<" "<<i.second->freq<<endl;
        print_Tree(i.second);
    }

}

auto FP_Growth(vector<vector<int>> transactions)
{
    vector<int> v;
    for(auto i : transactions)
    {
        for(auto j : i)
        {
            m0[j]++;
        }
    }
    for(auto i : m0)
    {
        v.push_back(i.first);
    }
    sort(v.begin(),v.end(),compareInterval);

    for(int i = 0; i<transactions.size();i++)
    {
        sort(transactions[i].begin(),transactions[i].end(),compareInterval);
    }

    node *a = new node();
    a->item = -1;
    a->freq = 0;
    a->parent = -1;

    for(auto i : transactions)
    {
        node *cur = a;
        for(auto j : i)
        {
            if(cur->pointer.find(j) != cur->pointer.end())
            {
                cur->pointer[j]->freq++;
                cur = cur->pointer[j];
                // cout<<j<<" "<<"value\n";
            }
            else
            {
                node *temp = new node();
                temp->item = j;
                temp->freq = 1;
                temp->parent = cur;
                cur->pointer[j] = temp;
                cur = cur->pointer[j];
            }
        }
    }
    
    print_Tree(a);
    // FP tree start at node a

    return a;
}
 
signed main()
{
    // step 1 - use Curl to read the file eg. curl 'http://www.philippe-fournier-viger.com/spmf/datasets/BMS1_spmf' > abc.txt 
    ifstream fin("test.txt", ios::in);
    string line;
    int i = 0;
    vector<vector<int>> v;
    while(getline(fin, line))
    {
        // cout<<line<<endl;
        stringstream s(line);
        string word;
        vector<int> t;
        while(getline(s,word,' '))
        {
            int temp = stoi(word);
            if(temp > 0)
            {
                // cout<<temp<<" ";
                t.push_back(temp);
            }
        }
        v.push_back(t);
        // cout <<endl;
    }
    // for(auto i : v)
    // {
    //     for(auto j : i)
    //     {
    //         cout<<j<<" ";
    //     }
    //     cout <<endl;
    // }
    FP_Growth(v);
	return 0;

}

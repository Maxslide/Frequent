#include<bits/stdc++.h>
#include<fstream>
using namespace std;
#define int long long

struct node
{
    unordered_map<int,node*> pointer;
    int freq;
    int item;
    node* parent;
};


vector<vector<int>> finalans;
string get_binary(int n)
{
    string ret = "";
    while(n > 0)
    {
        int t = n%2;
        if(t == 0)
            ret += '0';
        else
            ret += '1';
        n = n/2;
    }
    return ret;
}
  
void print_Tree(node *cur)
{
    for(auto i: cur->pointer)
    {
        //cout <<cur->item<<" "<<i.second->item<<" "<<i.second->freq<<endl;
        print_Tree(i.second);
    }

}
void FP_Mine(vector<pair<int,node*>> backtrack,vector<int> v,vector<int> alpha,int min_sup);
void FP_Growth(vector<vector<int>> transactions, int min_sup, vector<int> alpha);

void FP_Mine_Merge(vector<pair<int,node*>> backtrack,vector<int> v,vector<int> alpha,int min_sup);
void FP_Growth_Merge(vector<vector<int>> transactions, int min_sup, vector<int> alpha);

void FP_Mine(vector<pair<int,node*>> backtrack,vector<int> v,vector<int> alpha,int min_sup)
{
    unordered_map <int,vector<node *>> mp;
    reverse(v.begin(),v.end());
    for(auto i:backtrack)
    {
        mp[i.first].push_back(i.second);
    }
    // for(auto i: mp)
    // {
    //     //cout<<i.first<<" -> ";
    //     for(auto j : i.second)
    //         //cout<<j<<" ";
    //     //cout <<endl;
    // }
    //cout <<endl;
    for(auto i:v)
    {
        vector<vector<int>> transaction;
        for(auto j : mp[i])
        {
            // Put in the new transaction array here.
            node *cur = j;
            vector<int> tra;
            while(cur->parent->item != -1)
            {
                tra.push_back(cur->parent->item);
                cur = cur->parent;
            }
            for(int i = 0; i< j->freq; i++)
                transaction.push_back(tra);
            // rememer to push the answers in the vector
        }
        vector<int> beta;
        //cout <<"ALPHA : ";
        for(auto j : alpha)
        {
            beta.push_back(j);
            //cout <<j<<" ";
        }
        beta.push_back(i);
        //cout <<i<<" "<<endl;
        finalans.push_back(beta);
        for(auto i : transaction)
        {
            for(auto j: i)
            {
                //cout <<j<<" ";
            }
            //cout <<endl;
        }
        FP_Growth(transaction,min_sup,beta);
        // break;
        // Call the make tree function here
    }
}

void FP_Growth(vector<vector<int>> transactions, int min_sup, vector<int> alpha)
{
    vector<pair<int,node*>> backtrack;
    unordered_map<int,int> m0;
    vector<int> v;
    for(auto i : transactions)
    {
        for(auto j : i)
        {
            m0[j]++;
        }
    }
    set<int> dellist;
    for(auto i : m0)
    {
        if(i.second >= min_sup)
            v.push_back(i.first);
        else
        {
            dellist.insert(i.first);
        } 
    }
    //cout <<"DelList\n";
    for(auto i : dellist)
    {
        //cout <<i<<" ";
    }
    //cout <<"Deldone\n";
    sort(v.begin(),v.end(),[m0](int i1,int i2) -> bool {  
    if(m0.at(i1) == m0.at(i2))
        return i1 < i2;  
    return m0.at(i1) > m0.at(i2); 
    });
    for(auto i: v)
    {
        //cout <<i<<" ";
    }
    //cout <<endl;

    for(int i = 0; i<transactions.size();i++)
    {
        sort(transactions[i].begin(),transactions[i].end(),[m0](int i1,int i2) -> bool {
            if(m0.at(i1) == m0.at(i2))
                return i1 < i2;  
    return m0.at(i1) > m0.at(i2); 
    });
    }

    node *a = new node();
    a->item = -1;
    a->freq = 0;
    a->parent = NULL;

    for(auto i : transactions)
    {
        node *cur = a;
        for(auto j : i)
        {
            if(dellist.find(j) != dellist.end())
            {
                break;
            }
            if(cur->pointer.find(j) != cur->pointer.end())
            {
                cur->pointer[j]->freq++;
                cur = cur->pointer[j];
                // //cout<<j<<" "<<"value\n";
            }
            else
            {
                node *temp = new node();
                temp->item = j;
                temp->freq = 1;
                temp->parent = cur;
                cur->pointer[j] = temp;
                cur = cur->pointer[j];
                backtrack.push_back({temp->item,temp});
            }
        }
    }
    //cout <<"printing Tree\n";
    print_Tree(a);
    node *cur = a;
    int flag = 0;
    while(cur != NULL)
    {
        if(cur->pointer.size() > 1)
        {
            flag = 1;
            break;
        }
        else if(cur->pointer.size() == 0)
        {
            break;
        }
        else
        {
            for(auto j : cur->pointer)
            {
                cur = j.second;
            }
        }  
    }
    //cout <<flag<<endl;
    if(flag == 0)
    {
        node *cur = a;
        vector<int> gen;
        while(1)
        {
            if(cur->pointer.size() == 0)
                break;
            auto i = cur->pointer.begin();
            gen.push_back(i->first);
            cur = i->second;
        }
        int num = pow(2,gen.size());
        for(int i = 1; i<num;i++)
        {
            string s = get_binary(i);
            vector<int> ans_add;
            for(auto i : alpha)
            {
                ans_add.push_back(i);
            }
            for(int i = 0; i<gen.size();i++)
            {
                if(s[i] == '1')
                    ans_add.push_back(gen[i]);
            }
            finalans.push_back(ans_add);
        }
        node *ret = nullptr;
        return ;
    }
    sort(backtrack.begin(),backtrack.end(),[m0](pair<int,node*> i1, pair<int,node*> i2)->bool {
        if(m0.at(i1.first) == m0.at(i2.first))
    {
        return i1 > i2;
    }
    return m0.at(i1.first) < m0.at(i2.first); 
    });
    FP_Mine(backtrack,v,alpha,min_sup);
    // FP tree start at node a
    // Call FP MINING ALGO HERE
    return ;
}

void FP_Mine_Merge(vector<pair<int,node*>> backtrack,vector<int> v,vector<int> alpha,int min_sup)
{
    unordered_map <int,vector<node *>> mp;
    reverse(v.begin(),v.end());
    for(auto i:backtrack)
    {
        mp[i.first].push_back(i.second);
    }
    map<int,vector<vector<int>>> final_transaction;
    unordered_map<node*,int> visit;
    for(auto i:backtrack)
    {
        visit[i.second] = 0;
    }
    for(auto bt:backtrack)
    {
        if(visit[bt.second] == 1)
            continue;
        else
        {
            node *cur = bt.second;
            vector<int> tra;
            vector<node*> tra2;
            while(cur->parent->item != -1)
            {
                tra.push_back(cur->parent->item);
                tra2.push_back(cur->parent);
                cur = cur->parent;
            }
            int size = tra.size();
            node *temp = bt.second;
            for(int i = 0; i<size;i++)
            {
                if(visit[temp] == 1)
                    continue;
                vector<int> newtr;
                for(int k = i; k<size;k++)
                {
                    newtr.push_back(tra[k]);
                }
                for(int j = 0; j<temp->freq;j++)
                {
                    final_transaction[temp->item].push_back(newtr);
                }
                visit[temp] = 1;
                temp = tra2[i];
            }
                
        }
    }
    for(auto i:v)
    {
        vector<vector<int>> transaction;
        for(auto i : final_transaction[i])
        {
            vector<int> tra;
            for(auto j:i)
            {
                tra.push_back(j);
            }
            transaction.push_back(tra);
        }
        vector<int> beta;
        //cout <<"ALPHA : ";
        for(auto j : alpha)
        {
            beta.push_back(j);
            //cout <<j<<" ";
        }
        beta.push_back(i);
        //cout <<i<<" "<<endl;
        finalans.push_back(beta);
        FP_Growth_Merge(transaction,min_sup,beta);
        // break;
        // Call the make tree function here
    }
}


void FP_Growth_Merge(vector<vector<int>> transactions, int min_sup, vector<int> alpha)
{
    vector<pair<int,node*>> backtrack;
    unordered_map<int,int> m0;
    vector<int> v;
    for(auto i : transactions)
    {
        for(auto j : i)
        {
            m0[j]++;
        }
    }
    set<int> dellist;
    for(auto i : m0)
    {
        if(i.second >= min_sup)
            v.push_back(i.first);
        else
        {
            dellist.insert(i.first);
        } 
    }
    //cout <<"DelList\n";
    for(auto i : dellist)
    {
        //cout <<i<<" ";
    }
    //cout <<"Deldone\n";
    sort(v.begin(),v.end(),[m0](int i1,int i2) -> bool {  
    if(m0.at(i1) == m0.at(i2))
        return i1 < i2;  
    return m0.at(i1) > m0.at(i2); 
    });
    for(auto i: v)
    {
        //cout <<i<<" ";
    }
    //cout <<endl;

    for(int i = 0; i<transactions.size();i++)
    {
        sort(transactions[i].begin(),transactions[i].end(),[m0](int i1,int i2) -> bool {
            if(m0.at(i1) == m0.at(i2))
                return i1 < i2;  
    return m0.at(i1) > m0.at(i2); 
    });
    }

    node *a = new node();
    a->item = -1;
    a->freq = 0;
    a->parent = NULL;

    for(auto i : transactions)
    {
        node *cur = a;
        for(auto j : i)
        {
            if(dellist.find(j) != dellist.end())
            {
                break;
            }
            if(cur->pointer.find(j) != cur->pointer.end())
            {
                cur->pointer[j]->freq++;
                cur = cur->pointer[j];
                // //cout<<j<<" "<<"value\n";
            }
            else
            {
                node *temp = new node();
                temp->item = j;
                temp->freq = 1;
                temp->parent = cur;
                cur->pointer[j] = temp;
                cur = cur->pointer[j];
                backtrack.push_back({temp->item,temp});
            }
        }
    }
    sort(backtrack.begin(),backtrack.end(),[m0](pair<int,node*> i1, pair<int,node*> i2)->bool {
    if(m0.at(i1.first) == m0.at(i2.first))
    {
        return i1 > i2;
    }
    return m0.at(i1.first) < m0.at(i2.first); 
    });
    //cout <<"printing Tree\n";
    print_Tree(a);
    node *cur = a;
    int flag = 0;
    while(cur != NULL)
    {
        if(cur->pointer.size() > 1)
        {
            flag = 1;
            break;
        }
        else if(cur->pointer.size() == 0)
        {
            break;
        }
        else
        {
            for(auto j : cur->pointer)
            {
                cur = j.second;
            }
        }  
    }
    //cout <<flag<<endl;
    if(flag == 0)
    {
        node *cur = a;
        vector<int> gen;
        while(1)
        {
            if(cur->pointer.size() == 0)
                break;
            auto i = cur->pointer.begin();
            gen.push_back(i->first);
            cur = i->second;
        }
        int num = pow(2,gen.size());
        for(int i = 1; i<num;i++)
        {
            string s = get_binary(i);
            vector<int> ans_add;
            for(auto i : alpha)
            {
                ans_add.push_back(i);
            }
            for(int i = 0; i<gen.size();i++)
            {
                if(s[i] == '1')
                    ans_add.push_back(gen[i]);
            }
            finalans.push_back(ans_add);
        }
        node *ret = nullptr;
        return ;
    }
    
    FP_Mine_Merge(backtrack,v,alpha,min_sup);
    // FP tree start at node a
    // Call FP MINING ALGO HERE
    return ;
}

signed main()
{
    // step 1 - use Curl to read the file eg. curl 'http://www.philippe-fournier-viger.com/spmf/datasets/BMS1_spmf' > abc.txt 
    ifstream fin("abc.txt", ios::in);
    string line;
    int i = 0;
    vector<vector<int>> v;
    // int abc = 1;
    while(getline(fin, line))
    {
        // //cout<<line<<endl;
        stringstream s(line);
        string word;
        vector<int> t;
        // cout << abc<<endl;
        // abc++;
        while(getline(s,word,' '))
        {
            int temp = stoi(word);
            if(temp > 0)
            {
                // //cout<<temp<<" ";
                t.push_back(temp);
            }
        }
        v.push_back(t);
        // //cout <<endl;
    }
    // for(auto i : v)
    // {
    //     for(auto j : i)
    //     {
    //         //cout<<j<<" ";
    //     }
    //     //cout <<endl;
    // }
    vector<int> alpha;
    FP_Growth_Merge(v,410,alpha);
    cout <<"Printing Frequent\n";
    sort(finalans.begin(),finalans.end(),[](vector<int> v1,vector<int>v2)->bool
    {
        return v1.size() < v2.size();
    });
    int cursize = 1;
    for(auto i : finalans)
    {
        if(i.size() > cursize)
            cout <<endl;
        for(auto j : i)
        {
            cout <<j<<" ";
        }
        cout <<" | ";
        cursize = i.size();
    }
	return 0;

}

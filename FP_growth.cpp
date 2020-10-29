#include<bits/stdc++.h>
#include<fstream>
using namespace std;
using namespace std::chrono;
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
void FP_Growth(vector<set<int>> transactions, int min_sup, vector<int> alpha);

void FP_Mine_Merge(vector<pair<int,node*>> backtrack,vector<int> v,vector<int> alpha,int min_sup);
void FP_Growth_Merge(vector<set<int>> transactions, int min_sup, vector<int> alpha);

void FP_Mine(vector<pair<int,node*>> backtrack,vector<int> v,vector<int> alpha,int min_sup)
{
    // cout <<"Here FPMINE\n";
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
        vector<set<int>> transaction;
        for(auto j : mp[i])
        {
            // Put in the new transaction array here.
            node *cur = j;
            set<int> tra;
            while(cur->parent->item != -1)
            {
                tra.insert(cur->parent->item);
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
        FP_Growth(transaction,min_sup,beta);
        // break;
        // Call the make tree function here
    }
}

void FP_Growth( vector< set<int> > transactions, int min_sup, vector<int> alpha )
{
    // cout <<"Here\n";
    vector<pair<int,node*>> backtrack;
    unordered_map<int,int> m0;
    // int hash[1000000] = {0};
    vector<int> v;
    //SORTING THE TRANSACTIONS BASED ON THE OCCURENCE OF EACH ITEM
        for(auto i : transactions)
        {
            for(auto j : i)
            {
                m0[j]++;
                // hash[j]++;
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
        if(v.empty())
        {
            return;
        }
        sort(v.begin(),v.end(),[m0](int i1,int i2) -> bool {  
        if(m0.at(i1) == m0.at(i2))
            return i1 < i2;  
        return m0.at(i1) > m0.at(i2); 
        });
        vector<vector<int>> transac_Gen;
        for(int i = 0; i<transactions.size();i++)
        {
            vector<int> vt;
            for(auto j:v)
            {
                if(transactions[i].find(j) != transactions[i].end())
                    vt.push_back(j);
            } 
            transac_Gen.push_back(vt);
        }
    // FINISHED SORTING EACH TRANSACTION


    // BUILD TREE
        node *a = new node();
        a->item = -1;
        a->freq = 0;
        a->parent = NULL;

        for(auto i : transac_Gen)
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
    // FINISH BUILD TREE


    // CHECK FOR SINGLE PATH TREE AND PERFORM THE OPERATION FOR THAT ACCORDINGLY
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
            return;
        }
    // FINISH CHECK FOR SINGLE PATH TREE


    // CALL FP_MINE IF NOT SINGLE PATH
        // sort(backtrack.begin(),backtrack.end(),[m0](pair<int,node*> i1, pair<int,node*> i2)->bool {
        //         if(m0.at(i1.first) == m0.at(i2.first))
        //         {
        //             return i1 > i2;
        //         }
        //         return m0.at(i1.first) < m0.at(i2.first); 
        //         });
        FP_Mine(backtrack,v,alpha,min_sup);
    // FINISH CALL FP_MINE
    
    return;
}

void FP_Mine_Merge(vector<pair<int,node*>> backtrack,vector<int> v,vector<int> alpha,int min_sup)
{
    unordered_map <int,vector<node *>> mp;
    reverse(v.begin(),v.end());
    for(auto i:backtrack)
    {
        mp[i.first].push_back(i.second);
    }
    map<int,vector<set<int>>> final_transaction;
    unordered_map<node*,int> visit;
    for(auto i:backtrack)
    {
        visit[i.second] = 0;
    }
    for(auto ab:v)
    {

    for(auto bt:mp[ab])
    {
        if(visit[bt] == 1)
            continue;
        else
        {
            node *cur = bt;
            vector<int> tra;
            vector<node*> tra2;
            while(cur->parent->item != -1)
            {
                tra.push_back(cur->parent->item);
                tra2.push_back(cur->parent);
                cur = cur->parent;
            }
            int size = tra.size();
            node *temp = bt;
            for(int i = 0; i<size;i++)
            {
                if(visit[temp] == 1)
                    continue;
                set<int> newtr;
                for(int k = i; k<size;k++)
                {
                    newtr.insert(tra[k]);
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
    }
    for(auto i:v)
    {
        vector<vector<int>> transaction;
        // for(auto i : final_transaction[i])
        // {
        //     vector<int> tra;
        //     for(auto j:i)
        //     {
        //         tra.push_back(j);
        //     }
        //     transaction.push_back(tra);
        // }
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
        FP_Growth_Merge(final_transaction[i],min_sup,beta);
        // break;
        // Call the make tree function here
    }
}

void FP_Growth_Merge(vector<set<int>> transactions, int min_sup, vector<int> alpha)
{
    // cout <<"Here\n";
    vector<pair<int,node*>> backtrack;
    unordered_map<int,int> m0;
    // int hash[1000000] = {0};
    vector<int> v;
    //SORTING THE TRANSACTIONS BASED ON THE OCCURENCE OF EACH ITEM
        for(auto i : transactions)
        {
            for(auto j : i)
            {
                m0[j]++;
                // hash[j]++;
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
        if(v.empty())
        {
            return;
        }
        sort(v.begin(),v.end(),[m0](int i1,int i2) -> bool {  
        if(m0.at(i1) == m0.at(i2))
            return i1 < i2;  
        return m0.at(i1) > m0.at(i2); 
        });
        vector<vector<int>> transac_Gen;
        for(int i = 0; i<transactions.size();i++)
        {
            vector<int> vt;
            for(auto j:v)
            {
                if(transactions[i].find(j) != transactions[i].end())
                    vt.push_back(j);
            } 
            transac_Gen.push_back(vt);
        }
    // FINISHED SORTING EACH TRANSACTION


    // BUILD TREE
        node *a = new node();
        a->item = -1;
        a->freq = 0;
        a->parent = NULL;

        for(auto i : transac_Gen)
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
    // FINISH BUILD TREE


    // CHECK FOR SINGLE PATH TREE AND PERFORM THE OPERATION FOR THAT ACCORDINGLY
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
            return;
        }
    // FINISH CHECK FOR SINGLE PATH TREE


    // CALL FP_MINE IF NOT SINGLE PATH
        // sort(backtrack.begin(),backtrack.end(),[m0](pair<int,node*> i1, pair<int,node*> i2)->bool {
        //         if(m0.at(i1.first) == m0.at(i2.first))
        //         {
        //             return i1 > i2;
        //         }
        //         return m0.at(i1.first) < m0.at(i2.first); 
        //         });
        FP_Mine(backtrack,v,alpha,min_sup);
    // FINISH CALL FP_MINE
    
    return;
}

void show_results( vector <vector <int> >  answer)
{
	vector <vector <vector <int> > > ans;
    int si=1;
    vector < vector <int> > temp;
    for(auto it:finalans)
    {
    	if(it.size()==si)
    	{
    		temp.push_back(it);
    	}
    	else
    	{
    		ans.push_back(temp);
    		temp.clear();
    		si++;
    		temp.push_back(it);
    	}
    }
    if(temp.size()!=0)
    {
    	ans.push_back(temp);
    }
	int z=0,j,k;
	if(ans.size()!=0)
    for(auto i : ans)
    {
    	cout << "\nNUMBER OF FREQUENT ITEMSETS OF SIZE=" << ++z << " ARE " << i.size() << " WHICH ARE-\n\n[";
        for(j=0;j<i.size()-1;j++)
        {
        	cout << "[";
        	for(k=0;k<i[j].size()-1;k++)
        	{
        		cout << i[j][k] << ",";
        	}
        	cout << i[j][k] << "],";
        }
        cout << "[";
        for(k=0;k<i[j].size()-1;k++)
        {
        	cout << i[j][k] << ",";
        }
        cout << i[j][k] << "]";
        cout << "]\n";
    }
    else
    {
    	cout << "\nNO FREQUENT ITEMSETS\n";
    }
    cout << "\n\n";
}

signed main()
{
    // step 1 - use Curl to read the file eg. curl 'http://www.philippe-fournier-viger.com/spmf/datasets/BMS1_spmf' > abc.txt 
    string file_path,line;
	cout << "Input The Path To The File -> ";
	cin >> file_path;
	ifstream fin(file_path, ios::in);
    int i = 0;
    vector<set<int>> v;
    int abc = 0;
    while(getline(fin, line))
    {
        // //cout<<line<<endl;
        stringstream s(line);
        string word;
        set<int> t;
        abc++;
        while(getline(s,word,' '))
        {
            int temp = stoi(word);
            if(temp > 0)
            {
                t.insert(temp);
            }
        }
        v.push_back(t);
    }
    double min_sup,n;
    cout << "Input The Minimum Support Count (in percentage) -> ";
    cin >> min_sup;
    n=(double)v.size();
    n*=min_sup;
    n/=100;
    int minsup=n;
    vector<int> alpha;
    auto start = high_resolution_clock::now();
    FP_Growth(v,minsup,alpha);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); 
    cout << "\nTIME TAKEN BY ALGORITHM WITHOUT OPTIMISATION "<< float(duration.count()/1000000.0) << " seconds" << endl;

    sort(finalans.begin(),finalans.end(),[](vector<int> v1,vector<int>v2)->bool
    {
        return v1.size() < v2.size();
    });
    show_results(finalans);

    finalans.clear();
    alpha.clear();
    start = high_resolution_clock::now();
    FP_Growth_Merge(v,minsup,alpha);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start); 
    cout << "\nTIME TAKEN BY ALGORITHM WITH MERGE OPTIMISATION "<< float(duration.count()/1000000.0) << " seconds" << endl;
    sort(finalans.begin(),finalans.end(),[](vector<int> v1,vector<int>v2)->bool
    {
        return v1.size() < v2.size();
    });
    show_results(finalans);
	return 0;
}

#include<bits/stdc++.h>
using namespace std;
#define pb push_back
#define mp make_pair
typedef long long int ll;
typedef string str;

bool has_infrequent_subset(vector <ll> c,set <vector<ll> > lk)
{
	for(ll i=0;i<c.size();i++)
	{
		vector <ll> tem(c.begin(),c.end());
		tem.erase(tem.begin()+i);
		if(lk.find(tem)==lk.end())
			return false;
	}
	return true;
}

vector <vector<ll> > apriori_gen(vector <vector<ll> > lk,ll k)
{
	ll i,n=lk.size();
	vector <vector <ll> > ck;
	set <vector<ll> > lk1;
	for(auto it:lk)
	{
		// sort(it.begin(),it.end());
		lk1.insert(it);
	}
	for(ll l=0;l<n;l++)
	{
		for(ll m=l+1;m<n;m++)
		{
			for(i=0;i<k;i++)
			{
				if(lk[l][i]!=lk[m][i])
				{
					break;
				}
			}
			if(i==k-1)
			{
				vector <ll> v(lk[l].begin(), lk[l].end());
				// for(ll j=0;j<lk[m].size();j++)
					// cout << lk[m][j] << " ";
				v.pb(lk[m][k-1]);
				// sort(v.begin(),v.end());
				if(has_infrequent_subset(v,lk1))
				{
					ck.pb(v);
				}
			}
		}
	}
	return ck; 	
}

vector <vector <vector <ll> > > apriori(vector <set<ll> > v, ll sup)
{
	ll k=1;
	vector <vector <vector <ll> > > ans;
	vector <vector <ll> > lk,ck;
	// For frequent itemsets of size 1 using hash map
	map <ll ,ll> m0;
	for(ll i=0;i<v.size();i++)
	{
		for(auto j:v[i])
			m0[j]++;
	}
	vector <ll> tem;
	for(auto it:m0)
	{
		if(it.second >= sup)
		{
			tem.pb(it.first);
		}
	}
	if(tem.size()!=0)
	{
		sort(tem.begin(),tem.end());
		for(auto it: tem)
		{
			vector <ll> p; 
			p.pb(it);
			lk.pb(p);
    	}
    }
    // For frequent itemsets of size greater than 1
	while(lk.size()!=0)
	{
		ans.pb(lk);
		ck=apriori_gen(lk,k);
		lk.clear();
		k++;
		ll n=ck.size();
		// cout << n << " " << k << " Normal\n";	
		ll cnt[n+2]={0};
		for(auto s:v)
		{
			if(k>s.size())
				continue;
			for(ll l=0;l<n;l++)
			{
				ll o=ck[l].size(),m;
				for(m=0;m<o;m++)
				{
					if(s.find(ck[l][m])==s.end())
					{
						break;
					}
				}
				if(m==o)
					cnt[l]++;
			}
		}
		for(ll l=0;l<n;l++)
		{
			if(cnt[l] >= sup)
			{
				lk.pb(ck[l]);
			}
		}
		
	}
	cout << "\nOUTPUT USING NORMAL APRIORI\n";
	return ans;
}

vector <vector <vector <ll> > > hashed_apriori(vector <set<ll> > v, ll sup)
{
	ll k=1;
	vector <vector <vector <ll> > > ans;
	vector <vector <ll> > lk,ck;

	// For frequent itemsets of size 1 and size 2 using hash map
	map <ll ,ll> m0;
	map <pair<ll,ll> , ll> m1;	
	for(ll i=0;i<v.size();i++)
	{
		for(auto it=v[i].begin();it!=v[i].end();it++)
		{
			it++;
			auto it1=it;
			it--;
			for(;it1!=v[i].end();it1++)
			{
				m1[mp(*it,*it1)]++;
			}
			m0[*it]++;
		}
	}

	vector <ll> tem;
	for(auto it:m0)
	{
		if(it.second >= sup)
		{
			tem.pb(it.first);
		}
	}
	if(tem.size()!=0)
	{
		sort(tem.begin(),tem.end());
		for(auto it: tem)
		{
			vector <ll> p; 
			p.pb(it);
			lk.pb(p);
    	}
    	ans.pb(lk);
    	k++;
    	lk.clear();
    }
    vector < pair<ll,ll> > tem1;
    for(auto it:m1)
    {
    	if(it.second >= sup)
    	{
    		tem1.pb(it.first);
    	}
    }

    if(tem1.size()!=0)
    {
    	for(auto it:tem1) 
		{
			vector <ll> p; 
   			p.pb(it.first);
   			p.pb(it.second);
        	lk.pb(p);  
    	}
    }
    // For frequent itemsets of size greater than 1
	while(lk.size()!=0)
	{
		ans.pb(lk);
		ck=apriori_gen(lk,k);
		lk.clear();
		k++;
		ll n=ck.size();
		// cout << n << " " << k << " Hashed\n";
		ll cnt[n+2]={0};
		for(auto s:v)
		{
			if(k>s.size())
				continue;
			for(ll l=0;l<n;l++)
			{
				ll o=ck[l].size(),m;
				for(m=0;m<o;m++)
				{
					if(s.find(ck[l][m])==s.end())
					{
						break;
					}
				}
				if(m==o)
					cnt[l]++;
			}
		}
		for(ll l=0;l<n;l++)
		{
			if(cnt[l] >= sup)
			{
				lk.pb(ck[l]);
			}
		}
		
	}
	cout << "\n\n\nOUTPUT USING HASHED BASED TECHNIQUE IN APRIORI ALGORITHM\n";
	return ans;
}

void show_results(vector <vector <vector <ll> > > ans)
{
	ll z=0,j,k;
	if(ans.size()!=0)
    for(auto i : ans)
    {
    	cout << "\nLk for k=" << ++z << "\n" << i.size() << "\n[";
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
    	cout << "No Frequent Itemset\n";
    }
}

int main()
{
	ifstream fin("abc.txt", ios::in);
    string line;
    ll i = 0;
    vector<set<ll>> v;
    while(getline(fin, line))
    {
        stringstream s(line);
        string word;
        set<ll> t;
        while(getline(s,word,' '))
        {
            ll temp = stoi(word);
            if(temp > 0)
            {
                t.insert(temp);
            }
        }
        v.pb(t);
        i++;
    }
    ll sup;
    cin >> sup;
    vector <vector <vector <ll> > > ans,ans1;
    ans=apriori(v,sup);
    show_results(ans);
    ans1=hashed_apriori(v,sup);
    show_results(ans1);
    return 0;
}
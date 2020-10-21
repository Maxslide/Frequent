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
	priority_queue <pair<ll,ll> > tem;
	for(auto it:m0)
	{
		if(it.second > sup)
		{
			tem.push(mp(it.second,it.first));
		}
	}

	while(!tem.empty()) 
	{
		vector <ll> p; 
        p.pb(tem.top().second);
        lk.pb(p);  
        tem.pop(); 
    }
    // For frequent itemsets of size greater than 1
	while(lk.size()!=0)
	{
		ans.pb(lk);
		ck=apriori_gen(lk,k);
		lk.clear();
		k++;
		ll n=ck.size();
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
			if(cnt[l]>sup)
			{
				lk.pb(ck[l]);
			}
		}
		
	}
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

	priority_queue <pair<ll,ll> > tem;
	for(auto it:m0)
	{
		if(it.second > sup)
		{
			tem.push(mp(it.second,it.first));
		}
	}

	while(!tem.empty()) 
	{
		vector <ll> p; 
        p.pb(tem.top().second);
        lk.pb(p);  
        tem.pop(); 
    }
    
    ans.pb(lk);
    k++;
    lk.clear();

    priority_queue < pair<ll,pair<ll,ll> > > tem1;
    for(auto it:m1)
    {
    	if(it.second > sup)
    	{
    		tem1.push(mp(it.second,it.first));
    	}
    }
    while(!tem1.empty()) 
	{
		vector <ll> p; 
        p.pb(tem1.top().second.first);
        p.pb(tem1.top().second.second);
        lk.pb(p);  
        tem1.pop(); 
    }

    // For frequent itemsets of size greater than 1
	while(lk.size()!=0)
	{
		ans.pb(lk);
		ck=apriori_gen(lk,k);
		lk.clear();
		k++;
		ll n=ck.size();
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
			if(cnt[l]>sup)
			{
				lk.pb(ck[l]);
			}
		}
		
	}
	return ans;
}

void show_results(vector <vector <vector <ll> > > ans)
{
	ll z=0;
    for(auto i : ans)
    {
    	cout << ++z << "\n" << i.size() << "\n";
        for(auto j:i)
        {
        	cout << "[";
            for(auto k:j)
            {
            	cout  <<k << ",";
            }
            cout << "]";
        }
        cout << "\n";
    }
}

int main()
{
	ifstream fin("pqr.txt", ios::in);
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
    vector <vector <vector <ll> > > ans=apriori(v,sup),ans1=hashed_apriori(v,sup);
    show_results(ans);
    show_results(ans1);
    return 0;
}
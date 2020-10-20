#include<bits/stdc++.h>
#include<fstream>
using namespace std;
#define int long long
 
signed main()
{
    // step 1 - use Curl to read the file eg. curl 'http://www.philippe-fournier-viger.com/spmf/datasets/BMS1_spmf' > abc.txt 
    ifstream fin("abc.txt", ios::in);
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
                cout<<temp<<" ";
                t.push_back(temp);
            }
        }
        v.push_back(t);
        cout <<endl;
        if(i == 4)
            break;
        i++;
    }
    for(auto i : v)
    {
        for(auto j:i)
        {
            cout <<j<<" ";
        }
        cout <<endl;
    }
	return 0;

}

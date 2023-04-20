#include<bits/stdc++.h>
using namespace std;

int titie = 0;

struct node
{
	bool fin;
	map<char, vector<node*>> trans;
	int id;
	node()
	{
		fin = false;
		id = titie;
		titie++;
	}
};

struct nfa
{
	node *start, *end;
	nfa(char c)
	{
		start = new node();
		end = new node();
		start->trans[c].push_back(end);
		end->fin = true;
	}
};

stack<char> chars;
stack<nfa*> nfas;
vector<nfa*> arr;

void uni()
{
	nfa* t1 = nfas.top();
	nfas.pop();
	nfa* t2 = nfas.top();
	nfas.pop();
	t1->end->fin = false;
	t1->end->trans['0'].push_back(t2->end);
	t1->start->trans['0'].push_back(t2->start);
	t2->start = t1->start;
	nfas.push(t2);
	return;
}

void prod()
{
	nfa* t1 = nfas.top();
	nfas.pop();
	nfa* t2 = nfas.top();
	nfas.pop();
	t2->end->fin = false;
	t2->end->trans['0'].push_back(t1->start);
	t2->end = t1->end;
	nfas.push(t2);
	return;
}

void one_or_more()
{
	nfa* t2 = nfas.top();
	nfas.pop();
	t2->end->trans['0'].push_back(t2->start);
	nfas.push(t2);
	return;
}

void z_or_more()
{	
	nfa* t2 = nfas.top();
	nfas.pop();
	node* temp1 = new node();
	node* temp2 = new node();
	t2->end->fin = false;
	temp2->fin = true;
	t2->end->trans['0'].push_back(temp2);
	t2->end->trans['0'].push_back(t2->start);
	temp1->trans['0'].push_back(t2->start);
	temp1->trans['0'].push_back(temp2);
	t2->start = temp1;
	t2->end = temp2;
	nfas.push(t2);
	return;
}

void reg_to_nfa(string& reg)
{
	nfa* basic;
	for(int i = 0;i<reg.length();i++)
	{
		if(reg[i] == '(' || reg[i] == '.' || reg[i] == '|')
		{
			chars.push(reg[i]);
			continue;
		}
		if(reg[i] == ')')
		{
			chars.pop();
			if(!chars.empty())
			{
				if(chars.top() == '|')
				{
					chars.pop();
					uni();
				}
				else if(chars.top() == '.')
				{
					chars.pop();
					prod();
				}
			}
			continue;
		}
		if(reg[i] == '+')
		{
			one_or_more();
			continue;
		}
		if(reg[i] == '*')
		{
			z_or_more();
			continue;
		}
		basic = new nfa(reg[i]);
		nfas.push(basic);
	}
	return;
}

bool mine(int ind, int n, node* nigga, string& s)
{
	if(ind > n && nigga->fin)
	{
		return true;
	}
	bool retval = false;
	if(nigga->trans.find('0') != nigga->trans.end())
	{
		for(auto nigger:nigga->trans['0'])
		{
			retval = (retval | mine(ind, n, nigger, s));
			if(retval)
			{
				return retval;
			}
		}
	}
	if(ind <= n && nigga->trans.find(s[ind]) != nigga->trans.end())
	{
		for(auto nigger:nigga->trans[s[ind]])
		{
			retval = (retval | mine(ind + 1, n, nigger, s));
			if(retval)
			{
				return retval;
			}
		}
	}
	return retval;
}

string parser(string& s)
{
	int n = s.length() - 1;
	node* starter = nfas.top()->start;
	string ans = "";
	int i = 0;
	while(i <= n)
	{
		int big = 0, j = 0;//j is actually length - 1
		while(i + j <= n)
		{
			bool rec = mine(i, i + j, starter, s);
			if(rec)
			{
				if(j + 1 > big)
				{
					big = j + 1;
				}
			}
			j++;
		}
		if(big == 0)
		{
			ans.push_back('@');
			ans.push_back(s[i]);
			i++;
		}
		else
		{
			ans.push_back('$');
			ans.append(s.substr(i, big));
			i = i + big;
		}
	}
	ans.push_back('#');
	return ans;
}

string multiple_parser(string& s)
{
	int n = s.length() - 1;
	int m = arr.size();
	string ans = "";
	int i = 0;
	while(i <= n)
	{
		int big_fr = 0, big_fr_nfa_ind;
		for(int counter = 1;counter<=m;counter++)
		{
			int big = 0, j = 0;//j is actually length - 1
			node* starter = arr[counter - 1]->start;
			while(i + j <= n)
			{
				bool rec = mine(i, i + j, starter, s);
				if(rec)
				{
					if(j + 1 > big)
					{
						big = j + 1;
					}
				}
				j++;
			}
			if(big > big_fr)
			{
				big_fr = big;
				big_fr_nfa_ind = counter;
			}
		}
		if(big_fr == 0)
		{
			ans.push_back('@');
			ans.push_back(s[i]);
			i++;
		}
		else
		{
			ans.push_back('$');
			ans.push_back(big_fr_nfa_ind + '0');
			i = i + big_fr;
		}
	}
	ans.push_back('#');
	return ans;
}

string input_string()
{
	string s;
	cin>>s;
	return s;
}

void print(string& s)
{
	cout<<s<<endl;
	return;
}

string insert_product_sign(string& reg)
{
	string temp = "";
	for(int i = 0;i<reg.length();i++)
	{
		if(temp.length() && temp[temp.length() - 1] == ')' && reg[i] == '(')
			temp.push_back('.');
		temp.push_back(reg[i]);
	}
	return temp;
}

void multiple_input()
{
	int n;
	cin>>n;
	vector<pair<string, string>> s(n);
	for(int i = 0;i<n;i++)
	{
		cin>>s[i].first>>s[i].second;
	}
	for(int i = 0;i<n;i++)
	{
		s[i].first = insert_product_sign(s[i].first);
		reg_to_nfa(s[i].first);
		string ans = parser(s[i].second);
		cout<<ans<<endl;
	}
	return;	
}

void multiple_inputs()
{
	int n;
	cin>>n;
	for(int i = 0;i<n;i++)
	{
		string reg = input_string();
		reg = insert_product_sign(reg);
		reg_to_nfa(reg);
		arr.push_back(nfas.top());
		nfas.pop();
	}
	string s = input_string();
	string ans = multiple_parser(s);
	print(ans);
	return;
}

void single_input()
{
	//multiple_input();
    //return 0;
	string reg = input_string();
	string s = input_string();
	reg = insert_product_sign(reg);
	reg_to_nfa(reg);
	string realans = parser(s);
	print(realans);
	return;
}

int main()
{

	#ifndef INPUT
   	freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
   	#endif
   
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    //single_input();
    multiple_inputs();
    return 0;
}	
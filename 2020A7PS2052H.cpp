#include<bits/stdc++.h>
using namespace std;

int titie = 0;

struct node
{
	bool fin;
	map<char, vector<node*>> trans;//epsilon is '0'
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
		//anything between '' is a node
		//---char---> indicates that this is a transition to be taken from a node if char is encountered
		//'start'---c--->'end'
		start = new node();
		end = new node();
		start->trans[c].push_back(end);
		end->fin = true;
	}
};

stack<char> chars;
stack<nfa*> nfas;
vector<nfa*> arr;

//add functions for +, |, * and .
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
	//this is what you submitted in assignment question 1 and this is incorrect
	/*
	nfa* t2 = nfas.top();
	nfas.pop();
	node* temp = new node();
	temp->fin = true;
	t2->end->fin = false;
	t2->end->trans['0'].push_back(t2->start);
	t2->start->trans['0'].push_back(temp);
	t2->end = temp;
	nfas.push(t2);
	return;*/

	//this is what you should have submitted for assignment 1 question 1, this is correct
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
	//concat is '.'
	//zero or more is '*'
	//union is '|'
	//one or more is '+'
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
		//we reach here only if the current character is 'a' or 'b'
		basic = new nfa(reg[i]);
		nfas.push(basic);
	}
	return;
}

void check_nfa_validity()
{
	nfa* on_top = nfas.top();
	node* tempo = on_top->start;
	int c = 0;//the number of nodes in the nfa
	map<node*, int> vis;
	queue<node*> q;
	q.push(tempo);
	vis[tempo] = 1;
	while(!q.empty())
	{
		c++;
		node* t = q.front();
		q.pop();
		cout<<1<<" "<<t->id<<" "<<t->fin<<endl;
		for(auto it:t->trans)
		{
			cout<<2<<" "<<it.first<<endl;
			for(auto bot:it.second)
			{
				cout<<3<<" "<<bot->id<<endl;
				if(vis[bot] == 0)
				{
					vis[bot] = 1;
					q.push(bot);
				}
			}
		}
		cout<<endl;
	}
	cout<<c<<endl;
}

//dfs function to check whether the entire string passed can be accepted by this regex or not
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
	// int n = s.length() - 1;
	// node* starter = nfas.top()->start;
	// nfas.pop();
	// string ans = "";
	// int i = 0;
	// while(i <= n)
	// {
	// 	int big = 0, bigj = i, j = i;
	// 	while(j <= n)
	// 	{
	// 		string temp = s.substr(i, j - i + 1);
	// 		bool rec = mine(0, j - i, starter, temp);
	// 		if(rec)
	// 		{
	// 			if(j - i + 1 > big)
	// 			{
	// 				big = j - i + 1;
	// 				bigj = j;
	// 			}
	// 		}
	// 		j++;
	// 	}
	// 	if(big == 0)
	// 	{
	// 		ans.push_back('@');
	// 		ans.push_back(s[i]);
	// 	}
	// 	else
	// 	{
	// 		ans.push_back('$');
	// 		ans.append(s.substr(i, bigj - i + 1));
	// 	}
	// 	i = bigj + 1;
	// }
	// ans.push_back('#');
	// return ans;

	//optimised version
	//you will actually never even consider an empty string to be accepted

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
		//cout<<big<<" "<<i<<" "<<j<<endl;
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
	//optimised version

	int n = s.length() - 1;
	int m = arr.size();
	//node* starter = nfas.top()->start;
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

bool will_accept_empty_or_not(node* nigga)
{
	if(nigga->fin)
	{
		return true;
	}
	bool retval = false;
	if(nigga->trans.find('0') != nigga->trans.end())
	{
		for(auto nigger:nigga->trans['0'])
		{
			retval = (retval | will_accept_empty_or_not(nigger));
			if(retval)
			{
				return retval;
			}
		}
	}
	return retval;
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

bool in_to_fin_for_free(string& s)
{
	bool rec = will_accept_empty_or_not(nfas.top()->start);
	bool retval = false;
	if(s.length() == 0)
	{
		if(rec)
		{
			cout<<"$#"<<endl;
			retval = true;
		}
		else
		{
			cout<<"@#"<<endl;
			retval = true;
		}
	}
	return retval;
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
	//cout<<reg<<endl;
	reg_to_nfa(reg);
	if(in_to_fin_for_free(s))
		return;
	//check_nfa_validity();
	//int n = s.length() - 1;
	//node* starter = nfas.top()->start;
	//cout<<mine(0, 2, starter, s)<<endl;
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
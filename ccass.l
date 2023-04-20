%{
    #include<stdlib.h>
    #include<stdio.h>
    #include<string.h>
    #include<math.h>
    #include<unistd.h>
    struct node
    {
        char cust[100];
        int val, cnt;
    };
    struct node mp[31][13];
    char temp[100];
    int d, m, v;
    FILE *ptr, *out;
%}

%x doll 
%x customer
%x dd
%x mm
%x value
%x semic

%%

<doll>" "* {;}
<doll>[A-Z]+[0-9]+ {
                    strcpy(temp, yytext);
                    BEGIN(customer);
}
<doll>[a-z0-9/;] {BEGIN(INITIAL);}
<doll>\n {BEGIN(INITIAL);}

<customer>" "* {;}
<customer>[0-9][0-9] {
                        d = atoi(yytext);
                        if(d > 30)
                        {
                            fprintf(out, "0\n");
                            exit(0);
                        }
                        BEGIN(dd);
}
<customer>[a-zA-Z/;] {BEGIN(INITIAL);}
<customer>\n {BEGIN(INITIAL);}

<dd>"/" {;}
<dd>[0-9][0-9] {
                m = atoi(yytext);
                if(m > 12)
                {
                    fprintf(out, "0\n");
                    exit(0);
                }
                BEGIN(mm);
}
<dd>\n {BEGIN(INITIAL);}
<dd>[a-zA-Z;] {BEGIN(INITIAL);}

<mm>" "* {;}
<mm>[0-9]+ {
                v = atoi(yytext);
                if(strlen(mp[d][m].cust) == 0)
                {
                    strcpy(mp[d][m].cust, temp);
                    mp[d][m].val = v;
                    mp[d][m].cnt++;
                }
                else
                {
                    if(mp[d][m].val < v)
                    {
                        strcpy(mp[d][m].cust, temp);
                        mp[d][m].val = v;
                    }
                    mp[d][m].cnt++;
                }
                BEGIN(value);
}
<mm>\n {BEGIN(INITIAL);}
<mm>[a-zA-Z;/] {BEGIN(INITIAL);}

<value>" "* {;}
<value>";" {BEGIN(semic);}
<value>[a-zA-Z/] {BEGIN(INITIAL);}
<value>\n {BEGIN(INITIAL);}

<semic>" "* {;}
<semic>\$ {BEGIN(doll);}
<semic>[0-9][0-9] {
                        d = atoi(yytext);
                        if(d > 30)
                        {
                            fprintf(out, "0\n");
                            exit(0);
                        }
                        BEGIN(dd);
}
<semic>\n {BEGIN(INITIAL);}
<semic>[a-zA-Z;/] {BEGIN(INITIAL);}

\$ {BEGIN(doll);}
"//".* {;}
[a-zA-Z0-9/;]* {;}
" "* {;}
\n {;}

%%

void main(){
    for(int i = 1;i<=30;i++)
    {
        for(int j = 1;j<=12;j++)
        {
            mp[i][j].val = 0;
            mp[i][j].cnt = 0;
        }
    }
    yyin = fopen("data.txt", "r");
    ptr = fopen("input.txt", "r");
    out = fopen("output.txt", "w+");
    int m_m, d_d;
    fscanf(ptr, "%d/%d", &d_d, &m_m);
    yylex();
    if(strlen(mp[d_d][m_m].cust) == 0)
    {
        fprintf(out, "$0$#\n");
    }
    else
    {        
        fprintf(out, "$%d$%s#\n", mp[d_d][m_m].cnt, mp[d_d][m_m].cust);
    }
    return;
}

int yywrap()
{ 
    return 1;
}
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>
using namespace std;


typedef struct{
    string tokenNome;
    string tokenTipo;
    int linha;
}token;

vector <char> leArquivo(const char *arq)
{
    ifstream arquivo;
    arquivo.open(arq, ios::in);
    vector <char> a;
    token aux;
    char b;
    while(arquivo>>noskipws>>b)//
    {
        a.push_back(b);
    }
    arquivo.close();
    b = '\0'; //POSSIVELMENTE RETIRAR DEPOIS
    a.push_back(b);

    return a;
}



vector <token> Tabela(vector <char> textoIn)
{
    int linhaAtual = 1;
    int posicao = 0;
    vector <token> tokenLista;
    vector <token> error;
	string backup;
    token aux;
    int espaco = 0;
    int tab = 0;
    char arrayaux[40];
    while(textoIn[posicao] != '\0')
    {
        int posicaoChar = 0;

        if(textoIn[posicao]=='\n'||textoIn[posicao]=='\r')
        {
            linhaAtual++;
        }
        if(textoIn[posicao]== '/' && textoIn[posicao + 1]=='/')
            while(textoIn[posicao+1]!='\n')
            {
                posicao++;
            }
        else if(textoIn[posicao] == '{')
        {
            while(!(textoIn[posicao] == '}'))
            {
                if(textoIn[posicao] == '\n')
                {
                    linhaAtual++;
                }
                else if(textoIn[posicao + 1] == '\0')
                {
                    cout << "COMENTARIO NAO FECHADO";
                    return error;
                }
                posicao++;
            }
        }
        else if(textoIn[posicao]>= '0' && textoIn[posicao] <= '9')
        {
            int posicaoprevia = 0, x = 0, y = 0, z = 0;
            int real = 0;
            while(textoIn[posicao]!= '\0')
            {

                if(textoIn[posicao] >= '0' && textoIn[posicao] <= '9')
                {
                    arrayaux[posicaoChar] = textoIn[posicao];
                    posicaoChar++;
                    posicao++;
                }
                else if((textoIn[posicao] == '.') && !(real))
                {
                    arrayaux[posicaoChar] = textoIn[posicao];
                    posicaoChar++;
                    posicao++;
                    real = 1;
					
                }
                else
                {
                    posicao--;
                    break;
                }

            }
            if(real)
            {
				
				posicaoprevia = posicao;
                string lido(arrayaux, posicaoChar);
                if(textoIn[posicao+1]=='x')
                {
                    
                    arrayaux[posicaoChar] = textoIn[posicao + 1];
                    x = 1;
                    posicaoChar++;
                    posicao++;
					
                    if(textoIn[posicao+1]>= '0' && textoIn[posicao+1] <= '9')
                    {
                        posicao++;
						real = 0;
                        while(textoIn[posicao]!= '\0')
                        {
                        if(textoIn[posicao] >= '0' && textoIn[posicao] <= '9')
                        {
                            arrayaux[posicaoChar] = textoIn[posicao];
                            posicaoChar++;
                            posicao++;
                        }
                        else if((textoIn[posicao] == '.') && !(real))
                        {
                            arrayaux[posicaoChar] = textoIn[posicao];
                            posicaoChar++;
                            posicao++;
                            real = 1;
                        }
                        else{
                            posicao--;
                            break;
                        }
						
						}
                        if(real)
                        {

                            if(textoIn[posicao+1]=='y')
                            {
								y = 1;
								arrayaux[posicaoChar] = textoIn[posicao + 1];
								posicaoChar++;
								posicao++;
								
								if(textoIn[posicao+1] >= '0' && textoIn[posicao+1] <= '9')
								{
									posicao++;
									real = 0;
									while(textoIn[posicao]!='\0')
									{
										if(textoIn[posicao] >= '0' && textoIn[posicao] <= '9')
										{
											arrayaux[posicaoChar] = textoIn[posicao];
											posicaoChar++;
											posicao++;
										}
										else if((textoIn[posicao] == '.') && !(real))
										{
											arrayaux[posicaoChar] = textoIn[posicao];
											posicaoChar++;
											posicao++;
											real = 1;
										}
										else
										{
											posicao--;
											break;
										}
									}
									if(real)
									{
										if(textoIn[posicao+1]=='z')
										{
											
											z = 1;
											arrayaux[posicaoChar] = textoIn[posicao + 1];
											posicaoChar++;
											aux.linha = linhaAtual;
											aux.tokenTipo = "Real3D";
											string lido2(arrayaux, posicaoChar);
											memset(arrayaux, 0, sizeof(arrayaux));
											aux.tokenNome = lido2;
											tokenLista.push_back(aux);
										}
									}
								}
							}
							
                        }
                    }
                }
                if(!x||!y||!z)
                {	
					aux.linha = linhaAtual;
					aux.tokenTipo = "Real";
					aux.tokenNome = lido;
					tokenLista.push_back(aux);
					posicao = posicaoprevia;
                }
            }
            else
            {
                aux.linha = linhaAtual;
                aux.tokenTipo = "Numero Inteiro";
                string lido(arrayaux, posicaoChar);
                memset(arrayaux, 0, sizeof(arrayaux));
                aux.tokenNome = lido;
                tokenLista.push_back(aux);
				posicao;
            }
            //posicao++;
        }
        else if(textoIn[posicao]>= 'a' && textoIn[posicao] <= 'z'||textoIn[posicao]>= 'A' && textoIn[posicao] <= 'Z')
        {

            while(textoIn[posicao]!='\0')
            {
                if((textoIn[posicao] >= 'a') && (textoIn[posicao] <= 'z')||((textoIn[posicao] >= '0') && (textoIn[posicao] <= '9'))||((textoIn[posicao] >= 'A'&& (textoIn[posicao] <= 'Z')||(textoIn[posicao] == '_' ))))
                {
                    arrayaux[posicaoChar] = textoIn[posicao];
                    posicaoChar++;
                    posicao++;
                }
                else
                {
                    posicao--;
                    break;
                }
            }
            string lido(arrayaux, posicaoChar);
            memset(arrayaux, 0, sizeof(arrayaux));
            if((lido == "program")||
               (lido == "var")||
               (lido == "integer")||
               (lido == "real")||
               (lido == "procedure")||
               (lido == "begin")||
               (lido == "end")||
               (lido == "if")||
               (lido == "then")||
               (lido == "else")||
               (lido == "while")||
               (lido == "do")||
               (lido == "not"))
            {
                aux.linha = linhaAtual;
                aux.tokenTipo = "Palavra Chave";
                aux.tokenNome = lido;
                tokenLista.push_back(aux);
            }
            else if(lido == "or")
            {
                aux.linha = linhaAtual;
                aux.tokenTipo = "Operador Aditivo";
                aux.tokenNome = lido;
                tokenLista.push_back(aux);
            }
            else if(lido == "and")
            {
                aux.linha = linhaAtual;
                aux.tokenTipo = "Operador multiplicativo";
                aux.tokenNome = lido;
                tokenLista.push_back(aux);
            }
            else
            {
                aux.linha = linhaAtual;
                aux.tokenTipo = "Identificador";
                aux.tokenNome = lido;
                tokenLista.push_back(aux);
            }
        }
        else if(textoIn[posicao] == ';' || textoIn[posicao] == '.' || textoIn[posicao] == ':' || textoIn[posicao] == '(' || textoIn[posicao] == ')' || textoIn[posicao] == ',')//Delimitadores
        {
            if(textoIn[posicao] == ':' && textoIn[posicao+1] == '=')
            {
                posicao++;
                aux.tokenNome = ":=";
                aux.tokenTipo = "Atribuicao";//IMPORTANTE NO FUTURO
                aux.linha = linhaAtual;
                tokenLista.push_back(aux);
            }
            else
            {
                stringstream ss;
                string s;
                ss << textoIn[posicao];
                ss >> s;
                aux.tokenNome = s;
                aux.tokenTipo = "Delimitador";
                aux.linha = linhaAtual;
                tokenLista.push_back(aux);

            }
        }
        else if(textoIn[posicao] == '<' || textoIn[posicao]  == '>' || textoIn[posicao] == '=')//RELACIONAIS
        {
            stringstream ss, dd;
            string s, extra, extra2;
            if(textoIn[posicao] == '=')
            {
                ss << textoIn[posicao];
                ss >> s;
                aux.tokenNome = s;
                aux.tokenTipo = "Operador Relacional";
                aux.linha = linhaAtual;
                tokenLista.push_back(aux);
            }
            else if(textoIn[posicao] == '<' || textoIn[posicao] == '>')
            {
                if(textoIn[posicao+1] >= '<'|| textoIn[posicao+1] <= '>')
                {
                    ss << textoIn[posicao];
                    ss >> s;
                    dd << textoIn[posicao + 1];
                    dd >> extra;
                    extra2 = s + extra;
                    if(extra2 == ">="||
                       extra2 == "<="||
                       extra2 == "<>")
                    {
                        aux.tokenNome = extra2;
                        aux.tokenTipo = "Operador Relacional";
                        aux.linha = linhaAtual;
                        tokenLista.push_back(aux);
                        posicao++;
                    }else
                    {
                    aux.tokenNome = s;
                    aux.tokenTipo = "Operador Relacional";
                    aux.linha = linhaAtual;
                    tokenLista.push_back(aux);
                    //posicao++;
                    }
                }
            }
        }
        else if(textoIn[posicao] == '+' || textoIn[posicao] == '-')//ADITIVOS
        {
            stringstream ss;
            string s;
            ss << textoIn[posicao];
            ss >> s;
            aux.tokenNome = s;
            aux.tokenTipo = "Operador Aditivo";
            aux.linha = linhaAtual;
            tokenLista.push_back(aux);
        /*Não otimizado***
            if(textoIn[posicao] == '+')
            {
                aux.tokenNome = "+";
                aux.tokenTipo = "Operador Aditivo";
                aux.linha = linhaAtual;
                tokenLista.push_back(aux);
            }
            else if(textoIn[posicao] == '-')
            {
                aux.tokenNome = "-";
                aux.tokenTipo = "Operador Aditivo";
                aux.linha = linhaAtual;
                tokenLista.push_back(aux);
            }*/
        }
        else if(textoIn[posicao] == '/' || textoIn[posicao] == '*')//MULTIPLICATIVOS
        {
            stringstream ss;
            string s;
            ss << textoIn[posicao];
            ss >> s;
            aux.tokenNome = s;
            aux.tokenTipo = "Operador Multiplicativo";
            aux.linha = linhaAtual;
            tokenLista.push_back(aux);
        }
            //Não otimizado
            /*
            if(textoIn[posicao] == '/')
            {
                aux.tokenNome = "/";
                aux.tokenTipo = "Operador Multiplicativo";
                aux.linha = linhaAtual;
                tokenLista.push_back(aux);
            }
            else if(textoIn[posicao] == '*')
            {
                aux.tokenNome = "*";
                aux.tokenTipo = "Operador Multiplicativo";
                aux.linha = linhaAtual;
                tokenLista.push_back(aux);
            }*/

        else if(textoIn[posicao] == ' '||textoIn[posicao] == 32)
        {
            espaco = 1;
        }
        else if(textoIn[posicao] == '   '||(textoIn[posicao] >= 9 && textoIn[posicao] <= 13))
        {
            tab = 1;
        }
        else if((!espaco)||(!tab))
        {
            cout << "Caractere não reconhecido: " << textoIn[posicao] << endl;
            cout << "Linha " << linhaAtual << endl;
            return error;
        }
        posicao++;
        espaco = 0;
        tab = 0;

    }
    return tokenLista;
}

int main()
{
    vector <char> x;
    vector <token> y;
    x = leArquivo("arquivo.txt");
    y = Tabela(x);
    int tamTokenLista = y.size();
    for(int i = 0; i < tamTokenLista; i++)
    {
        cout << y[i].tokenNome << " -- " << y[i].tokenTipo << " -- " << y[i].linha << endl;
    }

    return 0;
}

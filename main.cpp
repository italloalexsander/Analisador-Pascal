#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>
using namespace std;

int pos = 0;


typedef struct
{
    string var;
    string tipo;
}Pilha;

typedef struct{
    string tokenNome;
    string tokenTipo;
    int linha;
}token;

Pilha pilha[200];
Pilha checagemTipo[40];
int contEscopo = 0;
int pontPilha = 0;
int marcadorPilha = 0;
int pontPilhaChecagem = 0;
int contTipo = 0;
int tipoOp;


int tipo(vector <token> tabela, int must);
int lista_de_expressoes(vector <token> tabela, int must);
int declaracoes_de_subprogramas(vector <token> tabela, int must);
int fator(vector <token> tabela, int must);
int comando(vector <token> tabela, int must);
int comando_composto(vector <token> tabela, int must);
int expressao(vector <token> tabela, int must);


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
											posicao++;
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
        /*N�o otimizado***
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
            //N�o otimizado
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
            cout << "Caractere n�o reconhecido: " << textoIn[posicao] << endl;
            cout << "Linha " << linhaAtual << endl;
            return error;
        }
        posicao++;
        espaco = 0;
        tab = 0;

    }
    return tokenLista;
}

int lista_de_identificadores2(vector <token> tabela, int must)
{
	if(tabela[pos].tokenNome == ",")
	{
		pos++;
		if(tabela[pos].tokenTipo == "Identificador")
		{
		    marcadorPilha = pontPilha;
		    while(pilha[marcadorPilha].var != "$")
            {
                if(pilha[marcadorPilha].var == tabela[pos].tokenNome)
                {
                    cout << "\nIdentificador repetido: " << tabela[pos].tokenNome << endl;
                }
                marcadorPilha--;
                if(pilha[marcadorPilha].var == "$")//Checar essa operação depois
                {
                    pilha[pontPilha].var = tabela[pos].tokenNome;
                    pontPilha++;
                    contTipo++;
                }
            }
            pos++;
            if(lista_de_identificadores2(tabela, must))
            {
                return 1;
            }

            }
            else
            {
                printf("\nERRO linha %d ID", tabela[pos].linha);
                return 1;
            }
	}

	return 0;
}

int lista_de_identificadores(vector <token> tabela, int must)
{
	if(tabela[pos].tokenTipo == "Identificador")//Possivel altera��o depois para strcmp
	{
	    marcadorPilha = pontPilha;
	    while(pilha[marcadorPilha].var != "$")
        {
            if(pilha[marcadorPilha].var == tabela[pos].tokenNome)
            {
                cout << "\nIdentificador repetido: " << tabela[pos].tokenNome << endl;
            }

            marcadorPilha--;
            if(pilha[marcadorPilha].var == "$")
            {
                pilha[pontPilha].var = tabela[pos].tokenNome;
                pontPilha++;
                contTipo++;
            }
        }
		pos++;
		if(lista_de_identificadores2(tabela, 0))
		{
			return 1;
		}

		return 0;
	}
	else
	{
		if(must)
		{
			printf("\nERRO linha %d ID", tabela[pos].linha);
			return 1;
		}

	}

	return 1;
}


int lista_declaracoes_variaveis2(vector <token> tabela, int must)
{
	if(!lista_de_identificadores(tabela, must))
	{
		if(tabela[pos].tokenNome == ":")
		{
			pos++;
			if(!tipo(tabela, 1))
			{
				pos++;
				if(tabela[pos].tokenNome == ";")
				{
					pos++;
					if(lista_declaracoes_variaveis2(tabela, must))
					{
						return 1;
					}
					return 0;

				}
				else
				{
					printf("\nERROR linha %d expected ';'", tabela[pos].linha);
					return 1;
				}
			}
		}
		else
		{
			printf("\nERROR linha %d expected ':'", tabela[pos].linha);
			return 1;
		}
	}
	else
	{
		pos--;
	}
	return 0;
}

int tipo(vector <token> tabela, int must)
{
	if(tabela[pos].tokenNome == "integer")
	{
	    while(contTipo > 0)
        {
            pilha[pontPilha - contTipo].tipo = "Numero Inteiro";
            contTipo--;
        }
		return 0;
	}

	else if(tabela[pos].tokenNome == "real")
	{
	    while(contTipo > 0)
        {
            pilha[pontPilha - contTipo].tipo = "Numero Real";
            contTipo--;
        }
		return 0;
	}

	else if(tabela[pos].tokenNome == "boolean")
	{
	    while(contTipo > 0)
        {
            pilha[pontPilha - contTipo].tipo = "boolean";
            contTipo--;
        }
		return 0;
	}

	if(must)
	{
		printf("\nERRO linha %d TIPO", tabela[pos].linha);
		pos--;
		return 0;
	}

	return 1;
}

int lista_declaracoes_variaveis(vector <token> tabela, int must)
{
	lista_de_identificadores(tabela, must);
	if(tabela[pos].tokenNome == ":")
	{
		pos++;
		if(!tipo(tabela, 1))
		{
			pos++;
			if(tabela[pos].tokenNome == ";")
			{
				pos++;
				if(lista_declaracoes_variaveis2(tabela, 0))
				{
					return 1;
				}

				return 0;
			}
			else
			{
				if(must)
				{
					printf("\nERRO linha %d expected ';'", tabela[pos].linha);
				}
			}
		}
	}
	else
	{
		if(must)
		{
			printf("\nERRO linha %d expected ':'", tabela[pos].linha);
		}

	}

	return 1;
}

int declaracoes_variaveis(vector <token> tabela, int must)
{
	if(tabela[pos].tokenNome == "var")
	{
		pos++;
		lista_declaracoes_variaveis(tabela, 1);
	}
	else
	{
		pos--;
	}
	return 0;
}

int lista_de_parametros2(vector <token> tabela, int must)
{
	if(tabela[pos].tokenNome == ";")
	{
		pos++;
		if(!lista_de_identificadores(tabela, 1))
		{
			pos++;
			if(tabela[pos].tokenNome == ":")
			{
				pos++;
				if(!tipo(tabela, 1))
				{
					pos++;
					if(!lista_de_parametros2(tabela, 0))
					{
						return 0;
					}

				}
			}
			else
			{
				printf("\nERRO linha %d expected ':' ola", tabela[pos].linha);
			}
		}

		return 1;
	}
	else
	{
		pos--;
	}

	return 0;
}

int lista_de_parametros(vector <token> tabela, int must)
{

	if(!lista_de_identificadores(tabela, must))
	{

		if(tabela[pos].tokenNome == ":")
		{
			pos++;
			if(!tipo(tabela, 1))
			{
				pos++;
				if(!lista_de_parametros2(tabela, 0))
				{
					return 0;
				}
			}
		}
		else
		{
			printf("\nERRO linha %d expected ':' ha", tabela[pos].linha);
		}
	}
	return 1;
}

int argumentos(vector <token> tabela, int must)
{
	if(tabela[pos].tokenNome == "(")
	{
		pos++;
		if(!lista_de_parametros(tabela, 1))
		{
			pos++;
			if(tabela[pos].tokenNome == ")")
			{
				return 0;
			}
			else
			{
				printf("\nERRO linha %d expected ')'", tabela[pos].linha);
				return 1;
			}

		}

		return 1;
	}
	else
	{
		pos--;
	}

	return 0;
}

int sinal(vector <token> tabela, int must)
{
	if(tabela[pos].tokenNome == "+")
	{
		return 0;
	}
	else if(tabela[pos].tokenNome == "-")
	{
		return 0;
	}
	if(must)
	{
		printf("\nERRO linha %d SINAL", tabela[pos].linha);
	}
	return 1;
}

int op_multiplicativo(vector <token> tabela, int must)
{
	if(tabela[pos].tokenNome == "*")
	{
	    if(tipoOp != 0 && tipoOp != 2)
        {
            cout << "\n ERRO Uso indevido do operador aritmetico '*' linha ->" << tabela[pos].linha << endl;
        }
        if(tipoOp == 2)
        {
            checagemTipo[pontPilhaChecagem].var = "Operador Aritmetico";
            pontPilhaChecagem++;
        }
		return 0;
	}
	else if(tabela[pos].tokenNome == "/")
	{
	    if(tipoOp != 0 && tipoOp != 2)
        {
            cout << "\n ERRO Uso indevido do operador aritmetico '/' linha ->" << tabela[pos].linha << endl;
        }
        if(tipoOp == 2)
        {
            checagemTipo[pontPilhaChecagem].var = "Operador Aritmetico";
            pontPilhaChecagem++;
        }
		return 0;
	}
	else if(tabela[pos].tokenNome == "and")
	{
	    if(tipoOp != 1 && tipoOp != 2)
        {
            cout << "\n ERRO Uso indevido do operador logico 'and' linha ->" << tabela[pos].linha << endl;
        }
        if(tipoOp == 2)
        {
            checagemTipo[pontPilhaChecagem].var = "Operador Logico";
            pontPilhaChecagem++;
        }
		return 0;
	}
	if(must)
	{
		printf("\nERRO linha %d expected OP_MULTIPLICATIVO", tabela[pos].linha);
	}
	return 1;
}

int op_aditivo(vector <token> tabela, int must)
{
	if(tabela[pos].tokenNome == "+")
	{
	    if(tipoOp != 0 && tipoOp != 2)
        {
            cout << "\n ERRO Uso indevido do operador aritmetico '+' linha ->" << tabela[pos].linha << endl;
        }
        if(tipoOp == 2)
        {
            checagemTipo[pontPilhaChecagem].var = "Operador Aritmetico";
            pontPilhaChecagem++;
        }
		return 0;
	}
	else if(tabela[pos].tokenNome == "-")
	{
	    if(tipoOp != 0 && tipoOp != 2)
        {
            cout << "\n ERRO Uso indevido do operador aritmetico '-' linha ->" << tabela[pos].linha << endl;
        }
        if(tipoOp == 2)
        {
            checagemTipo[pontPilhaChecagem].var = "Operador Aritmetico";
            pontPilhaChecagem++;
        }
		return 0;
	}
	else if(tabela[pos].tokenNome == "or")
	{
	    if(tipoOp != 1 && tipoOp != 2)
        {
            cout << "\n ERRO Uso indevido do operador logico 'or' linha ->" << tabela[pos].linha << endl;
        }
        if(tipoOp == 2)
        {
            checagemTipo[pontPilhaChecagem].var = "Operador Logico";
            pontPilhaChecagem++;
        }
		return 0;
	}

	if(must)
	{
		printf("\nERRO linha %d expected OP_ADITIVO", tabela[pos].linha);
	}
	return 1;
}

int op_relacional(vector <token> tabela, int must)
{
	if(tabela[pos].tokenNome == "=")
	{
	    checagemTipo[pontPilhaChecagem].var = "Operador Relacional";
        pontPilhaChecagem++;
		return 0;
	}
	else if(tabela[pos].tokenNome == "<")
	{
	    checagemTipo[pontPilhaChecagem].var = "Operador Relacional";
        pontPilhaChecagem++;
		return 0;
	}
	else if(tabela[pos].tokenNome == ">")
	{
	    checagemTipo[pontPilhaChecagem].var = "Operador Relacional";
        pontPilhaChecagem++;
		return 0;
	}
	else if(tabela[pos].tokenNome == "<=")
	{
	    checagemTipo[pontPilhaChecagem].var = "Operador Relacional";
        pontPilhaChecagem++;
		return 0;
	}
	else if(tabela[pos].tokenNome == ">=")
	{
	    checagemTipo[pontPilhaChecagem].var = "Operador Relacional";
        pontPilhaChecagem++;
		return 0;
	}
	else if(tabela[pos].tokenNome == "<>")
	{
	    checagemTipo[pontPilhaChecagem].var = "Operador Relacional";
        pontPilhaChecagem++;
		return 0;
	}
	if(must)
	{
		printf("\nERRO linha %d expected OP_RELACIONAL", tabela[pos].linha);
	}
	return 1;
}

int termo2(vector <token> tabela, int must)
{
	if(!op_multiplicativo(tabela, must))
	{
		pos++;
		if(!fator(tabela, 1))
		{
			pos++;
			if(!termo2(tabela, 0))
			{
				return 0;
			}
		}

		return 1;
	}
	else
	{
		pos--;
	}
	return 0;
}

int termo(vector <token> tabela, int must)
{
	if(!fator(tabela, must))
	{
		pos++;
		if(!termo2(tabela, 0))
		{
			return 0;
		}
	}
	if(must)
	{
		printf("\nERRO linha %d TERMO INDETERMINADO", tabela[pos].linha);
	}
	return 1;
}

int fator(vector <token> tabela, int must)
{
	if(tabela[pos].tokenTipo == "Numero Inteiro")
	{
	    checagemTipo[pontPilhaChecagem].var = tabela[pos].tokenTipo;
	    if(tipoOp == 2)
        {
            checagemTipo[pontPilhaChecagem].var = "Numero";
        }
        pontPilhaChecagem++;
		return 0;
	}
	else if(tabela[pos].tokenTipo == "Numero Real")
	{
	    checagemTipo[pontPilhaChecagem].var = tabela[pos].tokenTipo;
	    if(tipoOp == 2)
        {
            checagemTipo[pontPilhaChecagem].var = "Numero";
        }
        pontPilhaChecagem++;
		return 0;
	}
	else if(tabela[pos].tokenNome == "true")
	{
        checagemTipo[pontPilhaChecagem].var = "boolean";
        pontPilhaChecagem++;
		return 0;
	}
	else if(tabela[pos].tokenNome == "false")
	{
        checagemTipo[pontPilhaChecagem].var = "boolean";
        pontPilhaChecagem++;
		return 0;
	}

	else if(tabela[pos].tokenNome == "(")
	{
		pos++;
		if(!expressao(tabela, 1))
		{
			pos++;
			if(tabela[pos].tokenNome == ")")
			{
				return 0;
			}
			else
			{
				printf("\nERRO linha %d expected ')'", tabela[pos].linha);
			}
		}

		return 1;
	}
	else if(tabela[pos].tokenNome == "not")
	{
		pos++;
		if(!fator(tabela, 1))
		{
			return 0;
		}
	}
    else if(tabela[pos].tokenTipo == "Identificador")
	{
        marcadorPilha = pontPilha;
        while(marcadorPilha >= 0)
        {
            if(pilha[marcadorPilha].var == tabela[pos].tokenNome)
            {
                break;
            }
            marcadorPilha--;
        }
        if(marcadorPilha < 0)
        {
            cout << "\n Identificador nao reconhecido: " << tabela[pos].tokenNome << endl;
        }
        else
        {
            checagemTipo[pontPilhaChecagem].var = pilha[marcadorPilha].tipo;
            if(tipoOp == 2 && pilha[marcadorPilha].tipo != "boolean")
            {
                checagemTipo[pontPilhaChecagem].var = "Numero";
            }
            pontPilhaChecagem++;
        }
		pos++;
		if(tabela[pos].tokenNome == "(")
		{
			pos++;
			if(!lista_de_expressoes(tabela, 1))
			{
				pos++;
				if(tabela[pos].tokenNome == ")")
				{
					return 0;
				}
				else
				{
					printf("\nERRO linha %d expected ')'", tabela[pos].linha);
				}
			}
			return 1;
		}
		else
		{
			pos--;
			return 0;
		}
	}
	if(must)
	{
		printf("\nERRO linha %d FATOR INDETERMINADO", tabela[pos].linha);
	}
	return 1;
}

int expressao_simples2(vector <token> tabela, int must)
{
	if(!op_aditivo(tabela, must))
	{
		pos++;
		if(!termo(tabela, 1))
		{
			pos++;
			if(!expressao_simples2(tabela, 0))
			{
				return 0;
			}
		}
		return 1;
	}
	else
	{
		pos--;
	}
	return 0;
}

int expressao_simples(vector <token> tabela, int must)
{
	if(!termo(tabela, 0))
	{
		pos++;
		if(!expressao_simples2(tabela, 0))
		{
			return 0;
		}
	}
	else if(!sinal(tabela, 0))
	{
		pos++;
		if(!termo(tabela, 1))
		{
			pos++;
			if(!expressao_simples2(tabela, 0))
			{
				return 0;
			}
		}
	}

	if(must)
	{
		printf("\nERRO linha %d TERMO", tabela[pos].linha);
		return 1;
	}
	return 0;
}


int expressao(vector <token> tabela, int must)
{
	if(!expressao_simples(tabela, must))
	{
		pos++;
		if(!op_relacional(tabela, 0))
		{
			pos++;
			if(!expressao_simples(tabela, 1))
			{
				return 0;
			}
		}
		else
		{
		    pos--;
			return 0;
		}
	}

	return 1;
}

int parte_else(vector <token> tabela, int must)
{
	if(tabela[pos].tokenNome == "else")
	{
		pos++;
		if(comando(tabela, 1))
		{
			return 1;
		}
		else
		{
			return 0;//LEMBRETE
		}
	}
	else
	{
		pos--;
	}
	return 1;
}

int lista_de_expressoes2(vector <token> tabela, int must)
{
	if(tabela[pos].tokenNome == ",")
	{
		pos++;
		if(!expressao(tabela, 1))
		{
			pos++;
			if(!lista_de_expressoes2(tabela, 0))
			{
				return 0;
			}
		}

		return 1;
	}
	else
	{
		pos--;
	}
	return 0;
}

int lista_de_expressoes(vector <token> tabela, int must)
{
	if(!expressao(tabela, must))
	{
		pos++;
		if(!lista_de_expressoes2(tabela, 0))
		{
			return 0;
		}
	}
	return 1;
}

int ativacao_de_procedimento(vector <token> tabela, int must)
{

    if(tabela[pos].tokenNome == "(")
    {
        pos++;
        if(!lista_de_expressoes(tabela, must))
        {
            pos++;
            if(tabela[pos].tokenNome == ")")
            {
                return 0;
            }
            else
            {
                printf("\nERRO linha %d", tabela[pos].linha);
            }
        }
    }
    /*
		else
		{
			return 0;
		}
	}
	else
	{
		if(must)
		{
			printf("\nERRO linha %d", tabela[pos].linha);
		}
	}*/
	return 1;
}

int variavel(vector <token> tabela, int must)
{
	if(tabela[pos].tokenTipo == "Identificador")
	{
	    marcadorPilha = pontPilha;
	    while(marcadorPilha >= 0)
        {
            if(pilha[marcadorPilha].var == tabela[pos].tokenNome)
            {
                break;
            }
            marcadorPilha--;
        }
        if(marcadorPilha < 0)
        {
            cout << "\n Identificador nao reconhecido: " << tabela[pos].tokenNome << endl;
        }
		return 0;
	}
	if(must)
	{
		printf("\nERRO linha %d", tabela[pos].linha);
	}
	return 1;
}

int comando(vector <token> tabela, int must)
{
	if(!variavel(tabela, 0))
	{
		pos++;
		if(!ativacao_de_procedimento(tabela, 0))
        {
            return 0;
        }
		else if(tabela[pos].tokenNome == ":=")
		{
		    pontPilhaChecagem = 0;
		    checagemTipo[pontPilhaChecagem].var = pilha[marcadorPilha].tipo;
		    pontPilhaChecagem++;
		    tipoOp = 0;
		    if(checagemTipo[0].var == "boolean")
            {
                tipoOp = 1;
            }
			pos++;
			if(!expressao(tabela, 1))
			{
			    while((pontPilhaChecagem > 1) && (tipoOp == 0))
                {
                    if(pontPilhaChecagem == 2)
                    {
                        if(!(checagemTipo[pontPilhaChecagem-1].var!=checagemTipo[0].var))
                        {
                            if(checagemTipo[0].var != "Numero Real")
                            {
                                cout << "\n Atribuicao errada Real -> Inteiro " << tabela[pos].tokenNome << "\nlinha :" << tabela[pos].linha << endl;

                            }
                        }
                    }

                    pontPilhaChecagem--;
                    if((checagemTipo[pontPilhaChecagem].var == "boolean") || (checagemTipo[pontPilhaChecagem-1].var == "boolean"))
                    {
                        cout << "\n Atribuicao errada booleana -> operacao aritmetica " << tabela[pos].tokenNome << "\nlinha :" << tabela[pos].linha << endl;
                        break;
                    }
                    if(checagemTipo[pontPilhaChecagem].var != checagemTipo[pontPilhaChecagem-1].var)
                    {
                        checagemTipo[pontPilhaChecagem-1].var = "Numero Real";
                    }
                }
                while((pontPilhaChecagem > 0) && (tipoOp == 1))
				{
				    pontPilhaChecagem--;
				    if(checagemTipo[pontPilhaChecagem].var != "boolean")
                    {
                        cout << "\n Atribuicao errada numerico -> operacao booleana " << tabela[pos].tokenNome << "\nlinha :" << tabela[pos].linha << endl;
                        break;
                    }
				}
				return 0;
			}
		}
		else
		{
			printf("\nERRO linha %d :=", tabela[pos].linha);
		}

		return 1;

	}/*
	else if(!ativacao_de_procedimento(tabela, 0))
	{
		return 0;
	}*/
	else if(!comando_composto(tabela, 0))
	{
		return 0;
	}
	else if(tabela[pos].tokenNome == "if")
	{
	    pontPilhaChecagem = 0;
	    tipoOp = 2;
		pos++;
		if(!expressao(tabela, 1))
		{
		    for(int j = 0; j < pontPilhaChecagem; j++)
            {
                if(checagemTipo[j].var == "boolean")
                {
                    if(++j < pontPilhaChecagem)
                    {
                        if(checagemTipo[j].var != "Operador Logico")
                        {

                            cout << "\n Atribuicao errada booleana -> operacao aritmetica " << tabela[pos].tokenNome << "\nlinha :" << tabela[pos].linha << endl;

                        }
                    }
                }
                else if(checagemTipo[j].var == "Numero")
                {
                    j++;
                    if(checagemTipo[j].var == "Operador Aritmetico")
                    {
                        if(checagemTipo[j].var != "Numero")
                        {
                            cout << "\n ERRO Expected 'Numero' na expressao " << tabela[pos].tokenNome << "\nlinha :" << tabela[pos].linha << endl;
                        }

                        if((j+1) < pontPilhaChecagem)
                        {
                            while(++j < pontPilhaChecagem)
                            {
                                if(checagemTipo[j].var == "Operador Aritmetico")
                                {
                                    if(checagemTipo[++j].var == "Numero")
                                    {
                                        cout << "\n ERRO Expected 'Numero' na expressao " << tabela[pos].tokenNome << "\nlinha :" << tabela[pos].linha << endl;
                                    }
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                        else
                        {
                            cout << "\n ERRO Expected 'Operador Relacional' " << tabela[pos].tokenNome << "\nlinha :" << tabela[pos].linha << endl;
                            continue;
                        }
                    }
                    if(checagemTipo[j].var == "Operador Relacional")
                    {
                        if(checagemTipo[++j].var != "Numero")
                        {
                            cout << "\n ERRO Expected 'Numero' " << tabela[pos].tokenNome << "\nlinha :" << tabela[pos].linha << endl;
                        }
                        while(++j  < pontPilhaChecagem)
                        {
                            if(checagemTipo[j].var == "Operador Aritmetico")
                            {
                                if(checagemTipo[++j].var != "Numero")
                                {
                                    cout << "\n ERRO Expected 'Numero' " << tabela[pos].tokenNome << "\nlinha :" << tabela[pos].linha << endl;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        cout << "\n ERRO Not Expected:  " << tabela[pos].tokenNome << "\nlinha :" << tabela[pos].linha << endl;
                    }

                }
                else if(checagemTipo[j].var == "Operador Logico")
                {
                    cout << "\n ERRO Expected 'Operador Logico' " << tabela[pos].tokenNome << "\nlinha :" << tabela[pos].linha << endl;
                }
            }
		    pos++;
			if(tabela[pos].tokenNome == "then")
			{
				pos++;
				if(!comando(tabela, 1))
				{
				    pos++;
					if(!parte_else(tabela, 0))
					{
						return 0;
					}
					//pos++;
					return 0;
				}
			}
			else
			{
				printf("\nERRO linha %d", tabela[pos].linha);
			}
		}

		return 1;
	}
	else if(tabela[pos].tokenNome == "while")
	{
	    pontPilhaChecagem = 0;
	    tipoOp = 2;
		pos++;
		if(tabela[pos].tokenNome == "(")
        {
            pos++;
            if(!expressao(tabela, 1))
            {
                //pos++;
                for(int j = 0; j < pontPilhaChecagem; j++)
                {
                    if(checagemTipo[j].var == "boolean")
                    {
                        if(++j < pontPilhaChecagem)
                        {
                            if(checagemTipo[j].var != "Operador Logico")
                            {
                                cout << "\n ERRO Expected 'Operador Logico' " << tabela[pos].tokenNome << "\n linha: " << tabela[pos].linha << endl;
                            }
                        }
                    }
                    else if(checagemTipo[j].var == "Numero")
                    {
                        j++;
                        if(checagemTipo[j].var == "Operador Aritmetico")
                        {
                            if(checagemTipo[j].var != "Numero")
                            {
                                cout << "\n ERRO Expected 'Numero' " << tabela[pos].tokenNome << "\n linha: " << tabela[pos].linha << endl;
                            }

                            if((j+1) < pontPilhaChecagem)
                            {
                                while(++j < pontPilhaChecagem)
                                {
                                    if(checagemTipo[j].var == "Operador Aritmetico")
                                    {
                                        if(checagemTipo[j].var != "Numero")
                                        {
                                            cout << "\n ERRO Expected 'Numero' " << tabela[pos].tokenNome << "\n linha: " << tabela[pos].linha << endl;
                                        }
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                cout << "\n ERRO Expected 'Operador Relacional' " << tabela[pos].tokenNome << "\n linha: " << tabela[pos].linha << endl;
                                continue;
                            }
                        }
                        if(checagemTipo[j].var == "Operador Relacional")
                        {
                            if(checagemTipo[++j].var != "Numero")
                            {
                                cout << "\n ERRO Expected 'NUmero' " << tabela[pos].tokenNome << "\n linha: " << tabela[pos].linha << endl;
                            }
                            while(++j < pontPilhaChecagem)
                            {
                                if(checagemTipo[j].var == "Operador Aritmetico")
                                {
                                    if(checagemTipo[++j].var != "Numero")
                                    {
                                        cout << "\n ERRO Expected 'Numero' " << tabela[pos].tokenNome << "\n linha: " << tabela[pos].linha << endl;

                                    }
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                        else
                        {
                            cout << "\n ERRO Unexpected " << tabela[pos].tokenNome << "\n linha: " << tabela[pos].linha << endl;

                        }
                    }
                    else if(checagemTipo[j].var == "Operador Logico")
                    {
                        cout << "\n ERRO Expected 'Operador Logico' " << tabela[pos].tokenNome << "\n linha: " << tabela[pos].linha << endl;
                    }
                }
                pos++;
                if(tabela[pos].tokenNome == ")")
                {
                    pos++;
                    if(tabela[pos].tokenNome == "do")
                    {
                        if(!comando(tabela, 1))
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    return 1;
                    printf("Expected ')' Linha %d", tabela[pos].linha);
                }

            }
		}
		else
        {
            return 1;
            printf("Expected '(' Linha %d", tabela[pos].linha);
        }


	}

  /*else if(tabela[pos].tokenNome == "do")
	{
		pos++;
		if(!comando(tabela, 1))
		{
			if(tabela[pos].tokenNome == "while")
			{
				pos++;
                if(tabela[pos].tokenNome == "(")
                {
                    if(!expressao(tabela, 1))
                    {
                        if(tabela[pos].tokenNome == ")") return 0;
                    }
                }

			}
			else
			{
				printf("\nERRO linha %d", tabela[pos].linha);
			}
		}

		return 1;

	}
    */
	if(must)
	{
		printf("\nERRO linha %d", tabela[pos].linha);
		return 1;
	}

	return 1;
}

int lista_de_comandos2(vector <token> tabela, int must)
{
    pos++;
	if(tabela[pos].tokenNome == ";")
	{
		pos++;
		if(!comando(tabela, 1))
		{
			if(!lista_de_comandos2(tabela, 0))
			{
				return 0;
			}
		}
		return 1;
	}

	return 0;
}

int lista_de_comandos(vector <token> tabela, int must)
{
	if(!comando(tabela, must))
	{
		if(!lista_de_comandos2(tabela, 0))
		{
			return 0;
		}
	}

	return 1;
}

int comandos_opcionais(vector <token> tabela, int must)
{
	if(lista_de_comandos(tabela, must))
	{
		return  1;
	}
	return 0;
}

int comando_composto(vector <token> tabela, int must)
{
	if(tabela[pos].tokenNome == "begin")
	{
	    contEscopo++;
		pos++;
		comandos_opcionais(tabela, 0);
		//pos++;
		if(tabela[pos].tokenNome == "end")
		{
		    contEscopo--;
		    //Imprime pilha de escopo
		    printf("\n\n\n");
		    for(int k = 0; k < pontPilha; k++)
                {
                    cout << "\n Pilha pos " << k << " : " << pilha[k].var << " " << pilha[k].tipo << endl;
                }
			if(contEscopo == 0)
			{
			    while(pilha[pontPilha].var != "$")
                {
                    pontPilha--;
                }
                pilha[pontPilha].var == " ";
			}
			pos++;

			return 0;
		}
		else
		{
			if(must)
			{
				printf("\nERRO linha %d falta end", tabela[pos].linha);
				printf("\nCausa: ");
				cout << tabela[pos].tokenNome;
			}
		}
	}
	else
	{
		if(must)
		{
            printf("\nERRO2 linha %d falta begin", tabela[pos].linha);
            printf("\nCausa: ");
            cout << tabela[pos].tokenNome;
		}
	}
	return 1;
}

int declaracao_de_subprograma(vector <token> tabela, int must)
{
	if(tabela[pos].tokenNome == "procedure")
	{
		pos++;
		if(tabela[pos].tokenTipo == "Identificador")
		{
		    marcadorPilha = pontPilha;
		    while(pilha[marcadorPilha].var != "$")
            {
                if(pilha[marcadorPilha].var == tabela[pos].tokenNome)
                {
                    cout << "\n Identificador repetido: " << tabela[pos].tokenNome << endl;
                }
                marcadorPilha--;
                if(pilha[marcadorPilha].var == "$")
                {
                    pilha[pontPilha].var = tabela[pos].tokenNome;
                    pontPilha++;
                }
            }
			pos++;
			if(!argumentos(tabela, 0))
			{
				pos++;
				if(tabela[pos].tokenNome == ";")
				{
					pos++;
					declaracoes_variaveis(tabela, 0);
					pos++;
					declaracoes_de_subprogramas(tabela, 0);
					pos++;
					comando_composto(tabela, 1);
					pos--;
					if(tabela[pos].tokenNome == "end")
					{
						pos--;
						return 0;
					}

					return 1;
				}
				else
				{
					printf("\nERRO linha oi %d", tabela[pos].linha);
				}
			}
		}
		else
		{
			printf("\nERRO linha ola %d", tabela[pos].linha);
		}
	}
	else
	{
		pos--;
		if(must)
		{
			printf("\nERRO linha hihi %d", tabela[pos].linha);
			return 1;
		}
	}

	return 1;
}

int declaracoes_de_subprogramas2(vector <token> tabela, int must)
{
	if(declaracao_de_subprograma(tabela, 0))
	{
		return 1;
	}
	else
	{
		pos+2;//possivelmente pos+=2;
		if(tabela[pos].tokenNome == ";")
		{
			pos++;
			if(declaracoes_de_subprogramas2(tabela, 0))
			{
				return 1;
			}
		}
		else
		{
			if(must)
			{
				printf("\nERRO linha %d", tabela[pos].linha);
			}
			return 1;
		}
	}
	return 0;
}

int declaracoes_de_subprogramas(vector <token> tabela, int must)
{
	if(declaracoes_de_subprogramas2(tabela, must))
	{
		return 1;
	}

	return 0;
}

int programa(vector <token> tabela, int must)
{
	if(tabela[pos].tokenNome == "program")
	{
	    pilha[pontPilha].var = "$";
	    pontPilha++;

		pos++;
		if(tabela[pos].tokenTipo == "Identificador")
		{
			pos++;
			if(tabela[pos].tokenNome == ";")
			{
				pos++;
				declaracoes_variaveis(tabela, 0);

				pos++;
				declaracoes_de_subprogramas(tabela, 0);

				pos++;
				comando_composto(tabela, 1);

				if(tabela[pos].tokenNome == ".")
				{
					return 0;
				}
				else
				{
					if(must)
					{
						printf("\nERRO linha %d", tabela[pos].linha);
					}
				}
			}
			else
			{
				if(must)
				{
					printf("\nERRO linha %d", tabela[pos].linha);
				}
			}
		}
		else
		{
			if(must)
			{
				printf("\nERRO linha %d", tabela[pos].linha);
			}
		}
	}
	else
	{
		if(must)
		{
			printf("\nERRO linha %d", tabela[pos].linha);
		}
	}
	return 1;
}

int Sintatico(vector <token> tabela)
{
	if(programa(tabela, 1))
	{
		return 1;
	}
	else
	{
		printf("\n\nSINTATICO -> CONCLUIDO");
		return 0;
	}
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
	Sintatico(y);

    return 0;
}

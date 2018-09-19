#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>
using namespace std;

int pos = 0;
int erroPil = 0;
typedef struct{
    string tokenNome;
    string tokenTipo;
    int linha;
}token;

struct conteudoPil{
    string var;
    string tipo;
};


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

string opAri;
vector <string> Pil;
vector <conteudoPil> refPil;

void PilCond(){
    int topo = Pil.size() - 1;
    int subtopo = topo - 1;
    if(Pil[topo] == "boolean" && Pil[subtopo] == "boolean")
    {
        Pil.pop_back();
        Pil.pop_back();
    }
    else
    {
        if(!erroPil)
        {
            erroPil++;
            cout << "\nERROR (condicao)";//completar depois
        }
    }
    Pil.clear();
}

void PilArit()
{
    int topo = Pil.size() - 1;
    int subtopo = topo - 1;
    if(opAri == "and" || opAri == "or")
    {
        if(Pil[topo] == "boolean" && Pil[subtopo] == "boolean")
        {
            Pil.pop_back();
        }
        else
        {
            if(!erroPil)
            {
                erroPil++;
                cout << "\n ERROR (logico)";
            }
        }
    }
    else
    {
        if(Pil[topo] == "Numero Inteiro" && Pil[subtopo] == "Numero Inteiro")
        {
            Pil.pop_back();
        }
        else if(Pil[topo] == "Numero Inteiro" && Pil[subtopo] == "Numero Real")
        {
            Pil.pop_back();
        }
        else if(Pil[topo] == "Numero Real" && Pil[subtopo] == "Numero Inteiro")
        {
            Pil.pop_back();
            Pil.pop_back();
            Pil.push_back("Numero Real");
        }
        else if(Pil[topo] == "Numero Real" && Pil[subtopo] == "Numero Real")
        {
            Pil.pop_back();
        }
        else{
            if(!erroPil)
            {
                erroPil++;
                cout << "ERRO (Aritmetico)";
            }
        }
    }
}

void PilRel(vector <token> tabela)
{

    int topo = Pil.size() - 1;
    int subtopo = topo - 1;

    if(Pil[topo] == "Numero Inteiro" && Pil[subtopo] == "Numero Inteiro")
    {
        Pil.pop_back();
        Pil.pop_back();
        Pil.push_back("boolean");
    }
    else if(Pil[topo] == "Numero Inteiro" && Pil[subtopo] == "Numero Real")
    {
        Pil.pop_back();
        Pil.pop_back();
        Pil.push_back("boolean");
    }
    else if(Pil[topo] == "Numero Real" && Pil[subtopo] == "Numero Inteiro")
    {
        Pil.pop_back();
        Pil.pop_back();
        Pil.push_back("boolean");
    }
    else if(Pil[topo] == "Numero Real" && Pil[subtopo] == "Numero Real")
    {
        Pil.pop_back();
        Pil.pop_back();
        Pil.push_back("boolean");
    }
    else{
        if(!erroPil)
        {
            erroPil++;
            cout << "ERRO (Relacional)" << tabela[pos].linha;
        }
    }
}

int contia = 0;

void PilAtri(token erro)
{
    //cout << "\n " << erro.tokenTipo;
    int topo = Pil.size() - 1;
    int subtopo = topo - 1;
    contia++;
    cout<<"\n" << Pil[topo] << " " << Pil[subtopo];
    cout<<"\n" << contia;

    if(Pil[topo] == "Numero Inteiro" && Pil[subtopo] == "Numero Inteiro")
    {
        Pil.pop_back();
        Pil.pop_back();
    }
    else if(Pil[topo] == "Numero Inteiro" && Pil[subtopo] == "Numero Real")
    {
        Pil.pop_back();
        Pil.pop_back();
    }
    else if(Pil[topo] == "Numero Real" && Pil[subtopo] == "Numero Real")
    {
        Pil.pop_back();
        Pil.pop_back();
    }
    else if(Pil[topo] == "boolean" && Pil[subtopo] == "boolean")
    {
        Pil.pop_back();
        Pil.pop_back();
    }
    else
    {
        if(!erroPil)
        {

            erroPil++;
            cout << "\nERRO (Atribuicao)";
            cout << "\nSimbolo: " << erro.tokenNome << "\nlinha: " << erro.linha;
        }
    }
    Pil.clear();
}

void Pil_Inicio()
{
    conteudoPil duo;
    duo.var = "$";
    duo.tipo = "ref";
    refPil.push_back(duo);
}

int checaPil(vector <token> tabela)
{
    int aux = 0;
    for(int i = refPil.size() - 1; i > 0; --i)
    {
        if(refPil[i].var == tabela[pos].tokenNome)
        {
            aux++;
            break;
        }
    }
    if(!aux)
    {
        cout << "\n identificador nao reconhecido/declarado";
        erroPil++;
        return 0;
    }
    return 1;
}

void declaraPil(vector <token> tabela)
{
    conteudoPil duo;
    duo.var = tabela[pos].tokenNome;
    duo.tipo = "NULL";

    for(int i = refPil.size() - 1; i >= 0; --i){
        if(refPil[i].var == "$")
        {
            refPil.push_back(duo);
            break;
        }
        else if(refPil[i].var == tabela[pos].tokenNome)
        {
            cout << "ERRO Ja declarada nesse escopo";
            erroPil++;
            break;
        }
    }
}

void Escopo()
{
    conteudoPil duo;
    duo.var = "$";
    duo.tipo = "ref";
    refPil.push_back(duo);
}

void PopEscopo()
{
    while(refPil.back().var != "$")
    {
        refPil.pop_back();
    }
    refPil.pop_back();

}

void IdentificaPil(vector <token> tabela)
{
    for(int i = refPil.size() - 1; i >= 0; --i)
    {
        if(refPil[i].tipo != "NULL")
        {
            break;
        }
        else
        {
            if(tabela[pos].tokenNome == "integer")
                refPil[i].tipo = "Numero Inteiro";
            if(tabela[pos].tokenNome == "real")
                refPil[i].tipo = "Numero Real";
            if(tabela[pos].tokenNome == "boolean")
                refPil[i].tipo = "boolean";
        }
    }
}

string checaTipo(token aux)
{
    for(int i = refPil.size() - 1; i >= 0; --i)
    {
        if(refPil[i].var == aux.tokenNome)
        {
            return refPil[i].tipo;
        }
    }
    return " ";

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
		    declaraPil(tabela);
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
	    declaraPil(tabela);
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
					if(lista_declaracoes_variaveis2(tabela, must) == 1)
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
	    IdentificaPil(tabela);
		return 0;
	}

	else if(tabela[pos].tokenNome == "real")
	{
	    IdentificaPil(tabela);
		return 0;
	}

	else if(tabela[pos].tokenNome == "boolean")
	{
	    IdentificaPil(tabela);
		return 0;
	}

	if(must)
	{
		printf("\nERRO linha %d TIPO", tabela[pos].linha);
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
	    //pos++;
		if(tabela[pos].tokenNome == ":")
		{
			pos++;
			if(!tipo(tabela, must))
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
		return 0;
	}
	else if(tabela[pos].tokenNome == "/")
	{
		return 0;
	}
	else if(tabela[pos].tokenNome == "and")
	{
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
		return 0;
	}
	else if(tabela[pos].tokenNome == "-")
	{
		return 0;
	}
	else if(tabela[pos].tokenNome == "or")
	{
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
		return 0;
	}
	else if(tabela[pos].tokenNome == "<")
	{
		return 0;
	}
	else if(tabela[pos].tokenNome == ">")
	{
		return 0;
	}
	else if(tabela[pos].tokenNome == "<=")
	{
		return 0;
	}
	else if(tabela[pos].tokenNome == ">=")
	{
		return 0;
	}
	else if(tabela[pos].tokenNome == "<>")
	{
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
		    PilArit();
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

	if(tabela[pos].tokenTipo == "Identificador")
	{
	    //pos++;
	    if(checaPil(tabela))
        {
            token aux = tabela[pos];
            Pil.push_back(checaTipo(aux));
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
	else if(tabela[pos].tokenTipo == "Numero Inteiro")
	{
		return 0;
	}
	else if(tabela[pos].tokenTipo == "Real")
	{
		return 0;
	}
	else if(tabela[pos].tokenNome == "true")
	{
		return 0;
	}
	else if(tabela[pos].tokenNome == "false")
	{
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
		    PilArit();
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
			    PilRel(tabela);
				return 0;
			}
		}
		else
		{
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
	if(tabela[pos].tokenTipo == "Identificador")
	{
		pos++;
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
	}
	return 1;
}

int variavel(vector <token> tabela, int must)
{
	if(tabela[pos].tokenTipo == "Identificador")
	{
	    if(checaPil(tabela))
        {
            token aux = tabela[pos];
            Pil.push_back(checaTipo(aux));
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

        token aux = tabela[pos];
		pos++;
		if(!ativacao_de_procedimento(tabela, 0))
        {
            return 0;
        }
		else if(tabela[pos].tokenNome == ":=")
		{
		    Pil.push_back(checaTipo(aux));
			pos++;
			if(!expressao(tabela, 1))
			{
			    PilAtri(aux);
				return 0;
			}
		}
		else
		{
		    cout << "\n " <<tabela[pos].tokenNome;
			printf("\nERRO linha %d :=", tabela[pos].linha);
		}

		return 1;

	}
	/*else if(!ativacao_de_procedimento(tabela, 0))
	{
		return 0;
	}*/
	else if(!comando_composto(tabela, 0))
	{
		return 0;
	}
	else if(tabela[pos].tokenNome == "if")
	{
	    Pil.push_back("boolean");
		pos++;
		if(!expressao(tabela, 1))
		{
			if(tabela[pos].tokenNome == "then")
			{
				pos++;
				if(!comando(tabela, 1))
				{
					if(!parte_else(tabela, 0))
					{
						return 0;
					}
					pos++;
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
	    Pil.push_back("boolean");
		pos++;
		if(!expressao(tabela, 1))
		{
			if(tabela[pos].tokenNome == "do")
			{
				pos++;
				if(!comando(tabela, 1))
				{
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

  else if(tabela[pos].tokenNome == "do")
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

	if(must)
	{
		printf("\nERRO linha %d", tabela[pos].linha);
		return 1;
	}

	return 1;
}

int lista_de_comandos2(vector <token> tabela, int must)
{
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
		pos++;
		comandos_opcionais(tabela, 0);
		if(tabela[pos].tokenNome == "end")
		{
			pos++;
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
	return 1;
}

int declaracao_de_subprograma(vector <token> tabela, int must)
{
	if(tabela[pos].tokenNome == "procedure")
	{
	    Escopo();
		pos++;
		if(tabela[pos].tokenTipo == "Identificador")
		{
		    declaraPil(tabela);
		    refPil.back().tipo = "procedure";
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
					PopEscopo();
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
			printf("\nERRO linha %d", tabela[pos].linha);
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
		pos++;
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
    Pil_Inicio();
	if(tabela[pos].tokenNome == "program")
	{
		pos++;
		if(tabela[pos].tokenTipo == "Identificador")
		{
		    declaraPil(tabela);
		    refPil.back().tipo = "program";
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

#include"synt.h"


bool synt::isterminal(tokens symbol)
{
	for (int i = 0; i < terminals.size(); i++)
		if (terminals[i] == symbol)
			return true;
	return false;
}
bool synt::isnonterminal(tokens symbol)
{
	for (int i = 0; i < nonterminals.size(); i++)
		if (nonterminals[i] == symbol)
			return true;
	return false;
}
bool synt::belongs(tokens symbol, vector<tokens> V)
{
	for (int i = 0; i < V.size(); i++)
		if (V[i] == symbol)
			return true;
	return false;
}
vector<synt::tokens> synt::begin(tokens symbol)
{
	vector<tokens> res;
	vector<tokens> queue; //������� ������������, ���������� �� ������ ������ ������
	queue.push_back(symbol);//���������� � ������� ������ ����������� ����������

	while (true)//���� ������� �� �����
	{
		for (int i = 0; i < rules.size(); i++)//������� ������ �������
		{
			if (rules[i].first == symbol)
			{
				vector<tokens> V = rules[i].second;
				tokens S = V[0];
				if (isnonterminal(S))//������� � ���� ��� ����������. ���� ���������� � ���������, �� ������ ��������� ���� �������� �� ���������. ���� ����������, �� �������� ��� � ������� � ��������� �� ���������
					if (!belongs(S, queue))
						queue.push_back(S);
				if (!belongs(S, res))
					res.push_back(S);
			}
		}
		queue.erase(queue.begin());
		if (queue.empty())
			break;
		symbol = queue.front();
	}
	return res;
}
vector<synt::tokens> synt::back(tokens symbol)
{
	vector<tokens> res;
	vector<tokens> queue; //������� ������������, ���������� �� ������ ������ ������
	vector<tokens> help;
	queue.push_back(symbol);//���������� � ������� ������ ����������� ����������
	while (true)//���� ������� �� �����
	{
		for (int i = 0; i < rules.size(); i++)//������� ������ �������
		{
			if ((rules[i].first) == symbol)
			{
				vector<tokens> V = rules[i].second;
				tokens S = V[V.size() - 1];
				if (isnonterminal(S))//������� � ���� ��� ����������. ���� ���������� � ���������, �� ������ ��������� ���� �������� �� ���������. ���� ����������, �� �������� ��� � ������� � ��������� �� ���������
					if (!belongs(S, queue) && !belongs(S, help))
					{
						queue.push_back(S);
						help.push_back(S);
					}
				if (!belongs(S, res))
					res.push_back(S);
			}
		}
		queue.erase(queue.begin());
		if (queue.empty())
			break;
		symbol = queue.front();
	}
	return res;
}
vector<synt::tokens> synt::first(tokens symbol)
{
	vector<tokens> res;
	vector<tokens> queue; //������� ������������, ���������� �� ������ ������ ������
	queue.push_back(symbol);//���������� � ������� ������ ����������� ����������

	while (true)//���� ������� �� �����
	{
		for (int i = 0; i < rules.size(); i++)//������� ������ �������
		{
			if (rules[i].first == symbol)
			{
				vector<tokens> V = rules[i].second;
				tokens S = V[0];
				if (isnonterminal(S))//������� � ���� ��� ����������. ���� ���������� � ���������, �� ������ ��������� ���� �������� �� ���������. ���� ����������, �� ������ ��� � �������
				{
					if (!belongs(S, queue))
						queue.push_back(S);
				}
				else
				{
					if (!belongs(S, res))
						res.push_back(S);
				}
			}
		}
		queue.erase(queue.begin());
		if (queue.empty())
			break;
		symbol = queue.front();
	}
	return res;
}
void synt::set_base()
{
	for (int i = 0; i < rules.size(); i++)
	{
		vector<tokens> V = rules[i].second;
		for (int j = 0; j < V.size() - 1; j++)
		{
			tokens k = V[j];
			tokens m = V[j + 1];
			controle_table[(int)k][(int)m] = relations::base;
		}
	}
}
void synt::set_less()
{
	for (int i = 0; i < rules.size(); i++)
	{
		vector<tokens> V = rules[i].second;
		for (int j = 0; j < V.size() - 1; j++)
		{
			tokens k = V[j];
			tokens m = V[j + 1];
			if (isnonterminal(m))
			{
				vector<tokens> BEGIN = begin(m);
				for (int p = 0; p < BEGIN.size(); p++)
				{
					if (controle_table[(int)k][(int)BEGIN[p]] == relations::reject || controle_table[(int)k][(int)BEGIN[p]] == relations::less)
						controle_table[(int)k][(int)BEGIN[p]] = relations::less;
					else {
						cout << "�������� ��������� " << lexems[(int)k] << " � " << lexems[(int)BEGIN[p]] << ": ����� ���� ��� ����� =. " << endl;;
						exit(0);
					}
				}
			}
		}
	}
}
void synt::set_great()
{
	for (int i = 0; i < rules.size(); i++)
	{
		vector<tokens> V = rules[i].second;
		for (int j = V.size() - 1; 0 < j; j--)
		{
			tokens k = V[j];/*                    XC  -> back(m) greater first(k)*/
			tokens m = V[j - 1];

			if (isnonterminal(m) && isterminal(k))//���� X - ����������, � � - ��������
			{
				vector<tokens> BACK = back(m);
				for (int p = 0; p < BACK.size(); p++)
					if (controle_table[(int)BACK[p]][(int)k] == relations::reject || controle_table[(int)BACK[p]][(int)k] == relations::great)
						controle_table[(int)BACK[p]][(int)k] = relations::great;
					else
					{
						cout << "�������� ��������� " << lexems[(int)BACK[p]] << " � " << lexems[(int)k] << ": ����� ���� ��� ����� <." << endl;
						exit(0);
					}

			}
			else
				if (isnonterminal(m) && isnonterminal(k))//���� X - ����������, � � - ����������
				{
					vector<tokens> BACK = back(m);
					vector<tokens> FIRST = first(k);
					for (int p = 0; p < BACK.size(); p++)
						for (int q = 0; q < FIRST.size(); q++)
							if (controle_table[(int)BACK[p]][(int)FIRST[q]] == relations::reject || controle_table[(int)BACK[p]][(int)FIRST[q]] == relations::great)
								controle_table[(int)BACK[p]][(int)FIRST[q]] = relations::great;
							else {
								cout << "�������� ��������� " << lexems[(int)BACK[p]] << " � " << lexems[(int)FIRST[q]] << ": ����� ���� ��� ����� <." << endl;
								exit(0);
							}
				}
		}
	}
}
bool synt::dissasemble()
{
	vector<lexema> stack1;
	stack<lexema> stack2;
	vector<tokens> help;
	vector<tokens> help2;
	stack1.push_back({ tokens::$, "","", 0, 0, 0 });
	stack2.push({ tokens::$, "","", 0, 0, 0 });// � ����� 2 �������� ����������� �������+$
	TOKENS.pop_back();//������� FINAL
	auto it = TOKENS.rbegin();
	if (TOKENS.empty())
	{
		cout << "����������� ���������. ";
		return false;
	}
	for (; it < TOKENS.rend() - 1; it++)
		stack2.push(*it);
	stack2.push(*it);
	while (true)
	{
		tokens i = stack1.back().Lex_class;
		tokens j = stack2.top().Lex_class;

		if (controle_table[(int)i][(int)j] == relations::less)
		{
			stack1.push_back(stack2.top());
			stack2.pop();
		}

		if (controle_table[(int)i][(int)j] == relations::base)
		{
			stack1.push_back(stack2.top());
			stack2.pop();
		}

		if (controle_table[(int)i][(int)j] == relations::great)
		{
			int index_i = stack1.size(), index_j = stack1.size() - 1;
			do
			{
				index_i--;
				index_j--;
				help.push_back(stack1[index_i].Lex_class);

			} while (controle_table[(int)stack1[index_j].Lex_class][(int)stack1[index_i].Lex_class] != relations::less);

			for (auto it = help.rbegin(); it < help.rend(); it++)
				help2.push_back(*it);

			vector<pair<tokens, vector<tokens>>>::iterator it = rules.begin(); //������� �������, �� �������� ����������� 

			for (; it < rules.end(); it++)
				if (get<vector<tokens>>(*it) == help2)
					break;
			if (it == rules.end())
			{
				cout << "�� ������� �������: ";
				for (int i = 0; i < help2.size(); i++)
					cout << lexems[(int)help2[i]] << ' ';
				return false;
			}//���� ����������� ������� ���

			tokens Nonterminal = get<tokens>(*it);//����������, �� ��� �����������
			class_reg = Nonterminal;

			stack1.erase(stack1.begin() + index_i, stack1.end());
			stack1.push_back(create_lexema());

			while (!help2.empty())
				help2.pop_back();
			while (!help.empty())
				help.pop_back();
		}

		if (controle_table[(int)i][(int)j] == relations::allow)
			return true;

		if (controle_table[(int)i][(int)j] == relations::reject)
			return false;
	}
}
void synt::see_table()
{
	ofstream f("table.txt");

	f << setw(10) << left << "  ";


	for (int i = 0; i < terminals.size() + nonterminals.size() + 1; i++)
		f << setw(10) << right << lexems[i];

	endl(f);
	for (int i = 0; i < terminals.size() + nonterminals.size() + 1; i++)
	{
		f << setw(10) << left << lexems[i];
		for (int j = 0; j < terminals.size() + nonterminals.size() + 1; j++)
		{
			switch (controle_table[i][j])
			{
			case relations::reject:
				f << setw(10) << right << "- ";
				break;
			case relations::allow:
				f << setw(10) << right << "+ ";
				break;
			case relations::less:
				f << setw(10) << right << ".< ";
				break;
			case relations::base:
				f << setw(10) << right << ".= ";
				break;
			case relations::great:
				f << setw(10) << right << ".> ";
				break;
			}
		}
		endl(f);
	}
}

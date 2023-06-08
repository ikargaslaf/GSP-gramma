#pragma once


#include"lex.h"
#include<stack>
#include<iomanip>

class synt : public lexical
{
private:
	enum class relations { base, less, great, reject, allow };
	vector<lexical::tokens> terminals = {
		tokens::Var,
		tokens::Const,
		tokens::Semicolon,
		tokens::Colon,
		tokens::Comma,
		tokens::L_bracket,
		tokens::R_bracket,
		tokens::plus,
		tokens::minus,
		tokens::mult,
		tokens::div,
		tokens::mod,
		tokens::l,
		tokens::g,
		tokens::e,
		tokens::le,
		tokens::ge,
		tokens::ne,
		tokens::Cin_thing,
		tokens::Cout_thing,
		tokens::Label,
		tokens::Comment,
		tokens::Declare,
		tokens::Each, 
		tokens::Else, 
		tokens::End, 
		tokens::Exception, 
		tokens::Goto, 
		tokens::If,
		tokens::In, 
		tokens::Let, 
		tokens::Otherwise, 
		tokens::Read, 
		tokens::Sizet, 
		tokens::Switch, 
		tokens::Then, 
		tokens::Vector, 
		tokens::When, 
		tokens::While,
		tokens::Write,
		tokens::Do 
	};

	vector<lexical::tokens> nonterminals = { 
		tokens::P, 
		tokens::OP, 
		tokens::TYPE, 
		tokens::LET, 
		tokens::VLIST, 
		tokens::E, 
		tokens::Ee,
		tokens::T, 
		tokens::Tt, 
		tokens::F, 
		tokens::READ, 
		tokens::VAR, 
		tokens::COUT1, 
		tokens::COUT2, 
		tokens::EX, 
		tokens::CASES, 
		tokens::CASE, 
		tokens::DEFAULT, 
		tokens::SWITCH, 
		tokens::CLIST, 
		tokens::TEST, 
		tokens::RELATION, 
		tokens::WHILE, 
		tokens::FOR1, 
		tokens::FOR2, 
		tokens::FOR3, 
		tokens::IF, 
		tokens::ELSE 
	};

	vector<pair<tokens, vector<tokens>>> rules = {

		{ tokens::P,        /*->*/  { tokens::OP, tokens::P }},
		{ tokens::P,        /*->*/  { tokens::OP}},

		//пустой оператор(работает)
		{ tokens::OP,       /*->*/  { tokens::Semicolon}},

		//Коментарий(работает)
		{ tokens::OP,       /*->*/  { tokens::Comment}},

		//Исключение(работает)
		{ tokens::OP,       /*->*/  { tokens::Exception}},

		//Метка и переход к метке(работает)
		{ tokens::OP,       /*->*/  { tokens::Label}},
		{ tokens::OP,       /*->*/  { tokens::Goto, tokens::Label }},

		//Обьявление переменной(работает)
		{ tokens::OP,       /*->*/  { tokens::Declare, tokens::Var, tokens::TYPE, tokens::Semicolon }},
		{ tokens::TYPE,     /*->*/  { tokens::Sizet }},
		{ tokens::TYPE,     /*->*/  { tokens::Vector }},

		//Присваивание(работает)
		{ tokens::OP,       /*->*/  { tokens::LET, tokens::e, tokens::E, tokens:: Semicolon }},
		{ tokens::LET,      /*->*/  { tokens::Let, tokens::Var }},

		//Ввод(Работает)
		{ tokens::OP,       /*->*/  { tokens::READ, tokens::Semicolon }},
		{ tokens::READ,     /*->*/  { tokens::VLIST }},
		{ tokens::VLIST,    /*->*/  { tokens::VLIST, tokens::Comma, tokens::Var }},
		{ tokens::VLIST,    /*->*/  { tokens::Read, tokens::Var }},

		//Вывод(работает)
		{ tokens::OP,       /*->*/  { tokens::COUT1 }},
		{ tokens::COUT1,    /*->*/  { tokens::Write, tokens::COUT2 }},
		{ tokens::COUT2,    /*->*/  { tokens::Cout_thing, tokens::E, tokens::COUT2 }},
		{ tokens::COUT2,    /*->*/  { tokens::EX }},
		{ tokens::EX,       /*->*/  { tokens::Cout_thing, tokens::E , tokens::Semicolon}},

		//Условный оператор
		{ tokens::OP,       /*->*/  { tokens::IF, tokens::End, tokens::Semicolon }},
		{ tokens::OP,       /*->*/  { tokens::IF, tokens::ELSE, tokens::End, tokens::Semicolon }},
		{ tokens::IF,       /*->*/  { tokens::If, tokens::TEST, tokens::Then,tokens::P }},
		{ tokens::ELSE,     /*->*/  { tokens::Else, tokens::P }},

		//цикл(работает)
		{ tokens::OP,       /*->*/  { tokens::WHILE, tokens::End, tokens::Semicolon }},
		{ tokens::WHILE,    /*->*/  { tokens::While, tokens::TEST, tokens::Do , tokens::P }},

		//цикл с параметром
		{ tokens::OP,       /*->*/  { tokens::FOR1, tokens::End, tokens::Semicolon }},
		{ tokens::FOR1,     /*->*/  { tokens::FOR2, tokens::Do , tokens::P }},
		{ tokens::FOR2,     /*->*/  { tokens::For,  tokens::Each, tokens::Var, tokens::In, tokens:: E }},

		//swicth - case(работает)
		{ tokens::OP,       /*->*/  { tokens::CASES, tokens::DEFAULT, tokens::End, tokens::Semicolon }},
		{ tokens::OP,       /*->*/  { tokens::CASES,tokens::End, tokens::Semicolon }},
		{ tokens::CASES,    /*->*/  { tokens::CASES, tokens::CASE }},
		{ tokens::CASES,    /*->*/  { tokens::SWITCH, tokens::CASE }},
		{ tokens::CASE,     /*->*/  { tokens::CLIST, tokens::Colon, tokens::P}},
		{ tokens::CLIST,    /*->*/  { tokens::CLIST, tokens::Comma, tokens::Const }},
		{ tokens::CLIST,    /*->*/  { tokens::When, tokens::Const }},
		{ tokens::DEFAULT,  /*->*/  { tokens::Otherwise, tokens::Colon, tokens::P }},
		{ tokens::SWITCH,   /*->*/  { tokens::Switch, tokens::L_bracket, tokens::E, tokens::R_bracket }},

		//Выражения(работает)
		{ tokens::E,        /*->*/  { tokens::T }},
		{ tokens::E,        /*->*/  { tokens::T, tokens::Ee }}, 
		{ tokens::Ee,       /*->*/  { tokens::plus, tokens::T,tokens::Ee }},
		{ tokens::Ee,       /*->*/  { tokens::minus,tokens::T, tokens::Ee }},
		{ tokens::Ee,       /*->*/  { tokens::plus, tokens::T }},
		{ tokens::Ee,       /*->*/  { tokens::minus,tokens::T }},
		{ tokens::T,        /*->*/  { tokens::F }},
		{ tokens::T,        /*->*/  { tokens::F, tokens::Tt }},
		{ tokens::Tt,       /*->*/  { tokens::mult, tokens::F, tokens::Tt }},
		{ tokens::Tt,       /*->*/  { tokens::div, tokens::F, tokens::Tt }},
		{ tokens::Tt,       /*->*/  { tokens::mod, tokens::F, tokens::Tt }},
		{ tokens::Tt,       /*->*/  { tokens::mult, tokens::F }},
		{ tokens::Tt,       /*->*/  { tokens::div, tokens::F }},
		{ tokens::Tt,       /*->*/  { tokens::mod, tokens::F }},
		{ tokens::F,        /*->*/  { tokens::L_bracket, tokens::E, tokens::R_bracket }},
		{ tokens::F,        /*->*/  { tokens::Const }},
		{ tokens::F,        /*->*/  { tokens::Var }},

		//Тест(работает)
		{ tokens::TEST,     /*->*/  { tokens::E,  tokens::RELATION }},
		{ tokens::RELATION, /*->*/  { tokens::e,  tokens::E }},
		{ tokens::RELATION, /*->*/  { tokens::l,  tokens::E }},
		{ tokens::RELATION, /*->*/  { tokens::g,  tokens::E}},
		{ tokens::RELATION, /*->*/  { tokens::le, tokens::E }},
		{ tokens::RELATION, /*->*/  { tokens::ge, tokens::E }},
		{ tokens::RELATION, /*->*/  { tokens::ne, tokens::E }},
	};
	relations controle_table[100][100];
	bool isterminal(tokens symbol);
	bool isnonterminal(tokens symbol);
	bool belongs(tokens symbol, vector<tokens> V);
	vector<tokens> begin(tokens symbol);
	vector<tokens> back(tokens symbol);
	vector<tokens> first(tokens symbol);
	void set_base();
	void set_less();
	void set_great();
public:
	synt()
	{
		for (int i = 0; i < terminals.size() + nonterminals.size() + 1; i++)
			for (int j = 0; j < terminals.size() + nonterminals.size() + 1; j++)
				controle_table[i][j] = relations::reject;
		vector<tokens> P1 = begin(tokens::P);
		vector<tokens> P2 = back(tokens::P);

		for (int i = 0; i < P1.size(); i++)
			controle_table[(int)tokens::$][(int)P1[i]] = relations::less;
		for (int i = 0; i < P2.size(); i++)
			controle_table[(int)P2[i]][(int)tokens::$] = relations::great;

		controle_table[(int)tokens::P][(int)tokens::$] = relations::allow;

		set_base();
		set_less();
		set_great();
		see_table();
	};

	~synt() {};
	bool dissasemble();
	void see_table();
};

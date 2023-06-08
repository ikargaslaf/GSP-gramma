#pragma once

#include<iostream>
#include<string>
#include<fstream>
#include<vector>

using namespace std;

class lexical
{
protected:
	enum class symbols { letter, digit, ar_op, compare, space, bracketL, bracketR, SQRbracketL, SQRbracketR, comma, LF, Eof, colon, semicolon, other };
	enum class states { A1, B1, B2, V1, C1, R1, R2, L1, L2, L3, L4, L5, L6, G1, G2 };
	enum class tokens {
		/*Õ≈“≈–Ã»Õ¿À€*/P, OP, TYPE, LET, VLIST, E, Ee, T, Tt, F, READ, VAR, COUT1, COUT2, EX, CASES, CASE, DEFAULT, SWITCH, CLIST, TEST, RELATION, WHILE, FOR1, FOR2, FOR3, IF, ELSE,
		/*“≈–Ã»Õ¿À€*/Var, Const, Semicolon, Colon, Comma, L_bracket, R_bracket, plus, minus, mult, div, mod, l, g, e, le, ge, ne, Cin_thing, Cout_thing, Label, Comment, Declare, Each, Else, End, Exception, Goto, If, In, Let, Otherwise, Read, Sizet, Switch, Then, Vector, When, While, Write, Do, For, $,
		/*ƒÀﬂ –¿¡Œ“€ ¿¬“ŒÃ¿“¿*/ Error, Final, base_value
	};

	vector<string> lexems =
	{
		"P", "OP", "TYPE", "LET", "VLIST", "E", "Ee", "T", "Tt", "F", "READ", "VAR", "COUT1", "COUT2", "EX", "CASES", "CASE", "DEFAULT", "SWITCH", "CLIST", "TEST", "RELATION", "WHILE", "FOR1", "FOR2", "FOR3", "IF", "ELSE",
		"V", "C", ";", ":", ",", "(", ")", "+", "-", "*", "/", "%", "<", ">", "=", "<=", ">=", "<>", ">>", "<<", "Label", "comment", "declare", "each", "else", "end", "exception", "goto", "if", "in", "let", "otherwise", "read", "sizet", "switch", "then", "vector", "when", "while", "write", "do", "for", "$"
	};

	tokens class_reg = tokens::base_value;
	string varname_reg = "";
	string labname_reg = "";
	int value_reg = -1;
	int opercode_reg = -1;;
	int numstr_reg = 1;

	struct lexema
	{
		tokens Lex_class;
		string varname;
		string labname;
		int value;
		int opercode;
		int numstr;
	};

	states state = states::A1;
	char ch = '0';
	symbols symbol = symbols::other;
	int find_reg = -1;


	symbols trans(char ch)
	{
		if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
			return symbols::letter;
		if (ch >= '0' && ch <= '9')
			return symbols::digit;
		if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
			return symbols::ar_op;
		if (ch >= '<' && ch <= '>' || ch == '!' || ch == '=')
			return symbols::compare;
		if (ch == ' ')
			return symbols::space;
		if (ch == '\n')
			return symbols::LF;
		if (ch == ';')
			return symbols::semicolon;
		if (ch == ':')
			return symbols::colon;
		if (ch == ',')
			return symbols::comma;
		if (ch == EOF)
			return symbols::Eof;
		if (ch == '(')
			return symbols::bracketL;
		if (ch == ')')
			return symbols::bracketR;
		if (ch == '[')
			return symbols::SQRbracketL;
		if (ch == ']')
			return symbols::SQRbracketR;
		return symbols::other;
	}
	states(lexical::* table[15][15])();
	int V[26];

	vector<lexema> TOKENS;

public:
	lexical()
	{
		for (int i = 0; i < 26; i++)
			V[i] = 0;

		V['c' - 97] = 1;
		V['d' - 97] = 7;
		V['e' - 97] = 14 ;
		V['f' - 97] = 75;
		V['g' - 97] = 30;
		V['i' - 97] = 33;
		V['l' - 97] = 35;
		V['o' - 97] = 37;
		V['r' - 97] = 45;
		V['s' - 97] = 48;
		V['t' - 97] = 57;
		V['v' - 97] = 60;
		V['w' - 97] = 65;


		table[(int)symbols::letter][(int)states::A1] = &lexical::B1a;
		table[(int)symbols::letter][(int)states::B1] = &lexical::M1;
		table[(int)symbols::letter][(int)states::B2] = &lexical::V1b;
		table[(int)symbols::letter][(int)states::V1] = &lexical::V1b;
		table[(int)symbols::letter][(int)states::C1] = &lexical::error;
		table[(int)symbols::letter][(int)states::R1] = &lexical::error;
		table[(int)symbols::letter][(int)states::R2] = &lexical::L1a;
		table[(int)symbols::letter][(int)states::L1] = &lexical::L1b;
		table[(int)symbols::letter][(int)states::L2] = &lexical::error;
		table[(int)symbols::letter][(int)states::L3] = &lexical::error;
		table[(int)symbols::letter][(int)states::L4] = &lexical::error;
		table[(int)symbols::letter][(int)states::L5] = &lexical::L6a;
		table[(int)symbols::letter][(int)states::L6] = &lexical::L6b;
		table[(int)symbols::letter][(int)states::G1] = &lexical::error;
		table[(int)symbols::letter][(int)states::G2] = &lexical::G2;

		table[(int)symbols::digit][(int)states::A1] = &lexical::C1a;
		table[(int)symbols::digit][(int)states::B1] = &lexical::error;
		table[(int)symbols::digit][(int)states::B2] = &lexical::V1b;
		table[(int)symbols::digit][(int)states::V1] = &lexical::V1b;
		table[(int)symbols::digit][(int)states::C1] = &lexical::C1b;
		table[(int)symbols::digit][(int)states::R1] = &lexical::error;
		table[(int)symbols::digit][(int)states::R2] = &lexical::L1a;
		table[(int)symbols::digit][(int)states::L1] = &lexical::L1b;
		table[(int)symbols::digit][(int)states::L2] = &lexical::error;
		table[(int)symbols::digit][(int)states::L3] = &lexical::error;
		table[(int)symbols::digit][(int)states::L4] = &lexical::error;
		table[(int)symbols::digit][(int)states::L5] = &lexical::L6a;
		table[(int)symbols::digit][(int)states::L6] = &lexical::L6b;
		table[(int)symbols::digit][(int)states::G1] = &lexical::error;
		table[(int)symbols::digit][(int)states::G2] = &lexical::G2;

		table[(int)symbols::ar_op][(int)states::A1] = &lexical::A1op;
		table[(int)symbols::ar_op][(int)states::B1] = &lexical::error;
		table[(int)symbols::ar_op][(int)states::B2] = &lexical::error;
		table[(int)symbols::ar_op][(int)states::V1] = &lexical::error;
		table[(int)symbols::ar_op][(int)states::C1] = &lexical::error;
		table[(int)symbols::ar_op][(int)states::R1] = &lexical::error;
		table[(int)symbols::ar_op][(int)states::R2] = &lexical::error;
		table[(int)symbols::ar_op][(int)states::L1] = &lexical::error;
		table[(int)symbols::ar_op][(int)states::L2] = &lexical::error;
		table[(int)symbols::ar_op][(int)states::L3] = &lexical::error;
		table[(int)symbols::ar_op][(int)states::L4] = &lexical::error;
		table[(int)symbols::ar_op][(int)states::L5] = &lexical::error;
		table[(int)symbols::ar_op][(int)states::L6] = &lexical::error;
		table[(int)symbols::ar_op][(int)states::G1] = &lexical::error;
		table[(int)symbols::ar_op][(int)states::G2] = &lexical::G2;

		table[(int)symbols::compare][(int)states::A1] = &lexical::R1a;
		table[(int)symbols::compare][(int)states::B1] = &lexical::error;
		table[(int)symbols::compare][(int)states::B2] = &lexical::error;
		table[(int)symbols::compare][(int)states::V1] = &lexical::error;
		table[(int)symbols::compare][(int)states::C1] = &lexical::error;
		table[(int)symbols::compare][(int)states::R1] = &lexical::R2b;
		table[(int)symbols::compare][(int)states::R2] = &lexical::error;
		table[(int)symbols::compare][(int)states::L1] = &lexical::L2a;
		table[(int)symbols::compare][(int)states::L2] = &lexical::L3a;
		table[(int)symbols::compare][(int)states::L3] = &lexical::error;
		table[(int)symbols::compare][(int)states::L4] = &lexical::error;
		table[(int)symbols::compare][(int)states::L5] = &lexical::error;
		table[(int)symbols::compare][(int)states::L6] = &lexical::error;
		table[(int)symbols::compare][(int)states::G1] = &lexical::error;
		table[(int)symbols::compare][(int)states::G2] = &lexical::G2;

		table[(int)symbols::space][(int)states::A1] = &lexical::A1;
		table[(int)symbols::space][(int)states::B1] = &lexical::error;
		table[(int)symbols::space][(int)states::B2] = &lexical::A1kw;
		table[(int)symbols::space][(int)states::V1] = &lexical::A1v;
		table[(int)symbols::space][(int)states::C1] = &lexical::A1c;
		table[(int)symbols::space][(int)states::R1] = &lexical::A1r;
		table[(int)symbols::space][(int)states::R2] = &lexical::A1r;
		table[(int)symbols::space][(int)states::L1] = &lexical::error;
		table[(int)symbols::space][(int)states::L2] = &lexical::error;
		table[(int)symbols::space][(int)states::L3] = &lexical::A1Label;
		table[(int)symbols::space][(int)states::L4] = &lexical::L5;
		table[(int)symbols::space][(int)states::L5] = &lexical::error;
		table[(int)symbols::space][(int)states::L6] = &lexical::A1Label;
		table[(int)symbols::space][(int)states::G1] = &lexical::G2;
		table[(int)symbols::space][(int)states::G2] = &lexical::G2;

		table[(int)symbols::bracketL][(int)states::A1] = &lexical::A1l_br;
		table[(int)symbols::bracketL][(int)states::B1] = &lexical::error;
		table[(int)symbols::bracketL][(int)states::B2] = &lexical::error;
		table[(int)symbols::bracketL][(int)states::V1] = &lexical::error;
		table[(int)symbols::bracketL][(int)states::C1] = &lexical::error;
		table[(int)symbols::bracketL][(int)states::R1] = &lexical::error;
		table[(int)symbols::bracketL][(int)states::R2] = &lexical::error;
		table[(int)symbols::bracketL][(int)states::L1] = &lexical::error;
		table[(int)symbols::bracketL][(int)states::L2] = &lexical::error;
		table[(int)symbols::bracketL][(int)states::L3] = &lexical::error;
		table[(int)symbols::bracketL][(int)states::L4] = &lexical::error;
		table[(int)symbols::bracketL][(int)states::L5] = &lexical::error;
		table[(int)symbols::bracketL][(int)states::L6] = &lexical::error;
		table[(int)symbols::bracketL][(int)states::G1] = &lexical::error;
		table[(int)symbols::bracketL][(int)states::G2] = &lexical::G2;

		table[(int)symbols::bracketR][(int)states::A1] = &lexical::A1r_br;
		table[(int)symbols::bracketR][(int)states::B1] = &lexical::error;
		table[(int)symbols::bracketR][(int)states::B2] = &lexical::error;
		table[(int)symbols::bracketR][(int)states::V1] = &lexical::error;
		table[(int)symbols::bracketR][(int)states::C1] = &lexical::error;
		table[(int)symbols::bracketR][(int)states::R1] = &lexical::error;
		table[(int)symbols::bracketR][(int)states::R2] = &lexical::error;
		table[(int)symbols::bracketR][(int)states::L1] = &lexical::error;
		table[(int)symbols::bracketR][(int)states::L2] = &lexical::error;
		table[(int)symbols::bracketR][(int)states::L3] = &lexical::error;
		table[(int)symbols::bracketR][(int)states::L4] = &lexical::error;
		table[(int)symbols::bracketR][(int)states::L5] = &lexical::error;
		table[(int)symbols::bracketR][(int)states::L6] = &lexical::error;
		table[(int)symbols::bracketR][(int)states::G1] = &lexical::error;
		table[(int)symbols::bracketR][(int)states::G2] = &lexical::G2;

		table[(int)symbols::SQRbracketL][(int)states::A1] = &lexical::error;
		table[(int)symbols::SQRbracketL][(int)states::B1] = &lexical::error;
		table[(int)symbols::SQRbracketL][(int)states::B2] = &lexical::error;
		table[(int)symbols::SQRbracketL][(int)states::V1] = &lexical::error;
		table[(int)symbols::SQRbracketL][(int)states::C1] = &lexical::error;
		table[(int)symbols::SQRbracketL][(int)states::R1] = &lexical::error;
		table[(int)symbols::SQRbracketL][(int)states::R2] = &lexical::error;
		table[(int)symbols::SQRbracketL][(int)states::L1] = &lexical::error;
		table[(int)symbols::SQRbracketL][(int)states::L2] = &lexical::error;
		table[(int)symbols::SQRbracketL][(int)states::L3] = &lexical::error;
		table[(int)symbols::SQRbracketL][(int)states::L4] = &lexical::error;
		table[(int)symbols::SQRbracketL][(int)states::L5] = &lexical::error;
		table[(int)symbols::SQRbracketL][(int)states::L6] = &lexical::error;
		table[(int)symbols::SQRbracketL][(int)states::G1] = &lexical::error;
		table[(int)symbols::SQRbracketL][(int)states::G2] = &lexical::G2;

		table[(int)symbols::SQRbracketR][(int)states::A1] = &lexical::error;
		table[(int)symbols::SQRbracketR][(int)states::B1] = &lexical::error;
		table[(int)symbols::SQRbracketR][(int)states::B2] = &lexical::error;
		table[(int)symbols::SQRbracketR][(int)states::V1] = &lexical::error;
		table[(int)symbols::SQRbracketR][(int)states::C1] = &lexical::error;
		table[(int)symbols::SQRbracketR][(int)states::R1] = &lexical::error;
		table[(int)symbols::SQRbracketR][(int)states::R2] = &lexical::error;
		table[(int)symbols::SQRbracketR][(int)states::L1] = &lexical::error;
		table[(int)symbols::SQRbracketR][(int)states::L2] = &lexical::error;
		table[(int)symbols::SQRbracketR][(int)states::L3] = &lexical::error;
		table[(int)symbols::SQRbracketR][(int)states::L4] = &lexical::error;
		table[(int)symbols::SQRbracketR][(int)states::L5] = &lexical::error;
		table[(int)symbols::SQRbracketR][(int)states::L6] = &lexical::error;
		table[(int)symbols::SQRbracketR][(int)states::G1] = &lexical::error;
		table[(int)symbols::SQRbracketR][(int)states::G2] = &lexical::G2;

		table[(int)symbols::comma][(int)states::A1] = &lexical::A1comma;
		table[(int)symbols::comma][(int)states::B1] = &lexical::error;
		table[(int)symbols::comma][(int)states::B2] = &lexical::error;
		table[(int)symbols::comma][(int)states::V1] = &lexical::error;
		table[(int)symbols::comma][(int)states::C1] = &lexical::error;
		table[(int)symbols::comma][(int)states::R1] = &lexical::error;
		table[(int)symbols::comma][(int)states::R2] = &lexical::error;
		table[(int)symbols::comma][(int)states::L1] = &lexical::error;
		table[(int)symbols::comma][(int)states::L2] = &lexical::error;
		table[(int)symbols::comma][(int)states::L3] = &lexical::error;
		table[(int)symbols::comma][(int)states::L4] = &lexical::error;
		table[(int)symbols::comma][(int)states::L5] = &lexical::error;
		table[(int)symbols::comma][(int)states::L6] = &lexical::error;
		table[(int)symbols::comma][(int)states::G1] = &lexical::error;
		table[(int)symbols::comma][(int)states::G2] = &lexical::G2;

		table[(int)symbols::semicolon][(int)states::A1] = &lexical::A1semicolon;
		table[(int)symbols::semicolon][(int)states::B1] = &lexical::error;
		table[(int)symbols::semicolon][(int)states::B2] = &lexical::error;
		table[(int)symbols::semicolon][(int)states::V1] = &lexical::error;
		table[(int)symbols::semicolon][(int)states::C1] = &lexical::error;
		table[(int)symbols::semicolon][(int)states::R1] = &lexical::error;
		table[(int)symbols::semicolon][(int)states::R2] = &lexical::error;
		table[(int)symbols::semicolon][(int)states::L1] = &lexical::error;
		table[(int)symbols::semicolon][(int)states::L2] = &lexical::error;
		table[(int)symbols::semicolon][(int)states::L3] = &lexical::error;
		table[(int)symbols::semicolon][(int)states::L4] = &lexical::error;
		table[(int)symbols::semicolon][(int)states::L5] = &lexical::error;
		table[(int)symbols::semicolon][(int)states::L6] = &lexical::error;
		table[(int)symbols::semicolon][(int)states::G1] = &lexical::error;
		table[(int)symbols::semicolon][(int)states::G2] = &lexical::A1com;

		table[(int)symbols::colon][(int)states::A1] = &lexical::A1colon;
		table[(int)symbols::colon][(int)states::B1] = &lexical::error;
		table[(int)symbols::colon][(int)states::B2] = &lexical::error;
		table[(int)symbols::colon][(int)states::V1] = &lexical::error;
		table[(int)symbols::colon][(int)states::C1] = &lexical::error;
		table[(int)symbols::colon][(int)states::R1] = &lexical::error;
		table[(int)symbols::colon][(int)states::R2] = &lexical::error;
		table[(int)symbols::colon][(int)states::L1] = &lexical::error;
		table[(int)symbols::colon][(int)states::L2] = &lexical::error;
		table[(int)symbols::colon][(int)states::L3] = &lexical::error;
		table[(int)symbols::colon][(int)states::L4] = &lexical::error;
		table[(int)symbols::colon][(int)states::L5] = &lexical::error;
		table[(int)symbols::colon][(int)states::L6] = &lexical::error;
		table[(int)symbols::colon][(int)states::G1] = &lexical::error;
		table[(int)symbols::colon][(int)states::G2] = &lexical::G2;

		table[(int)symbols::LF][(int)states::A1] = &lexical::A1n;
		table[(int)symbols::LF][(int)states::B1] = &lexical::error;
		table[(int)symbols::LF][(int)states::B2] = &lexical::A1kw;
		table[(int)symbols::LF][(int)states::V1] = &lexical::A1vn;
		table[(int)symbols::LF][(int)states::C1] = &lexical::A1cn;
		table[(int)symbols::LF][(int)states::R1] = &lexical::A1rn;
		table[(int)symbols::LF][(int)states::R2] = &lexical::A1rn;
		table[(int)symbols::LF][(int)states::L1] = &lexical::error;
		table[(int)symbols::LF][(int)states::L2] = &lexical::error;
		table[(int)symbols::LF][(int)states::L3] = &lexical::A1Labeln;
		table[(int)symbols::LF][(int)states::L4] = &lexical::error;
		table[(int)symbols::LF][(int)states::L5] = &lexical::error;
		table[(int)symbols::LF][(int)states::L6] = &lexical::A1Labeln;
		table[(int)symbols::LF][(int)states::G1] = &lexical::error;
		table[(int)symbols::LF][(int)states::G2] = &lexical::error;

		table[(int)symbols::Eof][(int)states::A1] = &lexical::EXIT1;
		table[(int)symbols::Eof][(int)states::B1] = &lexical::error;
		table[(int)symbols::Eof][(int)states::B2] = &lexical::EXIT2;
		table[(int)symbols::Eof][(int)states::V1] = &lexical::EXIT3;
		table[(int)symbols::Eof][(int)states::C1] = &lexical::EXIT4;
		table[(int)symbols::Eof][(int)states::R1] = &lexical::EXIT5;
		table[(int)symbols::Eof][(int)states::R2] = &lexical::EXIT5;
		table[(int)symbols::Eof][(int)states::L1] = &lexical::error;
		table[(int)symbols::Eof][(int)states::L2] = &lexical::error;
		table[(int)symbols::Eof][(int)states::L3] = &lexical::EXIT6;
		table[(int)symbols::Eof][(int)states::L4] = &lexical::error;
		table[(int)symbols::Eof][(int)states::L5] = &lexical::error;
		table[(int)symbols::Eof][(int)states::L6] = &lexical::EXIT6;
		table[(int)symbols::Eof][(int)states::G1] = &lexical::error;
		table[(int)symbols::Eof][(int)states::G2] = &lexical::error;

		table[(int)symbols::other][(int)states::A1] = &lexical::error;
		table[(int)symbols::other][(int)states::B1] = &lexical::error;
		table[(int)symbols::other][(int)states::B2] = &lexical::error;
		table[(int)symbols::other][(int)states::V1] = &lexical::error;
		table[(int)symbols::other][(int)states::C1] = &lexical::error;
		table[(int)symbols::other][(int)states::R1] = &lexical::error;
		table[(int)symbols::other][(int)states::R2] = &lexical::error;
		table[(int)symbols::other][(int)states::L1] = &lexical::error;
		table[(int)symbols::other][(int)states::L2] = &lexical::error;
		table[(int)symbols::other][(int)states::L3] = &lexical::error;
		table[(int)symbols::other][(int)states::L4] = &lexical::error;
		table[(int)symbols::other][(int)states::L5] = &lexical::error;
		table[(int)symbols::other][(int)states::L6] = &lexical::error;
		table[(int)symbols::other][(int)states::G1] = &lexical::error;
		table[(int)symbols::other][(int)states::G2] = &lexical::G2;


	};
	~lexical() {};
	void start();

	lexema create_lexema();
	void vipe_registres();

	states B1a();
	states B1b();
	states A1kw();
	states A1kwn();
	states C1a();
	states A1op();
	states A1com();
	states R1a();
	states A1l_br();
	states A1r_br();
	states A1comma();
	states A1semicolon();
	states A1colon();
	states A1n();
	states A1();
	states G2();
	states M1();
	states V1b();
	states A1v();
	states A1vn();
	states C1b();
	states A1c();
	states A1cn();
	states R2b();
	states A1r();
	states A1rn();
	states L1a();
	states L1b();
	states L2a();
	states L3a();
	states L5();
	states L6a();
	states L6b();
	states A1Label();
	states A1Labeln();
	states error();

	// Î˛˜Â‚˚Â ÒÎÓ‚‡ (comment, declare, each, else, end, exception, goto, if, in, let, otherwise, read, sizet, switch, then, vector, when, while, write, do,)

	states EXIT1();
	states EXIT2();
	states EXIT3();
	states EXIT4();
	states EXIT5();
	states EXIT6();
	void SaveLexems();
};
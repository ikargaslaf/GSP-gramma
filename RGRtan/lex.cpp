#include"lex.h"
#include<stack>
#include<iomanip>

void lexical::start()
{

	fstream input_file("prog.txt");
	if (!input_file)
	{
		cout << "file prog.txt didn't open";
		return;
	}

	state = states::A1;
	while (!input_file.eof())
	{
		ch = input_file.get();
		symbol = trans(ch);
		state = (this->*table[(int)symbol][(int)state])();
	}
	SaveLexems();
	return;
}

lexical::lexema lexical::create_lexema()
{
	lexema L;
	L.Lex_class = class_reg;
	L.varname = varname_reg;
	L.labname = labname_reg;
	L.value = value_reg;
	L.opercode = opercode_reg;
	L.numstr = numstr_reg;
	return L;
}

void lexical::vipe_registres()
{
	lexical::class_reg = tokens::base_value;
	lexical::varname_reg = "";
	lexical::labname_reg = "";
	lexical::value_reg = -1;
	lexical::opercode_reg = -1;
}

lexical::states lexical::A1()
{
	return lexical::states::A1;
}

lexical::states lexical::G2()
{
	return lexical::states::G2;
}

lexical::states lexical::L5()
{
	lexical::TOKENS.push_back(create_lexema());
	vipe_registres();
	return lexical::states::L5;
}

lexical::states lexical::A1n()
{
	lexical::numstr_reg++;
	return lexical::states::A1;
}
//выдать запятую ,
lexical::states lexical::A1comma()
{
	lexical::class_reg = lexical::tokens::Comma;
	lexical::TOKENS.push_back(create_lexema());
	vipe_registres();
	return lexical::states::A1;
}
//выдать двоеточие :
lexical::states lexical::A1colon()
{
	lexical::class_reg = lexical::tokens::Colon;
	lexical::TOKENS.push_back(create_lexema());
	vipe_registres();
	return lexical::states::A1;
}
//выдать точку с запятой ;
lexical::states lexical::A1semicolon()
{
	lexical::class_reg = lexical::tokens::Semicolon;
	lexical::TOKENS.push_back(create_lexema());
	vipe_registres();
	return lexical::states::A1;
}
//выдать левую скобку (
lexical::states lexical::A1l_br()
{
	lexical::class_reg = lexical::tokens::L_bracket;
	lexical::TOKENS.push_back(create_lexema());
	vipe_registres();
	return lexical::states::A1;
}
//выдать правую скобку )
lexical::states lexical::A1r_br()
{
	lexical::class_reg = lexical::tokens::R_bracket;
	lexical::TOKENS.push_back(create_lexema());
	vipe_registres();
	return lexical::states::A1;
}
//Начало формирования константы. Кладем значение входного символа в регистр value_reg
lexical::states lexical::C1a()
{
	lexical::value_reg = ch - '0';
	return lexical::states::C1;
}
//Продолжаем формирование константы. 
lexical::states lexical::C1b()
{
	lexical::value_reg = lexical::value_reg * 10 + (ch - '0');
	return lexical::states::C1;
}
lexical::states lexical::V1b()
{
	lexical::varname_reg += ch;
	return lexical::states::V1;
}
//Выдаем константу
lexical::states lexical::A1c()
{
	lexical::class_reg = lexical::tokens::Const;
	lexical::TOKENS.push_back(create_lexema());
	vipe_registres();
	return lexical::states::A1;
}
//ВЫдаем переменную
lexical::states lexical::A1v()
{
	lexical::class_reg = lexical::tokens::Var;
	lexical::TOKENS.push_back(create_lexema());
	vipe_registres();
	return lexical::states::A1;
}
//выдаем арифметичекую операцию
lexical::states lexical::A1op()
{
	switch (ch)
	{
	case '+':
		class_reg = tokens::plus;
		break;
	case '-':
		class_reg = tokens::minus;
		break;
	case '*':
		class_reg = tokens::mult;
		break;
	case '/':
		class_reg = tokens::div;
		break;
	case '%':
		class_reg = tokens::mod;
		break;
	}

	lexical::TOKENS.push_back(create_lexema());
	vipe_registres();
	return lexical::states::A1;
}
//определяем операцию отношения
lexical::states lexical::R1a()
{
	switch (ch)
	{
	case '<':
		class_reg = tokens::l;
		break;
	case '>':
		class_reg = tokens::g;
		break;
	case '=':
		class_reg = tokens::e;
		A1r();
		return lexical::states::A1;
		break;
	}

	return lexical::states::R1;
}
//Сформирована опрерация отношения (или чтучка ввода/вывода). Надо дождаться пробела
lexical::states lexical::R2b()
{
	if (class_reg == tokens::l && ch == '<')// прочитан <<
		class_reg = tokens::Cout_thing;
	if (class_reg == tokens::g && ch == '>')// прочитан >>
		class_reg = tokens::Cin_thing;
	if (class_reg == tokens::l && ch == '=')// прочитан >=
		class_reg = tokens::le;
	if (class_reg == tokens::g && ch == '=')// прочитан <=
		class_reg = tokens::ge;
	if (class_reg == tokens::l && ch == '>')// прочитан <>
		class_reg = tokens::ne;

	return lexical::states::R2;
}
//Выдаем операцию отношенния (или штучку ввода/вывода)
lexical::states lexical::A1r()
{
	lexical::TOKENS.push_back(create_lexema());
	vipe_registres();
	return lexical::states::A1;
}
//после того, как прочитали << всретили букву или цифру. Значит читаем метку. Формуруем имя метки
lexical::states lexical::L1a()
{
	lexical::labname_reg = ch;
	return lexical::states::L1;
}
//Продолжаем формировать имя метки
lexical::states lexical::L1b()
{
	lexical::labname_reg += ch;
	return lexical::states::L1;
}

lexical::states lexical::L6a()
{
	lexical::labname_reg = ch;
	return lexical::states::L6;
}
//Продолжаем формировать имя метки
lexical::states lexical::L6b()
{
	lexical::labname_reg += ch;
	return lexical::states::L6;
}
//После имени метки сразу идет >>
lexical::states lexical::L2a()
{
	if (ch == '>')
		return lexical::states::L2;
	else
		error();
}
//Прочитали >>. Ожидаем пробел
lexical::states lexical::L3a()
{
	if (ch == '>')
		return lexical::states::L3;
	else
		error();
}
//Выдаем метку
lexical::states lexical::A1Label()
{
	lexical::class_reg = tokens::Label;
	lexical::TOKENS.push_back(create_lexema());
	vipe_registres();
	return lexical::states::A1;
}
//Начинаем читать ключевое слово (или переменную. как дльше пойдет)
lexical::states lexical::B1a()
{
	find_reg = V[ch - 'a'];
	varname_reg = ch;
	if (!find_reg)
		return states::V1;
	return states::B1;
}

lexical::states lexical::B1b()
{
	find_reg++;
	varname_reg += ch;
	return states::B1;
}
//Читаем ключевые слова
lexical::states lexical::M1()
{
	switch (find_reg)
	{
	case 1: 
		if (ch == 'o') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 2:
		if (ch == 'm') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 3:
		if (ch == 'm') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 4:
		if (ch == 'e') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 5:
		if (ch == 'n') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 6:
		if (ch == 't') { class_reg = tokens::Comment; varname_reg += ch; return states::G1; }//прочитано comment
		else
			find_reg = 0;
		break;
	case 7:
		if (ch == 'e') { B1b(); return states::B1; }
		else
			if (ch == 'o') { class_reg = tokens::Do; varname_reg += ch; return states::B2; }
			else 
				find_reg = 0;
		break;
	case 8:
		if (ch == 'c') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 9:
		if (ch == 'l') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 10:
		if (ch == 'a') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 11:
		if (ch == 'r') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 12:
		if (ch == 'e') { class_reg = tokens::Declare; varname_reg += ch; return states::B2; }
		else
			find_reg = 0;
		break;
	case 13:
		if (ch == 'o') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 14:
		if (ch == 'a') { B1b(); return states::B1; }
		else
			if (ch == 'l') { find_reg = 18; varname_reg += ch; }
			else 
				if (ch == 'n') { find_reg = 21; varname_reg += ch; }
				else
					if (ch == 'x') { find_reg = 23; varname_reg += ch; }
					else
						find_reg = 0;
		break;
	case 15:
		if (ch == 'c') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 16:
		if (ch == 'h') { class_reg = tokens::Each; varname_reg += ch; return states::B2; }
		else
			find_reg = 0;
		break;
	case 17:
		if (ch == 'l') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 18:
		if (ch == 's') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 19:
		if (ch == 'e') { class_reg = tokens::Else; varname_reg += ch; return states::B2; }
		else
			find_reg = 0;
		break;
	case 20:
		if (ch == 'n') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 21:
		if (ch == 'd') { class_reg = tokens::End; varname_reg += ch; return states::B2; }
		else
			find_reg = 0;
		break;
	case 22:
		if (ch == 'x') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 23:
		if (ch == 'c') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 24:
		if (ch == 'e') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 25:
		if (ch == 'p') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 26:
		if (ch == 't') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 27:
		if (ch == 'i') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 28:
		if (ch == 'o') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 29:
		if (ch == 'n') { class_reg = tokens::Exception; varname_reg += ch; return states::B2; }
		else
			find_reg = 0;
		break;
	case 30:
		if (ch == 'o') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 31:
		if (ch == 't') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 32:
		if (ch == 'o') { class_reg = tokens::Goto; varname_reg += ch; return states::L4; }
		else
			find_reg = 0;
		break;
	case 33:
		if (ch == 'f') { class_reg = tokens::If; varname_reg += ch; return states::B2; }
		else
			if (ch == 'n') { class_reg = tokens::In; varname_reg += ch; return states::B2; }
			else
				find_reg = 0;
		break;
	case 34:
		if (ch == 'n') { class_reg = tokens::In; varname_reg += ch; return states::B2; }
		else
			find_reg = 0;
		break;
	case 35:
		if (ch == 'e') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 36:
		if (ch == 't') { class_reg = tokens::Let; varname_reg += ch; return states::B2; }
		else
			find_reg = 0;
		break;
	case 37:
		if (ch == 't') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 38:
		if (ch == 'h') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 39:
		if (ch == 'e') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 40:
		if (ch == 'r') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 41:
		if (ch == 'w') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 42:
		if (ch == 'i') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 43:
		if (ch == 's') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 44:
		if (ch == 'e') { class_reg = tokens::Otherwise; varname_reg += ch; return states::B2; }
		else
			find_reg = 0;
		break;
	case 45:
		if (ch == 'e') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 46:
		if (ch == 'a') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 47:
		if (ch == 'd') { class_reg = tokens::Read; varname_reg += ch; return states::B2; }
		else
			find_reg = 0;
		break;
	case 48:
		if (ch == 'i') { B1b(); return states::B1; }
		else
			if (ch == 'w') { find_reg = 53; varname_reg += ch; }
			else
				find_reg = 0;
		break;
	case 49:
		if (ch == 'z') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 50:
		if (ch == 'e') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 51:
		if (ch == 't') { class_reg = tokens::Sizet; varname_reg += ch; return states::B2; }
		else
			find_reg = 0;
		break;
	case 52:
		if (ch == 'w') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 53:
		if (ch == 'i') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 54:
		if (ch == 't') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 55:
		if (ch == 'c') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 56:
		if (ch == 'h') { class_reg = tokens::Switch; varname_reg += ch; return states::B2; }
		else
			find_reg = 0;
		break;
	case 57:
		if (ch == 'h') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 58:
		if (ch == 'e') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 59:
		if (ch == 'n') { class_reg = tokens::Then; varname_reg += ch; return states::B2; }
		else
			find_reg = 0;
		break;
	case 60:
		if (ch == 'e') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 61:
		if (ch == 'c') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 62:
		if (ch == 't') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 63:
		if (ch == 'o') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 64:
		if (ch == 'r') { class_reg = tokens::Vector; varname_reg += ch; return states::B2; }
		else
			find_reg = 0;
		break;
	case 65:
		if (ch == 'h') { B1b(); return states::B1; }
		else
			if (ch == 'r') { find_reg = 69; varname_reg += ch; }
			else
				find_reg = 0;
		break;
	case 66:
		if (ch == 'e') { B1b(); return states::B1; }
		else
			if (ch == 'i') { find_reg = 73; varname_reg += ch; }
			else
				find_reg = 0;
		break;
	case 67:
		if (ch == 'n') { class_reg = tokens::When; varname_reg += ch; return states::B2; }
		else
			find_reg = 0;
		break;
	case 68:
		if (ch == 'r') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 69:
		if (ch == 'i') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 70:
		if (ch == 't') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 71:
		if (ch == 'e') { class_reg = tokens::Write; varname_reg += ch; return states::B2; }
		else
			find_reg = 0;
		break;
	case 72:
		if (ch == 'i') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 73:
		if (ch == 'l') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 74:
		if (ch == 'e') { class_reg = tokens::While; varname_reg += ch; return states::B2; }
		else
			find_reg = 0;
		break;
	case 75:
		if (ch == 'o') { B1b(); return states::B1; }
		else
			find_reg = 0;
		break;
	case 76:
		if (ch == 'r') { class_reg = tokens::For; varname_reg += ch; return states::B2;; }
		else
			find_reg = 0;
		break;
	}

	if (!find_reg)
	{
		class_reg = tokens::Var;
		varname_reg += ch;
		return states::V1;
	}

	return states::B1;
}

//Выдаем ключевое слово
lexical::states lexical::A1kw()
{
	TOKENS.push_back(create_lexema());
	vipe_registres();
	return states::A1;
}
//Выдаем ключевое слово
lexical::states lexical::A1kwn()
{
	TOKENS.push_back(create_lexema());
	vipe_registres(); A1n();
	return states::A1;
}
lexical::states lexical::A1vn()
{
	lexical::class_reg = lexical::tokens::Var;
	lexical::TOKENS.push_back(create_lexema());
	vipe_registres(); A1n();
	return lexical::states::A1;
}
lexical::states lexical::A1cn()
{
	lexical::class_reg = lexical::tokens::Const;
	lexical::TOKENS.push_back(create_lexema());
	vipe_registres(); A1n();
	return lexical::states::A1;
}
lexical::states lexical::A1rn()
{
	lexical::TOKENS.push_back(create_lexema());
	vipe_registres(); A1n();
	return lexical::states::A1;
}
lexical::states lexical::A1Labeln()
{
	lexical::class_reg = tokens::Label;
	lexical::TOKENS.push_back(create_lexema());
	vipe_registres(); A1n();
	return lexical::states::A1;
}
//выдаем коментарий
lexical::states lexical::A1com()
{
	class_reg = tokens::Comment;
	TOKENS.push_back(create_lexema());
	vipe_registres();
	return states::A1;
}
//выдаем ошибку
lexical::states lexical::error()
{
	class_reg = tokens::Error;
	TOKENS.push_back(create_lexema());
	vipe_registres();
	return states::A1;
}



lexical::states lexical::EXIT1()
{
	class_reg = tokens::Final;
	TOKENS.push_back(create_lexema());
	vipe_registres();
	return states::A1;
}

lexical::states lexical::EXIT2()
{
	TOKENS.push_back(create_lexema());
	vipe_registres();
	class_reg = tokens::Final;
	TOKENS.push_back(create_lexema());
	vipe_registres();
	return states::A1;
}

lexical::states lexical::EXIT3()
{
	class_reg = tokens::Var;
	TOKENS.push_back(create_lexema());
	vipe_registres();
	class_reg = tokens::Final;
	TOKENS.push_back(create_lexema());
	vipe_registres();
	return states::A1;
}

lexical::states lexical::EXIT4()
{
	class_reg = tokens::Const;
	TOKENS.push_back(create_lexema());
	vipe_registres();
	class_reg = tokens::Final;
	TOKENS.push_back(create_lexema());
	vipe_registres();
	return states::A1;
}

lexical::states lexical::EXIT5()
{
	TOKENS.push_back(create_lexema());
	vipe_registres();
	class_reg = tokens::Final;
	TOKENS.push_back(create_lexema());
	vipe_registres();
	return states::A1;
}

lexical::states lexical::EXIT6()
{
	class_reg = tokens::Label;
	TOKENS.push_back(create_lexema());
	vipe_registres();
	class_reg = tokens::Final;
	TOKENS.push_back(create_lexema());
	vipe_registres();
	return states::A1;
}

void lexical::SaveLexems()
{
	ofstream out("lexems.txt");
	for (int i = 0; i < TOKENS.size() - 1; i++)
	{
		switch (TOKENS[i].Lex_class)
		{
		case tokens::Var:
			out << TOKENS[i].numstr << ' ' << lexems[(int)TOKENS[i].Lex_class] << " Переменная " << TOKENS[i].varname << endl;
			break;
		case tokens::Const:
			out << TOKENS[i].numstr << ' ' << lexems[(int)TOKENS[i].Lex_class] << " константа " << TOKENS[i].value << endl;
			break;
		case tokens::Label:
			out << TOKENS[i].numstr << ' ' << lexems[(int)TOKENS[i].Lex_class] << " метка " << TOKENS[i].labname << endl;
			break;
		default:
			out << TOKENS[i].numstr << ' ' << lexems[(int)TOKENS[i].Lex_class] << endl;
			break;
		}
	}
}
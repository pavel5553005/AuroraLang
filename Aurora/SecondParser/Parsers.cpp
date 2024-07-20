#include "SecondParser.hpp"
#include "Nope.hpp"



 /**
 * Парсит входные узлы и создает вектор указателей на узлы, представляющий разобранный код.
 *
 * @param nodes Вектор ссылок на узлы FirstParser::Node для разбора.
 *
 * @return Вектор указателей на узлы, представляющий разобранный код.
 *
 * @throws ParserException в случае ошибки во время разбора.
 */

std::vector<Node*> SecondParser::parse(std::vector<FirstParser::Node>& nodes)
{
    std::vector<VariableDec*> orignalVariables = variables;
    std::vector<Node*> result;
    for (auto it = nodes.begin(); it != nodes.end(); it++)
    {
        if (isFunctionDec(it->line)) // Если узел - объявление функции
        {
            continue;
        }
        else if (isVariableDec(it->line)) // Если узел - объявление переменной
        {
            // Проверяем, что узел не имеет дочерних узлов
            if (it->children.size() != 0) throw ParserExeption("Tabulation error", it->line.line);
            auto v = parseVariableDec(it->line);
            result.push_back(v);
            continue;
        }
        else if (isAssignment(it->line)) // Если узел - присваивание
        {
            if (it->children.size() != 0) throw ParserExeption("Tabulation error", it->line.line);
            auto a = parseAssignment(it->line);
            result.push_back(a);
            continue;
        }
        else if (isForLoop(it->line)) // Если узел - цикл For
        {
            std::vector<VariableDec*> orignalVariables2 = variables;

            if (it->children.size() == 0) throw ParserExeption("Tabulation error", it->line.line);
            For* f = new For();
            Lexer::LineWithTokens line;
            auto itt = it->line.t.begin() + 1;
            for (; itt->type != Token::Type::Semicolon; itt++)
            {
                line.t.push_back(*itt);
            }
            Node* n = new Nope();
            if (line.t.size() != 0)
            {
                delete n;
                n = parseOperation(line);
            }
                n->line = it->line.line;
                f->addChild(n);
                itt++;
                line.t.clear();

            for (; itt->type != Token::Type::Semicolon; itt++)
            {
                line.t.push_back(*itt);
            }
            Expression* e;
            if (line.t.size() != 0)
            {
                e = parseExpression(line, 0);
            }
            else
            {
                e = new LiteralConst("true");
                e->type = Expression::Type::Bool;
            }
            e->line = it->line.line;
            if (e->type != Expression::Type::Bool) throw ParserExeption("For loop condition must be bool", it->line.line);
            f->addChild(e);
            itt++;
            line.t.clear();

            for (; itt != it->line.t.end(); itt++)
            {
                line.t.push_back(*itt);
            }
            Node* n2 = new Nope();
            if (line.t.size() != 0)
            {
                delete n2;
                n2 = parseOperation(line);
            }
            f->addChild(n2);
            result.push_back(f);

            f->addChildren(parse(it->children));

            variables = orignalVariables2;

            continue;
        }
        else if (isWhileLoop(it->line)) // Если узел - цикл While
        {
            if (it->children.size() == 0) throw ParserExeption("Tabulation error", it->line.line);
            While* w = new While();
            w->line = it->line.line;
            Expression* e = parseExpression({std::vector<Token>(it->line.t.begin() + 1, it->line.t.end()), it->line.line, it->line.spaceCount},  0);
            e->line = it->line.line;
            if (e->type != Expression::Type::Bool) throw ParserExeption("While loop condition must be bool", it->line.line);
            w->addChild(e);
            w->addChildren(parse(it->children));
            result.push_back(w);
            continue;
        }
        else if (it->line.t[0].value == "do") // Если узел - цикл Do While
        {
            if (it->children.size() == 0) throw ParserExeption("Tabulation error", it->line.line);
            if ((it + 1)->children.size() != 0) throw ParserExeption("Tabulation error", (it + 1)->line.line);
            if (!isWhileLoop((it + 1)->line)) throw ParserExeption("While loop expected", (it + 1)->line.line);
            DoWhile* d = new DoWhile();
            d->line = it->line.line;
            it++;
            Expression* e = parseExpression({std::vector<Token>(it->line.t.begin() + 1, it->line.t.end()), it->line.line, it->line.spaceCount},  0);
            e->line = it->line.line;
            d->addChild(e);
            d->addChildren(parse((it - 1)->children));
            result.push_back(d);
            continue;
        }
        else if (isIf(it->line)) // Если узел - условный оператор
        {
            if (it->children.size() == 0) throw ParserExeption("Tabulation error", it->line.line);

            If* i = new If();
            i->line = it->line.line;
            Expression* e = parseExpression({std::vector<Token>(it->line.t.begin() + 1, it->line.t.end()), it->line.line, it->line.spaceCount},  0);
            e->line = it->line.line;
            i->addChild(e);
            Node* n = new Node();
            n->line = it->line.line;
            n->addChildren(parse(it->children));
            it++;
            result.push_back(i);
            while (it != nodes.end() and it->line.t.size() > 1 and it->line.t[0].value == "else" and it->line.t[1].value == "if")
            {
                if (it->children.size() == 0) throw ParserExeption("Tabulation error", it->line.line);
                ElseIf* ei = new ElseIf();
                ei->line = it->line.line;
                Expression* e = parseExpression({std::vector<Token>(it->line.t.begin() + 2, it->line.t.end()), it->line.line, it->line.spaceCount},  0);
                e->line = it->line.line;
                ei->addChild(e);
                ei->addChildren(parse(it->children));
                i->addChild(ei);
                it++;
            }
            if (it  == nodes.end())
            {
                it--;
                continue;
            }
            if (it->line.t.size() == 1 and it->line.t[0].value == "else")
            {
                if (it->children.size() == 0) throw ParserExeption("Tabulation error", it->line.line);
                Else* ei = new Else();
                ei->line = it->line.line;
                ei->addChildren(parse(it->children));
                i->addChild(ei);
                continue;
            }
            else
            {
                it--;
                continue;
            }
        }
        else if (isFunctionCall(it->line)) // Если узел - вызов функции
        {
            FunctionCall* f = parseFunctionCall(it->line);
            result.push_back(f);
            continue;
        }
        else if (isReturn(it->line)) // Если узел - return
        {
            if (!isParsingFunction) throw ParserExeption("Return outside function", it->line.line);
            Return* r = new Return();
            r->type = functions.back()->type;
            r->line = it->line.line;
            if (functions.back()->type == Expression::Type::Void)
            {
                if (it->line.t.size() != 1) throw ParserExeption("Return value in void function", it->line.line);
                result.push_back(r);
                continue;
            }
            if (it->children.size() == 1) throw ParserExeption("Expected expression", it->line.line);
            Expression* e = parseExpression({std::vector<Token>(it->line.t.begin() + 1, it->line.t.end()), it->line.line, it->line.spaceCount},  0);
            e->line = it->line.line;
            if (e->type > functions.back()->type) throw ParserExeption("Wrong return type", it->line.line);
            if (e->type < functions.back()->type)
            {
                Convertion* c = new Convertion(functions.back()->type, e->type);
                c->line = it->line.line;
                c->addChild(e);
                r->addChild(c);
                result.push_back(r);
                continue;
            }
            r->type = e->type;
            r->addChild(e);
            result.push_back(r);
            continue;
        }
        else throw ParserExeption("Invalid operation", it->line.line);
    }
    variables = orignalVariables;

    return result;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/**
 * Разбирает выражение из заданной строки токенов на указанном уровне приоритета.
 *
 * @param line Строка токенов, из которой разбирается выражение.
 * @param level Уровень приоритета, на котором разбирается выражение.
 *
 * @return Указатель на разобранное выражение.
 *
 * @throws ParserExeption Если выражение не может быть разобран или если идентификатор не может быть найден.
 */

Expression* SecondParser::parseExpression(Lexer::LineWithTokens line, int level)
{
    if (line.t.size() == 0) throw ParserExeption("Expected expression", line.line);
    if (line.t.front().type == Token::Type::LeftParen and line.t.back().type == Token::Type::RightParen)
    {
        line.t.pop_back();
        line.t.erase(line.t.begin());
        level = 0;
    }
    int paren = 0;
    for (;level < OperatorList.size(); level++)
    {
        for (auto it = line.t.begin(); it != line.t.end(); it++)
        {
            if (it->type == Token::Type::LeftParen)
            {
                paren++;
                continue;
            }
            if (it->type == Token::Type::RightParen)
            {
                paren--;
                continue;
            }

            if (paren != 0) continue;

            if (it->type == Token::Type::Operator and isOperatorInList(it->value, level))
            {
                Operator* o = new Operator(it->value);
                o->line = line.line;
                std::vector<Token>t(line.t.begin(), it);
                auto l = parseExpression({t, line.line, line.spaceCount}, level + 1);
                l->line = line.line;
                t = std::vector<Token>(it + 1, line.t.end());
                auto r = parseExpression({t, line.line, line.spaceCount}, level + 1);
                r->line = line.line;
                if (l->type > r->type)
                {
                    o->addChild(l);
                    auto c = new Convertion(l->type, r->type);
                    o->type = c->type;
                    c->addChild(r);
                    o->addChild(c);
                }
                else if (l->type < r->type)
                {
                    o->addChild(r);
                    auto c = new Convertion(r->type, l->type);
                    o->type = c->type;
                    c->addChild(l);
                    o->addChild(c);
                }
                else
                {
                    o->addChild(l);
                    o->addChild(r);
                    o->type = l->type;
                }
                if (level == 0) o->type = Expression::Type::Bool;
                return o;
            }
        }
    }
    if (line.t[0].type == Token::Type::Type and Expression::getTypeFromToken(line.t[0]) != Expression::Type::Void)
    {
        // Если тип объявлен, то добавляем конвертацию
        auto e = parseExpression({std::vector<Token>(line.t.begin() + 1, line.t.end()), line.line, line.spaceCount}, 0);
        Convertion* c = new Convertion(Expression::getTypeFromToken(line.t[0]), e->type, true); // Конвертируем тип выражения в объявленный тип
        c->line = line.line;
        c->addChild(e);
        return c;
    }
    if (line.t[0].type == Token::Type::Identifier and line.t.size() == 1)
    {
        // Если идентификатор, то добавляем переменную
        Variable* v = new Variable();
        v->name = line.t[0].value;
        try
        {
            v->type = findIdentifier(line.t[0].value)->type;
        }
        catch(const std::exception& e)
        {
            throw ParserExeption(e.what(), line.line);
        }
        v->line = line.line;
        return v;
    }
    if (isFunctionCall(line)) return parseFunctionCall(line);
    // Если литеральное значение, то добавляем его
    LiteralConst* l = new LiteralConst(line.t[0].value);
    l->type = Expression::getTypeFromString(line.t[0].value);
    l->line = line.line;
    return l;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/**
 * Парсит объявление переменной.
 * @param node - узел дерева синтаксического анализа
 * @return - объявление переменной
 * @throws ParserExeption - если переменная уже объявлена или неправильный тип
 */

VariableDec* SecondParser::parseVariableDec(Lexer::LineWithTokens& line)
{
    bool isDeclaration = false;
    try
    {
        // Проверяем, что переменная не объявлена
        if (findIdentifier(line.t[1].value) != nullptr) isDeclaration = true;
    }
    catch (const std::runtime_error& e) { }
    if (isDeclaration) throw ParserExeption("Variable " + getBlue(line.t[1].value) + " is already declared", line.line);

    // Создаем объявление переменной
    VariableDec* v = new VariableDec(Expression::getTypeFromToken(line.t[0]), line.t[1].value);
    v->line = line.line;
    if (line.t[2].type == Token::Type::Operator and line.t[2].value == "=") // Если есть присваивание
    {
        v->isInitialized = true;
        Expression* e = parseExpression({std::vector<Token>(line.t.begin() + 3, line.t.end()), line.line, line.spaceCount}, 0);
        e->line = line.line;
        if (e->type != v->type)
        {
            Convertion* c = new Convertion(v->type, e->type); // Если типы не совпадают, добавляем конвертацию
            c->line = line.line;
            c->addChild(e);
            v->addChild(c);
        }
        else
        {
            v->addChild(e);
        }
    }
    variables.push_back(v);
    return v;
}

Assignment* SecondParser::parseAssignment(Lexer::LineWithTokens& line)
{
    Assignment* a = new Assignment();
    a->line = line.line;
    try
    {
        a->variable = findIdentifier(line.t[0].value);
    }
    catch(const std::runtime_error& e)
    {
        throw ParserExeption(e.what(), line.line);
    }
    Expression* e = parseExpression({std::vector<Token>(line.t.begin() + 2, line.t.end()), line.line, line.spaceCount}, 0);
    if (e->type != a->variable->type)
    {
        Convertion* c = new Convertion(a->variable->type, e->type);
        c->line = line.line;
        e->addChild(c);
    }
    e->line = line.line;
    a->addChild(e);
    return a;
}

Node* SecondParser::parseOperation(Lexer::LineWithTokens& line)
{
    if (isVariableDec(line)) return parseVariableDec(line);
    if (isAssignment(line)) return parseAssignment(line);
    if (isFunctionCall(line)) return parseFunctionCall(line);
    return nullptr;
}

FunctionCall* SecondParser::parseFunctionCall(Lexer::LineWithTokens& line)
{
    FunctionCall* f = new FunctionCall();
    int countOfParent = 0;
    std::vector<Expression*> args;
    for (auto it = line.t.begin() + 2; it != line.t.end();it++)
    {
        if (it->type == Token::Type::LeftParen) countOfParent++;
        if (it->type == Token::Type::RightParen) countOfParent--;

        if (countOfParent != 0) continue;

        Lexer::LineWithTokens t;
        t.line = line.line;
        t.spaceCount = line.spaceCount;
        while (it != line.t.end() and (it->type != Token::Type::Comma and it->type != Token::Type::RightParen)) t.t.push_back(*it++);
        f->children.push_back(parseExpression(t, 0));
        args.push_back(dynamic_cast<Expression*>(f->children.back()));
    }
    FunctionDec* function = findFunction(line.t[0].value, args);
    for (int i = 0; i < f->children.size(); i++)
    {
        if (function->args[i]->type != args[i]->type) 
        {
            Convertion* c = new Convertion(function->args[i]->type, args[i]->type);
            c->line = line.line;
            c->addChild(args[i]);
            f->children[i] = c;
        }
    }
    
    f->type = function->type;
    f->name = function->name;
    f->line = line.line;
    return f;
}
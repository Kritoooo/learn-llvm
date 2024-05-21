#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include "llvm/Support/raw_ostream.h"

class Parser {
private:
    Lexer &Lex;
    Token Tok;
    bool HasError;
    void error();
    void advance();
    bool expect(Token::TokenKind Kind);
    bool consume(Token::TokenKind Kind);
    AST *parseCalc();
    Expr *parseExpr();
    Expr *parseTerm();
    Expr *parseFactor();
public:
    Parser(Lexer &Lex) : Lex(Lex), HasError(false) { advance(); }
    bool hasError() { return HasError; }
    AST *parse();
};

#endif
#include "parser.h"

void Parser::error() {
    llvm::errs() << "Unexpected: " << Tok.getText() << "\n";
    HasError = true;
}

void Parser::advance() { Lex.next(Tok); }

bool Parser::expect(Token::TokenKind Kind) {
    if (Tok.getKind() != Kind) {
        error();
        return true;
    }
    return false;
}

bool Parser::consume(Token::TokenKind Kind) {
    if (expect(Kind)) return true;
    advance();
    return false;
}

AST *Parser::parse() {
    AST *Result = parseCalc();
    expect(Token::eoi);
    return Result;
}

AST *Parser::parseCalc() {
    Expr *E;
    llvm::SmallVector<llvm::StringRef, 8> Vars;
    if (Tok.is(Token::KW_with)) {
        advance();
    }
    if (expect(Token::ident))
        goto _error;
    Vars.push_back(Tok.getText());
    advance();
    while (Tok.is(Token::comma)) {
        advance();
        if (expect(Token::ident)) {
            goto _error;
        }
        Vars.push_back(Tok.getText());
        advance();
    }
    if (consume(Token::colon)) {
        goto _error;
    }
    E = parseExpr();
    if (Vars.empty()) return E;
    else return new WithDecl(Vars, E);
_error:
    while(!Tok.is(Token::eoi)) advance();
    return nullptr;
}
// expr : term (( "+" | "-" ) term)* ;
Expr *Parser::parseExpr() {
    Expr *Left = parseTerm();
    while (Tok.isOneOf(Token::plus, Token::minus)) {
        BinaryOp::Operator op = Tok.is(Token::plus) ? BinaryOp::Plus : BinaryOp::Minus;
        advance();
        Expr *Right = parseTerm();
        Left = new BinaryOp(op, Left, Right);
    }
    return Left;
}

Expr *Parser::parseTerm() {
    Expr *Left = parseFactor();
    while (Tok.isOneOf(Token::star, Token::slash)) {
        BinaryOp::Operator op = Tok.is(Token::star) ? BinaryOp::Mul : BinaryOp::Div;
        advance();
        Expr *Right = parseFactor();
        Left = new BinaryOp(op, Left, Right);
    }
    return Left;
}

Expr *Parser::parseFactor() {
    Expr *Res = nullptr;
    switch (Tok.getKind())
    {
    case Token::number:
        Res = new Factor(Factor::Number, Tok.getText());
        advance(); break;
    case Token::ident:
        Res = new Factor(Factor::Ident, Tok.getText());
        advance(); break;
    case Token::l_paren:
        advance();
        Res = parseExpr();
        if (!consume(Token::r_paren)) break;
    default:
        if (!Res) error();
        while(!Tok.isOneOf(Token::star, Token::plus, Token::eoi, Token::minus, Token::slash, Token::r_paren)) advance();
    }
    return Res;
}
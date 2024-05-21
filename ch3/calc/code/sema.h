#ifndef SEMA_H
#define SEMA_H

#include "ast.h"
#include "lexer.h"

class Sema {
public:
    bool semantic(AST *Tree);
};


#endif
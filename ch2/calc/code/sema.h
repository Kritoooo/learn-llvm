#ifndef SEMA_H
#define SEMA_H

#include "ast.h"
#include "lexer.h"
#include "llvm/Support/raw_ostream.h"
class Sema {
public:
    bool semantic(AST *Tree);
};


#endif
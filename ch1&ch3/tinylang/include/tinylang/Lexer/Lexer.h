#ifndef TINYLANG_LEXER_LEXER_H
#define TINYLANG_LEXER_LEXER_H

#include "tinylang/Basic/LLVM.h"
#include "tinylang/Basic/Diagnostic.h"
#include "tinylang/Lexer/Token.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/SourceMgr.h"

namespace tinylang {

class KeywordFilter {

    llvm::StringMap<tok::TokenKind> HashTable;
    void addkeyword(StringRef Keyword, tok::TokenKind TokenCode);

public:
    void addKeywords();

    tok::TokenKind getKeyword(StringRef Name, tok::TokenKind DefaultTokenCode = tok::unknown) {
        auto Result = HashTable.find(Name);
        if (Result != HashTable.end())
            return Result->second;
        return DefaultTokenCode;
    }
};

class Lexer {
    SourceMgr &SrcMgr;
    DiagnosticEngine &Diags;

    const char *CurPtr;
    StringRef CurBuf;

    unsigned CurBuffer = 0;

    KeywordFilter keywords;

public:
    Lexer(SourceMgr &SrcMgr, DiagnosticEngine &Diags) : SrcMgr(SrcMgr), Diags(Diags) {
        CurBuffer = SrcMgr.getMainFileID();
        CurBuf = SrcMgr.getMemoryBuffer(CurBuffer)->getBuffer();
        CurPtr = CurBuf.begin();
        keywords.addKeywords();
    }

    DiagnosticEngine &getDiagnostic() const { return Diags; }

    void next(Token &Result);

    StringRef getBuffer() const { return CurBuf; }

private:
    void identifier(Token &Result);
    void number(Token &Result);
    void string(Token &Result);
    void comment();

    SMLoc getLoc() { return SMLoc::getFromPointer(CurPtr); }

    void formToken(Token &Result, const char *TokEnd, tok::TokenKind Kind);
};

} // namespace tinylang

#endif // TINYLANG_LEXER_LEXER_H
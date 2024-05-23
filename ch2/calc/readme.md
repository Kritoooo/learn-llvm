# 文件说明
```
├── ast.h           抽象语法树
├── calc            生成LLVM IR
├── calc.cpp        
├── codegen.cpp     代码生成
├── codegen.h
├── expr            最终的可执行文件
├── expr.o
├── lexer.cpp       词法分析
├── lexer.h
├── parser.cpp      语法分析
├── parser.h
├── rtcalc.c        运行时的库
├── rtcalc.o
├── sema.cpp        语义检查
└── sema.h
```
# 如何使用？
```bash
cd code/
clang++ *.cpp -o calc `llvm-config --cxxflags --ldflags --system-libs --libs core`
./calc "with a: a*3" | llc -filetype=obj -relocation-model=pic -o=expr.o
clang expr.o rtcalc.c -o expr -v
./expr
```
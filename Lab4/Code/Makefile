# GNU make手册：http://www.gnu.org/software/make/manual/make.html
# ************ 遇到不明白的地方请google以及阅读手册 *************

# 编译器设定和编译选项
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c99

# 编译目标：src目录下的所有.c文件
CFILES = $(shell find ./ -name "*.c")
CPPFILES = $(shell find ./ -name "*.cpp")
OBJS = $(CFILES:.c=.o)
LFILE = $(shell find ./ -name "*.l")
YFILE = $(shell find ./ -name "*.y")
LFC = $(shell find ./ -name "*.l" | sed s/[^/]*\\.l/lex.yy.c/)
YFC = $(shell find ./ -name "*.y" | sed s/[^/]*\\.y/syntax.tab.c/)
LFO = $(LFC:.c=.o)
YFO = $(YFC:.c=.o)
T2SUPER = $(shell find ../Test_2_Super/ -name "single-*.cmm")

cleanC:
	rm -rf ast/cmake-build-debug/

parser: cleanC syntax lexical $(CFILES) $(CPPFILES)
	gcc -c -g main.c syntax.tab.c tree.c error.c -lfl -ly
	g++ -std=c++11 -g $(CPPFILES) main.o syntax.tab.o tree.o error.o -lfl -ly -o parser
	# @clear
	@echo  "Successfully get the parser!"


syntax: lexical syntax-c
	$(CC) -c $(YFC) -o $(YFO)

lexical: $(LFILE)
	$(FLEX) -o $(LFC) $(LFILE)

syntax-c: $(YFILE)
	$(BISON) -o $(YFC) -d -v $(YFILE)

-include $(patsubst %.o, %.d, $(OBJS))

# 定义的一些伪目标
.PHONY: clean test
test:
	./parser ../Test_Super/13Lab3WithArray.0.cmm ../Test_Super/13Lab3WithArray.0.ir
	

debug: lexical syntax-c $(CFILES) $(CPPFILES)
	gcc -DDEBUG -c -g main.c syntax.tab.c  -lfl -ly
	g++ -g $(CPPFILES) main.o syntax.tab.o -lfl -ly -o parser

clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
	rm -f scanner
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
	rm -f *~
	rm core

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
	./parser ../Test_Super/13Lab3WithArray.1.cmm ../Test_Super/13Lab3WithArray.1.ir
	./parser ../Test_Super/13Lab3WithArray.2.cmm ../Test_Super/13Lab3WithArray.2.ir
	./parser ../Test_Super/13Lab3WithArray.3.cmm ../Test_Super/13Lab3WithArray.3.ir
	./parser ../Test_Super/13Lab3WithArray.4.cmm ../Test_Super/13Lab3WithArray.4.ir
	./parser ../Test_Super/13Lab3WithArray.5.cmm ../Test_Super/13Lab3WithArray.5.ir
	./parser ../Test_Super/13Lab3WithArray.6.cmm ../Test_Super/13Lab3WithArray.6.ir
	./parser ../Test_Super/13Lab3WithArray.7.cmm ../Test_Super/13Lab3WithArray.7.ir
	./parser ../Test_Super/13Lab3WithArray.8.cmm ../Test_Super/13Lab3WithArray.8.ir
	./parser ../Test_Super/13Lab3WithArray.9.cmm ../Test_Super/13Lab3WithArray.9.ir
	./parser ../Test_Super/13Lab3WithArray.10.cmm ../Test_Super/13Lab3WithArray.10.ir
	./parser ../Test_Super/13Lab3WithArray.11.cmm ../Test_Super/13Lab3WithArray.11.ir
	./parser ../Test_Super/13Lab3WithArray.12.cmm ../Test_Super/13Lab3WithArray.12.ir
	./parser ../Test_Super/13Lab3WithArray.13.cmm ../Test_Super/13Lab3WithArray.13.ir
	./parser ../Test_Super/13Lab3WithArray.14.cmm ../Test_Super/13Lab3WithArray.14.ir
	./parser ../Test_Super/13Lab3WithArray.15.cmm ../Test_Super/13Lab3WithArray.15.ir
	./parser ../Test_Super/13Lab3WithArray.16.cmm ../Test_Super/13Lab3WithArray.16.ir
	./parser ../Test_Super/13Lab3WithArray.17.cmm ../Test_Super/13Lab3WithArray.17.ir
	./parser ../Test_Super/13Lab3WithArray.18.cmm ../Test_Super/13Lab3WithArray.18.ir
	./parser ../Test_Super/13Lab3WithArray.19.cmm ../Test_Super/13Lab3WithArray.19.ir
	./parser ../Test_Super/13Lab3WithArray.20.cmm ../Test_Super/13Lab3WithArray.20.ir
	./parser ../Test_Super/13Lab3WithArray.21.cmm ../Test_Super/13Lab3WithArray.21.ir
	./parser ../Test_Super/13Lab3WithArray.22.cmm ../Test_Super/13Lab3WithArray.22.ir
	./parser ../Test_Super/13Lab3WithArray.23.cmm ../Test_Super/13Lab3WithArray.23.ir
	./parser ../Test_Super/13Lab3WithArray.24.cmm ../Test_Super/13Lab3WithArray.24.ir
	./parser ../Test_Super/13Lab3WithArray.25.cmm ../Test_Super/13Lab3WithArray.25.ir
	./parser ../Test_Super/13Lab3WithArray.26.cmm ../Test_Super/13Lab3WithArray.26.ir
	./parser ../Test_Super/13Lab3WithArray.27.cmm ../Test_Super/13Lab3WithArray.27.ir
	./parser ../Test_Super/13Lab3WithArray.28.cmm ../Test_Super/13Lab3WithArray.28.ir
	./parser ../Test_Super/13Lab3WithArray.29.cmm ../Test_Super/13Lab3WithArray.29.ir
	./parser ../Test_Super/13Lab3WithArray.30.cmm ../Test_Super/13Lab3WithArray.30.ir
	./parser ../Test_Super/13Lab3WithArray.31.cmm ../Test_Super/13Lab3WithArray.31.ir
	./parser ../Test_Super/13Lab3WithArray.32.cmm ../Test_Super/13Lab3WithArray.32.ir
	./parser ../Test_Super/13Lab3WithArray.33.cmm ../Test_Super/13Lab3WithArray.33.ir
	./parser ../Test_Super/13Lab3WithArray.34.cmm ../Test_Super/13Lab3WithArray.34.ir
	./parser ../Test_Super/13Lab3WithArray.35.cmm ../Test_Super/13Lab3WithArray.35.ir
	./parser ../Test_Super/13Lab3WithArray.36.cmm ../Test_Super/13Lab3WithArray.36.ir
	./parser ../Test_Super/13Lab3WithArray.37.cmm ../Test_Super/13Lab3WithArray.37.ir
	./parser ../Test_Super/13Lab3WithArray.38.cmm ../Test_Super/13Lab3WithArray.38.ir
	./parser ../Test_Super/13Lab3WithArray.39.cmm ../Test_Super/13Lab3WithArray.39.ir
	./parser ../Test_Super/13Lab3WithArray.40.cmm ../Test_Super/13Lab3WithArray.40.ir
	./parser ../Test_Super/13Lab3WithArray.41.cmm ../Test_Super/13Lab3WithArray.41.ir
	./parser ../Test_Super/13Lab3WithArray.42.cmm ../Test_Super/13Lab3WithArray.42.ir
	./parser ../Test_Super/13Lab3WithArray.43.cmm ../Test_Super/13Lab3WithArray.43.ir
	./parser ../Test_Super/13Lab3WithArray.44.cmm ../Test_Super/13Lab3WithArray.44.ir
	./parser ../Test_Super/13Lab3WithArray.45.cmm ../Test_Super/13Lab3WithArray.45.ir
	./parser ../Test_Super/13Lab3WithArray.46.cmm ../Test_Super/13Lab3WithArray.46.ir
	./parser ../Test_Super/13Lab3WithArray.47.cmm ../Test_Super/13Lab3WithArray.47.ir
	./parser ../Test_Super/13Lab3WithArray.48.cmm ../Test_Super/13Lab3WithArray.48.ir
	./parser ../Test_Super/13Lab3WithArray.49.cmm ../Test_Super/13Lab3WithArray.49.ir
	
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

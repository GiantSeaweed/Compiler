#include "../tree.h"
#include "MipsBuilder.h"
#include<vector>
#include<string>
#include<string.h>
#include<map>
#include<fstream>
using namespace std;

vector<string*> *mipsList = new vector<string*>();
map<string, int> *offsetTable = new map<string,int>();
int global_offset = 0;
int param_index=0;
//TODO: mapping variable ==> location in memory

void printMipsList(){
    if (outFilename[0] != 0){
        ofstream out(outFilename);
        if (!out.is_open()){
            cerr << outFilename << " open failed!" << endl;
        }
        for (string* str: *mipsList)
            out << *str + "\n";
        out.close();
    }
    for (string* str: *mipsList)
        cout << *str << endl;
}

int getOffset(string str){
    map<string,int>::iterator it = offsetTable->find(str);
    if(it == offsetTable->end()){
        return -1;
    }
    return it->second;
}
void insertOffsetTable(string str){
    offsetTable->insert(pair<string, int>(str, global_offset));
    global_offset -= 4;
}

void mipsBuilder(vector<IRInstruction *> *irList)
{

    auto head = new string(R"(.data
__prompt: .asciiz "Enter an integer:"
__ret: .asciiz "\n"
.globl main
.text)");
    mipsList->push_back(head);

        for (IRInstruction *irInstr : *irList)
    {
        switch (irInstr->op){
            // src1
            case IR_LABEL:{ //LABEL X: ==> "X:"
                string* str = new string(irInstr->src1 + ":");
                mipsList->push_back(str);
                break;
            }
            case IR_FUN:{//"FUNCTION X :" ==> "X:"
                string* str = new string(irInstr->src1 +":");
                mipsList->push_back(str);
                global_offset = 0;
                param_index = 0;
                break;
            }
            case IR_GOTO:{//"GOTO X" ==> "j x"
                string* str = new string("j "+irInstr->src1);
                mipsList->push_back(str);
                break;
            }
            case IR_RETURN:{//"RETURN X"==>""
                int offset = getOffset(irInstr->src1);
                string *str = new string("lw $t1,"+ to_string(offset) + "($sp)");
                mipsList->push_back(str);
                str = new string("move $v0, $t1");
                mipsList->push_back(str);
                str = new string("jr $ra");
                mipsList->push_back(str);
                break;
            }
            case IR_ARG: {//not in table
                //lw reg, addr_of_op
                //sw addr_of_op, offset($sp)
                int offset = getOffset(irInstr->src1);
                string* str = new string("lw $t1, "
                                + to_string(offset)+"($sp)" );
                mipsList->push_back(str);

                str = new string("sw $t1, "+to_string(global_offset)+"($sp)");
                global_offset -= 4;
                mipsList->push_back(str);
                //offset-=4
                break;

            }
            case IR_PARAM: {//not in table
                //tmp = param_index * 4 + 4/*ret addr*/+ 4/*sp*/
                int tmp = param_index * 4 + 12;
                //lw $t1, tmp($sp)
                auto str = new string("lw $t1, "+to_string(tmp)+"($sp)");
                mipsList->push_back(str);
                //sw $t1, offset($sp)
                str = new string("sw $t1, "+to_string(global_offset)+"($sp)");
                mipsList->push_back(str);

                // add this param to the map
                offsetTable->insert(pair<string,int>(irInstr->src1, global_offset));
                //offset -=4;
                global_offset-=4;
                param_index++;
                break;
            }
            case IR_READ:{ //"READ"   //not in table
                string readCode = string("li $v0, 4") + "\n" 
                 + "la $a0, __prompt" + "\n" 
                 + "syscall" + "\n" 
                 + "li $v0, 5" + "\n" 
                 + "syscall";
                auto readCodeString = new string(readCode);
                mipsList->push_back(readCodeString);
                if (getOffset(irInstr->src1)==-1){
                    insertOffsetTable(irInstr->src1
                    );

                }
                int offset = getOffset(irInstr->src1);
                string *str = new string("sw $v0, " + 
                            to_string(offset) + "($sp)");
                    // storeWord(*intermediateCode.parameter, "$v0");

                mipsList->push_back(str);
                break;
            }
            case IR_WRITE:{ //"WRITE" //not in table
                int offset = getOffset(irInstr->src1);
                auto write = string("li $v0, 1") + "\n" 
                + "lw $a0, " + to_string(offset) + "($sp)\n" 
                + "syscall" + "\n" 
                + "li $v0, 4" + "\n" 
                + "la $a0, __ret" + "\n" 
                + "syscall";
                mipsList->push_back(new string(write));
                break;
            }
            //src1 dest
            case IR_DEC: {//not in table
                global_offset -= atoi(irInstr -> src1.c_str());
                offsetTable->insert(pair<string,int>(irInstr->dest,global_offset));
                break;
            }
            case IR_CALL:{//"x := CALL f"
                //save $31
                auto saveRA = new string("sw $31, "+to_string(global_offset)+"($sp)");
                global_offset-=4;
                mipsList->push_back(saveRA);
                //save $sp
                auto saveSp = new string("sw $sp,"+to_string(global_offset)+"($sp)");
                mipsList ->push_back(saveSp);
                global_offset-=4;
                auto calculateSp = new string("addi $sp, $sp,"+to_string(global_offset));
                
                mipsList ->push_back(calculateSp);
                string* str = new string("jal "+irInstr->src1);
                mipsList->push_back(str);
                //recover $31
                //recover $sp
                auto recoverRA = new string("lw $31, 8($sp)");
                mipsList->push_back(recoverRA);
                auto recoverSp = new string("lw $sp, 4($sp)");
                mipsList->push_back(recoverSp);
                

                str = new string("move $t1, $v0");
                mipsList->push_back(str);

                if (getOffset(irInstr->dest)==-1){
                    insertOffsetTable(irInstr->dest);
                }
                str = new string("sw $t1," +
                                     to_string(getOffset(irInstr->dest)) + "($sp)");
                mipsList->push_back(str);
                break;
            }
            case IR_ASSIGN_GETADDR: {//"x := &y" //not in table
                auto loadAddr = new string("addi $t1, $sp,"+to_string(getOffset(irInstr->src1)));
                mipsList -> push_back(loadAddr);
                int x_offset = getOffset(irInstr->dest);
                if (x_offset == -1)
                {
                    insertOffsetTable(irInstr->dest);
                }
                x_offset = getOffset(irInstr->dest);
                auto store = new string("sw $t1, "+to_string(x_offset)+"($sp)");
                mipsList->push_back(store);
                break;
            }
            case IR_ASSIGN_LOAD:{ //"x := *y"
                int x_offset = getOffset(irInstr->dest);
                if(x_offset == -1){
                    insertOffsetTable(irInstr->dest);
                }
                x_offset = getOffset(irInstr->dest);

                string *str = new string("lw $t1," +
                        to_string(getOffset(irInstr->src1)) + "($sp)");//y
                mipsList->push_back(str);
                str = new string("lw $t2," +
                        to_string(x_offset) + "($sp)");//x
                mipsList->push_back(str);
                str = new string("lw $t2, 0($t1)");
                mipsList->push_back(str);
                str = new string("sw $t2, " + 
                        to_string(x_offset) + "($sp)");
                mipsList->push_back(str);
                break;
            }
            case IR_ASSIGN_STORE:{//"*x := y"
                int x_offset = getOffset(irInstr->dest);
                if (x_offset == -1){
                    insertOffsetTable(irInstr->dest);
                }
                x_offset = getOffset(irInstr->dest);

                string *str = new string("lw $t1," +
                                         to_string(getOffset(irInstr->src1)) + "($sp)"); //y
                mipsList->push_back(str);
                str = new string("lw $t2," +
                                 to_string(x_offset) + "($sp)"); //x
                mipsList->push_back(str);
                str = new string("sw $t1, 0($t2)");
                mipsList->push_back(str);
                break;
            }
            case IR_ASSIGN_SINGLE:{//x := #k || x:= y
                int x_offset = getOffset(irInstr->dest);
                if (x_offset == -1){
                    insertOffsetTable(irInstr->dest);
                }
                x_offset = getOffset(irInstr->dest);
                string *str = new string("lw $t1, " + 
                        to_string(x_offset) + "($sp)");
                mipsList->push_back(str);

                if (irInstr->src1[0] == '#'){
                    str = new string("li $t2, " + irInstr->src1.substr(1));
                    mipsList->push_back(str);
                }else{
                    int y_offset = getOffset(irInstr->src1);
                    str = new string("lw $t2, " + to_string(y_offset) + "($sp)");
                    mipsList->push_back(str);
                }

                str = new string("move $t1, $t2");
                mipsList->push_back(str);
                str = new string("sw $t1, "+ 
                        to_string(x_offset) + "($sp)");
                mipsList->push_back(str);
                break;
            }
            //src1 src2 dest
            case IR_ASSIGN_PLUS: //x=y+z || x=y+#k
            case IR_ASSIGN_MINUS: //x=y-z || x=y-#k
            case IR_ASSIGN_MUL:
            case IR_ASSIGN_DIV:{
                int x_offset = getOffset(irInstr->dest);
                if (x_offset == -1){
                    insertOffsetTable(irInstr->dest);
                }
                x_offset = getOffset(irInstr->dest);
                string *str = new string("lw $t1, " +
                                         to_string(x_offset) + "($sp)");
                mipsList->push_back(str);
                if (irInstr->src1[0] == '#'){//y
                    str = new string("li $t2, " + irInstr->src1.substr(1));
                }else{
                    int y_offset = getOffset(irInstr->src1);
                    str = new string("lw $t2, " + to_string(y_offset) + "($sp)");
                }
                mipsList->push_back(str);
                if (irInstr->src2[0] == '#'){//z
                    str = new string("li $t3, " + irInstr->src2.substr(1));
                }
                else{
                    int z_offset = getOffset(irInstr->src2);
                    str = new string("lw $t3, " + to_string(z_offset) + "($sp)");
                }
                mipsList->push_back(str);

                string op;
                if (irInstr->op == IR_ASSIGN_PLUS)
                    op = "add";
                else if (irInstr->op == IR_ASSIGN_MINUS)
                    op = "sub";
                else if (irInstr->op == IR_ASSIGN_MUL)
                    op = "mul";
                else if (irInstr->op == IR_ASSIGN_DIV)
                    op = "div";
                str = new string(op+" $t1, $t2, $t3");
                mipsList->push_back(str);

                str = new string("sw $t1, " +
                                 to_string(x_offset) + "($sp)");
                mipsList->push_back(str);
                break;
            }
            
            case IR_IF_GE:
            case IR_IF_GT:
            case IR_IF_LE:
            case IR_IF_LT:
            case IR_IF_NE:
            case IR_IF_EQ:{
                string cmd;
                if (irInstr->op== IR_IF_EQ) 
                    cmd = "beq";
                else if (irInstr->op == IR_IF_NE) 
                    cmd = "bne";
                else if (irInstr->op == IR_IF_GT)
                    cmd = "bgt";
                else if (irInstr->op == IR_IF_GE)
                    cmd = "bge";
                else if (irInstr->op == IR_IF_LT)
                    cmd = "blt";
                else if (irInstr->op == IR_IF_LE)
                    cmd = "ble";

                string *x = new string("lw $t1," +
                    to_string(getOffset(irInstr->src1)) + "($sp)");
                mipsList->push_back(x);
                string *y = new string("lw $t2," +
                    to_string(getOffset(irInstr->src2)) + "($sp)");
                mipsList->push_back(y);
                string* str = new string(cmd+" $t1, $t2, "+irInstr->dest);
                mipsList->push_back(str);
                break;
            }
    
            default: 
                cerr << "Invalid IR operator" << endl;
                break;
        }
    }
}

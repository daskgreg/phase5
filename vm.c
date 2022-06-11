#include <stdio.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <assert.h>
#include <string.h>
/* #include "structs.h" */
#include "vm.h"
#include "functions.h"
#include "arithmetics_instructions.h"

unsigned consts_newstring(char *s);
unsigned consts_newnumber(double n);
unsigned libfuncs_newused(char *s);

void make_operand(expr *e, vmarg *arg)
{
    switch (e->type)
    {

        /* Storage Variables */

    case var_e:
    case tableitem_e:
    case arithexpr_e:
    case boolexpr_e:
    case newtable_e:
    {
        arg->val = e->sym->offset;

        switch (e->sym->space)
        {
        case programvar:
            arg->type = global_a;
            break;
        case functionlocal:
            arg->type = local_a;
            break;
        case formalarg:
            arg->type = formal_a;
            break;
        }
    }

        /* Constants */

    case constbool_e:
    {
        arg->val = e->boolConst;
        arg->type = bool_a;
        break;
    }

    case conststring_e:
    {
        arg->val = consts_newstring(e->strConst);
        arg->type = string_a;
        break;
    }

    case constnum_e:
    {
        arg->val = consts_newnumber(e->numConst);
        arg->type = number_a;
        break;
    }

    case nill_e:
        arg->type = nil_a;
        break;

        /* Functions */

    case programfunc_e:
    {
        arg->type = userfunc_a;
        arg->val = e->sym->taddress;
        break;
    }

    case libraryfunc_e:
    {
        arg->type = libfunc_a;
        arg->val = libfuncs_newused(e->sym->name);
        break;
    }

        default(assert(0));
    }
}

execute_func_t executeFuncs[] = {

    execute_assign,
    execute_add,
    execute_sub,
    execute_mul,
    execute_div,
    execute_mod,
    execute_uminus,
    execute_and,
    execute_or,
    execute_not,
    execute_jeq,
    execute_jne,
    execute_jle,
    execute_jge,
    execute_jlt,
    execute_jgt,
    execute_call,
    execute_pusharg,
    execute_ret,
    execute_getretval,
    execute_funcenter,
    execute_funcexit,
    execute_jump,
    execute_newtable,
    execute_tablegetelem,
    execute_tablesetelem,
    execute_nop};
avm_memcell stack[AVM_STACKSIZE];
unsigned char executionFinished = 0;  /* An exei teliwsei to execution tou programmatos */
unsigned pc = 0;                      /* Poia entoli ekteloume twra */
unsigned currLine = 0;                /* Se poio source line einai to instruction pou trexoume authn thn stigmh*/
unsigned codeSize = 0;                /* Tha apothikeusoume sthn arxh otan fortosoume ton kwdika to megethos tou*/
instruction *code = (instruction *)0; /*Ton kodika pou tha ginei initiliase sto fortoma tou vm*/
#define AVM_ENDING_PC codeSize

void execute_cycle(void)
{
    if (executionFinished) /* An exei teleiwsei*/
        return;
    else if (pc == AVM_ENDING_PC) /* An to pc einai meta to last instruction */
    {
        executionFinished = 1;
        return;
    }
    else
    {
        assert(pc < AVM_ENDING_PC);     /* Corruption check */
        instruction *instr = code + pc; /* Pairnoume to instruction pou prepei na treksoume authn thn stigmh */
        assert(                         /* Koitame oti to instruction exei valid opcode*/
               instr->opcode >= 0 && instr->opcode <= AVM_MAX_INSTRUCTIONS);
        if (instr->srcLine) /* Apothikeuoume to instruction pou trexoume*/
            currLine = instr->srcLine;
        unsigned oldPC = pc;                   /* Apothikeuoume to old Pc ( tha xreiastei stin sunexeia)*/
        (*executeFuncs[instr->opcode])(instr); /*Dispatch vash tou pinaka executeFuncs[] An to opcode einai to 0 tote kaleitai profanws to execute_assign*/
        if (pc == oldPC)                       /* Elegxoume tis sunartiseis pou mporei na xalasoun ton pc*/
            ++pc;
    }
}

avm_memcell *avm_translate_operand(vmarg *arg, avm_memcell *reg)
{
    /*

    avm_memcell* reg = register ( ton gemizoume kai ton epistrefoum e);

    AVM_STACKSIZE-1 = to telos
    arg->val = offset

    retval = eidiko retval register; Panta prepei na apothikeuoume mono otan theloume na epistrefoume
    mia timh apo sunarthsh kai meta theloume na prosvoume.

    */
    switch (arg->type)
    {
    case global_a:
        return &stack[AVM_STACKSIZE - 1 - arg->val];
    case local_a:
        return &stack[topsp - arg->val];
    case formal_a:
        return &stack[topsp + AVM_STACKENV_SIZE + 1 + arg->val];
    case retval_a:
        return &retval;
    case number_a:
    {
        reg->type = number_m;
        reg->data.numVal = consts_getnumber(arg->val);
        /* reg->data.numVal = consts_getnumber(arg->val).value->doubletype; */
        return reg;
    }
    case string_a:
    {
        reg->type = string_m;
        reg->data.strVal = strdup(consts_getstring(arg->val));
        return reg;
    }
    case bool_a:
    {
        reg->type = bool_m;
        reg->data.boolVal = arg->val;
        return reg;
    }
    case nil_a:
        reg->type = nil_m;
        return reg;
    case userfunc_a:
    {
        reg->type = userfunc_m;
        reg->data.funcVal = arg->val;
        return reg;
    }
    case libfunc_a:
    {
        reg->type = libfunc_m;
        reg->data.libfuncVal = libfuncs_getused(arg->val);
        return reg;
    }
    default:
        assert(0);
    }
}

/*

Apo dialekseis enas dispatcher pou den kanei tipota ektos apo string
Opou se kathe periptwsei (string) prepei na ginetai free
Gia Tables Prepei na kanoume reference counting, tha prepei na
ginetai decrease to reference counting

*/
 void avm_memcellclear(avm_memcell *element)
    if(avm_memcell_t[element] != undef_m)
    {
        element->type = undef_m;
    }
 }

void avm_tostring(avmcell *element)
{
    if (assert(element->data.strVal))
    {
        free(element->data.strVal);
        
    }
    else
    {
        avm_error("avm_to_string() went wrong");
    }
}

executeBin() {}

void main()
{
    /*
     */
    if (executionFinished != 1)
    {
        executeBin();
        while (executionFinished == 0)
        {
            execute_cycle(); /* oso den exei teleiwsei to programma synexeise mexri error h na teliwsei */
        }
    }

    if (executeFinished == 1)
    {
        fprintf(sterr, "Instructions: ");
    }
    return 0;
}
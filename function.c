
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "structs.h"
#include "vm.h"
#include "functions.h"
#include "arithmetics_instructions.h"
unsigned totaßActuals = 0;

/* push arg PART 8/11 */
/* print to call prepei na valoume ta arguments */
void avm_push_enviroment_arg(instruction *instr)
{
    avm_assign(&stack[top], arg); /* execute assign , tha paei sto memcell pou &stack[top] kai tha valei to arguments*/
    ++totalActuals;
    avm_dec_top(); /* Prepei na pame sto epomeno top opote tha kanoume -- alla prepei na checkaroume prota */
}

void avm_dec_top(void)
{
    if (!top)
    {
        avm_error("stack overflow"); /* apo dialekseis */
    }
    else
        --top;
}

/* Call PART 8/11 */
void avm_callsavenviroment(void)
{
    avm_push_envvalue(totalActuals);           /* Posa einai ta totalArguments */
    avm_push_envvalue(pc + 1);                 /* To pou na epistrepsei, to +1 einai gia na treksoume to epomeno pc*/
    avm_push_envvalue(top + totalActuals + 2); /**/
    avm_push_envvalue(topsp);                  /**/
}

void execute_call_enviroment(instruction *instr)
{
    avm_memcell *func = avm_translate_operand(instr->result, &ax);
    assert(func);

    switch (func->type)
    {
    case userfunc_m: /* Pairnoume to pc kai vazoume to taddress(offeset) / o arithmos tou instruction diaforetika*/
    {
        pc = func->data.funcValue;
        assert(pc < AVM_ENDING_PC);
        assert(code[pc].opcode == funcenter_v);
        break;
    }
    case string_m:
        avm_calllibfunc(func->data.strVal);
        break;
    case libfunc_m:
        avm_calllibfunc(func->data.libfuncVal);
        break;
    default:
    {
        avm_error("Cannot find function");
    }
        /* leipei mia case functor i opoia einai otan anoigw kai kleinw agili se ena pedio */
    }
}

void execute_func_enter(instruction *instr) /**/
{
    avm_memcell *func = avm_translate_operand(instr->result, &ax);
    assert(func);

    totalActuals = 0;
    userfunc *funcInfo = avm_getfuncinfo(pc);
    topsp = top;
    top = top - funcInfo->localSize;
}

void execute_func_exit(instruction *unused) /* Afairoume olo to avm_callavenviroment(void)*/
{
    unsigned oldTop = top;
    top = avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
    pc = avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
    topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);

    while (oldTop++ <= top)
    {
        avm_memcellclear(&stack[oldTop]); /*Apo dialekseis enas dispatcher pou den kanei tipota ektos apo string
                                            Opou se kathe periptwsei (string) prepei na ginetai free
                                            Gia Tables Prepei na kanoume reference counting, tha prepei na
                                            ginetai decrease to reference counting
                                            */
    }
}

unsigned avm_totalactuals(void)
{
    return avm_get_envvalue(topsp + AVM_NUMACTUALS_OFFSET);
}

avm_memcell *avm_getactual(unsigned ßi)
{
    assert(i < avm_totalactuals());
    return &stack[topsp + AVM_STACKENV_SIZE + 1 + i]
}
void libfunc_print(void)
{
    unsigned n = avm_totalactuals();
    for (unsigned i = 0; i < n; i++)
    {
        char *s = avm_tostring(avm_getactual(i));
        puts(strdup(s));
        free(s);
    }
}
int consts_getnumber(int num);

void libfunc_typeof(void)
{
    unsigned n = avm_totalactuals();

    if (n != 1)
    {
        avm_error("one argument (not %d) expected in 'typeof'!", n);
    }
    else
    {ß
        avm_memcellclear(&retval);
        retval.type = string_m;
        retval.data.strVal = strdup(typeStrings[avm_getactual(0)->type]);
    }
}
/* p_topsp = previous_top_spß */
void libfunc_totalarguments(void){
    unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    avm_memcellclear(&retval);

    if(!p_topsp){
        avm_error("'totalarguments' called outside a function!");
        retval.type = nil_m;
    }else{
        retval.type = number_m;
        retval.data.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
    }
}
void avm_registerlibfunc(char *id, library_func_t addr);

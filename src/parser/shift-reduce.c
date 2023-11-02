#include <string.h>
#include "ale/vector.h"
#include "ale/parser.h"
#include "ale/error.h"



void
parser_shift_init(struct parser_action *pa, size_t next_state)
{
  pa->type = PARSER_ACTION_SHIFT; 
  pa->shift.next_state = next_state;
}

struct parser_action*
parser_shift_new(size_t next_state)
{
  struct parser_action *pa = malloc(sizeof(struct parser_action));
  
  parser_shift_init(pa,  next_state);

  return pa;
}

struct parser_action*
parser_shift_pool_new(struct mem_pool *pool, size_t next_state)
{
  struct parser_action *pa = mem_malloc(pool, sizeof(struct parser_action));
  
  parser_shift_init(pa,  next_state);

  return pa;
}


struct stack_elem
{
  size_t state;
  void *value;
};

VECTOR_INIT_FULL(stack, struct stack_elem, sizeof(struct stack_elem) * (1u << 16));

struct stack
{
  size_t n;
  struct vector_stack data;
};

void
stack_init(struct stack *stack)
{
  stack->n = 0;
  vector_stack_init(&stack->data);
}


void
stack_destroy(struct stack *stack)
{
  vector_stack_destroy(&stack->data);
}

void
stack_push(struct stack *stack, struct stack_elem elem)
{
  vector_stack_set(&stack->data, stack->n++, elem);
}

struct stack_elem*
stack_top(struct stack *stack)
{
  if (0 == stack->n)
    return NULL;

  return stack->data.data + stack->n - 1;
}

struct stack_elem*
stack_topn(struct stack *stack, size_t n)
{
  if (stack->n < n)
    return NULL;

  return stack->data.data - stack->n;
}

int
stack_popn(struct stack *stack, size_t n)
{
  if (stack->n < n)
    return -1;

  stack->n -= n;
  
  return 0;
}

/* https://lambda.uta.edu/cse5317/notes/node18.html */
/*   push(0); */
/* read_next_token(); */
/* for(;;) */
/* {  s = top();    /\* current state is taken from top of stack *\/ */
/*    if (ACTION[s,current_token] == 'si')   /\* shift and go to state i *\/ */
/*    {  push(i); */
/*       read_next_token(); */
/*    } */
/*    else if (ACTION[s,current_token] == 'ri') */
/*    /\* reduce by rule i: X ::= A1...An *\/ */
/*    {  perform pop() n times; */
/*       s = top();    /\* restore state before reduction from top of stack *\/ */
/*       push(GOTO[s,X]);   /\* state after reduction *\/ */
/*    } */
/*    else if (ACTION[s,current_token] == 'a') */
/*       success!! */
/*    else error(); */
/* } */

void*
parser_shift_reduce(struct parser_shift_reduce *sr)
{
  struct stack stack;
  struct parser_token *token;
  void *ret = NULL;
  
  stack_init(&stack);

  stack_push(&stack, (struct stack_elem) {.state = 0} );
  token = sr->next_token(0, sr->cls); 

  while (1)
    {
      struct stack_elem *top = stack_top(&stack);
      
      ERROR_CUSTOM_RET(NULL == top, PARSER_ERROR_EMPTY_STACK, NULL);

      struct parser_action *action = sr->action_table(top->state, token->id, sr->cls);
      switch (action->type)
	{
	case PARSER_ACTION_SHIFT:
	  
	  stack_push(&stack, (struct stack_elem) { .state = action->shift.next_state, .value = token->str });
	  token = sr->next_token(top->state, sr->cls);
	  
	  break;
	  
	case PARSER_ACTION_REDUCE:
	  void  *val = NULL;
	  if (NULL != action->reduce.callback)
	    {
	      void* ptr[action->reduce.rhs_n];
	      struct stack_elem *elems = stack_topn(&stack, action->reduce.rhs_n);

	      for (size_t i = 0 ; i <  action->reduce.rhs_n ; i++)
		ptr[i] = elems[i].value;
	      
	      val = action->reduce.callback(action->reduce.rhs_n, ptr);
	    }
	  
	  stack_popn(&stack, action->reduce.rhs_n);
	  top = stack_top(&stack);
	  ERROR_CUSTOM_RET(NULL == top, PARSER_ERROR_EMPTY_STACK, NULL);
	  stack_push(&stack, (struct stack_elem) { .state = sr->goto_table(top->state, action->reduce.lhs, sr->cls), .value = val} );
	  
	  break;
	  
	case PARSER_ACTION_ACCEPT:
	  ret = top->value;
	  goto OUT_OF_LOOP;
	  
	  break;

	default:
	  ret = NULL;
	  goto OUT_OF_LOOP;
	}
    }

 OUT_OF_LOOP:
  stack_destroy(&stack);
  
  return ret;
}

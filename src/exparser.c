#include <ale/error.h>

#include "exparser.h"

static struct ep_token 
get_token(void *cls)
{
  return (struct ep_token) { .token = EXPR_TOK_EOF, .value = NULL};
}

static int 
consume(struct ep_parser *ep)
{
  ep->token = ep->get_token(ep->cls);
  ERROR_CUSTOM_RET(-1 == ep->token.token, -1, EXPR_ERR_LEX);

  return ERR_SUCCESS;
}

int 
expr_parser_init(struct ep_parser *ep)
{
  ep->get_token = get_token;
  ep->cls = NULL;
  ep->token = get_token(NULL);

  for (int i = 0 ; i < EXPR_MAX_OP ; i++)
    {
      ep->binop[i] = NULL;
      ep->unop[i] = NULL;
      ep->leaf[i] = NULL;
    }

  return ERR_SUCCESS;
}

int 
expr_parser_destroy(struct ep_parser *ep)
{

  return ERR_SUCCESS;
}

int 
expr_set_lexer(struct ep_parser *ep, struct ep_token (*get_token)(void *cls), void *cls)
{
  ep->get_token = get_token;
  ep->cls = cls;
  ep->token = get_token(cls);

  ERROR_RET(-1 == ep->token.token, -1);
 
  return ERR_SUCCESS;
}

int 
expr_binop_add(struct ep_parser *ep, struct ep_binop *binop)
{
  ERROR_CUSTOM_RET(binop->token >= EXPR_MAX_OP, -1, EXPR_ERR_RANGE);
  ERROR_CUSTOM_RET(EXPR_ASSOC_LEFT != binop->associativity && EXPR_ASSOC_RIGHT != binop->associativity, -1, EXPR_ERR_RANGE);

  ep->binop[binop->token] = binop;

  return ERR_SUCCESS;
}

int 
expr_unop_add(struct ep_parser *ep, struct ep_unop *unop)
{

  ERROR_CUSTOM_RET(unop->token >= EXPR_MAX_OP, -1, EXPR_ERR_RANGE);

  ep->unop[unop->token] = unop;

  return ERR_SUCCESS;
}

int 
expr_leaf_add(struct ep_parser *ep, struct ep_leaf *leaf)
{
  ERROR_CUSTOM_RET(leaf->token >= EXPR_MAX_OP, -1, EXPR_ERR_RANGE);

  ep->leaf[leaf->token] = leaf;

  return ERR_SUCCESS;
}

static void* expr(struct ep_parser *ep, int p);
static void* prod(struct ep_parser *ep);


void* 
expr_parse(struct ep_parser *ep)
{
  void *t = expr(ep, 0);

  ERROR_CUSTOM_RET(EXPR_TOK_EOF != ep->token.token, NULL, EXPR_ERR_NEOF); // TODO: Clean t
   
  return t;
}

static void*
expr(struct ep_parser *ep, int p)
{
  void *t = prod(ep);

  ERROR_RET(NULL == t, NULL);

  while (ep->token.token < EXPR_MAX_OP && NULL != ep->binop[ep->token.token])
    {
      struct ep_binop *op;
      int q;
      void *t1;
      
      op = ep->binop[ep->token.token];
      if (op->precedence < p)
	break;

      ERROR_RET(-1 == consume(ep), NULL); // TODO: Clean t
      
      q = op->precedence;
      if (EXPR_ASSOC_LEFT == op->associativity)
	q++;

      ERROR_RET(NULL == (t1 = expr(ep, q)), NULL); // TODO: Clean t

      t = op->func(op, t, t1);
    }

  return t;
}

static void*
prod(struct ep_parser *ep)
{
  void *t = NULL;

  if (ep->token.token < EXPR_MAX_OP && NULL != ep->unop[ep->token.token])
    {
      struct ep_unop *op = ep->unop[ep->token.token];

      ERROR_RET(-1 == consume(ep), NULL);
      ERROR_RET(NULL == (t = expr(ep, op->precedence)), NULL);
    }
  else if (EXPR_TOK_LPAR == ep->token.token)
    {
      ERROR_RET(-1 == consume(ep), NULL);
      ERROR_RET(NULL == (t = expr(ep, 0)), NULL);
      ERROR_CUSTOM_RET(EXPR_TOK_RPAR != ep->token.token, NULL, EXPR_ERR_NLPAR); // TODO: Clean t
    }
  else if (ep->token.token < EXPR_MAX_OP && NULL != ep->leaf[ep->token.token])
    {
      struct ep_leaf *leaf = ep->leaf[ep->token.token];
	    
      t = leaf->func(&ep->token);
      ERROR_RET(-1 == consume(ep), NULL); // TODO: Clean t
    }
  else
    ERROR_CUSTOM_RET(1, NULL, EXPR_ERR_UTOK);

  return t;
}

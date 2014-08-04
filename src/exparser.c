#include <ale/error.h>

#include "exparser.h"

static struct ep_token 
get_token(void *cls)
{
  return (struct ep_token) { .token = EXPR_TOK_EOF, .value = NULL};
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
  ERROR_UNDEF_RET(binop->token >= EXPR_MAX_OP, -1);

  ep->binop[binop->token] = binop;

  return ERR_SUCCESS;
}

int 
expr_unop_add(struct ep_parser *ep, struct ep_unop *unop)
{

  ERROR_UNDEF_RET(unop->token >= EXPR_MAX_OP, -1);

  ep->unop[unop->token] = unop;

  return ERR_SUCCESS;
}

void* 
expr_parse(struct ep_parser *ep)
{

  return NULL;
}

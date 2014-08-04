#ifndef __EXPARSER_H
#define __EXPARSER_H

#define EXPR_TOK_EOF 0
#define EXPR_MAX_OP 128

struct ep_token {
  int token;
  void *value;
};

struct ep_unop {
  int token;
  int precedence;
  void* (*func)(void *right);
};

struct ep_binop {
  int token;
  int precedence;
  int associativity;
  void* (*func)(void *left, void *right);
};

struct ep_parser {
  struct ep_token token;
  void *cls;
  struct ep_unop *unop[EXPR_MAX_OP];
  struct ep_binop *binop[EXPR_MAX_OP];
  struct ep_token (*get_token)(void *cls);  
};

int expr_parser_init(struct ep_parser *ep);
int expr_parser_destroy(struct ep_parser *ep);
int expr_set_lexer(struct ep_parser *ep, struct ep_token (*get_token)(void *cls), void *cls);
int expr_binop_add(struct ep_parser *ep, struct ep_binop *binop);
int expr_unop_add(struct ep_parser *ep, struct ep_unop *unop);

void* expr_parse(struct ep_parser *ep);

#endif

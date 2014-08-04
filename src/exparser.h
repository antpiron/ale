#ifndef __EXPARSER_H
#define __EXPARSER_H

enum {
  EXPR_TOK_EOF = 0,
  EXPR_TOK_LPAR,
  EXPR_TOK_RPAR,
  EXPR_TOK_LAST
};

#define EXPR_MAX_OP 128

#define EXPR_ASSOC_LEFT 0
#define EXPR_ASSOC_RIGHT 0

#define EXPR_ERR_NEOF 1
#define EXPR_ERR_UOP 2
#define EXPR_ERR_LEX 3
#define EXPR_ERR_RANGE 4
#define EXPR_ERR_UTOK 5
#define EXPR_ERR_NLPAR 6

struct ep_token {
  int token;
  void *value;
};

struct ep_unop {
  int token;
  int precedence;
  void* (*func)(struct ep_unop *op, void *right);
};

struct ep_binop {
  int token;
  int precedence;
  int associativity;
  void* (*func)(struct ep_binop *op, void *left, void *right);
};

struct ep_leaf {
  int token;
  void* (*func)(struct ep_token *op);
};

struct ep_parser {
  struct ep_token token;
  void *cls;
  struct ep_unop *unop[EXPR_MAX_OP];
  struct ep_binop *binop[EXPR_MAX_OP];
  struct ep_leaf *leaf[EXPR_MAX_OP];
  struct ep_token (*get_token)(void *cls);  
};

int expr_parser_init(struct ep_parser *ep);
int expr_parser_destroy(struct ep_parser *ep);
int expr_set_lexer(struct ep_parser *ep, struct ep_token (*get_token)(void *cls), void *cls);
int expr_binop_add(struct ep_parser *ep, struct ep_binop *binop);
int expr_unop_add(struct ep_parser *ep, struct ep_unop *unop);
int expr_leaf_add(struct ep_parser *ep, struct ep_leaf *leaf);

void* expr_parse(struct ep_parser *ep);

#endif

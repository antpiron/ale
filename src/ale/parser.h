#ifndef __PARSER_H
#define __PARSER_H

#include <stdlib.h>
#include <stddef.h>
#include <ale/memory.h>
#include <ale/stringutils.h>


/* ============ */
/* Grammar */

enum { PARSER_GRAMMAR_EOR, PARSER_GRAMMAR_STR, PARSER_GRAMMAR_REGEX, PARSER_GRAMMAR_NONTERMINAL };

struct parser_terminal_string
{
  char *str;
};

struct parser_terminal_regex
{
  char *str;
};

struct parser_grammar_node
{
  int type;
  union
  {
    struct parser_terminal_string string;
    struct parser_terminal_regex regex;
    size_t nonterminal;
  };
};

struct parser_rule_rhs
{
  size_t n;
  struct parser_grammar_node *node;
};

struct parser_rule
{
  struct string name;
  size_t n;
  struct parser_rule_rhs *rhs;
};

struct parser_grammar
{
  size_t n;
  struct parser_rule *rules;
};

struct parser_rule_item
{
  struct parser_rule *rule;
  size_t dot;
  size_t n;
  int *follow;
};

void grammar_init(struct parser_grammar *g);
void grammar_destroy(struct parser_grammar *g);

struct parser_grammar_node* grammar_node_regex(char *regex);
struct parser_grammar_node* grammar_node_string(char *str);
struct parser_grammar_node* grammar_node_nonterminal(size_t id);

ssize_t grammar_add_rule(struct parser_grammar *g, size_t lhs, ...);

/* ============ */
/* Lexer */

struct lexer_token
{
  size_t id;
  char *str;
};


/* ============ */
/* Shift-Reduce automata */

enum {
  PARSER_ACTION_ERROR,
  PARSER_ACTION_SHIFT,
  PARSER_ACTION_REDUCE,
  PARSER_ACTION_ACCEPT
};

enum {
  PARSER_TOKEN_EOF,
  PARSER_TOKEN_STRING,
  PARSER_TOKEN_REGEX
};

struct parser_shift_action
{
  size_t next_state;
};

struct parser_reduce_action
{
  size_t lhs;
  size_t rhs_n;
  void* (*callback)(size_t n, void* rhs[n], void *cls);
};

struct parser_action
{
  int type;
  union
  {
    struct parser_shift_action shift;
    struct parser_reduce_action reduce;
  };
};

void parser_shift_init(struct parser_action *pa, size_t next_state);
void parser_shift_destroy(struct parser_action *pa);
void parser_shift_free(struct parser_action *pa);
struct parser_action* parser_shift_new(size_t next_state);
struct parser_action* parser_shift_pool_new(struct mem_pool *pool, size_t next_state);

void* parser_tree_callback(size_t n, void* rhs[n], void *cls);
void parser_reduce_init(struct parser_action *pa, size_t lhs, size_t rhs_n,
			void* (*callback)(size_t n, void* rhs[n], void *cls));

struct parser_shift_reduce
{
  struct mem_pool pool;
  ssize_t (*goto_table)(size_t state, size_t lhs, void *cls);
  struct parser_action* (*action_table)(size_t state, size_t token, void *cls);
  struct lexer_token* (*next_token)(size_t state, void *cls);
};


enum { PARSER_ERROR_EMPTY_STACK = 1, PARSER_ERROR_STACK_TOO_SMALL, PARSER_ERROR_REDUCE, PARSER_ERROR };

void parser_shift_reduce_init(struct parser_shift_reduce *sr,
			      ssize_t (*goto_table)(size_t state, size_t lhs, void *cls),
			      struct parser_action* (*action_table)(size_t state, size_t token, void *cls),
			      struct lexer_token* (*next_token)(size_t state, void *cls));
void parser_shift_reduce_destroy(struct parser_shift_reduce *sr);

int parser_shift_reduce(struct parser_shift_reduce *sr, void *value , void *cls);

#endif

#ifndef __PARSER_H
#define __PARSER_H

#include <stdlib.h>
#include <stddef.h>
#include <ale/memory.h>
#include <ale/stringutils.h>
#include <ale/index.h>
#include <ale/vector.h>

/* ============ */
/* Grammar */

enum { GRAMMAR_TERMINAL, GRAMMAR_NON_TERMINAL };

struct grammar_node
{
  int type;
  size_t index;
};

VECTOR_INIT(grammar_node, struct grammar_node)

struct rule
{
  size_t lhs;
  size_t n_rhs;
  struct vector_grammar_node rhs;
};
  
struct parser_grammar
{
  struct mem_pool pool;
  size_t n_terminals, n_nonterminals;
  struct index terminals, nonterminals;
};

struct parser_item
{
  size_t rule;
  size_t dot;
  size_t n;
  size_t *follow;
};

struct parser_item_set
{
  size_t n;
  struct parser_item *item;
  size_t n_next;
  struct parser_item_set **next;
};

void grammar_init(struct parser_grammar *g);
void grammar_destroy(struct parser_grammar *g);

/* struct parser_grammar_node* grammar_node_regex(char *regex); */
/* struct parser_grammar_node* grammar_node_string(char *str); */
/* struct parser_grammar_node* grammar_node_nonterminal(size_t id); */

ssize_t grammar_add_nonterminal(struct parser_grammar *g, const char *name);
ssize_t grammar_add_terminal(struct parser_grammar *g, const char *str);

#define G_NT(G, NAME) &(struct grammar_node) { .type = GRAMMAR_NON_TERMINAL, .index = grammar_add_nonterminal(G, NAME) }
#define G_T(G, STR) &(struct grammar_node) { .type = GRAMMAR_TERMINAL, .index = grammar_add_terminal(G, STR) }

ssize_t grammar_add_rule_va(struct parser_grammar *g, size_t lhs, ...);
ssize_t grammar_set_start(struct parser_grammar *g, size_t lhs, size_t n, size_t follow[n]);
ssize_t grammar_set_start_va(struct parser_grammar *g, size_t lhs, ...);




// grammar_item_first(struct parser_grammar *g, struct parser_item);

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

/*
void parser_shift_init(struct parser_action *pa, size_t next_state);
void parser_shift_destroy(struct parser_action *pa);
void parser_shift_free(struct parser_action *pa);
struct parser_action* parser_shift_new(size_t next_state);
struct parser_action* parser_shift_pool_new(struct mem_pool *pool, size_t next_state);

void* parser_tree_callback(size_t n, void* rhs[n], void *cls);
void parser_reduce_init(struct parser_action *pa, size_t lhs, size_t rhs_n,
			void* (*callback)(size_t n, void* rhs[n], void *cls));
*/

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

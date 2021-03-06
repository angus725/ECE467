
#ifndef AST_H_
#define AST_H_ 1

#include <stdarg.h>
#include <string>
#include <stdint.h>


// Dummy node just so everything compiles, create your own node/nodes
//
// The code provided below is an example ONLY. You can use/modify it,
// but do not assume that it is correct or complete.
//
// There are many ways of making AST nodes. The approach below is an example
// of a descriminated union. If you choose to use C++, then I suggest looking
// into inheritance.

// forward declare
struct node_;
typedef struct node_ node;
extern node *ast;

typedef enum {
    UNKNOWN,
    SCOPE,
    MULTI_NODE,
    DECLARATION,
    IF_STATEMENT,
    ASSIGN_STATEMENT,
    TYPE,
    EXPRESSION_START,
    VARIABLE,
    FUNC_CALL_EXP,
    CONSTRUCTOR_EXP,
    UNARY_EXP,
    BINARY_EXP,
    LITERAL_EXP,
    EXPRESSION_END,
    ARGUMENTS,
    ARGUMENTS_OPT
} node_kind;

enum data_type
{
    TYPE_UNKNOWN = 0,
	TYPE_BOOL,
	TYPE_BVEC2,
	TYPE_BVEC3,
	TYPE_BVEC4,
    TYPE_INT,
	TYPE_IVEC2,
	TYPE_IVEC3,
	TYPE_IVEC4,
    TYPE_FLOAT,
	TYPE_VEC2,
	TYPE_VEC3,
	TYPE_VEC4,
    TYPE_ANY, // for bypassing errors

    TYPE_INVALID = -1,
    WRITE_ONLY = -2,
    NOT_FOUND = -3,
};

enum func_type
{
    FUNC_DP3 = 0,
    FUNC_LIT = 1,
    FUNC_RSQ = 2,
    FUNC_ANY = 3, // for bypassing errors

};

enum unary_opt
{
    UOPT_NEG,
    UOPT_NOT,
};

enum binary_opt
{
    BOPT_AND,
    BOPT_OR,
    BOPT_EXPO,
    BOPT_EQ,
    BOPT_NEQ,
    BOPT_LT,
    BOPT_LEQ,
    BOPT_GT,
    BOPT_GEQ,
    BOPT_ADD,
    BOPT_SUB,
    BOPT_MUL,
    BOPT_DIV,
};


void varTypeToText(enum data_type data_type, std::string &result);

struct node_
{

    //  node_(): kind(UNKNOWN), line_num(0), constantValue(0), variable{} { };
    // ~node_(){};

    // an example of tagging each node with a type
    node_kind kind;
    int line_num;
    int constantValue; // for symatic checker, don't worry about this during parsing.
    union {
	struct
	{
	    node *declarations;
	    node *statements;
	} scope;

	struct
	{
	    node *nodes;
	    node *cur_node;
	    // char *last_var_result_type; //Need to be filled from semantic analysis
	    // if the var types are not identical, it should be TYPE_ANY
	    // if there is no result type, it should be 0
	} multi_node;

	struct
	{
	    int is_const;
	    node *type;
	    char *identifier;
	    node *expression;
	} declaration;

	struct
	{
	    node *variable;
	    node *expression;
	} assignment_statement;

	struct
	{
	    node *if_confition;
	    node *if_body;
	    node *else_body;
	} if_statement;

	struct
	{
	    enum data_type var_type;
	    int array_bound;
	} type;

	struct
	{
	    node *type;
	    node *args_opt;
	} constructor_exp;

	struct
	{
	    enum func_type func;
	    enum data_type result_type;
	    node *args_opt;
	} func_call_exp;

	struct
	{
	    enum unary_opt uopt;
	    node *operand;
	    enum data_type result_type;
	} unary_exp;

	struct
	{
	    enum binary_opt bopt;
	    node *operand1;
	    node *operand2;
	    enum data_type result_type; //Need to be filled from semantic analysis
	} binary_exp;

	struct
	{
	    enum data_type lit_type; // used by symantic checker "getExpressionResultType"
	    union {
		int val_bool;
		int val_int;
		double val_float;
	    };
	} literal_exp;

	struct
	{
	    char *identifier;
	    int array_index; // ie, the second 4 in vec4 vector[4]
	    int has_index;
	    int array_bound;	//Need to be filled from sematic analysis
	    enum data_type var_type; //Need to be filled from sematic analysis
	} variable;
    };
};

node *ast_allocate(node_kind type, ...);
void ast_free(node *ast);
void ast_print(node *ast);
void ast_traversal(
    node *ast,
    void (*pre_order_func)(node *N),
    void (*post_order_func)(node *N));
int is_expression(node_kind kind);
char *type_to_str(enum data_type type);
enum data_type var_type_to_base (enum data_type var_type);

#endif /* AST_H_ */

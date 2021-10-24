#ifndef AST_H
# define AST_H


typedef struct class_node class_node;
typedef struct object_ty_node object_ty_node;

typedef struct ty_node {
  enum { int_ty, string_ty, array_ty, object_ty } kind;
  union { struct ty_node* inner_ty;  // only for array_ty
          struct object_ty_node* object;
  }data;
} ty_node;

ty_node* IntTyNode();
ty_node* StringTyNode();
ty_node* ArrayTyNode(ty_node* ty);
ty_node* ObjectTyNode(object_ty_node* object);

typedef struct exp_node exp_node;

typedef struct lval_node {
  enum { id_lval, array_lval } kind;
  union { char* name; struct { struct lval_node* base; exp_node* index; } array_ops; } data;
} lval_node;

lval_node* IdNode();
lval_node* ArrayNode(lval_node* base, exp_node* index);

typedef enum { plus_op, minus_op, times_op, div_op, eq_op, neq_op, lt_op, le_op, and_op, or_op } binop;

typedef struct exp_list exp_list;

struct exp_node {
  enum { lval_exp, int_exp, string_exp, binop_exp, call_exp, variableofobject_exp, functionofobject_exp } kind;
  union { lval_node* lval; int ival; char* sval; struct { binop op; struct exp_node* e1; struct exp_node* e2; } bin_ops;
          struct { char* name; exp_list* args; } call_ops; 
          struct { char* objectname; char* variablename; } objectvariable_ops;
          struct { char* objectname; char* functionName; exp_list* args; } objectfunction_ops; } data;
};

exp_node* LvalNode(lval_node* l);
exp_node* IntNode(int val);
exp_node* StringNode(char* val);
exp_node* BinopNode(binop op, exp_node* left, exp_node* right);
exp_node* CallNode(char* name, exp_list* args);
exp_node* VariableCallOnObject(char* objectname, char* variablename);
exp_node* FunctionCallOnObject(char* objectname, char* functionName, exp_list* args);

typedef struct exp_list {
  exp_node* head;
  struct exp_list* next;
}exp_list;

exp_list* ExpList(exp_node* hd, exp_list* tl);

typedef struct param_list {
  ty_node* ty;
  char* name;
  struct param_list* next;
} param_list;

param_list* ParamList(ty_node* ty, char* name, param_list* tl);

typedef struct stmt_list stmt_list;
typedef struct func_list func_list;
typedef struct vardec_list vardec_list;
typedef struct func_ty_node func_ty_node;
typedef struct vardec_ty_node vardec_ty_node;


typedef struct stmt_node {
  enum { vardec_stmtty, if_stmt, block_stmt, while_stmt, fundec_stmt, constructor_call } kind;
  union { vardec_ty_node* vardec;
          struct { exp_node* cond; struct stmt_node* then_stmt; struct stmt_node* else_stmt; } if_ops;
          struct { exp_node* cond; struct stmt_node* body; } while_ops;
          stmt_list* body;
          func_ty_node* fundec;
          struct {char* name; exp_list* params;}constructor_ops;
        } data;
} stmt_node;

stmt_node* VardecStmtNode(vardec_ty_node* vardec);
stmt_node* FundecStmtNode(func_ty_node* fundec);
stmt_node* IfNode(exp_node* cond, stmt_node* then_stmt, stmt_node* else_stmt);
stmt_node* WhileNode(exp_node* cond, stmt_node* body);
stmt_node* BlockNode(stmt_list* body);
stmt_node* ContructorCallNode(char* name, exp_list* par);

typedef struct ret_stmt{
  exp_node* exp;
}ret_stmt;
ret_stmt* RetNode(exp_node* e);

typedef struct stmt_list {
  stmt_node* head;
  struct stmt_list* next;
}stmt_list;

stmt_list* StmtList(stmt_node* hd, stmt_list* tl);

struct object_ty_node{
  enum {declaration_ty, instantiation_ty } kind;
  union {
    char* name; 
    class_node* class;
  } data;
};

object_ty_node* declarationTyNode(char* name);
object_ty_node* InstantiationTyNode(class_node* class);

struct class_node {
  char* name;
  vardec_list* vardec_list;
  func_list* func_list;
}; 

class_node* ClassDecNode(char* name, vardec_list* vardec_list, func_list* func_list);

typedef struct class_list {
  class_node* head;
  struct class_list* next;
}class_list;

class_list* ClassList(class_node* hd, class_list* tl);

struct vardec_list {
  vardec_ty_node* head;
  struct vardec_list* next;
};

vardec_list* VardecList(vardec_ty_node* h, vardec_list* n);

typedef struct var_ty_list
{
  char* name;
  ty_node* ty;
  struct var_ty_list* next;
}var_ty_list;

typedef struct func_ty_list
{
  char* name;
  ty_node* retty;
  param_list* params;
  struct func_ty_list* next;
}func_ty_list;


struct vardec_ty_node{
  enum { assign_stmt, vardec_stmt, arraydec_stmt, objectdecinstant_stmt, objectdec_stmt, objectinstant_stmt}kind;
  union { struct { lval_node* lhs; exp_node* rhs; } assign_ops;
          struct { ty_node* ty; char* name; exp_node* rhs; } vardec_ops;
          struct { ty_node* ty; int size; char* name; exp_node* rhs; } arraydec_ops;
          struct { char* decType; char* instantType; char* name; exp_list* arg; }objectdecinstant_ops;
          struct { char* decType; char* name; }objectdec_ops;
          struct { char* instantType; lval_node* lhs; exp_list* arg; }objectinstant_ops;
  }data;
};

vardec_ty_node* AssignNode(lval_node* lhs, exp_node* rhs);
vardec_ty_node* VardecNode(ty_node* ty, char* name, exp_node* rhs);
vardec_ty_node* ArraydecNode(ty_node* ty, int size, char* name, exp_node* rhs);
vardec_ty_node* ObjectDecInstantNode(char* decType, char* instantType, char* name, exp_list* arg);
vardec_ty_node* ObjectDecNode(char* decType, char* name);
vardec_ty_node* ObjectInstantNode(char* instantType, lval_node* lhs, exp_list* arg);

struct func_list{
  func_ty_node* head;
  struct func_list* next;
};

func_list* FuncList(func_ty_node* f, func_list* n);

struct func_ty_node{
  enum { constructor_func, function }kind;  
  union {
    struct {char* name; param_list* params; stmt_list* stmt;}constructor_ops;
    struct {ty_node* retty; char* name; param_list* params; stmt_list* body;ret_stmt* ret;}function_ops;
  }data;  
};

func_ty_node* ConstFun(char* name, param_list* param, stmt_list* stmt);
func_ty_node* FundecNode(ty_node* retty, char* name, param_list* params, stmt_list* body, ret_stmt* ret);

const char* ty_to_string(ty_node* ty);
void print_ty(ty_node* ty);
void print_lval(lval_node* lval);
void print_exp(exp_node* exp);
void print_exp_list(exp_list* l);
void print_stmt(stmt_node* stmt, int indent);

void print_class_node(class_node* class);
void print_class_list(class_list* l);
void print_vardec_ty_node(vardec_ty_node* vardec);
void print_vardec_list(vardec_list* l);
void print_func_ty_node(func_ty_node* func_ty_node);
void print_func_list(func_list* l);
void print_tree(class_list* program);

#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "ast.h"

ty_node* IntTyNode(){
  ty_node* node = malloc(sizeof(ty_node));
  node->kind = int_ty;
  node->data.inner_ty = NULL;
  return node;
}

ty_node* StringTyNode(){
  ty_node* node = malloc(sizeof(ty_node));
  node->kind = string_ty;
  node->data.inner_ty = NULL;
  return node;
}

ty_node* ArrayTyNode(ty_node* ty){
  ty_node* node = malloc(sizeof(ty_node));
  node->kind = array_ty;
  node->data.inner_ty = ty;
  return node;
}

ty_node* ObjectTyNode(object_ty_node* object){
  ty_node* node = malloc(sizeof(ty_node));
  node->kind = object_ty;
  node->data.object = object;
  return node;
}


object_ty_node* declarationTyNode(char* name){
  object_ty_node* node = malloc(sizeof(object_ty_node));
  node->kind = declaration_ty;
  node->data.name = name;
  return node;
}

object_ty_node* InstantiationTyNode(class_node* class){
  object_ty_node* node = malloc(sizeof(object_ty_node));
  node->kind = instantiation_ty;
  node->data.class = class;
  return node;
}

lval_node* IdNode(char* name){
  lval_node* node = malloc(sizeof(lval_node));
  node->kind = id_lval;
  node->data.name = String(name);
  return node;
}

lval_node* ArrayNode(lval_node* base, exp_node* index){
  lval_node* node = malloc(sizeof(lval_node));
  node->kind = array_lval;
  node->data.array_ops.base = base;
  node->data.array_ops.index = index;
  return node;
}

exp_node* LvalNode(lval_node* l){
  exp_node* node = malloc(sizeof(exp_node));
  node->kind = lval_exp;
  node->data.lval = l;
  return node;
}

exp_node* IntNode(int val){
  exp_node* node = malloc(sizeof(exp_node));
  node->kind = int_exp;
  node->data.ival = val;
  return node;
}

exp_node* StringNode(char* val){
  exp_node* node = malloc(sizeof(exp_node));
  node->kind = string_exp;
  node->data.sval = String(val);
  return node;
}

exp_node* BinopNode(binop op, exp_node* left, exp_node* right){
  exp_node* node = malloc(sizeof(exp_node));
  node->kind = binop_exp;
  node->data.bin_ops.op = op;
  node->data.bin_ops.e1 = left;
  node->data.bin_ops.e2 = right;
  return node;
}

exp_node* CallNode(char* name, exp_list* args){
  exp_node* node = malloc(sizeof(exp_node));
  node->kind = call_exp;
  node->data.call_ops.name = String(name);
  node->data.call_ops.args = args;
  return node;
}

exp_node* VariableCallOnObject(char* objectname, char* variablename){
  exp_node* node = malloc(sizeof(exp_node));
  node->kind = variableofobject_exp;
  node->data.objectvariable_ops.objectname = String(objectname);
  node->data.objectvariable_ops.variablename = String(variablename);
  return node;
}

exp_node* FunctionCallOnObject(char* objectname, char* functionName, exp_list* args){
  exp_node* node = malloc(sizeof(exp_node));
  node->kind = functionofobject_exp;
  node->data.objectfunction_ops.objectname = String(objectname);
  node->data.objectfunction_ops.functionName = String(functionName);
  node->data.objectfunction_ops.args = args;
  return node;
}


exp_list* ExpList(exp_node* hd, exp_list* tl){
  exp_list* l = malloc(sizeof(exp_list));
  l->head = hd;
  l->next = tl;
  return l;
}

param_list* ParamList(ty_node* ty, char* name, param_list* tl){
  param_list* l = malloc(sizeof(param_list));
  l->ty = ty;
  l->name = String(name);
  l->next = tl;
  return l;
}

vardec_ty_node* AssignNode(lval_node* lhs, exp_node* rhs){
  vardec_ty_node* node = malloc(sizeof(vardec_ty_node));
  node->kind = assign_stmt;
  node->data.assign_ops.lhs = lhs;
  node->data.assign_ops.rhs = rhs;
  return node;
}

vardec_ty_node* VardecNode(ty_node* ty, char* name, exp_node* rhs){
  vardec_ty_node* node = malloc(sizeof(vardec_ty_node));
  node->kind = vardec_stmt;
  node->data.vardec_ops.ty = ty;
  node->data.vardec_ops.name = String(name);
  node->data.vardec_ops.rhs = rhs;
  return node;
}

vardec_ty_node* ArraydecNode(ty_node* ty, int size, char* name, exp_node* rhs){
  vardec_ty_node* node = malloc(sizeof(vardec_ty_node));
  node->kind = arraydec_stmt;
  node->data.arraydec_ops.ty = ty;
  node->data.arraydec_ops.size = size;
  node->data.arraydec_ops.name = String(name);
  node->data.arraydec_ops.rhs = rhs;
  return node;
}


vardec_ty_node* ObjectDecInstantNode(char* decType, char* instantType, char* name, exp_list* arg){
  vardec_ty_node* node = malloc(sizeof(vardec_ty_node));
  node->kind = objectdecinstant_stmt;
  node->data.objectdecinstant_ops.decType = String(decType);
  node->data.objectdecinstant_ops.instantType = String(instantType);
  node->data.objectdecinstant_ops.name = String(name);
  node->data.objectdecinstant_ops.arg = arg;
  return node;
}

vardec_ty_node* ObjectDecNode(char* decType, char* name){
  vardec_ty_node* node = malloc(sizeof(vardec_ty_node));
  node->kind = objectdec_stmt;
  node->data.objectdec_ops.decType = String(decType);
  node->data.objectdec_ops.name = String(name);
  return node;
}

vardec_ty_node* ObjectInstantNode(char* instantType, lval_node* lhs, exp_list* arg){
  vardec_ty_node* node = malloc(sizeof(vardec_ty_node));
  node->kind = objectinstant_stmt;
  node->data.objectinstant_ops.instantType = String(instantType);
  node->data.objectinstant_ops.lhs = lhs;
  node->data.objectinstant_ops.arg = arg;
  return node;
}

func_ty_node* FundecNode(ty_node* retty, char* name, param_list* params, stmt_list* body, ret_stmt* ret){
  func_ty_node* node = malloc(sizeof(func_ty_node));
  node->kind = function;
  node->data.function_ops.retty = retty;
  node->data.function_ops.name = String(name);
  node->data.function_ops.params = params;
  node->data.function_ops.body = body;
  node->data.function_ops.ret = ret;
  return node;
}

func_ty_node* ConstFun(char* name, param_list* params, stmt_list* body){
  func_ty_node* node = malloc(sizeof(func_ty_node));
  node->kind = constructor_func;
  node->data.constructor_ops.name = String(name);
  node->data.constructor_ops.params = params;
  node->data.constructor_ops.stmt = body;
  return node;
}

stmt_node* FundecStmtNode(func_ty_node* fundec){
  stmt_node* stmt = malloc(sizeof(stmt));
  stmt->kind = fundec_stmt;
  stmt->data.fundec = fundec;
  return stmt;
}

stmt_node* VardecStmtNode(vardec_ty_node* vardec){
  stmt_node* stmt = malloc(sizeof(stmt));
  stmt->kind = vardec_stmtty;
  stmt->data.vardec = vardec;
  return stmt;
}


stmt_node* IfNode(exp_node* cond, stmt_node* then_stmt, stmt_node* else_stmt){
  stmt_node* node = malloc(sizeof(stmt_node));
  node->kind = if_stmt;
  node->data.if_ops.cond = cond;
  node->data.if_ops.then_stmt = then_stmt;
  node->data.if_ops.else_stmt = else_stmt;
  return node;
}

stmt_node* WhileNode(exp_node* cond, stmt_node* body){
  stmt_node* node = malloc(sizeof(stmt_node));
  node->kind = while_stmt;
  node->data.while_ops.cond = cond;
  node->data.while_ops.body = body;
  return node;
}

stmt_node* BlockNode(stmt_list* body){
  stmt_node* node = malloc(sizeof(stmt_node));
  node->kind = block_stmt;
  node->data.body = body;
  return node;
}

stmt_node* ContructorCallNode(char* name, exp_list* params){
  stmt_node* node = malloc(sizeof(stmt_node));
  node->kind = constructor_call;
  node->data.constructor_ops.name = name;
  node->data.constructor_ops.params = params;
  return node;
}

ret_stmt* RetNode(exp_node* e){
  ret_stmt* node = malloc(sizeof(ret_stmt));
  node->exp = e;
  return node;
}

stmt_list* StmtList(stmt_node* hd, stmt_list* tl){
  stmt_list* l = malloc(sizeof(stmt_list));
  l->head = hd;
  l->next = tl;
  return l;
}

class_node* ClassDecNode(char* name, vardec_list* vardec_list, func_list* func_list){
  class_node* class = malloc(sizeof(class_node));
  class->name = name;
  class->func_list = func_list;
  class->vardec_list = vardec_list;
  return class;
}

class_list* ClassList(class_node* hd, class_list* tl){
  class_list* l = malloc(sizeof(class_list));
  l->head = hd;
  l->next = tl;
  return l;
}

func_list* FuncList(func_ty_node* f, func_list* n){
  func_list* l = malloc(sizeof(func_list));
  l->head = f;
  l->next = n;
  return l;
}

vardec_list* VardecList(vardec_ty_node* f, vardec_list* n){
  vardec_list* l = malloc(sizeof(vardec_list));
  l->head = f;
  l->next = n;
  return l;
}

const char* ty_to_string(ty_node* ty){
  if(!ty) return "missing type";
  switch(ty->kind){
    case int_ty:
      return "int";
    case string_ty:
      return "string";
    case array_ty: {
      const char* inner = ty_to_string(ty->data.inner_ty);
      char* result = malloc(strlen(inner) + 3);
      strcpy(result, inner);
      strcat(result, "[]");
      return result;
    }
    default:
      return "unknown type";
  }
}

void print_ty(ty_node* ty){
  if(!ty) return;
  switch(ty->kind){
    case int_ty:
      printf("int"); return;
    case string_ty:
      printf("string"); return;
    case array_ty:
      print_ty(ty->data.inner_ty);
      printf("[]");
      return;
    default:
      printf("Unrecognized type kind %d\n", ty->kind);
  }
  return;
}

void print_lval(lval_node* lval){
  if(!lval) return;
  switch(lval->kind){
    case id_lval:
      printf("%s", lval->data.name);
      return;
    case array_lval:
      print_lval(lval->data.array_ops.base);
      printf("[");
      print_exp(lval->data.array_ops.index);
      printf("]");
      return;
    default:
      printf("Unrecognized lvalue kind %d\n", lval->kind);
  }
}

void print_binop(binop op){
  switch(op){
    case plus_op: printf("+"); return;
    case minus_op: printf("-"); return;
    case times_op: printf("*"); return;
    case div_op: printf("/"); return;
    case eq_op: printf("=="); return;
    case neq_op: printf("<>"); return;
    case lt_op: printf("+"); return;
    case le_op: printf("+"); return;
    case and_op: printf("+"); return;
    case or_op: printf("+"); return;
    default: printf("Unrecognized binop kind %d\n", op);
  }
  return;
}

void print_exp_list(exp_list* l){
  if(!l) return;
  print_exp(l->head);
  if(l->next){
    printf(", ");
    print_exp_list(l->next);
  }
  return;
}

void print_exp(exp_node* exp){
  if(!exp) return;
  switch(exp->kind){
    case int_exp:
      printf("%d", exp->data.ival);
      return;
    case string_exp:
      printf("\"%s\"", exp->data.sval);
      return;
    case lval_exp:
      print_lval(exp->data.lval);
      return;
    case binop_exp:
      print_binop(exp->data.bin_ops.op);
      printf(" (");
      print_exp(exp->data.bin_ops.e1);
      printf(") (");
      print_exp(exp->data.bin_ops.e2);
      printf(")");
      return;
    case call_exp:
      printf("%s(", exp->data.call_ops.name);
      print_exp_list(exp->data.call_ops.args);
      printf(")");
      return;
    default:
      printf("Unrecognized expression kind %d\n", exp->kind);
  }
  return;
}

void print_param_list(param_list* l){
  if(!l) return;
  print_ty(l->ty);
  printf(" %s", l->name);
  if(l->next){
    printf(", ");
    print_param_list(l->next);
  }
  return;
}

void print_stmt_list(stmt_list* l, int indent){
  if(!l) return;
  print_stmt(l->head, indent);
  print_stmt_list(l->next, indent);
  return;
}

void print_stmt(stmt_node* stmt, int indent){
  if(!stmt) return;
  for(int i = 0; i < indent; i++) printf(" ");
  switch(stmt->kind){
    case fundec_stmt:
      print_func_ty_node(stmt->data.fundec);
      return;
    case vardec_stmtty:
      print_vardec_ty_node(stmt->data.vardec);
      return;  
    case if_stmt:
      printf("if (");
      print_exp(stmt->data.if_ops.cond);
      printf(")\n");
      print_stmt(stmt->data.if_ops.then_stmt, indent + 2);
      if(stmt->data.if_ops.else_stmt){
        printf("else\n");
        print_stmt(stmt->data.if_ops.else_stmt, indent + 2);
      }
      return;
    case while_stmt:
      printf("while (");
      print_exp(stmt->data.while_ops.cond);
      printf(")\n");
      print_stmt(stmt->data.while_ops.body, indent + 2);
      return;
    case block_stmt:
      printf("{\n");
      print_stmt_list(stmt->data.body, indent + 2);
      printf("}\n");
      return;
    // case ret_stmt:
    //   printf("return ");
    //   print_exp(stmt->data.ret_exp);
    //   printf(";\n");
    //   return;
    default:
      printf("Unrecognized statement kind %d\n", stmt->kind);
  }
  return;
}

void print_func_ty_node(func_ty_node* f){
  if(!f) return;
  if(f->kind == function){
    printf("fun %s returns ", f->data.function_ops.name);
    print_ty(f->data.function_ops.retty);
    printf(" (");
    print_param_list(f->data.function_ops.params);
    printf(") {\n");
    print_stmt_list(f->data.function_ops.body, 2);
    printf("return ");
    print_exp(f->data.function_ops.ret->exp);
    printf("}\n");
  }else if(f->kind == constructor_func){
    printf("fun %s", f->data.function_ops.name);
    printf(" (");
    print_param_list(f->data.function_ops.params);
    printf(") {\n");
    print_stmt_list(f->data.function_ops.body, 2);
    printf("}\n");
  }
  return;
}

void print_func_list(func_list* l){
  if(!l) return;
  print_func_ty_node(l->head);
  print_func_list(l->next);
  return;
}

void print_vardec_ty_node(vardec_ty_node* v){
  if(!v) return;
  switch(v->kind){
    case assign_stmt:
      print_lval(v->data.assign_ops.lhs);
      printf(" = ");
      print_exp(v->data.assign_ops.rhs);
      printf(";\n");
      return;
    case vardec_stmt:
      print_ty(v->data.vardec_ops.ty);
      printf(" %s", v->data.vardec_ops.name);
      if(v->data.vardec_ops.rhs){
        printf(" = ");
        print_exp(v->data.vardec_ops.rhs);
      }
      printf(";\n");
      return;
    case arraydec_stmt:
      print_ty(v->data.arraydec_ops.ty);
      printf("[%d] %s", v->data.arraydec_ops.size, v->data.arraydec_ops.name);
      if(v->data.arraydec_ops.rhs){
        printf(" = ");
        print_exp(v->data.arraydec_ops.rhs);
      }
      printf(";\n");
      return;
    default:
      printf("Unrecognized expression kind %d\n", v->kind);
  }
  return;
}

void print_vardec_list(vardec_list* l){
  if(!l) return;
  print_vardec_ty_node(l->head);
  print_vardec_list(l->next);
  return;
}

void print_class_node(class_node* c){
  if(!c) return;
  printf("class %s {\n",c->name);
  print_vardec_list(c->vardec_list);
  print_func_list(c->func_list);
  return;
}

void print_class_list(class_list* l){
  if(!l) return;
  print_class_node(l->head);
  print_class_list(l->next);
  return;
}

void print_tree(class_list* prog){
  print_class_list(prog);
  return;
}

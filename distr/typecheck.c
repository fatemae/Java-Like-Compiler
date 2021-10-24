#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "typecheck.h"
#include "symbol.h"

S_table types;
S_table args;
S_table classes;
S_table class_fun;
S_table class_var;
S_table current;

char* sym_class="current_class";
char* sym_function="current_function";


// question 1
ty_node* string_array_array(){ 
    // replace this
    return ArrayTyNode(ArrayTyNode(StringTyNode()));
}

ty_node* get_inner_type(ty_node* t){
    // replace this
    if(t->kind == array_ty)
        return t->data.inner_ty;
    else
        return NULL;
}

ty_node* type_of_exp(exp_node* exp);
var_ty_list* getGlobalVariableOfClass(char* name);
func_ty_list* getFunctionsOfClass(char* name);

char* findKey(char* name ,param_list* l){
    // char* key=name;
    char *key = malloc(strlen(name) + 1); 
    strcpy(key, name);
    while(l!=NULL){
        int len = strlen(key);
        ty_node* ty = l->ty;
        switch(ty->kind){
            case int_ty: 
                        key[len] = 'i';
                        key[len+1] = '\0';
                        break;
            case string_ty: 
                        key[len] = 's';
                        key[len+1] = '\0';
                        break;
            case array_ty: 
                        key[len] = 'a';
                        key[len+1] = '\0';
                        break;
            default: printf("Cannot put Object Type in Constructor arguments\n"); break;
        }
        l=l->next;
    }
    return key;
}

char* findKeyForCheck(char* name ,exp_list* l){
    // char* key=name;
    char *key = malloc(strlen(name) + 1); 
    strcpy(key, name);
    while(l!=NULL){
        int len = strlen(key);
        ty_node* ty = type_of_exp(l->head);
        switch(ty->kind){
            case int_ty: 
                        key[len] = 'i';
                        key[len+1] = '\0';
                        break;
            case string_ty: 
                        key[len] = 's';
                        key[len+1] = '\0';
                        break;
            case array_ty: 
                        key[len] = 'a';
                        key[len+1] = '\0';
                        break;
            default: printf("Cannot put Object Type in Constructor arguments\n"); break;
        }
        l=l->next;
    }
    return key;
}


void record_type(char* var, ty_node* ty){
    S_enter(types, S_Symbol(var), ty);
}

ty_node* lookup_type(char* var){
    return S_look(types, S_Symbol(var));
}

ty_node* type_of_lval(lval_node* l);



int same_type(ty_node* ty1, ty_node* ty2){
    if(!ty1 || !ty2) return 0;
    if(ty1->kind != ty2->kind) return 0;
    if(ty1->kind == array_ty) return same_type(ty1->data.inner_ty, ty2->data.inner_ty);
    else return 1;
}

// Call this function to check whether two types match.
// The first argument is the type that should be there, and the second is the type that you want to check against that.
void check_types(ty_node* expected, ty_node* found){
    if(!same_type(expected, found))
        printf("Expected type %s but found type %s\n", ty_to_string(expected), ty_to_string(found));
    return;
}



ty_node* type_of_lval(lval_node* l){
    if(!l) return NULL;
    switch(l->kind){
        case id_lval: {
            ty_node* ty = lookup_type(l->data.name);
            if(!ty){
                printf("Undefined variable %s\n", l->data.name);
            }
            return ty;
        }
        case array_lval: {
            ty_node* index_ty = type_of_exp(l->data.array_ops.index);
            ty_node* base_ty = type_of_lval(l->data.array_ops.base);
            ty_node* i = IntTyNode();
            check_types(i, index_ty);
            if(!base_ty || base_ty->kind != array_ty){
                printf("Type %s is not an array type\n", ty_to_string(base_ty));
                return NULL;
            }
            else return base_ty->data.inner_ty;
        }
    }
}

void check_func_args(char* n, exp_list* arg){
    if(!n) return;
    param_list* list=S_look(args, S_Symbol(n));
    while(list!=NULL){
        if(!arg){
            printf("Number of function Arguments in method call and declaration doesn't match\n");
            return;
        }
        // print_ty(list->type);
        check_types(list->ty,type_of_exp(arg->head));
        arg=arg->next;
        list=list->next;
    }
    if(list==NULL && arg!=NULL){
        printf("Number of function Arguments in method call and declaration doesn't match\n");
    }
}

void check_constructor_arg(char* name, exp_list* l){
    if(!name) return;
    char* key=findKeyForCheck(name,l);
    // printf("key : %s\n",key);
    param_list* list=S_look(args, S_Symbol(key));
    while(list!=NULL && list->ty!=NULL){
        if(l==NULL){
            // printf("1");
            printf("No matching Constructor Found\n");
            return;
        }
        // print_ty(list->type);
        check_types(list->ty,type_of_exp(l->head));
        l=l->next;
        list=list->next;
    }
    if(list==NULL && l!=NULL)
        printf("No matching Constructor Found\n");
                
    return;
}

ty_node* getVariableTypeFromList(var_ty_list* l,char* name){
    if(!l && !name) return NULL;
    while(l){
        if(strcmp(name,l->name)==0)
            return l->ty;
        l=l->next;
    }
    return NULL;
}

ty_node* getFunctionTypeFromList(func_ty_list* l,char* name, exp_list* arg){
    if(!l && !name) return NULL;
    while(l){
        if(strcmp(name,l->name)==0){
            param_list* list=l->params;
            while(list!=NULL){
                if(!arg){
                    printf("Number of function Arguments in method call and declaration doesn't match\n");
                    return NULL;
                }
                // print_ty(list->type);
                check_types(list->ty,type_of_exp(arg->head));
                arg=arg->next;
                list=list->next;
            }
            if(list==NULL && arg!=NULL){
                printf("Number of function Arguments in method call and declaration doesn't match\n");
                return NULL;
            }
            return l->retty;
        }
        l=l->next;
    }
    return NULL;
}

ty_node* type_of_exp(exp_node* e){
    if(!e) return NULL;
    switch(e->kind){
        case int_exp: return IntTyNode();
        case string_exp: return StringTyNode();
        case lval_exp: return type_of_lval(e->data.lval);
        case call_exp: check_func_args(e->data.call_ops.name , e->data.call_ops.args);
            return lookup_type(e->data.call_ops.name);
        // question 2
        case binop_exp:{
            ty_node* e1 = type_of_exp(e->data.bin_ops.e1);
            ty_node* e2 = type_of_exp(e->data.bin_ops.e2); 
            check_types(IntTyNode(),e1);
            check_types(IntTyNode(),e2);
            return IntTyNode();
        }
        case variableofobject_exp:{
            
            ty_node* ty = lookup_type(e->data.objectvariable_ops.objectname);
            // printf(ty->data.object->data.name,"\n");
            if(!ty){
                printf("Object %s not found.\n",e->data.objectvariable_ops.objectname);
                return NULL;
            }else if (ty->data.object->kind == declaration_ty){
                printf("Object %s id not instantiated before use.\n",e->data.objectvariable_ops.objectname);
                return NULL;
            }else if (ty->data.object->kind == instantiation_ty){
                var_ty_list* l = getGlobalVariableOfClass(ty->data.object->data.class->name);
                return getVariableTypeFromList(l,e->data.objectvariable_ops.variablename);
            }else{
                printf("%s is not of object type\n",e->data.objectvariable_ops.objectname);
                return NULL;
            }

        }
        case functionofobject_exp:{
            ty_node* ty = lookup_type(e->data.objectvariable_ops.objectname);
            if(!ty){
                printf("Object %s not found.\n",e->data.objectvariable_ops.objectname);
                return NULL;
            }else if (ty->data.object->kind == declaration_ty){
                printf("Object %s id not instantiated before use.\n",e->data.objectvariable_ops.objectname);
                return NULL;
            }else if (ty->data.object->kind == instantiation_ty){
                func_ty_list* l = getFunctionsOfClass(ty->data.object->data.class->name);
                return getFunctionTypeFromList(l,e->data.objectfunction_ops.functionName,e->data.objectfunction_ops.args);
            }else{
                printf("%s is not of object type\n",e->data.objectvariable_ops.objectname);
                return NULL;
            }
        }
        default: return NULL;
    }
}

void record_params(param_list* l){
    if(!l) return;
    record_type(l->name, l->ty);
    record_params(l->next);
}

void record_arg_params(char* name ,param_list* l){
    if(!l) return;
    S_enter(args, S_Symbol(name), l);
}



void record_const_arg_params(char* name ,param_list* l){
    if(!name) return;
    param_list* temp=l;
    char* key = findKey(name,l);
    // printf("key : %s\n",key);
    S_enter(args, S_Symbol(key), l);
}

void typecheck_stmts(stmt_list* l);
void typecheck_vardecNode(vardec_ty_node* v);
void typecheck_funcNode(func_ty_node* v);

void typecheck_stmt(stmt_node* s){
    if(!s) return;
    switch(s->kind){
        case vardec_stmtty: {
            typecheck_vardecNode(s->data.vardec);
            return;
        }
        case fundec_stmt:
            typecheck_funcNode(s->data.fundec);
            return;
        case block_stmt:
            typecheck_stmts(s->data.body);
            return;
        // question 3
        case if_stmt:
            check_types(IntTyNode(), type_of_exp(s->data.if_ops.cond));
            typecheck_stmt(s->data.if_ops.then_stmt);
            typecheck_stmt(s->data.if_ops.else_stmt);
            return;
        case while_stmt:
            check_types(IntTyNode(), type_of_exp(s->data.while_ops.cond));
            typecheck_stmt(s->data.while_ops.body);
            return;
        case constructor_call:{
            char* name = S_look(current,S_Symbol(sym_function));
            // printf("%s\n",name);
            // printf("%s\n",s->data.constructor_ops.name);
            if(strcmp(name,s->data.constructor_ops.name)==0){
                check_constructor_arg(s->data.constructor_ops.name, s->data.constructor_ops.params);
            }else{
                printf("Expected an L-value to store function result.\n");
            }
            return;
        }  
        default:
            printf("Found unsupported statement type\n");
    }
}

void typecheck_stmts(stmt_list* s){
    if(!s) return;
    typecheck_stmt(s->head);
    typecheck_stmts(s->next);
    return;
}

void typecheck_vardecNode(vardec_ty_node* v){
    if(!v) return;
    switch(v->kind){
        case assign_stmt:{
            ty_node* rhs = type_of_exp(v->data.assign_ops.rhs);
            ty_node* lhs = type_of_lval(v->data.assign_ops.lhs);
            check_types(lhs, rhs);
            return;
        }
        case vardec_stmt:{
            exp_node* rhs = v->data.vardec_ops.rhs;
            if(rhs) check_types(v->data.vardec_ops.ty, type_of_exp(rhs));
            record_type(v->data.vardec_ops.name, v->data.vardec_ops.ty);
            return;
        }    
        case arraydec_stmt:{
            exp_node* rhs = v->data.vardec_ops.rhs;    
            if(rhs) check_types(v->data.vardec_ops.ty, type_of_exp(rhs));
            record_type(v->data.arraydec_ops.name, ArrayTyNode(v->data.arraydec_ops.ty));
            return;
            
        }
        case objectdec_stmt:{
            class_node* c = S_look(classes,S_Symbol(v->data.objectdec_ops.decType));
            if(!c){
                printf("Undefined class %s\n",v->data.objectdec_ops.decType);
                return;
            }
            record_type(v->data.objectdec_ops.name, ObjectTyNode(declarationTyNode(v->data.objectdec_ops.decType)));

            return;
        }
        case objectdecinstant_stmt:{
            class_node* c = S_look(classes,S_Symbol(v->data.objectdecinstant_ops.decType));
            if(!c){
                printf("Undefined class %s\n",v->data.objectdecinstant_ops.decType);
                return;
            }
            if(strcmp(v->data.objectdecinstant_ops.decType,v->data.objectdecinstant_ops.instantType)!=0){
                printf("Incompatible Types of Object declaration type %s and Instantiation type %s\n",
                v->data.objectdecinstant_ops.decType, v->data.objectdecinstant_ops.instantType);
                return;
            }
            check_constructor_arg(v->data.objectdecinstant_ops.decType,v->data.objectdecinstant_ops.arg);
            record_type(v->data.objectdecinstant_ops.name, ObjectTyNode(InstantiationTyNode(c)));
            return;
        }
        case objectinstant_stmt:{
            ty_node* ty =  S_look(types,S_Symbol(v->data.objectinstant_ops.lhs->data.name));
            if(!ty){
                printf("Object %s not declared before instatiation\n",v->data.objectinstant_ops.lhs->data.name);
                return;
            }
            class_node* c = S_look(classes,S_Symbol(v->data.objectinstant_ops.instantType));
            if(ty->kind==object_ty){
                object_ty_node* obj_ty = ty->data.object;
                if(obj_ty->kind==declaration_ty){
                    if(strcmp(obj_ty->data.name,v->data.objectinstant_ops.instantType)!=0){
                        printf("Incompatible Types of Object declaration type %s and Instantiation type %s\n",
                            obj_ty->data.name, v->data.objectinstant_ops.instantType);
                        return;
                    }else{
                        check_constructor_arg(v->data.objectinstant_ops.instantType,v->data.objectinstant_ops.arg);
                        record_type(v->data.objectinstant_ops.lhs->data.name,ObjectTyNode(InstantiationTyNode(c)));
                        return;
                    }
                }else{
                    if(strcmp(obj_ty->data.class->name,v->data.objectinstant_ops.instantType)!=0){
                        printf("Incompatible Types of Object declaration type %s and Instantiation type %s\n",
                            obj_ty->data.class->name, v->data.objectinstant_ops.instantType);
                        return;
                    }else{
                        check_constructor_arg(v->data.objectinstant_ops.instantType,v->data.objectinstant_ops.arg);
                        record_type(v->data.objectinstant_ops.lhs->data.name,ObjectTyNode(InstantiationTyNode(c)));
                        return;
                    }
                }
            }else{
                printf("Incompatible Types of Object declared and Instantiated\n");
                return;
            }
            return;

        }
        default: printf("Found unsupported Variable/ Object Declaration type\n");
    }
}

void typecheck_vardecList(vardec_list* list){
    if(!list) return;
    typecheck_vardecNode(list->head);
    typecheck_vardecList(list->next);
    return;
}

void typecheck_funcNode(func_ty_node* f){
    if(!f) return;
    switch (f->kind)
    {
        case constructor_func:{
            char* name = S_look(current,S_Symbol(sym_class));
            if(strcmp(name,f->data.constructor_ops.name)==0){
                record_type(f->data.constructor_ops.name, NULL);
                S_beginScope(types);
                S_enter(current,S_Symbol(sym_function),f->data.constructor_ops.name);
                record_params(f->data.constructor_ops.params);
                record_const_arg_params(f->data.constructor_ops.name, f->data.constructor_ops.params);
                typecheck_stmts(f->data.constructor_ops.stmt);
                S_pop(current);
                S_endScope(types);
            }else{
                printf("Return Type Expected to the Function\n");
            }
            return;
        }
        case function:{
            record_type(f->data.function_ops.name, f->data.function_ops.retty);
            S_beginScope(types);
            S_enter(current,S_Symbol(sym_function),f->data.function_ops.name);
            record_arg_params(f->data.function_ops.name, f->data.function_ops.params);
            record_params(f->data.function_ops.params);
            typecheck_stmts(f->data.function_ops.body);
            check_types(f->data.function_ops.retty,type_of_exp(f->data.function_ops.ret->exp));
            S_pop(current);
            S_endScope(types);
            return;
        }
        default: printf("Found unsupported Vardec type\n");
    }
}

void typecheck_funcList(func_list* list){
    if(!list) return;
    typecheck_funcNode(list->head);
    typecheck_funcList(list->next);
    return;
}

var_ty_list* getGlobalVariableOfClass(char* name){
    if(!name) return NULL;
    vardec_list* l = S_look(class_var,S_Symbol(name));
    if(!l) return NULL;
    var_ty_list* list; 
    var_ty_list* temp = malloc(sizeof(var_ty_list*));
    list=temp;
    while(l){
        vardec_ty_node* v = l->head;
        switch (v->kind)
        {
            case assign_stmt:{
                lval_node* lhs = v->data.assign_ops.lhs;
                temp->name=lhs->data.name;
                temp->ty=type_of_lval(lhs);
                break;
            }
            case vardec_stmt:{
                temp->name = v->data.vardec_ops.name;
                temp->ty = v->data.vardec_ops.ty;
                break;
            }
            case arraydec_stmt:{
                temp->name = v->data.arraydec_ops.name;
                temp->ty = v->data.arraydec_ops.ty;
                break;
            }
            case objectdecinstant_stmt: {
                temp->name = v->data.objectdecinstant_ops.name;
                class_node* c = S_look(classes,S_Symbol(v->data.objectdecinstant_ops.decType));
                temp->ty = ObjectTyNode(InstantiationTyNode(c));
                break;
            }
            case objectdec_stmt: {
                temp->name = v->data.objectdec_ops.name;
                temp->ty = ObjectTyNode(declarationTyNode(v->data.objectdec_ops.decType));
                break;
            }
            case objectinstant_stmt:{
                temp->name = v->data.objectdecinstant_ops.name;
                class_node* c = S_look(classes,S_Symbol(v->data.objectdecinstant_ops.decType));
                temp->ty = ObjectTyNode(InstantiationTyNode(c));
                break;
            }
            default:
                break;
        }
        var_ty_list* temp1 = malloc(sizeof(var_ty_list*));
        temp->next=temp1;
        temp=temp->next;
        l=l->next;
    }
    return list;

}

func_ty_list* getFunctionsOfClass(char* name){
    if(!name) return NULL;
    func_list* l = S_look(class_fun,S_Symbol(name));
    if(!l) return NULL;
    func_ty_list* list; 
    func_ty_list* temp = malloc(sizeof(func_ty_list*));
    list=temp;
    while(l){
        func_ty_node* f = l->head;
        switch (f->kind)
        {
            case function:{
                temp->name = f->data.function_ops.name;
                temp->retty = f->data.function_ops.retty;
                temp->params = f->data.function_ops.params;
                func_ty_list* temp1 = malloc(sizeof(func_ty_list*));
                temp->next=temp1;
                temp=temp->next;
                break;
            }
            default:
                break;
        }
        l=l->next;
    }
    return list;

}


void typecheck_class(class_node* c){
    if(!c) return;
    S_enter(current,S_Symbol(sym_class),c->name);
    S_beginClassScope(types);
    S_enter(classes, S_Symbol(c->name),c);
    typecheck_vardecList(c->vardec_list);
    S_enter(class_var,S_Symbol(c->name),c->vardec_list);
    typecheck_funcList(c->func_list);
    S_enter(class_fun,S_Symbol(c->name),c->func_list);
    S_endClassScope(types);
    S_pop(current);
    return;
}

void typecheck_classes(class_list* c){
    if(!c) return;
    typecheck_class(c->head);
    typecheck_classes(c->next);
    return;
}

void typecheck(class_list* prog){
    types = S_empty();
    args = S_empty();
    classes = S_empty();
    class_fun = S_empty();
    class_var = S_empty();
    current = S_empty();
    typecheck_classes(prog);
}

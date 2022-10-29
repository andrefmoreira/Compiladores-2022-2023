%{//Trabalho realizado por: Andre Filipe de Oliveira Moreira n. 2020239416 Joao Pedro Ventura Pinto n. 2020220907
%}

%{//yacc -d -t -v
%}
%union{
char* string1;
}



%token AND ASSIGN STAR COMMA DIV EQ GE GT LBRACE LE LPAR LSQ LT MINUS MOD NE NOT OR PLUS RBRACE RPAR RSQ SEMICOLON ARROW LSHIFT RSHIFT XOR BOOL CLASS DOTLENGTH DOUBLE ELSE IF INT PRINT PARSEINT PUBLIC RETURN STATIC STRING VOID WHILE RESERVED
%token <string1> ID INTLIT REALLIT STRLIT BOOLLIT

%nonassoc LE LT EQ GE GT NE ASSIGN SEMICOLON

%right MINUS PLUS NOT
%left STAR DIV MOD
%right LPAR
%left RPAR
%right IF
%right ELSE
%left RSHIFT LSHIFT
%left XOR
%left AND
%left OR

%%

Program: CLASS ID LBRACE Program_ex RBRACE
Program_ex: 
        | Program_ex MethodDecl
        | Program_ex FieldDecl 
        | Program_ex SEMICOLON


MethodDecl: PUBLIC STATIC MethodHeader MethodBody

FieldDecl: PUBLIC STATIC Type ID FieldDecl_ex SEMICOLON
FieldDecl_ex: 
            | FieldDecl_ex COMMA ID

Type: BOOL 
    | INT 
    | DOUBLE

MethodHeader: MethodHeader_ex2 ID LPAR MethodHeader_ex RPAR

MethodHeader_ex2:Type
                |VOID
MethodHeader_ex: 
                | FormalParams

FormalParams: Type ID FormalParams_ex
    | STRING LSQ RSQ ID
    ;
FormalParams_ex:
                |FormalParams_ex COMMA Type ID


MethodBody: LBRACE MethodBody_ex RBRACE

MethodBody_ex:
            |MethodBody_ex Statement
            |MethodBody_ex VarDecl

VarDecl: Type ID VarDecl_ex SEMICOLON

VarDecl_ex:
        |VarDecl_ex COMMA ID

Statement: LBRACE Statement_ex RBRACE
    | IF LPAR Expr RPAR Statement Statement_ex1
    | WHILE LPAR Expr RPAR Statement
    | RETURN Statement_ex2 SEMICOLON
    | Statement_ex3 SEMICOLON
    | PRINT LPAR Statement_ex5 RPAR SEMICOLON
    ;

Statement_ex:
            |Statement_ex Statement

Statement_ex1:
            |ELSE Statement

Statement_ex2:
            |Expr

Statement_ex3:
            |Statement_ex4

Statement_ex4:MethodInvocation
            |Assignment
            |ParseArgs

Statement_ex5:Expr
            |STRLIT

MethodInvocation: ID LPAR MethodInvocation_ex RPAR

MethodInvocation_ex:
                | Expr MethodInvocation_ex1

MethodInvocation_ex1:
                |MethodInvocation_ex1 COMMA Expr

Assignment: ID ASSIGN Expr

ParseArgs: PARSEINT LPAR ID LSQ Expr RSQ RPAR

Expr: Expr Expr_ex Expr
    | Expr Expr_ex1 Expr
    | Expr Expr_ex2 Expr
    | Expr_ex3 Expr
    | LPAR Expr RPAR
    | MethodInvocation | Assignment | ParseArgs
    | ID Expr_ex4
    | INTLIT | REALLIT | BOOLLIT
    ;

Expr_ex:PLUS
        |MINUS
        |STAR
        |DIV
        |MOD

Expr_ex1:AND
        |OR
        |XOR
        |LSHIFT
        |RSHIFT

Expr_ex2:EQ
        |GE
        |GT
        |LE
        |LT
        |NE

Expr_ex3:MINUS
        |NOT
        |PLUS

Expr_ex4:
        |DOTLENGTH

%%

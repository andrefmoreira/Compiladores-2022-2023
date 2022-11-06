%{//Trabalho realizado por: Andre Filipe de Oliveira Moreira n. 2020239416 Joao Pedro Ventura Pinto n. 2020220907
%}

%{//yacc -d -t -v
%}

%{
  #include <stdio.h>
  #include "tree.h"
  extern int yylex(void);
  extern void yyerror(const char*);
  extern char* yytext;
  extern int yyleng;
  int line = 1;
  int coluna = 1;    
  int yacc_print = 0;
  tnode* arvore;
  char tipo[SIZE];
  int num_statements = 0;
%}

%union{
char* str1;
tnode* node;
lnode* lnode;
}



%token AND ASSIGN STAR COMMA DIV EQ GE GT LBRACE LE LPAR LSQ LT MINUS MOD NE NOT OR PLUS RBRACE RPAR RSQ SEMICOLON ARROW LSHIFT RSHIFT XOR BOOL CLASS DOTLENGTH DOUBLE ELSE IF INT PRINT PARSEINT PUBLIC RETURN STATIC STRING VOID WHILE RESERVED
%token <str1> ID INTLIT REALLIT STRLIT BOOLLIT

%right LE LT EQ GE GT NE ASSIGN 

%left STAR DIV MOD
%right LPAR
%left RPAR
%right IF ELSE
%left MINUS PLUS
%left RSHIFT LSHIFT
%left XOR
%left AND
%left OR
%right NOT

%type <node> Program Program_ex FieldDecl_ex MethodHeader_ex MethodInvocation_aux FormalParams_ex MethodBody_ex VarDecl_ex Statement_ex Statement_aux MethodInvocation_ex  MethodDecl FieldDecl Type MethodHeader FormalParams MethodBody VarDecl Statement MethodInvocation Assignment ParseArgs Expr Expr_2

%%
Program: CLASS ID LBRACE Program_ex RBRACE                   {$$ = arvore = add_filho(add_node("Program",NULL), add_irmao(add_node("Id",$2),$4));}
        ;
Program_ex:                                                     {;}
        | Program_ex MethodDecl                                 {$$ = add_irmao($1, $2);}
        | Program_ex FieldDecl                                  {$$ = add_irmao($1, $2);}
        | Program_ex SEMICOLON                                  {$$ = $1;}
        ;       

MethodDecl: PUBLIC STATIC MethodHeader MethodBody                               {$$ = add_filho(add_node("MethodDecl",NULL), add_irmao($3,$4));}

FieldDecl: PUBLIC STATIC Type ID FieldDecl_ex SEMICOLON                         {$$ = add_irmao(add_filho(add_node("FieldDecl",NULL),add_irmao($3,add_node("Id",$4))),$5);}
        | error SEMICOLON                                                       {;}

FieldDecl_ex:                                               {;}
            | FieldDecl_ex COMMA ID                         {$$ = add_irmao(add_filho(add_node("FieldDecl",NULL),add_irmao(add_node(tipo,NULL),add_node("Id",$3))),$1);}
            ;

Type: BOOL                           {$$ = add_filho("Bool",NULL);strcpy(tipo,"Bool");}
    | INT                            {$$ = add_filho("Int",NULL);strcpy(tipo,"Int");;}
    | DOUBLE                         {$$ = add_filho("Double",NULL);strcpy(tipo,"Double");;}
    ;
MethodHeader: Type ID LPAR MethodHeader_ex RPAR                      {$$ = add_filho(add_node("MethodHeader",NULL),add_irmao($1,add_irmao(add_node("Id",$2),add_filho(add_node("MethodParams",NULL),$4))));}
             |VOID ID LPAR MethodHeader_ex RPAR                      {$$ = add_filho(add_node("MethodHeader", NULL), add_irmao(add_node("Void", NULL), add_irmao(add_node("Id", $2), add_filho(add_node("MethodParams", NULL), $4))));}

MethodHeader_ex:                                {;}
                | FormalParams                  {;}
                ;

FormalParams: Type ID FormalParams_ex           {$$ = add_irmao(add_filho(add_node("ParamDecl", NULL), add_irmao($1 , add_node("Id", $2))), $3);}
    | STRING LSQ RSQ ID                         {$$ = add_filho(add_node("ParamDecl", NULL), add_irmao(add_node("StringArray", NULL) , add_node("Id", $4)));}
    ;
FormalParams_ex:                                                       {;}
                |FormalParams_ex COMMA Type ID                         {$$ = add_irmao(add_filho(add_node("ParamDecl", NULL), add_irmao($3, add_node("Id", $4))), $1);}
                ;


MethodBody: LBRACE MethodBody_ex RBRACE                                 {$$ = add_filho(add_node("MethodBody", NULL), $2);}

MethodBody_ex:                                                          {;}
            |MethodBody_ex Statement                                    {if($2 == NULL)$$ = $1; else $$ = add_irmao($2, $1);}
            |MethodBody_ex VarDecl                                      {$$ = add_irmao($2, $1);}
            ;

VarDecl: Type ID VarDecl_ex SEMICOLON                                   {$$ = add_irmao(add_filho(add_node("VarDecl", NULL), add_irmao($1, add_node("Id", $2))), $3); strcpy(tipo, $1->tipo);}


VarDecl_ex:                                                             {;}
        |VarDecl_ex COMMA ID                                            {$$ = add_irmao(add_filho(add_node("VarDecl", NULL), add_irmao(add_node(tipo, NULL), add_node("Id", $3))), $1);}
        ;

Statement: LBRACE Statement_ex RBRACE                                   {if(num_statements >= 2)$$ = add_filho(add_node("Block", NULL), $2); else $$ = $2; num_statements = 0;}
    | IF LPAR Expr RPAR Statement                                       {if($5) $$ = add_filho(add_node("If", NULL), add_irmao($3, add_irmao($5, add_node("Block", NULL)))); 
                                                                        else $$ = add_filho(add_node("If", NULL), add_irmao($3, add_irmao(add_node("Block", NULL), add_node("Block", NULL))));}
    | IF LPAR Expr RPAR Statement ELSE Statement                        {if($5 && $7){$$ = add_filho(add_node("If", NULL), add_irmao($3, add_irmao($5, $7)));} 
                                                                        else if($5) $$ = add_filho(add_node("If", NULL), add_irmao($3, add_irmao($5, add_node("Block", NULL)))); 
                                                                        else if($7) $$ = add_filho(add_node("If", NULL), add_irmao($3, add_irmao(add_node("Block", NULL), $7))); 
                                                                        else $$ = add_filho(add_node("If", NULL), add_irmao($3, add_irmao(add_node("Block", NULL), add_node("Block", NULL))));}
    | WHILE LPAR Expr RPAR Statement                                    {if($5) $$ = add_filho(add_node("While", NULL), add_irmao($3, $5)); 
                                                                        else $$ = add_filho(add_node("While", NULL), add_irmao($3, add_node("Block", NULL)));}
    | RETURN SEMICOLON                                                  {$$ = add_node("Return", NULL);}
    | RETURN Expr SEMICOLON                                             {$$ = add_filho(add_node("Return", NULL), $2);}
    | SEMICOLON                                                         {$$ = NULL;}
    | MethodInvocation SEMICOLON                                        {$$ = $1;}
    | Assignment SEMICOLON                                              {$$ = $1;}
    | ParseArgs SEMICOLON                                               {$$ = $1;}
    | PRINT LPAR Statement_aux RPAR SEMICOLON                           {$$ = add_filho(add_node("Print", NULL), $3);}
    | error SEMICOLON                                                   {;}
    ;

Statement_ex:                                           {$$ = NULL;}
            |Statement_ex Statement                     {if($1){num_statements++;$$ = add_irmao($2, $1);} else $$ = $1;}
            ;

Statement_aux:Expr                  {$$ = $1;}
            | STRLIT                {$$ = add_node("StrLit",$1);}

MethodInvocation: ID LPAR RPAR              {$$ = add_filho(add_node("Call", NULL), add_irmao(add_node("Id", $1), NULL));}
                | ID LPAR MethodInvocation_aux RPAR              {$$ = add_filho(add_node("Call", NULL), add_irmao(add_node("Id", $1), $3));}
                | ID LPAR error RPAR                                  {$$ = NULL;}
                ;

MethodInvocation_aux: Expr MethodInvocation_ex           {$$ = add_irmao($1,$2);}
                ;

MethodInvocation_ex:                                           {$$ = NULL;}
                |MethodInvocation_ex COMMA Expr                {$$ = add_irmao($3,$1);}
                ;


Assignment: ID ASSIGN Expr                                      {$$ = add_filho(add_node("Assign", NULL), add_irmao(add_node("Id", $1), $3));}

ParseArgs: PARSEINT LPAR ID LSQ Expr RSQ RPAR                   {$$ = add_filho(add_node("ParseArgs", NULL), add_irmao(add_node("Id", $3), $5));}
          |  PARSEINT LPAR error RPAR                           {$$ = NULL;}

Expr: Expr_2 PLUS Expr_2                                          {$$ = add_filho(add_node("Add", NULL), add_irmao($1, $3));}
    | Expr_2 MINUS Expr_2                                         {$$ = add_filho(add_node("Sub", NULL), add_irmao($1, $3));}
    | Expr_2 STAR Expr_2                                          {$$ = add_filho(add_node("Mul", NULL), add_irmao($1, $3));}
    | Expr_2 DIV Expr_2                                           {$$ = add_filho(add_node("Div", NULL), add_irmao($1, $3));}
    | Expr_2 MOD Expr_2                                           {$$ = add_filho(add_node("Mod", NULL), add_irmao($1, $3));}                                
    | Expr_2 AND Expr_2                                           {$$ = add_filho(add_node("And", NULL), add_irmao($1, $3));}
    | Expr_2 OR Expr_2                                            {$$ = add_filho(add_node("Or", NULL), add_irmao($1, $3));}
    | Expr_2 XOR Expr_2                                           {$$ = add_filho(add_node("Xor", NULL), add_irmao($1, $3));}
    | Expr_2 LSHIFT Expr_2                                        {$$ = add_filho(add_node("Lshift", NULL), add_irmao($1, $3));}
    | Expr_2 RSHIFT Expr_2                                        {$$ = add_filho(add_node("Rshift", NULL), add_irmao($1, $3));}
    | Expr_2 EQ Expr_2                                            {$$ = add_filho(add_node("Eq", NULL), add_irmao($1, $3));}
    | Expr_2 GE Expr_2                                            {$$ = add_filho(add_node("Ge", NULL), add_irmao($1, $3));}
    | Expr_2 GT Expr_2                                            {$$ = add_filho(add_node("Gt", NULL), add_irmao($1, $3));}
    | Expr_2 LE Expr_2                                            {$$ = add_filho(add_node("Le", NULL), add_irmao($1, $3));}
    | Expr_2 LT Expr_2                                            {$$ = add_filho(add_node("Lt", NULL), add_irmao($1, $3));}
    | Expr_2 NE Expr_2                                            {$$ = add_filho(add_node("Ne", NULL), add_irmao($1, $3));}                                      
    | MINUS Expr_2                                                {$$ = add_filho(add_node("Minus", NULL), $2);}
    | NOT Expr_2                                                  {$$ = add_filho(add_node("Not", NULL), $2);}
    | PLUS Expr_2                                                 {$$ = add_filho(add_node("Plus", NULL), $2);}                                               
    | LPAR Expr RPAR                                              {$$ = $2;}                                                    
    | MethodInvocation                                            {$$ = $1;}
    | Assignment                                                  {$$ = $1;}
    | ParseArgs                                                   {$$ = $1;}                         
    | ID                                                          {$$ = add_node("Id",$1);}     
    | ID DOTLENGTH                                                {$$ = add_filho(add_node("Length", NULL),add_node("Id", $1));}                                                       
    | INTLIT                                                      {$$ = add_node("DecLit",  $1);}
    | REALLIT                                                     {$$ = add_node("RealLit",  $1);}
    | BOOLLIT                                                     {$$ = add_node("BoolLit",  $1);} 
    | LPAR error RPAR                                             {$$ = NULL;}                                                   
    ;

Expr_2: Expr_2 PLUS Expr_2                                        {$$ = add_filho(add_node("Add", NULL), add_irmao($1, $3));}
    | Expr_2 MINUS Expr_2                                         {$$ = add_filho(add_node("Sub", NULL), add_irmao($1, $3));}
    | Expr_2 STAR Expr_2                                          {$$ = add_filho(add_node("Mul", NULL), add_irmao($1, $3));}
    | Expr_2 DIV Expr_2                                           {$$ = add_filho(add_node("Div", NULL), add_irmao($1, $3));}
    | Expr_2 MOD Expr_2                                           {$$ = add_filho(add_node("Mod", NULL), add_irmao($1, $3));}                                
    | Expr_2 AND Expr_2                                           {$$ = add_filho(add_node("And", NULL), add_irmao($1, $3));}
    | Expr_2 OR Expr_2                                            {$$ = add_filho(add_node("Or", NULL), add_irmao($1, $3));}
    | Expr_2 XOR Expr_2                                           {$$ = add_filho(add_node("Xor", NULL), add_irmao($1, $3));}
    | Expr_2 LSHIFT Expr_2                                        {$$ = add_filho(add_node("Lshift", NULL), add_irmao($1, $3));}
    | Expr_2 RSHIFT Expr_2                                        {$$ = add_filho(add_node("Rshift", NULL), add_irmao($1, $3));}                                                             
    | Expr_2 EQ Expr_2                                            {$$ = add_filho(add_node("Eq", NULL), add_irmao($1, $3));}
    | Expr_2 GE Expr_2                                            {$$ = add_filho(add_node("Ge", NULL), add_irmao($1, $3));}
    | Expr_2 GT Expr_2                                            {$$ = add_filho(add_node("Gt", NULL), add_irmao($1, $3));}
    | Expr_2 LE Expr_2                                            {$$ = add_filho(add_node("Le", NULL), add_irmao($1, $3));}
    | Expr_2 LT Expr_2                                            {$$ = add_filho(add_node("Lt", NULL), add_irmao($1, $3));}
    | Expr_2 NE Expr_2                                            {$$ = add_filho(add_node("Ne", NULL), add_irmao($1, $3));}                                      
    | MINUS Expr_2                                            {$$ = add_filho(add_node("Minus", NULL), $2);}
    | NOT Expr_2                                              {$$ = add_filho(add_node("Not", NULL), $2);}
    | PLUS Expr_2                                             {$$ = add_filho(add_node("Plus", NULL), $2);}                                               
    | LPAR Expr RPAR                                        {$$ = $2;}                                                    
    | MethodInvocation                                        {$$ = $1;}            
    | ParseArgs                                               {$$ = $1;}                         
    | ID                                                      {$$ = add_node("Id",$1)}     
    | ID DOTLENGTH                                            {$$ = add_filho(add_node("Length",NULL),add_node("Id",$1))}                                                        
    | INTLIT                                                  {$$ = add_node("DecLit",  $1);}
    | REALLIT                                                 {$$ = add_node("RealLit",  $1);}
    | BOOLLIT                                                 {$$ = add_node("BoolLit",  $1);}  
    | LPAR error RPAR                                         {$$ = NULL;}                                      
    ;


%%

void yyerror (const char *s) { 
    printf ("Line %d, col %d: %s: %s\n",line, coluna-yyleng ,s, yytext);
}
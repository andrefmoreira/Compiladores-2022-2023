%{//Trabalho realizado por: Andre Filipe de Oliveira Moreira n. 2020239416 Joao Pedro Ventura Pinto n. 2020220907
%}

%{//yacc -d -t -v
%}

%{
  #include <stdio.h>
  #include "tree.h"
  #include <string.h>
  extern int yylex(void);
  extern void yyerror(const char*);
  extern char* yytext;
  extern int yyleng;
  int line = 1;
  int coluna = 1;    
  int yacc_print = 0;
  struct tnode* arvore;
  char tipo[SIZE];
  int num_statements = 0;
  extern int error;
%}

%union{
char* str1;
struct tnode* node;
}



%token AND ASSIGN STAR COMMA DIV EQ GE GT LBRACE LE LPAR LSQ LT MINUS MOD NE NOT OR PLUS RBRACE RPAR RSQ SEMICOLON ARROW LSHIFT RSHIFT XOR BOOL CLASS DOTLENGTH DOUBLE ELSE IF INT PRINT PARSEINT PUBLIC RETURN STATIC STRING VOID WHILE RESERVED UNARY
%token <str1> ID INTLIT REALLIT STRLIT BOOLLIT

%right ASSIGN 
%left OR
%left AND
%left XOR
%left EQ NE
%left GT GE LT LE
%left RSHIFT LSHIFT
%left PLUS MINUS
%left STAR DIV MOD
%right UNARY
%right NOT
%right RPAR
%left LPAR
%right IF ELSE

%type <node> Program Program_ex FieldDecl_ex MethodHeader_ex MethodInvocation_aux FormalParams_ex MethodBody_ex VarDecl_ex Statement_ex Statement_aux MethodInvocation_ex  MethodDecl FieldDecl Type MethodHeader FormalParams MethodBody VarDecl Statement MethodInvocation Assignment ParseArgs Expr Expr_2

%%
Program: CLASS ID LBRACE Program_ex RBRACE                   {$$ = arvore = add_filho(add_node("Program",NULL,line,coluna-yyleng), add_irmao(add_node("Id",$2,line,coluna-yyleng),$4));}
        ;
Program_ex:                                                     {$$ = NULL;}
        | MethodDecl Program_ex                                 {$$ = add_irmao($1, $2);}
        | FieldDecl Program_ex                                  {$$ = add_irmao($1, $2);}
        | SEMICOLON Program_ex                                 {$$ = $2;}
        ;       

MethodDecl: PUBLIC STATIC MethodHeader MethodBody                               {$$ = add_filho(add_node("MethodDecl",NULL,line,coluna-yyleng), add_irmao($3,$4));}

FieldDecl: PUBLIC STATIC Type ID FieldDecl_ex SEMICOLON                         {$$ = add_irmao(add_filho(add_node("FieldDecl",NULL,line,coluna-yyleng),add_irmao($3,add_node("Id",$4,line,coluna-yyleng))),$5);}
        | error SEMICOLON                                                       {$$ = NULL;}

FieldDecl_ex:                                               {$$ = NULL;}
            | COMMA ID FieldDecl_ex                        {$$ = add_irmao(add_filho(add_node("FieldDecl",NULL,line,coluna-yyleng),add_irmao(add_node(strdup(tipo),NULL,line,coluna-yyleng),add_node("Id",$2,line,coluna-yyleng))),$3);}
            ;

Type: BOOL                           {$$ = add_node("Bool",NULL,line,coluna-yyleng);strcpy(tipo,"Bool");}
    | INT                            {$$ = add_node("Int",NULL,line,coluna-yyleng);strcpy(tipo,"Int");}
    | DOUBLE                         {$$ = add_node("Double",NULL,line,coluna-yyleng);strcpy(tipo,"Double");}
    ;
MethodHeader: Type ID LPAR MethodHeader_ex RPAR                      {$$ = add_filho(add_node("MethodHeader",NULL,line,coluna-yyleng),add_irmao($1,add_irmao(add_node("Id",$2,line,coluna-yyleng),add_filho(add_node("MethodParams",NULL,line,coluna-yyleng),$4))));}
             |VOID ID LPAR MethodHeader_ex RPAR                      {$$ = add_filho(add_node("MethodHeader", NULL,line,coluna-yyleng), add_irmao(add_node("Void", NULL,line,coluna-yyleng), add_irmao(add_node("Id", $2,line,coluna-yyleng), add_filho(add_node("MethodParams", NULL,line,coluna-yyleng), $4))));}

MethodHeader_ex:                                {$$ = NULL;}
                | FormalParams                  {$$ = $1;}
                ;

FormalParams: Type ID FormalParams_ex           {$$ = add_irmao(add_filho(add_node("ParamDecl", NULL,line,coluna-yyleng), add_irmao($1 , add_node("Id", $2,line,coluna-yyleng))), $3);}
    | STRING LSQ RSQ ID                         {$$ = add_filho(add_node("ParamDecl", NULL,line,coluna-yyleng), add_irmao(add_node("StringArray", NULL,line,coluna-yyleng) , add_node("Id", $4,line,coluna-yyleng)));}
    ;
FormalParams_ex:                                                       {$$ = NULL;}
                | COMMA Type ID FormalParams_ex                        {$$ = add_irmao(add_filho(add_node("ParamDecl", NULL,line,coluna-yyleng), add_irmao($2, add_node("Id", $3,line,coluna-yyleng))), $4);}
                ;


MethodBody: LBRACE MethodBody_ex RBRACE                                 {$$ = add_filho(add_node("MethodBody", NULL,line,coluna-yyleng), $2);}

MethodBody_ex:                                                          {$$ = NULL;}
            |Statement MethodBody_ex                                     {if($1 == NULL)$$ = $2; else $$ = add_irmao($1, $2);}
            |VarDecl MethodBody_ex                                       {$$ = add_irmao($1, $2);}
            ;

VarDecl: Type ID VarDecl_ex SEMICOLON                                   {$$ = add_irmao(add_filho(add_node("VarDecl", NULL,line,coluna-yyleng), add_irmao($1, add_node("Id", $2,line,coluna-yyleng))), $3); strcpy(tipo, $1->tipo);}

VarDecl_ex:                                                             {$$ = NULL;}
        |COMMA ID VarDecl_ex                                             {$$ = add_irmao(add_filho(add_node("VarDecl", NULL,line,coluna-yyleng), add_irmao(add_node(strdup(tipo), NULL,line,coluna-yyleng), add_node("Id", $2,line,coluna-yyleng))), $3);}
        ;

Statement: LBRACE Statement_ex RBRACE                                   {if(num_statements >= 2)$$ = add_filho(add_node("Block", NULL,line,coluna-yyleng), $2); else $$ = $2; num_statements = 0;}
    | IF LPAR Expr RPAR Statement                                       {if($5) $$ = add_filho(add_node("If", NULL,line,coluna-yyleng), add_irmao($3, add_irmao($5, add_node("Block", NULL,line,coluna-yyleng)))); 
                                                                        else $$ = add_filho(add_node("If", NULL,line,coluna-yyleng), add_irmao($3, add_irmao(add_node("Block", NULL,line,coluna-yyleng), add_node("Block", NULL,line,coluna-yyleng))));}
    | IF LPAR Expr RPAR Statement ELSE Statement                        {if($5 && $7){$$ = add_filho(add_node("If", NULL,line,coluna-yyleng), add_irmao($3, add_irmao($5, $7)));} 
                                                                        else if($5) $$ = add_filho(add_node("If", NULL,line,coluna-yyleng), add_irmao($3, add_irmao($5, add_node("Block", NULL,line,coluna-yyleng)))); 
                                                                        else if($7) $$ = add_filho(add_node("If", NULL,line,coluna-yyleng), add_irmao($3, add_irmao(add_node("Block", NULL,line,coluna-yyleng), $7))); 
                                                                        else $$ = add_filho(add_node("If", NULL,line,coluna-yyleng), add_irmao($3, add_irmao(add_node("Block", NULL,line,coluna-yyleng), add_node("Block", NULL,line,coluna-yyleng))));}
    | WHILE LPAR Expr RPAR Statement                                    {if($5) $$ = add_filho(add_node("While", NULL,line,coluna-yyleng), add_irmao($3, $5)); 
                                                                        else $$ = add_filho(add_node("While", NULL,line,coluna-yyleng), add_irmao($3, add_node("Block", NULL,line,coluna-yyleng)));}
    | RETURN SEMICOLON                                                  {$$ = add_node("Return", NULL,line,coluna-yyleng);}
    | RETURN Expr SEMICOLON                                             {$$ = add_filho(add_node("Return", NULL,line,coluna-yyleng), $2);}
    | SEMICOLON                                                         {$$ = NULL;}
    | MethodInvocation SEMICOLON                                        {$$ = $1;}
    | Assignment SEMICOLON                                              {$$ = $1;}
    | ParseArgs SEMICOLON                                               {$$ = $1;}
    | PRINT LPAR Statement_aux RPAR SEMICOLON                           {$$ = add_filho(add_node("Print", NULL,line,coluna-yyleng), $3);}
    | error SEMICOLON                                                   {$$ = NULL;}
    ;

Statement_ex:                                                           {$$ = NULL;}
            |Statement Statement_ex                                     {if($1){num_statements++;$$ = add_irmao($1, $2);} else $$ = $2;}
            ;

Statement_aux:Expr                                                      {$$ = $1;}
            | STRLIT                                                    {$$ = add_node("StrLit",$1,line,coluna-yyleng);}

MethodInvocation: ID LPAR RPAR                                        {$$ = add_filho(add_node("Call", NULL,line,coluna-yyleng), add_irmao(add_node("Id", $1,line,coluna-yyleng), NULL));}
                | ID LPAR MethodInvocation_aux RPAR                   {$$ = add_filho(add_node("Call", NULL,line,coluna-yyleng), add_irmao(add_node("Id", $1,line,coluna-yyleng), $3));}
                | ID LPAR error RPAR                                  {$$ = NULL;}
                ;

MethodInvocation_aux: Expr MethodInvocation_ex                 {$$ = add_irmao($1,$2);}
                ;

MethodInvocation_ex:                                           {$$ = NULL;}
                | COMMA Expr MethodInvocation_ex               {$$ = add_irmao($2,$3);}
                ;


Assignment: ID ASSIGN Expr                                      {$$ = add_filho(add_node("Assign", NULL,line,coluna-yyleng), add_irmao(add_node("Id", $1,line,coluna-yyleng), $3));}

ParseArgs: PARSEINT LPAR ID LSQ Expr RSQ RPAR                   {$$ = add_filho(add_node("ParseArgs", NULL,line,coluna-yyleng), add_irmao(add_node("Id", $3,line,coluna-yyleng), $5));}
          |  PARSEINT LPAR error RPAR                           {$$ = NULL;}

Expr: Expr_2 PLUS Expr_2                                          {$$ = add_filho(add_node("Add", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 MINUS Expr_2                                         {$$ = add_filho(add_node("Sub", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 STAR Expr_2                                          {$$ = add_filho(add_node("Mul", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 DIV Expr_2                                           {$$ = add_filho(add_node("Div", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 MOD Expr_2                                           {$$ = add_filho(add_node("Mod", NULL,line,coluna-yyleng), add_irmao($1, $3));}                                
    | Expr_2 AND Expr_2                                           {$$ = add_filho(add_node("And", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 OR Expr_2                                            {$$ = add_filho(add_node("Or", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 XOR Expr_2                                           {$$ = add_filho(add_node("Xor", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 LSHIFT Expr_2                                        {$$ = add_filho(add_node("Lshift", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 RSHIFT Expr_2                                        {$$ = add_filho(add_node("Rshift", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 EQ Expr_2                                            {$$ = add_filho(add_node("Eq", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 GE Expr_2                                            {$$ = add_filho(add_node("Ge", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 GT Expr_2                                            {$$ = add_filho(add_node("Gt", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 LE Expr_2                                            {$$ = add_filho(add_node("Le", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 LT Expr_2                                            {$$ = add_filho(add_node("Lt", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 NE Expr_2                                            {$$ = add_filho(add_node("Ne", NULL,line,coluna-yyleng), add_irmao($1, $3));}                                      
    | MINUS Expr_2              %prec UNARY                       {$$ = add_filho(add_node("Minus", NULL,line,coluna-yyleng), $2);}
    | NOT Expr_2                %prec UNARY                       {$$ = add_filho(add_node("Not", NULL,line,coluna-yyleng), $2);}
    | PLUS Expr_2               %prec UNARY                       {$$ = add_filho(add_node("Plus", NULL,line,coluna-yyleng), $2);}                                               
    | LPAR Expr RPAR                                              {$$ = $2;}                                                    
    | MethodInvocation                                            {$$ = $1;}
    | Assignment                                                  {$$ = $1;}
    | ParseArgs                                                   {$$ = $1;}                         
    | ID                                                          {$$ = add_node("Id",$1,line,coluna-yyleng);}     
    | ID DOTLENGTH                                                {$$ = add_filho(add_node("Length", NULL,line,coluna-yyleng),add_node("Id", $1,line,coluna-yyleng));}                                                       
    | INTLIT                                                      {$$ = add_node("DecLit",  $1,line,coluna-yyleng);}
    | REALLIT                                                     {$$ = add_node("RealLit",  $1,line,coluna-yyleng);}
    | BOOLLIT                                                     {$$ = add_node("BoolLit",  $1,line,coluna-yyleng);} 
    | LPAR error RPAR                                             {$$ = NULL;}                                                   
    ;

Expr_2: Expr_2 PLUS Expr_2                                        {$$ = add_filho(add_node("Add", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 MINUS Expr_2                                         {$$ = add_filho(add_node("Sub", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 STAR Expr_2                                          {$$ = add_filho(add_node("Mul", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 DIV Expr_2                                           {$$ = add_filho(add_node("Div", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 MOD Expr_2                                           {$$ = add_filho(add_node("Mod", NULL,line,coluna-yyleng), add_irmao($1, $3));}                                
    | Expr_2 AND Expr_2                                           {$$ = add_filho(add_node("And", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 OR Expr_2                                            {$$ = add_filho(add_node("Or", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 XOR Expr_2                                           {$$ = add_filho(add_node("Xor", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 LSHIFT Expr_2                                        {$$ = add_filho(add_node("Lshift", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 RSHIFT Expr_2                                        {$$ = add_filho(add_node("Rshift", NULL,line,coluna-yyleng), add_irmao($1, $3));}                                                             
    | Expr_2 EQ Expr_2                                            {$$ = add_filho(add_node("Eq", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 GE Expr_2                                            {$$ = add_filho(add_node("Ge", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 GT Expr_2                                            {$$ = add_filho(add_node("Gt", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 LE Expr_2                                            {$$ = add_filho(add_node("Le", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 LT Expr_2                                            {$$ = add_filho(add_node("Lt", NULL,line,coluna-yyleng), add_irmao($1, $3));}
    | Expr_2 NE Expr_2                                            {$$ = add_filho(add_node("Ne", NULL,line,coluna-yyleng), add_irmao($1, $3));}                                      
    | MINUS Expr_2                %prec UNARY                     {$$ = add_filho(add_node("Minus", NULL,line,coluna-yyleng), $2);}
    | NOT Expr_2                  %prec UNARY                     {$$ = add_filho(add_node("Not", NULL,line,coluna-yyleng), $2);}
    | PLUS Expr_2                 %prec UNARY                     {$$ = add_filho(add_node("Plus", NULL,line,coluna-yyleng), $2);}                                               
    | LPAR Expr RPAR                                              {$$ = $2;}                                                    
    | MethodInvocation                                        {$$ = $1;}            
    | ParseArgs                                               {$$ = $1;}                         
    | ID                                                      {$$ = add_node("Id",$1,line,coluna-yyleng);}     
    | ID DOTLENGTH                                            {$$ = add_filho(add_node("Length",NULL,line,coluna-yyleng),add_node("Id",$1,line,coluna-yyleng));}                                                        
    | INTLIT                                                  {$$ = add_node("DecLit",  $1,line,coluna-yyleng);}
    | REALLIT                                                 {$$ = add_node("RealLit",  $1,line,coluna-yyleng);}
    | BOOLLIT                                                 {$$ = add_node("BoolLit",  $1,line,coluna-yyleng);}  
    | LPAR error RPAR                                         {$$ = NULL;}                                      
    ;


%%

void yyerror (const char *s) {
    error = 1; 
    printf ("Line %d, col %d: %s: %s\n",line, coluna-yyleng ,s, yytext);
}
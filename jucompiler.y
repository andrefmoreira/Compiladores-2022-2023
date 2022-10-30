%{//Trabalho realizado por: Andre Filipe de Oliveira Moreira n. 2020239416 Joao Pedro Ventura Pinto n. 2020220907
%}

%{//yacc -d -t -v
%}

%{
  #include <stdio.h>
  extern int yylex(void);
  extern void yyerror(const char*);
  extern char* yytext;
        
%}


%token AND ASSIGN STAR COMMA DIV EQ GE GT LBRACE LE LPAR LSQ LT MINUS MOD NE NOT OR PLUS RBRACE RPAR RSQ SEMICOLON ARROW LSHIFT RSHIFT XOR BOOL CLASS DOTLENGTH DOUBLE ELSE IF INT PRINT PARSEINT PUBLIC RETURN STATIC STRING VOID WHILE RESERVED
%token ID INTLIT REALLIT STRLIT BOOLLIT

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
Program: CLASS ID LBRACE Program_ex RBRACE                   {printf("%s\n" , $1);}
        ;
Program_ex:                                                     {;}
        | Program_ex MethodDecl                                 {$$ = $1;}
        | Program_ex FieldDecl                                  {$$ = $1;}
        | Program_ex SEMICOLON                                  {$$ = $1;}
        ;       

MethodDecl: PUBLIC STATIC MethodHeader MethodBody                               {$$ = $1;}

FieldDecl: PUBLIC STATIC Type ID FieldDecl_ex SEMICOLON                         {$$ = $1;}
FieldDecl_ex:                                               {;}
            | FieldDecl_ex COMMA ID                         {$$ = $1;}
            ;

Type: BOOL                           {$$ = $1;}
    | INT                            {$$ = $1;}
    | DOUBLE                         {$$ = $1;}
    ;
MethodHeader: Type|VOID ID LPAR MethodHeader_ex RPAR                      {$$ = $1;}

MethodHeader_ex:                                {;}
                | FormalParams                  {$$ = $1;}
                ;

FormalParams: Type ID FormalParams_ex           {$$ = $1;}
    | STRING LSQ RSQ ID                         {$$ = $1;}
    ;
FormalParams_ex:                                                       {;}
                |FormalParams_ex COMMA Type ID                         {$$ = $1;}
                ;


MethodBody: LBRACE MethodBody_ex RBRACE                                 {$$ = $1;}

MethodBody_ex:                                                          {;}
            |MethodBody_ex Statement                                    {$$ = $1;}
            |MethodBody_ex VarDecl                                      {$$ = $1;}
            ;

VarDecl: Type ID VarDecl_ex SEMICOLON                                   {$$ = $1;}


VarDecl_ex:                                                             {;}
        |VarDecl_ex COMMA ID                                            {$$ = $1;}
        ;

Statement: LBRACE Statement_ex RBRACE                                   {$$ = $1;}
    | IF LPAR Expr RPAR Statement Statement_ex1                         {$$ = $1;}
    | WHILE LPAR Expr RPAR Statement                                    {$$ = $1;}
    | RETURN Statement_ex2 SEMICOLON                                    {$$ = $1;}
    | Statement_ex3 SEMICOLON                                           {$$ = $1;}
    | PRINT LPAR Expr|STRLIT RPAR SEMICOLON                           {$$ = $1;}
    ;

Statement_ex:                                           {;}
            |Statement_ex Statement                     {$$ = $1;}
            ;
Statement_ex1:                                          {;}
            |ELSE Statement                             {$$ = $1;}
            ;
Statement_ex2:                                          {;}
            |Expr                                       {$$ = $1;}
            ;
Statement_ex3:                                          {;}
            |MethodInvocation                           {$$ = $1;}
            |Assignment                                 {$$ = $1;}
            |ParseArgs                                  {$$ = $1;}
            ;


MethodInvocation: ID LPAR MethodInvocation_ex RPAR              {$$ = $1;}
                ;
MethodInvocation_ex:                                            {;}
                | Expr MethodInvocation_ex1                     {$$ = $1;}
                ;
MethodInvocation_ex1:                                           {;}
                |MethodInvocation_ex1 COMMA Expr                {$$ = $1;}
                ;
Assignment: ID ASSIGN Expr                                      {$$ = $1;}

ParseArgs: PARSEINT LPAR ID LSQ Expr RSQ RPAR                   {$$ = $1;}

Expr: Expr PLUS|MINUS|STAR|DIV|MOD Expr                                 {$$ = $1;}
    | Expr AND|OR|XOR|LSHIFT|RSHIFT Expr                                {$$ = $1;}                              
    | Expr EQ|GE|GT|LE|LT|NE Expr                                       {$$ = $1;}
    | Expr_ex1 Expr                                               {$$ = $1;}
    | LPAR Expr RPAR                                                    {$$ = $1;}
    | MethodInvocation | Assignment | ParseArgs                         {$$ = $1;}
    | ID Expr_ex                                                        {$$ = $1;}
    | INTLIT | REALLIT | BOOLLIT                                        {$$ = $1;}
    ;

Expr_ex1:MINUS  {$$ = $1;}
        |NOT    {$$ = $1;}
        |PLUS   {$$ = $1;}
        ;

Expr_ex:                                {;}
        |DOTLENGTH                      {$$ = $1;}
        ;
%%

void yyerror (const char *s) { 
  printf ("%s: %s\n", s, yytext);
}

%{//Trabalho realizado por: Andre Filipe de Oliveira Moreira n. 2020239416 Joao Pedro Ventura Pinto n. 2020220907
%}

%{//yacc -d -t -v
%}

%union{
char* str1;

}


%{
  #include <stdio.h>
  extern int yylex(void);
  extern void yyerror(const char*);
  extern char* yytext;
        
%}


%token AND ASSIGN STAR COMMA DIV EQ GE GT LBRACE LE LPAR LSQ LT MINUS MOD NE NOT OR PLUS RBRACE RPAR RSQ SEMICOLON ARROW LSHIFT RSHIFT XOR BOOL CLASS DOTLENGTH DOUBLE ELSE IF INT PRINT PARSEINT PUBLIC RETURN STATIC STRING VOID WHILE RESERVED
%token <str1> ID INTLIT REALLIT STRLIT BOOLLIT

%nonassoc LE LT EQ GE GT NE ASSIGN

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

%type <str1> Program Program_ex FieldDecl_ex MethodHeader_ex FormalParams_ex MethodBody_ex VarDecl_ex Statement_ex Statement_ex1 Statement_ex2 MethodInvocation_ex MethodInvocation_ex1 Expr_ex MethodDecl FieldDecl Type MethodHeader FormalParams MethodBody VarDecl Statement MethodInvocation Assignment ParseArgs Expr

%%
Program: CLASS ID LBRACE Program_ex RBRACE                   {printf("%s\n" , $2);}
        ;
Program_ex:                                                     {;}
        | Program_ex MethodDecl                                 {;}
        | Program_ex FieldDecl                                  {;}
        | Program_ex SEMICOLON                                  {printf("%s\n",$2);}
        ;       

MethodDecl: PUBLIC STATIC MethodHeader MethodBody                               {;}

FieldDecl: PUBLIC STATIC Type ID FieldDecl_ex SEMICOLON                         {printf("%s\n" , $4);}

FieldDecl_ex:                                               {;}
            | FieldDecl_ex COMMA ID                         {printf("%s\n" , $3);}
            ;

Type: BOOL                           {;}
    | INT                            {;}
    | DOUBLE                         {;}
    ;
MethodHeader: Type ID LPAR MethodHeader_ex RPAR                                                  {printf("%s\n" , $2);}
             |VOID ID LPAR MethodHeader_ex RPAR                      {printf("%s\n" , $2);}

MethodHeader_ex:                                {;}
                | FormalParams                  {;}
                ;

FormalParams: Type ID FormalParams_ex           {printf("%s\n" , $2);}
    | STRING LSQ RSQ ID                         {printf("%s\n" , $4);}
    ;
FormalParams_ex:                                                       {;}
                |FormalParams_ex COMMA Type ID                         {printf("%s\n" , $4);}
                ;


MethodBody: LBRACE MethodBody_ex RBRACE                                 {;}

MethodBody_ex:                                                          {;}
            |MethodBody_ex Statement                                    {;}
            |MethodBody_ex VarDecl                                      {;}
            ;

VarDecl: Type ID VarDecl_ex SEMICOLON                                   {printf("%s\n" , $2);}


VarDecl_ex:                                                             {;}
        |VarDecl_ex COMMA ID                                            {printf("%s\n" , $3);}
        ;

Statement: LBRACE Statement_ex RBRACE                                   {;}
    | IF LPAR Expr RPAR Statement                                       {;}
    | IF LPAR Expr RPAR ELSE Statement                                  {;}
    | WHILE LPAR Expr RPAR Statement                                    {;}
    | RETURN Statement_ex1 SEMICOLON                                    {;}
    | Statement_ex2 SEMICOLON                                           {;}
    | PRINT LPAR Expr RPAR SEMICOLON                                    {;}
    | PRINT LPAR STRLIT RPAR SEMICOLON                                  {;}
    ;

Statement_ex:                                           {;}
            |Statement_ex Statement                     {;}
            ;
Statement_ex1:                                          {;}
            |Expr                                       {;}
            ;
Statement_ex2:                                          {;}
            |MethodInvocation                           {;}
            |Assignment                                 {;}
            |ParseArgs                                  {;}
            ;


MethodInvocation: ID LPAR MethodInvocation_ex RPAR              {printf("%s\n" , $1);}
                ;
MethodInvocation_ex:                                            {;}
                | Expr MethodInvocation_ex1                     {;}
                ;
MethodInvocation_ex1:                                           {;}
                |MethodInvocation_ex1 COMMA Expr                {;}
                ;
Assignment: ID ASSIGN Expr                                      {printf("%s\n" , $1);}

ParseArgs: PARSEINT LPAR ID LSQ Expr RSQ RPAR                   {printf("%s\n" , $3);}

Expr: Expr PLUS Expr                                          {;}
    | Expr MINUS Expr                                         {;}
    | Expr STAR Expr                                          {;}
    | Expr DIV Expr                                           {;}
    | Expr MOD Expr                                           {;}                                
    | Expr AND Expr                                           {;}
    | Expr OR Expr                                            {;}
    | Expr XOR Expr                                           {;}
    | Expr LSHIFT Expr                                        {;}
    | Expr RSHIFT Expr                                        {;}                                                             
    | Expr EQ Expr                                            {;}
    | Expr GE Expr                                            {;}
    | Expr GT Expr                                            {;}
    | Expr LE Expr                                            {;}
    | Expr LT Expr                                            {;}
    | Expr NE Expr                                            {;}                                      
    | MINUS Expr                                              {;}
    | NOT Expr                                                {;}
    | PLUS Expr                                               {;}                                               
    | LPAR Expr RPAR                                          {;}                                                    
    | MethodInvocation                                        {;}
    | Assignment                                              {;}
    | ParseArgs                                               {;}                         
    | ID Expr_ex                                              {printf("%s\n" , $1);}                                                       
    | INTLIT                                                  {printf("%s\n" , $1);}
    | REALLIT                                                 {printf("%s\n" , $1);}
    | BOOLLIT                                                 {printf("%s\n" , $1);}                                        
    ;

Expr_ex:                                {;}
        |DOTLENGTH                      {;}
        ;
%%

void yyerror (const char *s) { 
  printf ("%s: %s\n", s, yytext);
}

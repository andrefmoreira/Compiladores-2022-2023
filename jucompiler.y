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
  extern int yyleng;
  int line = 1;
  int coluna = 1;    
  int yacc_print = 0;   
%}


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

%type <str1> Program Program_ex FieldDecl_ex MethodHeader_ex FormalParams_ex MethodBody_ex VarDecl_ex Statement_ex MethodInvocation_ex  MethodDecl FieldDecl Type MethodHeader FormalParams MethodBody VarDecl Statement MethodInvocation Assignment ParseArgs Expr

%%
Program: CLASS ID LBRACE Program_ex RBRACE                   {if(yacc_print) printf("%s\n" , $2);}
        ;
Program_ex:                                                     {;}
        | Program_ex MethodDecl                                 {;}
        | Program_ex FieldDecl                                  {;}
        | Program_ex SEMICOLON                                  {;}
        ;       

MethodDecl: PUBLIC STATIC MethodHeader MethodBody                               {;}

FieldDecl: PUBLIC STATIC Type ID FieldDecl_ex SEMICOLON                         {if(yacc_print) printf("%s\n" , $4);}
        | error SEMICOLON                                                       {;}

FieldDecl_ex:                                               {;}
            | FieldDecl_ex COMMA ID                         {if(yacc_print) printf("%s\n" , $3);}
            ;

Type: BOOL                           {;}
    | INT                            {;}
    | DOUBLE                         {;}
    ;
MethodHeader: Type ID LPAR MethodHeader_ex RPAR                                                  {if(yacc_print) printf("%s\n" , $2);}
             |VOID ID LPAR MethodHeader_ex RPAR                      {if(yacc_print) printf("%s\n" , $2);}

MethodHeader_ex:                                {;}
                | FormalParams                  {;}
                ;

FormalParams: Type ID FormalParams_ex           {if(yacc_print) printf("%s\n" , $2);}
    | STRING LSQ RSQ ID                         {if(yacc_print) printf("%s\n" , $4);}
    ;
FormalParams_ex:                                                       {;}
                |FormalParams_ex COMMA Type ID                         {if(yacc_print) printf("%s\n" , $4);}
                ;


MethodBody: LBRACE MethodBody_ex RBRACE                                 {;}

MethodBody_ex:                                                          {;}
            |MethodBody_ex Statement                                    {;}
            |MethodBody_ex VarDecl                                      {;}
            ;

VarDecl: Type ID VarDecl_ex SEMICOLON                                   {if(yacc_print) printf("%s\n" , $2);}


VarDecl_ex:                                                             {;}
        |VarDecl_ex COMMA ID                                            {if(yacc_print) printf("%s\n" , $3);}
        ;

Statement: LBRACE Statement_ex RBRACE                                   {;}
    | IF LPAR Expr RPAR Statement                                       {;}
    | IF LPAR Expr RPAR Statement ELSE Statement                        {;}
    | WHILE LPAR Expr RPAR Statement                                    {;}
    | RETURN SEMICOLON                                                  {;}
    | RETURN Expr SEMICOLON                                             {;}
    | SEMICOLON                                                         {;}
    | MethodInvocation SEMICOLON                                        {;}
    | Assignment SEMICOLON                                              {;}
    | ParseArgs SEMICOLON                                               {;}
    | PRINT LPAR Expr RPAR SEMICOLON                                    {if(yacc_print) printf("Print\n" );}
    | PRINT LPAR STRLIT RPAR SEMICOLON                                  {if(yacc_print) printf("Print\n" );;}
    | error SEMICOLON                                                   {;}
    ;

Statement_ex:                                           {;}
            |Statement_ex Statement                     {;}
            ;

MethodInvocation: ID LPAR RPAR              {if(yacc_print) printf("%s\n" , $1);}
                | ID LPAR Expr MethodInvocation_ex RPAR              {if(yacc_print) printf("%s\n" , $1);}
                | ID LPAR error RPAR                                  {;}
                ;

MethodInvocation_ex:                                           {;}
                |MethodInvocation_ex COMMA Expr                {;}
                ;
Assignment: ID ASSIGN Expr                                      {if(yacc_print) printf("%s\n" , $1);}

ParseArgs: PARSEINT LPAR ID LSQ Expr RSQ RPAR                   {if(yacc_print) printf("%s\n" , $3);}
          |  PARSEINT LPAR error RPAR                           {;}

Expr: Expr_2 PLUS Expr_2                                          {;}
    | Expr_2 MINUS Expr_2                                         {;}
    | Expr_2 STAR Expr_2                                          {;}
    | Expr_2 DIV Expr_2                                           {;}
    | Expr_2 MOD Expr_2                                           {;}                                
    | Expr_2 AND Expr_2                                           {;}
    | Expr_2 OR Expr_2                                            {;}
    | Expr_2 XOR Expr_2                                           {;}
    | Expr_2 LSHIFT Expr_2                                        {;}
    | Expr_2 RSHIFT Expr_2                                        {;}
    | Expr_2 EQ Expr_2                                            {;}
    | Expr_2 GE Expr_2                                            {;}
    | Expr_2 GT Expr_2                                            {;}
    | Expr_2 LE Expr_2                                            {;}
    | Expr_2 LT Expr_2                                            {;}
    | Expr_2 NE Expr_2                                            {;}                                      
    | MINUS Expr_2                                              {;}
    | NOT Expr_2                                                {;}
    | PLUS Expr_2                                               {;}                                               
    | LPAR Expr RPAR                                          {;}                                                    
    | MethodInvocation                                        {;}
    | Assignment                                              {;}
    | ParseArgs                                               {;}                         
    | ID                                              {if(yacc_print) printf("%s\n" , $1);}     
    | ID DOTLENGTH                                              {if(yacc_print) printf("%s\n" , $1);}                                                       
    | INTLIT                                                  {if(yacc_print) printf("%s\n" , $1);}
    | REALLIT                                                 {if(yacc_print) printf("%s\n" , $1);}
    | BOOLLIT                                                 {if(yacc_print) printf("%s\n" , $1);} 
    | LPAR error RPAR                                         {;}                                                   
    ;

Expr_2: Expr_2 PLUS Expr_2                                          {;}
    | Expr_2 MINUS Expr_2                                         {;}
    | Expr_2 STAR Expr_2                                          {;}
    | Expr_2 DIV Expr_2                                           {;}
    | Expr_2 MOD Expr_2                                           {;}                                
    | Expr_2 AND Expr_2                                           {;}
    | Expr_2 OR Expr_2                                            {;}
    | Expr_2 XOR Expr_2                                           {;}
    | Expr_2 LSHIFT Expr_2                                        {;}
    | Expr_2 RSHIFT Expr_2                                        {;}                                                             
    | Expr_2 EQ Expr_2                                            {;}
    | Expr_2 GE Expr_2                                            {;}
    | Expr_2 GT Expr_2                                            {;}
    | Expr_2 LE Expr_2                                            {;}
    | Expr_2 LT Expr_2                                            {;}
    | Expr_2 NE Expr_2                                            {;}                                      
    | MINUS Expr_2                                            {;}
    | NOT Expr_2                                              {;}
    | PLUS Expr_2                                             {;}                                               
    | LPAR Expr RPAR                                        {;}                                                    
    | MethodInvocation                                        {;}            
    | ParseArgs                                               {;}                         
    | ID                                                      {if(yacc_print) printf("%s\n" , $1);}     
    | ID DOTLENGTH                                            {if(yacc_print) printf("%s\n" , $1);}                                                        
    | INTLIT                                                  {if(yacc_print) printf("%s\n" , $1);}
    | REALLIT                                                 {if(yacc_print) printf("%s\n" , $1);}
    | BOOLLIT                                                 {if(yacc_print) printf("%s\n" , $1);}  
    | LPAR error RPAR                                         {;}                                      
    ;


%%

void yyerror (const char *s) { 
    printf ("Line %d, col %d: %s: %s\n",line, coluna-yyleng ,s, yytext);
}

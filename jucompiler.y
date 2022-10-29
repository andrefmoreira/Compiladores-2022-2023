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
Program: CLASS ID LBRACE Program_ex RBRACE                      {printf("%s\n" , $1);}

Program_ex:                                                     {$$ = $1;}
        | Program_ex MethodDecl                                 {;}
        | Program_ex FieldDecl                                  {;}
        | Program_ex SEMICOLON                                  {;}
        ;       

MethodDecl: PUBLIC STATIC MethodHeader MethodBody                               {$$ = $1;}

FieldDecl: PUBLIC STATIC Type ID FieldDecl_ex SEMICOLON                         {$$ = $1;}
FieldDecl_ex:                                               {$$ = $1;}
            | FieldDecl_ex COMMA ID                         {;}
            ;

Type: BOOL                           {$$ = $1;}
    | INT                            {$$ = $1;}
    | DOUBLE                         {$$ = $1;}
    ;
MethodHeader: MethodHeader_ex2 ID LPAR MethodHeader_ex RPAR                      {$$ = $1;}

MethodHeader_ex2:Type                           {$$ = $1;}
                |VOID                           {$$ = $1;}
                ;
MethodHeader_ex:                                {$$ = $1;}
                | FormalParams                  {;}
                ;

FormalParams: Type ID FormalParams_ex           {$$ = $1;}
    | STRING LSQ RSQ ID                         {$$ = $1;}
    ;
FormalParams_ex:                                                       {$$ = $1;}
                |FormalParams_ex COMMA Type ID                         {;}
                ;


MethodBody: LBRACE MethodBody_ex RBRACE                                 {$$ = $1;}

MethodBody_ex:                                                          {$$ = $1;}
            |MethodBody_ex Statement                                    {;}
            |MethodBody_ex VarDecl                                      {;}
            ;

VarDecl: Type ID VarDecl_ex SEMICOLON                                   {$$ = $1;}


VarDecl_ex:                                                             {$$ = $1;}
        |VarDecl_ex COMMA ID                                            {;}
        ;

Statement: LBRACE Statement_ex RBRACE                                   {;}
    | IF LPAR Expr RPAR Statement Statement_ex1                         {;}
    | WHILE LPAR Expr RPAR Statement                                    {;}
    | RETURN Statement_ex2 SEMICOLON                                    {;}
    | Statement_ex3 SEMICOLON                                           {;}
    | PRINT LPAR Statement_ex5 RPAR SEMICOLON                           {;}
    ;

Statement_ex:                                           {$$ = $1;}
            |Statement_ex Statement                     {;}
            ;
Statement_ex1:                                          {$$ = $1;}
            |ELSE Statement                             {;}
            ;
Statement_ex2:                                          {$$ = $1;}
            |Expr                                       {;}
            ;
Statement_ex3:                                          {$$ = $1;}
            |Statement_ex4                              {;}
            ;
Statement_ex4:MethodInvocation                          {$$ = $1;}
            |Assignment                                 {$$ = $1;}
            |ParseArgs                                  {$$ = $1;}
            ;
Statement_ex5:Expr                              {$$ = $1;}
            |STRLIT                             {$$ = $1;}
            ;
MethodInvocation: ID LPAR MethodInvocation_ex RPAR              {$$ = $1;}
                ;
MethodInvocation_ex:                                            {$$ = $1;}
                | Expr MethodInvocation_ex1                     {;}
                ;
MethodInvocation_ex1:                                           {$$ = $1;}
                |MethodInvocation_ex1 COMMA Expr                {;}
                ;
Assignment: ID ASSIGN Expr                                      {$$ = $1;}

ParseArgs: PARSEINT LPAR ID LSQ Expr RSQ RPAR                   {$$ = $1;}

Expr: Expr Expr_ex Expr                                                 {$$ = $1;}
    | Expr Expr_ex1 Expr                                                {$$ = $1;}                              
    | Expr Expr_ex2 Expr                                                {$$ = $1;}
    | Expr_ex3 Expr                                                     {$$ = $1;}
    | LPAR Expr RPAR                                                    {$$ = $1;}
    | MethodInvocation | Assignment | ParseArgs                         {$$ = $1;}
    | ID Expr_ex4                                                       {$$ = $1;}
    | INTLIT | REALLIT | BOOLLIT                                        {$$ = $1;}
    ;

Expr_ex:PLUS                            {$$ = $1;}
        |MINUS                          {$$ = $1;}
        |STAR                           {$$ = $1;}
        |DIV                            {$$ = $1;}
        |MOD                            {$$ = $1;}
        ;
Expr_ex1:AND                            {$$ = $1;}
        |OR                             {$$ = $1;}
        |XOR                            {$$ = $1;}
        |LSHIFT                         {$$ = $1;}
        |RSHIFT                         {$$ = $1;}
        ;
Expr_ex2:EQ                             {$$ = $1;}
        |GE                             {$$ = $1;}
        |GT                             {$$ = $1;}
        |LE                             {$$ = $1;}
        |LT                             {$$ = $1;}
        |NE                             {$$ = $1;} 
        ;
Expr_ex3:MINUS                          {$$ = $1;}
        |NOT                            {$$ = $1;}
        |PLUS                           {$$ = $1;}
        ;
Expr_ex4:                               {$$ = $1;}
        |DOTLENGTH                      {;}
        ;
%%

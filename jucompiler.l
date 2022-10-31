%{//Trabalho realizado por: Andre Filipe de Oliveira Moreira n. 2020239416 Joao Pedro Ventura Pinto n. 2020220907
%}

number [0-9]
not_zero [1-9]
letter [a-zA-Z_$]
invalid_escape [^\n\rtnfr\\\"]
valid_escape [tnfr\\\"]

%X COMMENT1 STR ERROR


%{
#include "y.tab.h"   
int option = 0;   
int line = 1; 
int line_start = 1; 
int coluna = 1;
int coluna_start = 1;
char STR1[32654];
int i = 0;
%}


%%

"\n"|"\r\n"|"\r"    {line++;coluna = 1;}
"/*"    {BEGIN (COMMENT1);line_start = line;coluna_start=coluna;coluna+=yyleng;}
"//"[^\r\n]*        {coluna += yyleng;}
\"      {STR1[i] = '\"';i++;coluna_start = coluna;coluna+=yyleng;line_start = line;BEGIN (STR);}
[\t\f ]+   {coluna+=yyleng;}


({number}+("_"*{number}+)*"."({number}+("_"*{number}+)*)*|"."{number}+("_"*{number}+)*)(("e"|"E")("+"|"-")?({number}+("_"*{number}+)*))?   {if(option) printf("REALLIT(%s)\n",yytext);coluna+=yyleng;return REALLIT;}
{number}+("_"*{number}+)*(("e"|"E")("+"|"-")?({number}+("_"*{number}+)*))   {if(option) printf("REALLIT(%s)\n",yytext);coluna+=yyleng;yylval.str1=(char*)strdup(yytext);return REALLIT;}


"true"  {if(option) printf("BOOLLIT(%s)\n",yytext);coluna+=yyleng;yylval.str1=(char*)strdup(yytext);return BOOLLIT;}
"false" {if(option) printf("BOOLLIT(%s)\n",yytext);coluna+=yyleng;yylval.str1=(char*)strdup(yytext);return BOOLLIT;}
"&&"    {if(option) printf("AND\n");coluna+=yyleng;return AND;}


"="     {if(option) printf("ASSIGN\n");coluna+=yyleng; return ASSIGN;}
"*"     {if(option) printf("STAR\n");coluna+=yyleng;return STAR;}
","     {if(option) printf("COMMA\n");coluna+=yyleng;return COMMA;}
"/"     {if(option) printf("DIV\n");coluna+=yyleng;return DIV;}
"=="    {if(option) printf("EQ\n");coluna+=yyleng;return EQ;}
">="    {if(option) printf("GE\n");coluna+=yyleng;return GE;}
">"     {if(option) printf("GT\n");coluna+=yyleng;return GT;}
"{"     {if(option) printf("LBRACE\n");coluna+=yyleng;return LBRACE;}
"<="    {if(option) printf("LE\n");coluna+=yyleng;return LE;}
"<"     {if(option) printf("LT\n");coluna+=yyleng;return LT;}
"("     {if(option) printf("LPAR\n");coluna+=yyleng;return LPAR;}
"["     {if(option) printf("LSQ\n");coluna+=yyleng;return LSQ;}
"-"     {if(option) printf("MINUS\n");coluna+=yyleng;return MINUS;}
"%"     {if(option) printf("MOD\n");coluna+=yyleng;return MOD;}
"!="    {if(option) printf("NE\n");coluna+=yyleng;return NE;}
"!"     {if(option) printf("NOT\n");coluna+=yyleng;return NOT;}
"||"    {if(option) printf("OR\n");coluna+=yyleng;return OR;}
"+"     {if(option) printf("PLUS\n");coluna+=yyleng;return PLUS;}
"}"     {if(option) printf("RBRACE\n");coluna+=yyleng;return RBRACE;}
")"     {if(option) printf("RPAR\n");coluna+=yyleng;return RPAR;}
"]"     {if(option) printf("RSQ\n");coluna+=yyleng;return RSQ;}
";"     {if(option) printf("SEMICOLON\n");coluna+=yyleng;return SEMICOLON;}
"->"    {if(option) printf("ARROW\n");coluna+=yyleng;return ARROW;}
"<<"    {if(option) printf("LSHIFT\n");coluna+=yyleng;return LSHIFT;}
">>"    {if(option) printf("RSHIFT\n");coluna+=yyleng;return RSHIFT;}
"^"     {if(option) printf("XOR\n");coluna+=yyleng;return XOR;}

"int"   {if(option) printf("INT\n");coluna+=yyleng;return INT;}
"boolean" {if(option) printf("BOOL\n");coluna+=yyleng;return BOOL;}
"class" {if(option) printf("CLASS\n");coluna+=yyleng;return CLASS;}
".length" {if(option) printf("DOTLENGTH\n");coluna+=yyleng;return DOTLENGTH;}
"double"  {if(option) printf("DOUBLE\n");coluna+=yyleng;return DOUBLE;}
"else"  {if(option) printf("ELSE\n");coluna+=yyleng;return ELSE;}
"if"    {if(option) printf("IF\n");coluna+=yyleng;return IF;}
"System.out.print"  {if(option) printf("PRINT\n");coluna+=yyleng;return PRINT;}
"Integer.parseInt"  {if(option) printf("PARSEINT\n");coluna+=yyleng;return PARSEINT;}
"public"    {if(option) printf("PUBLIC\n");coluna+=yyleng;return PUBLIC;}
"return"    {if(option) printf("RETURN\n");coluna+=yyleng;return RETURN;}
"static"    {if(option) printf("STATIC\n");coluna+=yyleng;return STATIC;}
"STR"    {if(option) printf("STR\n");coluna+=yyleng;return STR;}
"void"  {if(option);coluna+=yyleng;return VOID;}
"while" {if(option) printf("WHILE\n");coluna+=yyleng;return WHILE;}


"++"|"--"|"null"|"Integer"|"System"|"abstract"|"continue"|"for"|"new"|"switch"|"assert"|"default"|"package"|"synchronized"|"do"|"goto"|"private"|"this"|"break"|"implements"|"protected"|"throw"|"byte"|"import"|"throws"|"case"|"enum"|"instanceof"|"transient"|"catch"|"extends"|"short"|"try"|"char"|"final"|"interface"|"finally"|"long"|"strictfp"|"volatile"|"const"|"float"|"native"|"super"|"System"|"Integer"|"null" {if(option) printf("RESERVED(%s)\n" , yytext);coluna+=yyleng;return RESERVED;}
{letter}+({number}|{letter})*   {if(option) printf("ID(%s)\n",yytext);coluna+=yyleng;yylval.str1=(char*)strdup(yytext);return ID;}
{number}|"_"*{not_zero}({number}*"_"*{number}*)*{number}  {if(option) printf("INTLIT(%s)\n" , yytext);coluna+=yyleng;yylval.str1=(char*)strdup(yytext);return INTLIT;}

<STR><<EOF>>     {printf("Line %d, col %d: unterminated STR literal\n" , line_start,coluna_start);BEGIN 0;}
<STR>\\{valid_escape}       {STR1[i] = '\\';i++;STR1[i] = yytext[1];i++ ;coluna += yyleng;}
<STR>\"                    {if(option) printf("STRLIT(%s\")\n",STR1);memset(STR1,0,strlen(STR1));i = 0;coluna+=yyleng;yylval.str1=(char*)strdup(yytext);BEGIN 0;}
<STR>"\n"|"\r\n"|"\r"     {printf("Line %d, col %d: unterminated STR literal\n" , line_start,coluna_start);coluna = 1 ;memset(STR1,0,strlen(STR1));i = 0; line++;BEGIN 0;}
<STR>\\{invalid_escape}?      {printf("Line %d, col %d: invalid escape sequence (%s)\n",line , coluna  ,yytext);coluna += yyleng;BEGIN ERROR;}
<STR>.       {STR1[i] = yytext[0];i++;coluna+=yyleng;}


<ERROR><<EOF>>     {printf("Line %d, col %d: unterminated STR literal\n" , line_start,coluna_start);memset(STR1,0,strlen(STR1));i = 0;coluna+=yyleng;BEGIN 0;}
<ERROR>\"         {memset(STR1,0,strlen(STR1));i = 0;coluna+=yyleng;BEGIN 0;}
<ERROR>\\{valid_escape}       {coluna += yyleng;}
<ERROR>"\n"|"\r\n"|"\r"     {printf("Line %d, col %d: unterminated STR literal\n" , line_start,coluna_start);coluna = 1; line++;memset(STR1,0,strlen(STR1));i = 0;BEGIN 0;}
<ERROR>\\{invalid_escape}?      {printf("Line %d, col %d: invalid escape sequence (%s)\n",line , coluna  ,yytext);coluna += yyleng;}
<ERROR>.       {coluna += yyleng;}


<COMMENT1>"\n"|"\r\n"|"\r"       {coluna = 1;line++;}
<COMMENT1><<EOF>>    {BEGIN 0;printf("Line %d, col %d: unterminated comment\n" , line_start,coluna_start);}
<COMMENT1>"*/"       {BEGIN 0;coluna+=yyleng;}
<COMMENT1>.          {coluna+=yyleng;}


.           {printf("Line %d, col %d: illegal character (%s)\n" , line , coluna , yytext);coluna+=yyleng;}

%%
int main(int argc, char *argv[])
{   
    if(argc == 2){
        if(!(strcmp(argv[1] , "-l"))){
            option = 1;
        }
    }
    yyparse();
    return 0;
}

int yywrap()
{

return 1;

}

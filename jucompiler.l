%{//Trabalho realizado por: Andre Filipe de Oliveira Moreira n. 2020239416 Joao Pedro Ventura Pinto n. 2020220907
%}

number [0-9]
not_zero [1-9]
letter [a-z]|[A-Z]|"_"|"$"
invalid_escape [^\n\rtnfr\\\"]
valid_escape [tnfr\\\"]

%X COMMENT1 STRING ERROR


%{
int option = 0;   
int line = 1; 
int line_start = 1; 
int coluna = 1;
int coluna_start = 1;
char string1[32654];
int i = 0;
%}


%%

"\n"|"\r\n"|"\r"    {line++;coluna = 1;}
"/*"    {BEGIN (COMMENT1);line_start = line;coluna_start=coluna;coluna+=yyleng;}
"//"[^\r\n]*        {coluna += yyleng;}
\"      {string1[i] = '\"';i++;coluna_start = coluna;coluna+=yyleng;line_start = line;BEGIN (STRING);}
[\t\f ]+   {coluna+=yyleng;}


({number}+("_"*{number}+)*"."({number}+("_"*{number}+)*)*|"."{number}+("_"*{number}+)*)(("e"|"E")("+"|"-")?({number}+("_"*{number}+)*))?   {printf("REALLIT(%s)\n",yytext);coluna+=yyleng;}
{number}+("_"*{number}+)*(("e"|"E")("+"|"-")?({number}+("_"*{number}+)*))   {printf("REALLIT(%s)\n",yytext);coluna+=yyleng;}


"true"  {if(option) printf("BOOLLIT(%s)\n",yytext);coluna+=yyleng;}
"false" {if(option) printf("BOOLLIT(%s)\n",yytext);coluna+=yyleng;}
"&&"    {if(option) printf("AND\n");coluna+=yyleng;}


"="     {if(option) printf("ASSIGN\n");coluna+=yyleng;}
"*"     {if(option) printf("STAR\n");coluna+=yyleng;}
","     {if(option) printf("COMMA\n");coluna+=yyleng;}
"/"     {if(option) printf("DIV\n");coluna+=yyleng;}
"=="    {if(option) printf("EQ\n");coluna+=yyleng;}
">="    {if(option) printf("GE\n");coluna+=yyleng;}
">"     {if(option) printf("GT\n");coluna+=yyleng;}
"{"     {if(option) printf("LBRACE\n");coluna+=yyleng;}
"<="    {if(option) printf("LE\n");coluna+=yyleng;}
"<"     {if(option) printf("LT\n");coluna+=yyleng;}
"("     {if(option) printf("LPAR\n");coluna+=yyleng;}
"["     {if(option) printf("LSQ\n");coluna+=yyleng;}
"-"     {if(option) printf("MINUS\n");coluna+=yyleng;}
"%"     {if(option) printf("MOD\n");coluna+=yyleng;}
"!="    {if(option) printf("NE\n");coluna+=yyleng;}
"!"     {if(option) printf("NOT\n");coluna+=yyleng;}
"||"    {if(option) printf("OR\n");coluna+=yyleng;}
"+"     {if(option) printf("PLUS\n");coluna+=yyleng;}
"}"     {if(option) printf("RBRACE\n");coluna+=yyleng;}
")"     {if(option) printf("RPAR\n");coluna+=yyleng;}
"]"     {if(option) printf("RSQ\n");coluna+=yyleng;}
";"     {if(option) printf("SEMICOLON\n");coluna+=yyleng;}
"->"    {if(option) printf("ARROW\n");coluna+=yyleng;}
"<<"    {if(option) printf("LSHIFT\n");coluna+=yyleng;}
">>"    {if(option) printf("RSHIFT\n");coluna+=yyleng;}
"^"     {if(option) printf("XOR\n");coluna+=yyleng;}

"int"   {if(option) printf("INT\n");coluna+=yyleng;}
"boolean" {if(option) printf("BOOL\n");coluna+=yyleng;}
"class" {if(option) printf("CLASS\n");coluna+=yyleng;}
".length" {if(option) printf("DOTLENGTH\n");coluna+=yyleng;}
"double"  {if(option) printf("DOUBLE\n");coluna+=yyleng;}
"else"  {if(option) printf("ELSE\n");coluna+=yyleng;}
"if"    {if(option) printf("IF\n");coluna+=yyleng;}
"System.out.print"  {if(option) printf("PRINT\n");coluna+=yyleng;}
"Integer.parseInt"  {if(option) printf("PARSEINT\n");coluna+=yyleng;}
"public"    {if(option) printf("PUBLIC\n");coluna+=yyleng;}
"return"    {if(option) printf("RETURN\n");coluna+=yyleng;}
"static"    {if(option) printf("STATIC\n");coluna+=yyleng;}
"String"    {if(option) printf("STRING\n");coluna+=yyleng;}
"void"  {if(option) printf("VOID\n");coluna+=yyleng;}
"while" {if(option) printf("WHILE\n");coluna+=yyleng;}


"++"|"--"|"null"|"Integer"|"System"|"abstract"|"continue"|"for"|"new"|"switch"|"assert"|"default"|"package"|"synchronized"|"do"|"goto"|"private"|"this"|"break"|"implements"|"protected"|"throw"|"byte"|"import"|"throws"|"case"|"enum"|"instanceof"|"transient"|"catch"|"extends"|"short"|"try"|"char"|"final"|"interface"|"finally"|"long"|"strictfp"|"volatile"|"const"|"float"|"native"|"super"|"System"|"Integer"|"null" {if(option) printf("RESERVED(%s)\n" , yytext);coluna+=yyleng;}
{letter}+({number}|{letter})*   {if(option) printf("ID(%s)\n",yytext);coluna+=yyleng;}
{number}|"_"*{not_zero}({number}*"_"*{number}*)*{number}  {if(option) printf("INTLIT(%s)\n" , yytext);coluna+=yyleng;}

<STRING><<EOF>>     {printf("Line %d, col %d: unterminated string literal\n" , line_start,coluna_start);BEGIN 0;}
<STRING>\\{valid_escape}       {string1[i] = '\\';i++;string1[i] = yytext[1];i++ ;coluna += yyleng;}
<STRING>\"                    {if(option) printf("STRLIT(%s\")\n",string1);memset(string1,0,strlen(string1));i = 0;coluna+=yyleng;BEGIN 0;}
<STRING>"\n"|"\r\n"|"\r"     {printf("Line %d, col %d: unterminated string literal\n" , line_start,coluna_start);coluna = 1 ;memset(string1,0,strlen(string1));i = 0; line++;BEGIN 0;}
<STRING>\\{invalid_escape}?      {printf("Line %d, col %d: invalid escape sequence (%s)\n",line , coluna  ,yytext);coluna += yyleng;BEGIN ERROR;}
<STRING>.       {string1[i] = yytext[0];i++;coluna+=yyleng;}


<ERROR><<EOF>>     {printf("Line %d, col %d: unterminated string literal\n" , line_start,coluna_start);memset(string1,0,strlen(string1));i = 0;coluna+=yyleng;BEGIN 0;}
<ERROR>\"         {memset(string1,0,strlen(string1));i = 0;coluna+=yyleng;BEGIN 0;}
<ERROR>\\{valid_escape}       {coluna += yyleng;}
<ERROR>"\n"|"\r\n"|"\r"     {printf("Line %d, col %d: unterminated string literal\n" , line_start,coluna_start);coluna = 1; line++;memset(string1,0,strlen(string1));i = 0;BEGIN 0;}
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
    yylex();
    return 0;
}

int yywrap()
{

return 1;

}

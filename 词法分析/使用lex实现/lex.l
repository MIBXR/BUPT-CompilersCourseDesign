%{
#include <stdio.h> 
#define LT					1
#define	LE					2
#define GT					3
#define	GE					4
#define	EQ					5
#define NE					6
 
#define INT					7
#define LONG				8
#define SHORT				9
#define FLOAT				10
#define DOUBLE				11
#define CHAR				12
#define UNSIGNED			13
#define SIGNED				14
#define CONST				15
#define VOID				16
#define STRUCT				17
#define UNION				18
#define IF					19
#define ELSE				20
#define GOTO				21
#define SWITCH				22
#define CASE				23
#define DO					24
#define WHILE				25
#define FOR					26
#define CONTINUE			27
#define BREAK				28
#define RETURN				29
#define DEFAULT				30
#define TYPRDEF				31
#define EXTERN				32
#define STATIC				33
#define MAIN				34

#define ID          		35
#define NUMBER      		36
#define SHIFT				37
#define RELOP       		38
#define LOGIC				39
#define ADDRE				40
#define ASSIN				41
#define OPERA				42
#define ANNOTA				43
#define STR					44
#define ACHAR				45
#define PRETRT				46
#define DELIMI				47
							
#define NEWLINE     		48
#define ERRORCHAR   		49

#define LK1					50
#define RK1					51
#define LK2					52
#define RK2					53
#define LK3					54
#define RK3					55
#define COM					56 /*逗号     */
#define SEM					57 /*分号     */
#define COL					58 /*冒号     */
#define DQM					59 /*双引号   */
#define SQM					60 /*单引号   */
#define LM 					61 /*左移位   */
#define RM 					62 /*右移位   */
#define AND					63 /*与       */
#define OR 					64 /*或       */
#define ADD					65 /*+        */
#define SUB					66 /*-        */
#define MUL					67 /**        */
#define DIV					68 /*/        */
#define REM					69 /*%        */
#define EQU					70 /*=        */
#define EADD				71 /*+=       */
#define ESUB				72 /*-=       */
#define EMUL				73 /**=       */
#define EDIV				74 /*/=       */
#define EREM				75 /*%=       */


int yylval;
%}
delim		[ \t \n]
ws			{delim}+
letter		[A-Za-z_]
digit		[0-9]
id			{letter}({letter}|{digit})*
number		{digit}+(\.{digit}+)?(E[+-]?{digit}+)?
annota		"/*"([^\*]|(\*)*[^\*/])*(\*)*"*/"
str			\"(\\.|[^"\\])*\"
achar		'.'
pretrt		#[^\n]*

%%
{ws}		{}
int			{return (INT);}
long		{return (LONG);}
short		{return (SHORT);}
float		{return (FLOAT);}
double		{return (DOUBLE);}
char		{return (CHAR);}
unsigned	{return (UNSIGNED);}
signed		{return (SIGNED);}
const		{return (CONST);}
void		{return (VOID);}
struct		{return (STRUCT);}
union		{return (UNION);}
if			{return (IF);}
else		{return (ELSE);}
goto		{return (GOTO);}
switch		{return (SWITCH);}
case		{return (CASE);}
do			{return (DO);}
while		{return (WHILE);}
for			{return (FOR);}
continue	{return (CONTINUE);}
break		{return (BREAK);}
return		{return (RETURN);}
default		{return (DEFAULT);}
typedef		{return (TYPRDEF);}
extern		{return (EXTERN);}
static		{return (STATIC);}
main		{return (MAIN);}

{id}		{yylval = installID (); return (ID);}
{number}	{yylval = installNum (); return (NUMBER);}
{annota}	{yylval = installAnnota (); return (ANNOTA);}
{str}		{yylval = installStr (); return (STR);}
{achar}		{yylval = installAChar (); return (ACHAR);}
{pretrt}	{yylval = installPretrt (); return (PRETRT);}
"<"			{yylval = LT; return (RELOP);}
"<="		{yylval = LE; return (RELOP);}
">"			{yylval = GT; return (RELOP);}
">="		{yylval = GE; return (RELOP);}
"=="		{yylval = EQ; return (RELOP);}
"!="		{yylval = NE; return (RELOP);}

"("			{yylval = LK1; return (DELIMI);}
")"			{yylval = RK1; return (DELIMI);}
"["			{yylval = LK2; return (DELIMI);}
"]"			{yylval = RK2; return (DELIMI);}
"{"			{yylval = LK3; return (DELIMI);}
"}"			{yylval = RK3; return (DELIMI);}
","			{yylval = COM; return (DELIMI);}
";"			{yylval = SEM; return (DELIMI);}
":"			{yylval = COL; return (DELIMI);}
"\""		{yylval = DQM; return (DELIMI);}
"\'"		{yylval = SQM; return (DELIMI);}
"<<"		{yylval = LT; return (SHIFT);}
">>"		{yylval = RM; return (SHIFT);}
"&&"		{yylval = AND; return (LOGIC);}
"||"		{yylval = OR; return (LOGIC);}
"="			{yylval = EQU; return (ASSIN);}
"+="		{yylval = EADD; return (ASSIN);}
"-="		{yylval = ESUB; return (ASSIN);}
"*="		{yylval = EMUL; return (ASSIN);}
"/="		{yylval = EDIV; return (ASSIN);}
"%="		{yylval = EREM; return (ASSIN);}
"+"			{yylval = ADD; return (OPERA);}
"-"			{yylval = SUB; return (OPERA);}
"*"			{yylval = MUL; return (OPERA);}
"/"			{yylval = DIV; return (OPERA);}
"%"			{yylval = REM; return (OPERA);}
 
.			{yylval = ERRORCHAR; return ERRORCHAR;}
 
%%
int installID () {
	return ID;
}
 
 
int installNum () {
	return NUMBER;
}


int installAnnota (){
	return ANNOTA;
}

int installStr (){
	return STR;
}

int installAChar (){
	return CHAR;
}

int installPretrt (){
	return PRETRT;
}


int yywrap (){
  return 1;
}
 
void writeout(int c){
  switch(c){
  	case ERRORCHAR:	break;
					
	case INT:		fprintf(yyout, "(INT,\"%s\") \n", yytext);break;
	case LONG:		fprintf(yyout, "(LONG,\"%s\") \n", yytext);break;
	case SHORT:		fprintf(yyout, "(SHORT,\"%s\") \n", yytext);break;
	case FLOAT:		fprintf(yyout, "(FLOAT,\"%s\") \n", yytext);break;
	case DOUBLE:	fprintf(yyout, "(DOUBLE,\"%s\") \n", yytext);break;
	case CHAR:		fprintf(yyout, "(CHAR,\"%s\") \n", yytext);break;
	case UNSIGNED:	fprintf(yyout, "(UNSIGNED,\"%s\") \n", yytext);break;
	case SIGNED:	fprintf(yyout, "(SIGNED,\"%s\") \n", yytext);break;
	case CONST:		fprintf(yyout, "(CONST,\"%s\") \n", yytext);break;
	case VOID:		fprintf(yyout, "(VOID,\"%s\") \n", yytext);break;
	case STRUCT:	fprintf(yyout, "(STRUCT,\"%s\") \n", yytext);break;
	case UNION:		fprintf(yyout, "(UNION,\"%s\") \n", yytext);break;
	case IF:		fprintf(yyout, "(IF,\"%s\") \n", yytext);break;
	case ELSE:		fprintf(yyout, "(ELSE,\"%s\") \n", yytext);break;
	case GOTO:		fprintf(yyout, "(GOTO,\"%s\") \n", yytext);break;
	case SWITCH:	fprintf(yyout, "(SWITCH,\"%s\") \n", yytext);break;
	case CASE:		fprintf(yyout, "(CASE,\"%s\") \n", yytext);break;
	case DO:		fprintf(yyout, "(DO,\"%s\") \n", yytext);break;
	case WHILE:		fprintf(yyout, "(WHILE,\"%s\") \n", yytext);break;
	case FOR:		fprintf(yyout, "(FOR,\"%s\") \n", yytext);break;
	case CONTINUE:	fprintf(yyout, "(CONTINUE,\"%s\") \n", yytext);break;
	case BREAK:		fprintf(yyout, "(BREAK,\"%s\") \n", yytext);break;
	case RETURN:	fprintf(yyout, "(RETURN,\"%s\") \n", yytext);break;
	case DEFAULT:	fprintf(yyout, "(DEFAULT,\"%s\") \n", yytext);break;
	case TYPRDEF:	fprintf(yyout, "(TYPRDEF,\"%s\") \n", yytext);break;
	case EXTERN:	fprintf(yyout, "(EXTERN,\"%s\") \n", yytext);break;
	case STATIC:	fprintf(yyout, "(STATIC,\"%s\") \n", yytext);break;
	case MAIN:		fprintf(yyout, "(MAIN,\"%s\") \n", yytext);break;
						
	case ID:		fprintf(yyout, "(ID,\"%s\") \n", yytext);break;
	case NUMBER:	fprintf(yyout, "(NUM,\"%s\") \n", yytext);break;
	case SHIFT:		fprintf(yyout, "(SHIFT,\"%s\") \n", yytext);break;
	case RELOP:		fprintf(yyout, "(RELOP,\"%s\") \n", yytext);break;  	 
	case LOGIC:		fprintf(yyout, "(LOGIC,\"%s\") \n", yytext);break;
	case ADDRE:		fprintf(yyout, "(ADDRE,\"%s\") \n", yytext);break;
	case ASSIN:		fprintf(yyout, "(ASSIN,\"%s\") \n", yytext);break;
	case OPERA:		fprintf(yyout, "(OPERA,\"%s\") \n", yytext);break;
	case ANNOTA:	fprintf(yyout, "(ANNOTA,\"%s\") \n", yytext);break;
	case STR:		fprintf(yyout, "(STR,\"%s\") \n", yytext);break;
	case ACHAR:		fprintf(yyout, "(ACHAR,\"%s\") \n", yytext);break;
	case PRETRT:	fprintf(yyout, "(PRETRT,\"%s\") \n", yytext);break;
	case DELIMI:	fprintf(yyout, "(DELIMI,\"%s\") \n", yytext);break;
	
	
    case NEWLINE:	fprintf(yyout, "\n");break;
    default:		break;
  }
  return;
}
 
int main (int argc, char ** argv){
	int c,j=0;
	if (argc>=2){
	  if ((yyin = fopen(argv[1], "r")) == NULL){
	    printf("Can't open file %s\n", argv[1]);
	    return 1;
	  }
	  if (argc>=3){
	    yyout=fopen(argv[2], "w");
	  }
	}
	while (c = yylex()){
		writeout(c);
		j++;
		if (j%5 == 0) writeout(NEWLINE);
	}
	if(argc>=2){
	  fclose(yyin);
	  if (argc>=3) fclose(yyout);
	}
	return 0;
}
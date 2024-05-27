/*Lucas Gallego Bravo Grupo 93*/
/*100429005@alumnos.uc3m.es*/

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define T_NUMBER 	1001
#define T_OPERATOR	1002		

int ParseExpression () ;		// Prototype for forward reference 

int token ;			// Here we store the current token/literal 
int old_token = -1; // Sometimes we need to check the previous token
int number ;		// The value of the number 
int old_number = 0 ;
int token_val ;		// or the arithmetic operator
int old_token_val = -1 ;
					// TO DO: Pack these variables in a struct

int line_counter = 1 ;

void update_old_token () 
{					// Sometimes we need to check the previous token
					// TO DO: Change to a more structured code
	old_token = token ;
	old_number = number ;
	old_token_val = token_val ;
}


int rd_lex ()
{
	int c ;
	
	do {
		c = getchar () ;
		if (c == '\n')
			line_counter++ ;	// info for rd_syntax_error()
	} while (c == ' ' || c == '\t') ;
	
	if (isdigit (c)) {
		ungetc (c, stdin) ;
		update_old_token () ;
		scanf ("%d", &number) ;
		token = T_NUMBER ;
		return (token) ;	// returns the Token for Number
	}

	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=') {
		update_old_token () ;
		token_val = c ;
		token = T_OPERATOR ;
		return (token) ;
	}						// returns the Token for Arithmetic Operators

	update_old_token () ;
	token = c ;
	return (token) ;		// returns a literal
}


void rd_syntax_error (int expected, int token, char *output) 
{
	fprintf (stderr, "ERROR in line %d ", line_counter) ;
	fprintf (stderr, output, expected, token) ;
	printf ("\nbye\n") ;
	exit (0) ;
}


void MatchSymbol (int expected_token)
{
	if (token != expected_token) {
		rd_syntax_error (expected_token, token, "token %d expected, but %d was read") ;
	}
    rd_lex();
}


#define ParseLParen() 	MatchSymbol ('(') ; // More concise and efficient definitions
#define ParseRParen() 	MatchSymbol (')') ; // rather than using functions
											// This is only useful for matching Literals

#define ParseEqual() 	MatchSymbol (T_OPERATOR) ; 


int ParseVariable () //V::=a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z
{
	/*Funcion que devuelve el ascii de la variable que se ha leido*/
    int ascii;
    ascii = token;
	// Este rd_lex() es necesario para el correcto funcionamiento del programa
	rd_lex();
    return ascii;
}


int ParseNumber() //N::=0|1|2|3|4|5|6|7|8|9....
{
	/*Funcion que devuelve el valor del numero leido*/
	int val = number;
	MatchSymbol (T_NUMBER) ;
	return val;

}

int ParseOperator () 
{
	/*Funcion que devuelve el operador leido*/
	// O::=+|-|*|/
	int val = token_val ;
	MatchSymbol (T_OPERATOR) ;
	return val ;
}

int ParseType ()
{ 	
	/*Se utiliza en el axioma. Para dar a la gramática T::=N|V|(E) */
	int val;
	if (token == '(') {

		ParseLParen () ;
		val = ParseExpression () ;
		ParseRParen () ;
		return val;

	} else if (token == T_NUMBER){

		val = ParseNumber () ;
        printf("%d ",val);
		return val;

	} else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z')){
		
		val = ParseVariable () ;
        printf("%c @ ",(char)val);
		return val;

	}
}

int ParseParameter ()
{
	/*Funcion que devuelve el valor de un parametro*/
	// P::=NP|VP|(E)P|lambda
	int val;
	int val2;

	if (token == ')' || token == '\n'){
		/*Este if simula el funcionamiento de lambda*/
		return 0;
	}

	else if (token == T_NUMBER){

		val = ParseNumber () ;
        printf("%d ",val);
		ParseParameter() ;

	} else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z')){
		
		
		val = ParseVariable () ;
        printf("%c @ ",(char)val);
		ParseParameter() ;

	} else {	
			ParseLParen () ;
			val = ParseExpression () ;
			ParseRParen () ;
			ParseParameter() ;
    }
}

int ParseOpExpression ()
{
	// H::=(E)P|NP|VP

	int val ;
	int val2 ;
	int operator ;

	if (token == T_NUMBER){	
		
		val = ParseNumber () ;
        printf("%d ",val);
		ParseParameter() ;	
	
	} else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z')){
		
		val = ParseVariable () ;
        printf("%c @ ",(char)val);
		ParseParameter () ;

	} else {

		ParseLParen () ;
		ParseExpression () ;
		ParseRParen () ;
		ParseParameter() ;
	}

	return val;

}

int ParseExpression () 		
{		
	/*Funcion que devuelve el valor de una expresion*/
	// E::=OH|!VT
	int val ;
    int operator;
    int variable;

    /*Este if es para la declaracion de variables*/
	if(token == T_OPERATOR && token_val == '='){  /* 61 es el ascii del simbolo '=' */
		
		ParseEqual();
		val = ParseVariable () ;
		ParseType();
		printf("dup %c ! ",(char)val);
	}

	else{
        
		/*En caso de entrar en este else, estamos en una operacion*/
        operator = ParseOperator();
        ParseOpExpression () ;
        printf("%c ",operator);
	}


    return 0;
}

int ParseAxiom () 		
{
    // A::= N|V|(E)|=VT

    int val;

	/*Si se ha introducido una asignacion de valor a una variable sin parentesis, usaremos este else if*/
	if(token == T_OPERATOR && token_val == '='){

		ParseEqual();
		val = ParseVariable () ;
		ParseType();
		printf("dup %c ! .",(char)val);
	}

	else{

		/*En caso de que se haya introducido un unico numero, usaremos este else if*/
		if (token == T_NUMBER){

				val = ParseNumber () ;
				printf("%d .",val);
		}

		/*En caso de que se haya introducido una unica variable, usaremos este else if*/
		else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z')){
			
			val = ParseVariable () ;
			printf("%c @ .",(char)val);
		}

		else{

			/* En caso de que no sea un unico numero o variable y empiece por un parentesis izquierdo vendremos aqui*/
			ParseLParen();
			ParseExpression();
			ParseRParen();
			printf(".");
		}
	}

    return 0;
}


int main (int argc, char **argv) 
{
// Usage :  drLL -s  ==> evaluate a single Input Line
//          drLL     ==> evalute multiple Input Lines until some error appears

	int flagMultiple = 1 ;
	
	// Definimos algunas variables necesarias para el interprete gforth
	printf("variable A	variable B	variable C	variable d	variable E\n");

	if (argc >= 2) {
		if (strcmp ("-s", argv [1]) == 0) {
			flagMultiple = 0 ;
		}
	}
	
	do {
		rd_lex () ;	
		if (token == -1){
			// Si llegamos al final de drLL.txt, debemos salir del archivo sin que de error
			break;
		}
		ParseAxiom () ;		
		printf("\n");
		fflush (stdout) ;
	} while (flagMultiple) ;
	
	
	printf ("\nbye\n") ;
}

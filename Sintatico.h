typedef struct {

  char token[200];
  char tipo[30];
  char valor[200];

} tabelaSintatica; 

typedef struct {

  char nome[200];
  char tipo[30][30];

} tabelaProcedimentos; 


_Bool buscarTabela(tabelaSintatica ** A,char *id, int *Pos);
void inserirTabela(tabelaSintatica ** A,char *token,char *tipo, char *valor,int *Pos,FILE *erros, int *linha);
void fator(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe,int *linha, tabelaSintatica ** Tabela, int * PosTabela, char *tipo);
void preencheTipo(tabelaSintatica ** A,char *tipo, int *inicio,int *fim);
void descobrirTipo(tabelaSintatica ** A,int * Pos,char * palavra,char * aux);
void checaTipo(char * palavra,char * aux, FILE *erros, int *linha);

void checaTipoProc(char * palavra,char * aux, FILE *erros, int *linha);
int QualProcedimento(tabelaProcedimentos **A, char *auxiliar,int *Pos);
void inserirProcedimento(tabelaProcedimentos ** A,char *token,int *Pos);
void inserirTipoProc(tabelaProcedimentos ** A, char * tipo, int inicio, int fim, int *Pos);



void expressao(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int *linha, tabelaSintatica ** Tabela, int * PosTabela, char *tipo);
void CMD(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int *linha, tabelaSintatica ** Tabela,
 tabelaProcedimentos ** Procedimentos, int * PosTabela, int * PosProc);
void Comandos(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int *linha, tabelaSintatica ** Tabela,
 tabelaProcedimentos ** Procedimentos, int * PosTabela, int * PosProc);

void ProcedimentoDC(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int *linha,tabelaSintatica ** Tabela, tabelaProcedimentos ** Procedimentos,
 int * PosTabela, int * PosProc);
void Sintatico(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int * linha);




//Procedimento fator
void fator(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int *linha, tabelaSintatica ** Tabela, int * PosTabela, char *tipo){



    //Precisa do identificador ou das outras
    if(strcmp(classe,"Identificador") == 0){
	if(!buscarTabela(Tabela,palavra, PosTabela)){fprintf(erros,"Erro Semantico: %s nao foi declarado! Linha: %d\n", palavra,*linha);}
	if(strcmp(tipo,"NULL")==0){descobrirTipo(Tabela,PosTabela,palavra,tipo);}
	else{
	    char aux[30];
	    descobrirTipo(Tabela,PosTabela,palavra,aux);
	    checaTipo(aux,tipo,erros,linha);
	}

        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }
    else if((strcmp(classe,"numInt") == 0) || (strcmp(classe,"numReal") == 0)){
	if(strcmp(tipo,"NULL")==0){descobrirTipo(Tabela,PosTabela,palavra,tipo);}
	else{
	    checaTipo(classe,tipo,erros,linha);
	}
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }
    else if(strcmp(classe,"abreP") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
	expressao(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,PosTabela,tipo);
	if(strcmp(classe,"fechaP") == 0){
	    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
	}
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de )! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0) & (strcmp(classe,"multiplicacao") != 0) &
		(strcmp(classe,"divisao") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }
    }
    else{
	fprintf(erros,"Erro Sintatico: %s Seu fator esta errado! Linha: %d\n", palavra,*linha);
       	while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
	      (strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)&(strcmp(classe,"else") != 0)){
	    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
        }
    }


}


//Procedimento expressão
void expressao(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int * linha, tabelaSintatica ** Tabela, int * PosTabela,char *tipo){

    //Pode ter ou nao o +/-, ou seja, nao precisa por erro, op un
    if((strcmp(classe,"mais") == 0) | (strcmp(classe,"menos") == 0) ){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }
    //fator
    fator(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,PosTabela,tipo);

    //pode ou nao ter, nao precisa ter erro, mais fatores
    while((strcmp(classe,"multiplicacao") == 0) | (strcmp(classe,"divisao") == 0)){//op mul
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
	fator(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,PosTabela,tipo);
    }

    //outros_termos, pode ou nao existir
    while((strcmp(classe,"mais") == 0) | (strcmp(classe,"menos") == 0) ){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
	if((strcmp(classe,"mais") == 0) | (strcmp(classe,"menos") == 0) ){//Pode ou nao ter
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    	}
	fator(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,PosTabela,tipo);

        while((strcmp(classe,"multiplicacao") == 0) | (strcmp(classe,"divisao") == 0) ){//op mul, pode ou nao ter
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
	    fator(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,PosTabela,tipo);
        }

    }

}


//Soh um CMD pode ser ativado por vez, entao if e else if pros outros
//Procedimento CMD
void CMD(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int *linha, tabelaSintatica ** Tabela,
 tabelaProcedimentos ** Procedimentos, int * PosTabela, int * PosProc ){

char aux[30];
strcpy(aux,"NULL");


//READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// read(Id1,Id2,...)
    if(strcmp(classe,"read") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);

        if(strcmp(classe,"abreP") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de ( ! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

        //Procedimento variaveis
        if(strcmp(classe,"Identificador") == 0){
	    if(!buscarTabela(Tabela,palavra, PosTabela)){fprintf(erros,"Erro Semantico: %s nao foi declarado! Linha: %d\n", palavra,*linha);}
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
 	    while(strcmp(classe,"Virgula") == 0){//mais var
                Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Virgula
 	        if(strcmp(classe,"Identificador") == 0){
		    if(!buscarTabela(Tabela,palavra, PosTabela)){fprintf(erros,"Erro Semantico: %s nao foi declarado! Linha: %d\n", palavra,*linha);}
        	    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
		}
		else{
	    	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um identificador! Linha: %d\n", palavra,*linha);
       	    	    while((strcmp(classe,"while") != 0) &(strcmp(classe,"fechaP") != 0) &(strcmp(classe,"for") != 0) &
			  (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
			  (strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){

			Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
	            }
	        }
	    }
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um identificador! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"while") != 0) &(strcmp(classe,"fechaP") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

        if(strcmp(classe,"fechaP") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um ) ! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)&(strcmp(classe,"else") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }
    }
//READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//WRITE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// write(Id1,Id2,...)
    else if(strcmp(classe,"write") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        if(strcmp(classe,"abreP") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de ( ! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

	//Procedimento variaveis
        if(strcmp(classe,"Identificador") == 0){
	    if(!buscarTabela(Tabela,palavra, PosTabela)){fprintf(erros,"Erro Semantico: %s nao foi declarado! Linha: %d\n", palavra,*linha);}
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
 	    while(strcmp(classe,"Virgula") == 0){//mais var
                Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Virgula
                if(strcmp(classe,"Identificador") == 0){
	            if(!buscarTabela(Tabela,palavra, PosTabela)){fprintf(erros,"Erro Semantico: %s nao foi declarado! Linha: %d\n", palavra,*linha);}
        	    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
		}
		else{
	    	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um identificador! Linha: %d\n", palavra,*linha);
       	    	    while((strcmp(classe,"while") != 0) &(strcmp(classe,"fechaP") != 0) &(strcmp(classe,"for") != 0) &
			  (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
			  (strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){

			Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
	            }
	        }
	    }
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um identificador! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"while") != 0) &(strcmp(classe,"fechaP") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }


        if(strcmp(classe,"fechaP") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um ) ! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)&(strcmp(classe,"else") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

    }
//WRITE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



//WHILE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// while( <condicao> ) do <cmd>
    else if(strcmp(classe,"while") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        if(strcmp(classe,"abreP") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um ( ! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		  (strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0) & (strcmp(classe,"numReal") != 0) &
		  (strcmp(classe,"numInt") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

    //Procedimento expressao
	expressao(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,PosTabela,aux);


    //relacao
        if((strcmp(classe,"Igual") == 0) | (strcmp(classe,"MaiorMenor") == 0) | (strcmp(classe,"maiorIgual") == 0) | 
		(strcmp(classe,"menorIgual") == 0) | (strcmp(classe,"Maior") == 0) | (strcmp(classe,"Menor") == 0)){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de uma relacao! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		  (strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0) & (strcmp(classe,"numReal") != 0) &
		  (strcmp(classe,"numInt") != 0)&(strcmp(classe,"else") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

    //Procedimento expressao
	expressao(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,PosTabela,aux);


        if(strcmp(classe,"fechaP") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um ) ! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"do") != 0) & (strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

        if(strcmp(classe,"do") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um do! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

	CMD(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,Procedimentos, PosTabela,PosProc);
    }
//WHILE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//IF ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// if <condicao> then <cmd> <pfalsa>
    else if(strcmp(classe,"if") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
//Procedimento condicao
    //Procedimento expressao
	expressao(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,PosTabela,aux);	
    //relacao
        if((strcmp(classe,"Igual") == 0) | (strcmp(classe,"MaiorMenor") == 0) | (strcmp(classe,"maiorIgual") == 0) | 
		(strcmp(classe,"menorIgual") == 0) | (strcmp(classe,"Maior") == 0) | (strcmp(classe,"Menor") == 0)){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de uma relacao! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		  (strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0) & (strcmp(classe,"numReal") != 0) &
		  (strcmp(classe,"numInt") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

    //Procedimento expressao
	expressao(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,PosTabela,aux);
	if(strcmp(classe,"then") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um then! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }


	CMD(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,Procedimentos,PosTabela,PosProc);
//Procedimento pfalsa, pode ou nao ter
	if(strcmp(classe,"else") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
            CMD(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,Procedimentos,PosTabela,PosProc);
        }
    }
//IF ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//ID ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// id := expressao
//ID ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// id ou id(a1;a2;...)
    else if(strcmp(classe,"Identificador") == 0){
        char auxiliar[200]; int n,k=0;
        strcpy(auxiliar,palavra);
    //Duas opcoes aqui
	if(!buscarTabela(Tabela,palavra, PosTabela)){fprintf(erros,"Erro Semantico: %s nao foi declarado! Linha: %d\n", palavra,*linha);}
	descobrirTipo(Tabela,PosTabela,palavra,aux);
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
	//expressao
        if(strcmp(classe,"DPIgual") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
//procedimento Expressao
            expressao(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,PosTabela,aux);
        }

	//lista arg, lembrar que lista arg pode ser vazio depois de Id
        else if(strcmp(classe,"abreP") == 0){
	    n = QualProcedimento(Procedimentos,auxiliar,PosProc);
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
//Procedimento argumentos
	    if(strcmp(classe,"Identificador") == 0){
		descobrirTipo(Tabela,PosTabela,palavra,auxiliar);
 	        checaTipoProc(auxiliar,(*Procedimentos)[n].tipo[k],erros,linha);

		k++;
  	        if(!buscarTabela(Tabela,palavra, PosTabela)){fprintf(erros,"Erro Semantico: %s nao foi declarado! Linha: %d\n", palavra,*linha);}
                Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
            }
	    else{
	        fprintf(erros,"Erro Sintatico: %s Precisavamos de um Identificador! Linha: %d\n", palavra,*linha);
       	        while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		      (strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0) & 
		      (strcmp(classe,"PeV") != 0) & (strcmp(classe,"fechaP") != 0)){
		          Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
                }
            }
	    while(strcmp(classe,"PeV") == 0){//Pode ou nao ter
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
  	        if(strcmp(classe,"Identificador") == 0){
		    if(strcmp(auxiliar,(*Procedimentos)[n].tipo[k])!=0){fprintf(erros,"Erro Semantico Tipos errados no procedimento! %s, Linha: %d\n", palavra,*linha);}
		    k++;
	    	    if(!buscarTabela(Tabela,palavra, PosTabela)){fprintf(erros,"Erro Semantico: %s nao foi declarado! Linha: %d\n", palavra,*linha);}
                    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
                }
	        else{
	            fprintf(erros,"Erro Sintatico: %s Precisavamos de um Identificador! Linha: %d\n", palavra,*linha);
       	            while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		          (strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0) & 
		          (strcmp(classe,"PeV") != 0) & (strcmp(classe,"fechaP") != 0)){
		              Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
                    }
                }
	    }

	    if(strcmp(classe,"fechaP") == 0){
                Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
            }
	    else{
	        fprintf(erros,"Erro Sintatico: %s Precisavamos de um ) ! Linha: %d\n", palavra,*linha);
       	        while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		      (strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)&(strcmp(classe,"else") != 0)){
		          Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
                }
            }
        }
	else if(strcmp(classe,"doisP") == 0){}//Nao da nada nesse caso, dois pontos sera consumido dps, chamada de procedimento sem argumentos
    }
//ID ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ID ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//begin ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//begin <comandos> end
    else if(strcmp(classe,"begin") == 0){
	Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        Comandos(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,Procedimentos,PosTabela,PosProc);
        if(strcmp(classe,"end") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um end! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)&(strcmp(classe,"else") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

    }
//begin ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//for ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// for a := numInt to numInt do <CMD>
    else if(strcmp(classe,"for") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        
	strcpy(aux,"numInt");
	expressao(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,PosTabela,aux);

        if(strcmp(classe,"DPIgual") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um := ! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"numInt") != 0) & (strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

	expressao(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,PosTabela,aux);

        if(strcmp(classe,"to") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um to! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"numInt") != 0) & (strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

	expressao(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,PosTabela,aux);


	//lista arg
        if(strcmp(classe,"do") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um do! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }
	CMD(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,Procedimentos,PosTabela,PosProc);
    }
    else{
	    fprintf(erros,"Nao chamou nenhum comando! Linha: %d\n",*linha);
    }
}



//Procedimento comandos
void Comandos(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int *linha, tabelaSintatica ** Tabela,
 tabelaProcedimentos ** Procedimentos, int * PosTabela, int * PosProc){

//Enquanto nao for end, vai fazendo os comandos do cmd
while((strcmp(classe,"end") != 0) & (strcmp(classe,"Fim") != 0)){
    //CMD
    CMD(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,Tabela,Procedimentos,PosTabela,PosProc);
    //consumir ponto e virgula
    if(strcmp(classe,"PeV") == 0){
	Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }
    else{
	fprintf(erros,"Erro Sintatico: %s deveria ser ; ! Linha: %d\n", palavra,*linha);
        while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){
	    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
        }
    } 
}

}


//Procedimento declaração, DC <- dc_c dc_v dc_p
void ProcedimentoDC(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe,int * linha, tabelaSintatica ** Tabela,
 tabelaProcedimentos ** Procedimentos, int * PosTabela, int * PosProc){

    int aux2,inicio=0,fim=0;
    char aux[200]= "NULL";
//printf("%s \n",Tabela[0].token);

//Procedimento dc_c, enquanto tiver dc_c vai fazendo
//Se for constante vai entrar aqui
    while(strcmp(classe,"constante") == 0){
	aux2 = 0;
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
//Tem que ser id
        if(strcmp(classe,"Identificador") == 0){
            strcpy(aux,palavra);
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
        }
        else{
	    fprintf(erros,"Erro Sintatico: %s deveria ser um identificador! Linha: %d\n", palavra,*linha);
            while((strcmp(classe,"Igual") != 0) & (strcmp(classe,"var") != 0) & (strcmp(classe,"procedure") != 0)& (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
	        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        } 	
//Igual
        if(strcmp(classe,"Igual") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome igual
        }
        else{
	    fprintf(erros,"Erro Sintatico: %s deveria ser um = ! Linha: %d\n", palavra,*linha);
            while((strcmp(classe,"menos") != 0) & (strcmp(classe,"numReal") != 0) & (strcmp(classe,"numInt") != 0)&
                  (strcmp(classe,"var") != 0) & (strcmp(classe,"procedure") != 0)& (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        } 
//Se numero real ou inteiro
//pode ou nao ser negativo (entao ter um menos na frente)
        if(strcmp(classe,"menos") == 0){
	    aux2++;
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
//tem que ser ou numero negativo ou nao, mas precisa ter um dos dois
        if((strcmp(classe,"numReal") == 0) || (strcmp(classe,"numInt") == 0)){
	    if(aux2==1){
		strcat(palavra,"-");
	        inserirTabela(Tabela,aux,classe,palavra,PosTabela,erros,linha);
	    }
	    else{
		inserirTabela(Tabela,aux,classe,palavra,PosTabela,erros,linha);
	    }
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome igual
        }
        else{
	    fprintf(erros,"Erro Sintatico: %s deveria ser um numero! Linha: %d\n", palavra,*linha);
            while((strcmp(classe,"PeV") != 0) & (strcmp(classe,"var") != 0) & (strcmp(classe,"procedure") != 0)& (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }


//PeV
        if(strcmp(classe,"PeV") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);// consome ;
        }
        else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um ;! Linha: %d\n", palavra,*linha);
            while((strcmp(classe,"var") != 0) & (strcmp(classe,"procedure") != 0)& (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

    }


//Procedimento dc_v, enquanto tiver dc_v vai fazendo
//Se for variavel entra aqui
    while(strcmp(classe,"variavel") == 0){

        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
//Procedimento variaveis
        if(strcmp(classe,"Identificador") == 0){
	    aux2 = *PosTabela;
	    inserirTabela(Tabela,palavra,classe,aux,PosTabela,erros,linha);
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
 	    while(strcmp(classe,"Virgula") == 0){//mais var
                Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Virgula
                if(strcmp(classe,"Identificador") == 0){
	            inserirTabela(Tabela,palavra,classe,aux,PosTabela,erros,linha);
        	    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
		}
                else{
	            fprintf(erros,"Erro Sintatico: %s Precisavamos de um identificador apos a virgula! Linha: %d\n", palavra,*linha);
                    while((strcmp(classe,"doisP") != 0) & (strcmp(classe,"procedure") != 0)& (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
                    }
                }
	    }

        }
        else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um identificador! Linha: %d\n", palavra,*linha);
            while((strcmp(classe,"doisP") != 0) & (strcmp(classe,"procedure") != 0)& (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

//Dois pontos
        if(strcmp(classe,"doisP") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome
        }
        else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de : ! Linha: %d\n", palavra,*linha);
            while((strcmp(classe,"real") != 0) &(strcmp(classe,"inteiro") != 0) & (strcmp(classe,"procedure") != 0)& (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }
//Procedimento tipo var
        if((strcmp(classe,"real") == 0) || (strcmp(classe,"inteiro") == 0) ){
            preencheTipo(Tabela,classe,&aux2,PosTabela);	
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome 
        }
        else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um tipo para sua variavel ! Linha: %d\n", palavra,*linha);
            while((strcmp(classe,"PeV") != 0) & (strcmp(classe,"procedure") != 0)& (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

//PeV
        if(strcmp(classe,"PeV") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);// consome 
        }
        else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de ; ! Linha: %d\n", palavra,*linha);
            while((strcmp(classe,"procedure") != 0)& (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

    }


    strcpy(aux,"Procedimento");
//Procedimento dc_p, enquanto tiver dc_p vai fazendo
//Se for procedure entra aqui
    while(strcmp(classe,"procedure") == 0){

        tabelaSintatica * Tabela2;
        int posTabela2 = 0; 
        Tabela2 = (tabelaSintatica *)malloc(sizeof(tabelaSintatica));

        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        if(strcmp(classe,"Identificador") == 0){
            strcpy(Tabela2[0].token,palavra);
	    strcpy(Tabela2[0].tipo,"Procedimento");
	    inserirProcedimento(Procedimentos,palavra, PosProc);
	    inserirTabela(Tabela,palavra,aux,palavra,PosTabela,erros,linha);
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
        }
        else{
            strcpy(Tabela2[0].token,"Procedimento1");
	    strcpy(Tabela2[0].tipo,"Procedimento");
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um identificador! Linha: %d\n", palavra,*linha);
            while((strcmp(classe,"abreP") != 0) & (strcmp(classe,"PeV") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

//Procedimentos parametros: ESSE PODE NAO EXISTIR, portanto, se for abreP entra
	if(strcmp(classe,"abreP") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Parenteses
            if(strcmp(classe,"Identificador") != 0){//Se nao for Id agr, eh erro
		    fprintf(erros,"Erro Sintatico: %s Precisavamos de um identificador! Linha: %d\n", palavra,*linha);	
                    while((strcmp(classe,"fechaP") != 0) & (strcmp(classe,"PeV") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
			Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            	    }
	    }
//Enquanto for identificador
            while(strcmp(classe,"Identificador") == 0){
		inicio = fim;
		aux2 = posTabela2;
  	        inserirTabela(&Tabela2,palavra,aux,palavra,&posTabela2,erros,linha);
	        if(fim>=29){fprintf(erros,"Erro Semantico limite de variaveis dentro do procedimentos! Linha: %d\n",*linha);break;}
                Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
 	        while(strcmp(classe,"Virgula") == 0){//mais var
                    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Virgula
                    if(strcmp(classe,"Identificador") == 0){
			fim++;
	                if(fim>=29){fprintf(erros,"Erro Semantico limite de variaveis dentro do procedimentos! Linha: %d\n",*linha);break;}
  	                inserirTabela(&Tabela2,palavra,aux,palavra,&posTabela2,erros,linha);
        	        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
		    }
        	    else{
	    	        fprintf(erros,"Erro Sintatico: %s Precisavamos de um identificador! Linha: %d\n", palavra,*linha);
                        while((strcmp(classe,"doisP") != 0) &  (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		    	    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
                  	}
            	    }

	        }	

                if(strcmp(classe,"doisP") == 0){
                    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome :
                }
                else{
		    fprintf(erros,"Erro Sintatico: %s Precisavamos de : ! Linha: %d\n", palavra,*linha);
       		    while((strcmp(classe,"real") != 0) & (strcmp(classe,"inteiro") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
			Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            	    }
        	}

                if((strcmp(classe,"real") == 0) || (strcmp(classe,"inteiro") == 0) ){
		    preencheTipo(&Tabela2,classe,&aux2,&posTabela2);
		    inserirTipoProc(Procedimentos,classe,inicio,fim,PosProc);
		    fim++;
                    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
                }

                else{
		    fprintf(erros,"Erro Sintatico: %s Precisavamos de um tipo para seu numero ! Linha: %d\n", palavra,*linha);
       		    while((strcmp(classe,"PeV") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
			Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            	    }
        	}

                if(strcmp(classe,"PeV") == 0){
                    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome :
                    if(strcmp(classe,"Identificador") != 0){//Se nao for Id agr, eh erro
		        fprintf(erros,"Erro Sintatico: %s Precisavamos de um identificador apos o ;! Linha: %d\n", palavra,*linha);	
                        while((strcmp(classe,"fechaP") != 0) & (strcmp(classe,"PeV") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
			    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            	        }
      	            }
                }
            }

            if(strcmp(classe,"fechaP") == 0){
                Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
            }
            else{
	        fprintf(erros,"Erro Sintatico: %s Precisavamos de ) ! Linha: %d\n", palavra,*linha);
                while((strcmp(classe,"PeV") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
                }
            }
        }

//PeV
        if(strcmp(classe,"PeV") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);// consome ;
        }
        else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de ; ! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"variavel") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }


//procedimento dc_loc(igual dc_v)
//Se tiver declaracao entra
        while(strcmp(classe,"variavel") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
//Procedimento variaveis
            if(strcmp(classe,"Identificador") == 0){
		aux2 = posTabela2;
  	        inserirTabela(&Tabela2,palavra,aux,palavra,&posTabela2,erros,linha);
                Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
 	        while(strcmp(classe,"Virgula") == 0){//mais var
                    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Virgula
                    if(strcmp(classe,"Identificador") == 0){
  	                inserirTabela(&Tabela2,palavra,aux,palavra,&posTabela2,erros,linha);
        	        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
		    }
                    else{
	                fprintf(erros,"Erro Sintatico: %s Precisavamos de um identificador apos a virgula! Linha: %d\n", palavra,*linha);
                        while((strcmp(classe,"doisP") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
                        }
                    }
	        }
            }
            else{
	        fprintf(erros,"Erro Sintatico: %s Precisavamos de um identificador! Linha: %d\n", palavra,*linha);
                while((strcmp(classe,"doisP") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
                }
            }


            if(strcmp(classe,"doisP") == 0){
                Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome ;
            }
            else{
	        fprintf(erros,"Erro Sintatico: %s Precisavamos de : ! Linha: %d\n", palavra,*linha);
                while((strcmp(classe,"real") != 0) &(strcmp(classe,"inteiro") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
                }
            }

            if((strcmp(classe,"real") == 0) || (strcmp(classe,"inteiro") == 0) ){
		preencheTipo(&Tabela2,classe,&aux2,&posTabela2);
                Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome igual
            }
            else{
	        fprintf(erros,"Erro Sintatico: %s Precisavamos de um tipo para seu numero! Linha: %d\n", palavra,*linha);
                while((strcmp(classe,"PeV") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
                }
            }

            if(strcmp(classe,"PeV") == 0){
                Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);// consome ;
            }
            else{
	        fprintf(erros,"Erro Sintatico: %s Precisavamos de ; ! Linha: %d\n", palavra,*linha);
                while((strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
                }
            }
        }

        if(strcmp(classe,"begin") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);// consome  begin
        }
        else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de begin! Linha: %d\n", palavra,*linha);
            while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }


//Procedimento Comandos
        Comandos(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,&Tabela2,Procedimentos,&posTabela2,PosProc);
        if(strcmp(classe,"end") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);// consome end
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de end! Linha: %d\n", palavra,*linha);
            while((strcmp(classe,"PeV") != 0) &(strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
	}
        if(strcmp(classe,"PeV") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);// consome ;
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de ; ! Linha: %d\n", palavra,*linha);
            while((strcmp(classe,"begin") != 0) & (strcmp(classe,"Fim") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
	}
    free(Tabela2);

    }

//DC pode ser vazio, mas pode ser q tenha escrito errado!

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void inserirProcedimento(tabelaProcedimentos ** A,char *token,int *Pos){

    strcpy((*A)[*Pos].nome,token);

    *Pos = *Pos +1;
    *A = (tabelaProcedimentos *)realloc(*A,(*Pos)*sizeof(tabelaProcedimentos));
    strcpy((*A)[*Pos].nome,"procedure");
    strcpy((*A)[*Pos].tipo[0],"NULL");

}

void inserirTipoProc(tabelaProcedimentos ** A, char * tipo, int inicio, int fim, int *Pos){

int i;
for(i = inicio;i<=fim;i++){
    strcpy((*A)[*Pos-1].tipo[i],tipo);
}
    strcpy((*A)[*Pos-1].tipo[fim+1],"NULL");


}


int QualProcedimento(tabelaProcedimentos **A, char *auxiliar,int *Pos){

int i=0;
while(i<=*Pos){
    if(strcmp((*A)[i].nome,auxiliar)==0){break;}
    i++;
}
return i;

}






void inserirTabela(tabelaSintatica ** A,char *token,char *tipo, char *valor,int *Pos,FILE *erros, int *linha){

    if(buscarTabela(A,token,Pos)){
	fprintf(erros,"Erro Semantico: %s ja declarado. Linha: %d\n", token,*linha);	
    }
    else{
	*Pos = *Pos+1;
	*A = (tabelaSintatica *)realloc(*A,(*Pos+1)*sizeof(tabelaSintatica));
	strcpy((*A)[*Pos].token,token);
	strcpy((*A)[*Pos].tipo,tipo);
	strcpy((*A)[*Pos].valor,valor); 
    }
}




_Bool buscarTabela(tabelaSintatica ** A,char *id, int *Pos){
    int i;

    for(i=0;i<=*Pos;i++){
	if(strcmp(id,(*A)[i].token) == 0){return(1);}
    }
    return(0);
}


void preencheTipo(tabelaSintatica ** A,char *tipo, int *inicio,int *fim){
int i;
    for(i =*inicio+1;i<=*fim;i++){
	strcpy((*A)[i].tipo,tipo);
    }
}

void descobrirTipo(tabelaSintatica ** A,int * Pos,char * palavra,char * aux){

    int i;
    for(i=0;i<=*Pos;i++){
	if(strcmp(palavra,(*A)[i].token) == 0){
	    strcpy(aux,(*A)[i].tipo);
	    break;
	}
    }

}

void checaTipo(char * palavra,char * aux, FILE *erros, int *linha){
    
    if(strcmp(aux,"numInt") == 0){strcpy(aux,"inteiro");}
    else if(strcmp(aux,"numReal") == 0){strcpy(aux,"Real");}    
    if(strcmp(palavra,"numInt") == 0){strcpy(palavra,"inteiro");}
    else if(strcmp(palavra,"numReal") == 0){strcpy(palavra,"Real");}    
     
    if(strcmp(palavra,aux)!=0){fprintf(erros,"Erro Semantico! Tipos diferem, devem ser inteiros ou reais! Linha: %d\n",*linha);}
    
}

void checaTipoProc(char * palavra,char * aux, FILE *erros, int *linha){
    
    if(strcmp(aux,"numInt")==0){strcpy(aux,"inteiro");}
    else if(strcmp(aux,"numReal")==0){strcpy(aux,"Real");}    
    if(strcmp(palavra,"numInt")==0){strcpy(palavra,"inteiro");}
    else if(strcmp(palavra,"numReal")==0){strcpy(palavra,"Real");}    
    if(strcmp(palavra,aux) != 0){fprintf(erros,"Erro Semantico Tipos errados no procedimento! %s, Linha: %d\n", palavra,*linha);}
    
}


//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void sintatico(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe,int *linha){


    tabelaSintatica * Tabela;
    tabelaProcedimentos * Procedimentos;
    int posTabela = 0, posProc =0; 
    Tabela = (tabelaSintatica *)malloc(sizeof(tabelaSintatica));
    Procedimentos = (tabelaProcedimentos *)malloc(sizeof(tabelaProcedimentos));

    strcpy(Procedimentos[0].nome,"procedure");


    //Chamar o Lexico
    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);

    //Temos a palavra e a sua classificacao
    if(strcmp(classe,"Programa") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }
    else{
	fprintf(erros,"Erro Sintatico: %s deveria ser Program! Linha: %d\n", palavra,*linha);
        while((strcmp(classe,"Identificador") != 0) & (strcmp(classe,"PeV") != 0) & (strcmp(classe,"Fim") != 0)){
	    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
        }
    } 

    if(strcmp(classe,"Identificador") == 0){
        strcpy(Tabela[0].token,palavra);
	strcpy(Tabela[0].tipo,"Programa");
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }
    else{
        strcpy(Tabela[0].token,"Programa1");
	strcpy(Tabela[0].tipo,"Program");
	fprintf(erros,"Erro Sintatico: %s deveriamos ter um identificador para o programa! Linha: %d\n", palavra,*linha);
        while((strcmp(classe,"PeV") != 0) & (strcmp(classe,"begin") != 0)& (strcmp(classe,"constante") != 0) &
		 (strcmp(classe,"variavel") != 0)& (strcmp(classe,"procedure") != 0)& (strcmp(classe,"Fim") != 0)){
	    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
        }
    }    
    if(strcmp(classe,"PeV") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }
    else{
	fprintf(erros,"Erro Sintatico: %s deveriamos ter Ponto e Virgula! Linha: %d\n", palavra,*linha);
        while((strcmp(classe,"begin") != 0)& (strcmp(classe,"constante") != 0) &
		 (strcmp(classe,"variavel") != 0)& (strcmp(classe,"procedure") != 0)& (strcmp(classe,"Fim") != 0)){
	    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
        }
    }
    
    
//Procedimento Declaracao de Variaveis
     ProcedimentoDC(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,&Tabela,&Procedimentos,&posTabela,&posProc); 

//Lembrar que DC pode ser vazio  
    if(strcmp(classe,"begin") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }
    else{
	fprintf(erros,"Erro Sintatico: %s deveriamos ter begin! Linha: %d\n", palavra,*linha);        
	while((strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){
	printf("Classe: %s, palavra: %s\n\n",classe,palavra);
	    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	

        }
    }
//Procedimento Comandos
    Comandos(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha,&Tabela,&Procedimentos,&posTabela,&posProc);

    if(strcmp(classe,"end") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha); 
    }
    else{
	fprintf(erros,"Erro Sintatico: %s deveriamos ter end! Linha: %d\n", palavra,*linha);
        while((strcmp(classe,"Ponto") != 0) & (strcmp(classe,"Fim") != 0)){
	    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
        }
    }
    

    if(strcmp(classe,"Ponto") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }
    else{
	fprintf(erros,"Erro Sintatico: %s deveriamos ter . ! Linha: %d\n", palavra,*linha);
        while((strcmp(classe,"Fim") != 0)){
	    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
        }
    }


    if(strcmp(classe,"Fim") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }
    else{
	fprintf(erros,"Esperavamos o final do programa! Linha: %d\n",*linha);
    }





    free(Tabela);
    free(Procedimentos);
}


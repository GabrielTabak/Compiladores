void fator(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe,int *linha);
void expressao(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int *linha);
void CMD(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int *linha);
void Comandos(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int *linha);
void ProcedimentoDC(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int *linha);
void Sintatico(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int * linha);



//Procedimento fator
void fator(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int *linha){

    //Precisa do identificador ou das outras
    if(strcmp(classe,"Identificador") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }
    else if((strcmp(classe,"numInt") == 0) || (strcmp(classe,"numReal") == 0)){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }
    else if(strcmp(classe,"abreP") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
	expressao(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
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
void expressao(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int * linha){

    //Pode ter ou nao o +/-, ou seja, nao precisa por erro, op un
    if((strcmp(classe,"mais") == 0) | (strcmp(classe,"menos") == 0) ){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }
    //fator
    fator(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);

    //pode ou nao ter, nao precisa ter erro, mais fatores
    while((strcmp(classe,"multiplicacao") == 0) | (strcmp(classe,"divisao") == 0)){//op mul
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
	fator(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }

    //outros_termos, pode ou nao existir
    while((strcmp(classe,"mais") == 0) | (strcmp(classe,"menos") == 0) ){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
	if((strcmp(classe,"mais") == 0) | (strcmp(classe,"menos") == 0) ){//Pode ou nao ter
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    	}
	fator(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        while((strcmp(classe,"multiplicacao") == 0) | (strcmp(classe,"divisao") == 0) ){//op mul, pode ou nao ter
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
	    fator(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }

    }

}


//Soh um CMD pode ser ativado por vez, entao if e else if pros outros
//Procedimento CMD
void CMD(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe, int *linha){

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
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
 	    while(strcmp(classe,"Virgula") == 0){//mais var
                Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Virgula
                if(strcmp(classe,"Identificador") == 0){
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
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
 	    while(strcmp(classe,"Virgula") == 0){//mais var
                Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Virgula
                if(strcmp(classe,"Identificador") == 0){
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
	expressao(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);


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
	expressao(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);


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

	CMD(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }
//WHILE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//IF ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// if <condicao> then <cmd> <pfalsa>
    else if(strcmp(classe,"if") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
//Procedimento condicao
    //Procedimento expressao
	expressao(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);

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
	expressao(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);

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

	CMD(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
//Procedimento pfalsa, pode ou nao ter
	if(strcmp(classe,"else") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
            CMD(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
    }
//IF ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//ID ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// id := expressao
//ID ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// id ou id(a1;a2;...)
    else if(strcmp(classe,"Identificador") == 0){
    //Duas opcoes aqui
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
	//expressao
        if(strcmp(classe,"DPIgual") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
//procedimento Expressao
            expressao(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }

	//lista arg, lembrar que lista arg pode ser vazio depois de Id
        else if(strcmp(classe,"abreP") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
//Procedimento argumentos
	    if(strcmp(classe,"Identificador") == 0){
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
        Comandos(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
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

        if(strcmp(classe,"Identificador") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um Identificador! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"DPIgual") != 0) & (strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

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

        if(strcmp(classe,"numInt") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um numero inteiro positivo! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"to") != 0) & (strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }

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

        if(strcmp(classe,"numInt") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
	else{
	    fprintf(erros,"Erro Sintatico: %s Precisavamos de um numero inteiro positivo! Linha: %d\n", palavra,*linha);
       	    while((strcmp(classe,"do") != 0) & (strcmp(classe,"while") != 0) &(strcmp(classe,"for") != 0) & (strcmp(classe,"end") != 0) & (strcmp(classe,"read") != 0) &(strcmp(classe,"write") != 0) &
		(strcmp(classe,"Fim") != 0) &(strcmp(classe,"if") != 0) & (strcmp(classe,"begin") != 0) & (strcmp(classe,"Identificador") != 0)){
		Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);	
            }
        }
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
	CMD(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }
    else{
	    fprintf(erros,"Nao chamou nenhum comando! Linha: %d\n",*linha);
    }
}



//Procedimento comandos
void Comandos(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe,int *linha){

//Enquanto nao for end, vai fazendo os comandos do cmd
while((strcmp(classe,"end") != 0) & (strcmp(classe,"Fim") != 0)){
    //CMD
    CMD(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
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
void ProcedimentoDC(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe,int * linha){

    
//Procedimento dc_c, enquanto tiver dc_c vai fazendo
//Se for constante vai entrar aqui
    while(strcmp(classe,"constante") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
//Tem que ser id
        if(strcmp(classe,"Identificador") == 0){
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
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        }
//tem que ser ou numero negativo ou nao, mas precisa ter um dos dois
        if((strcmp(classe,"numReal") == 0) || (strcmp(classe,"numInt") == 0)){
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
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
 	    while(strcmp(classe,"Virgula") == 0){//mais var
                Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Virgula
                if(strcmp(classe,"Identificador") == 0){
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


//Procedimento dc_p, enquanto tiver dc_p vai fazendo
//Se for procedure entra aqui
    while(strcmp(classe,"procedure") == 0){
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
        if(strcmp(classe,"Identificador") == 0){
            Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
        }
        else{
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
                Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
 	        while(strcmp(classe,"Virgula") == 0){//mais var
                    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Virgula
                    if(strcmp(classe,"Identificador") == 0){
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
                Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Id
 	        while(strcmp(classe,"Virgula") == 0){//mais var
                    Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);//consome Virgula
                    if(strcmp(classe,"Identificador") == 0){
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
        Comandos(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
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

    }

//DC pode ser vazio, mas pode ser q tenha escrito errado!

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void sintatico(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe,int *linha){


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
        Classe(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
    }
    else{
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
     ProcedimentoDC(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha); 
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
    Comandos(buffer,controle, contador,F,palavra,tamanho,erros,classe,linha);
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

}


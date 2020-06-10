#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//Funções trabalho 1

_Bool Espaco(char c){
    if((c == '\n')| (c == '\t') | (c ==' ')){
	return 1;
    }
    else{
	return 0;
    }

}


_Bool Identificador(char c){
    char letras[52] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i=0;
    while(i<52){    
	if(c == letras[i]){
		return 1;
    	}
	i++;
    }
    return 0;
}

_Bool Numero(char c){
    char numeros[10] = "0123456789";
    int i=0;
    while(i<10){    
	if(c == numeros[i]){
	    return 1;
    	}
	i++;
    }
    return 0;
}

_Bool Operador(char c){
    char operadores[13] = "<>=+-*/;,:().";
    int i=0;
    while(i<13){    
	if(c == operadores[i]){
	    return 1;
    	}
	i++;
    }
    return 0;
}


_Bool Comentario(char c){
    if(c == '{'){
	return 1;
    }
    else
        return 0;
}

int AutomatoId(char *buffer, int * contador, int * controle, FILE *f, char *palavra,FILE *erros,int *linha){

    char letras[62] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int verificador=0,i=0,tamanhoId=0;

while(1){

    while(i<62){    
        if(buffer[*(contador)] == letras[i]){
	    palavra[tamanhoId] = letras[i]; 
	    tamanhoId++;verificador++;
            break;
    	}
    i++;
    }
    if(verificador ==0){break;}
    verificador = 0;*(contador)=*(contador)+1;i=0;*(controle)=*(controle)+1;
    if(*contador >= 200){
	*contador = 0;
	fread(buffer,200,1,f);
    }
    if(tamanhoId == 200){fprintf(erros,"Erro Lexico: Identificador ultrapassou o tamanho maximo! Linha: %d\n",*linha);break;}
}	
palavra[tamanhoId] = '\0';
return(0);

}




int AutomatoNum(char *buffer, int * contador, int * controle, FILE *f, char *palavra, FILE *erros,int *linha){

    unsigned int verificador=0,tamanhoId=0,real = 0;

while(1){
        if(Numero(buffer[*(contador)])){
	    palavra[tamanhoId] = buffer[*(contador)]; 
	    tamanhoId++;verificador++;
	}

    if(buffer[*(contador)] == '.'){
	verificador++;real++;palavra[tamanhoId] = '.';tamanhoId++;
	if(!Numero(buffer[*(contador) +1])){real++; }
    }
    if(verificador ==0){break;}
    verificador = 0;     *(contador)=*(contador)+1;    *(controle)=*(controle)+1;
    if(*contador >= 200){
	*contador = 0;
	fread(buffer,200,1,f);
    }
    if(tamanhoId == 200){fprintf(erros,"Erro Lexico: Numero ultrapassou o tamanho maximo! Linha: %d\n",*linha);break;}
}	
palavra[tamanhoId] = '\0';
if(real>1){fprintf(erros,"Erro Lexico: Numero %s escrito errado! Linha: %d\n",palavra,*linha);}
if(real >=1){return 36;}
else{return 35;}

}



int AutomatoOp(char *buffer, int * contador, int * controle, FILE *f, char *palavra){

    char operandos1[9] = "+-*/;,().";
    char operandos2[4] = ":<>=";
    int i=0,tamanhoId=0, qual = 0,verifica=0;

    while(i<9){    
        if(buffer[*(contador)] == operandos1[i]){
	    palavra[tamanhoId] = operandos1[i];tamanhoId++; 
            *(contador)=*(contador)+1;*(controle)=*(controle)+1;qual = i;
	    verifica++;break;
    	}
    i++;
    }
    i=0;
    if(verifica==0){
    while(i<4){    
        if(buffer[*(contador)] == operandos2[i]){
	    palavra[tamanhoId] = operandos2[i];tamanhoId++; 
            *(contador)=*(contador)+1;*(controle)=*(controle)+1;qual = 9+i;
	    if(*contador >= 200){
		*contador = 0;
		fread(buffer,200,1,f);
    	    }
	    if(buffer[*(contador)] == '='){	    
		palavra[tamanhoId] = '=';tamanhoId++; qual+=4;
	        *(contador)=*(contador)+1;*(controle)=*(controle)+1;
	    }
	    else if(palavra[tamanhoId-1] == '<' && buffer[*(contador)] == '>'){
		palavra[tamanhoId] = buffer[*(contador)];tamanhoId++; qual=17;
	        *(contador)=*(contador)+1;*(controle)=*(controle)+1;		
	    }
            break;
    	}
    i++;
    }
    }

	
palavra[tamanhoId] = '\0';
return 17 + qual;
}






int AutomatoComentario(char *buffer, int * contador, int * controle, FILE *f, char *palavra,int tamanho, FILE *erros){

    while(buffer[*contador] != '}'){
	*contador = *contador +1;
	*controle = *controle +1;
	if(*contador >= 200){
	    *contador = 0;
	    fread(buffer,200,1,f);
    	}
	if(*controle >= tamanho - 1){
	    fprintf(erros,"Erro Lexico: Nao fechou o comentario. \n");palavra[0] = '{'; palavra[1] = '\0'; return 37;
	}	
    }
    *(contador)+=1;*controle = *controle +1;palavra[0] = '{';palavra[1] = '}';palavra[2] = '\0';
    return 37;

}


int Lexico(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros, int *linha){

    while(Espaco(buffer[*contador])){
	if(buffer[*contador] == '\n'){*linha = *linha+1;}
	*contador = *contador +1;*controle=*controle +1;
        if(*contador >= 200){
	    fread(buffer,200,1,F);
	    *contador = 0;
        }
    }	
    if(*controle >= tamanho){return(39);}

    if(Identificador(buffer[*contador])){	
	return(AutomatoId(buffer,contador,controle,F,palavra,erros,linha));
    }
    else if(Numero(buffer[*contador])){
	return(AutomatoNum(buffer,contador,controle,F,palavra,erros,linha));
    }
    else if(Operador(buffer[*contador])){
	return(AutomatoOp(buffer,contador,controle,F,palavra));	
    }
    else if(Comentario(buffer[*contador])){
	return(AutomatoComentario(buffer,contador,controle,F,palavra,tamanho,erros));
		
    }
    else{
	palavra[0] = buffer[*(contador)];palavra[1] = '\0';
	*contador = *contador +1;*controle=*controle +1;
	fprintf(erros,"Erro Lexico: %s nao pertence ao alfabeto! Linha: %d\n", palavra,*linha);
        if(*contador >= 200){
	    fread(buffer,200,1,F);
	    *contador = 0;
        }
	return(38);
    }


}


void Classe(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros,char * classe,int *linha){

char Reservadas[17][30] ={"program","var","const","real","integer","procedure","begin","end","else","read",
"write","while","do","if","then","for","to"};

char Classificacao[37][30] = {"Programa","variavel","constante","real","inteiro","procedure","begin","end","else","read",
"write","while","do","if","then","for","to","mais","menos","multiplicacao","divisao","PeV",
"Virgula","abreP","fechaP","Ponto","doisP","Menor","Maior",
"Igual","DPIgual","menorIgual","maiorIgual","igualIgual","MaiorMenor","numInt","numReal"};
int j,i,verif=0;
	
    i = Lexico(buffer,controle, contador,F,palavra,tamanho,erros,linha);
//37 -> Comentario e 38 -> Nao pertence ao alfabeto
    while(i==37||i==38){
	i = Lexico(buffer,controle, contador,F,palavra,tamanho,erros,linha);
    }
    if(i == 39){strcpy(classe,"Fim");}

    else if(i>16){
	strcpy(classe,Classificacao[i]);
    }
    else{
        for(j=0;j<17;j++){
    	    if(strcmp(Reservadas[j], palavra)==0){
	    	strcpy(classe,Classificacao[j]);verif++;break;
	    }
	}
	if(verif==0){strcpy(classe,"Identificador");}	
   }

}


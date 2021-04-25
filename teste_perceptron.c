#include "perceptron02_sig_v02.c"

int main(){
	float ent[]={ 1,  0,
		      1,  0,
		      0,  1,
		      0,  1,
		      0,  0,
		      0,  0 };

	float sai[]={ 1,
		      1,
		      0,
		      0,
		      0,
		      0 };

	float *desejado;
		       

	char op = ' ';
	srand(time(NULL));
	Rna *rna  = criar_rna(1, 2, 1);

	
	//rna->ent[0] = 10;
	//rna->ent[1] = -1;
	//rna->ent[2] = -7;
	int entrada = 0;
	int saida = 0;
	for(long con=0; con<999; con++){
		//system("clear");
		rna->ent[0] = ent[entrada];
		rna->ent[1] = ent[entrada+1];
		//rna->ent[2] = ent[entrada+2];
		desejado = &sai[saida];
	
		ativar_rna(rna, 1, 2, 1);
		printf("\nSaída RNA %f \n", rna->sai[0]);
		imprimir_rna(rna, 1, 2, 1);
		backprop_rna(rna, 1, 2, 1, desejado, 1);

		printf("\n\n%f %f ", rna->cam[0].neu[0].peso[0], rna->cam[0].neu[0].peso[1]);
	//	op = getchar();
		entrada +=2;
		saida +=1;
		if(entrada > 2*5) entrada = 0;
		if(saida > 1*5) saida = 0;
	}
	return 0;
}

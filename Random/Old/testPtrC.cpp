#include <stdio.h>
#include <stdlib.h>

int main()
{
	// Lettura di colonne e righe
    int colonne, righe;
    scanf("%d", &colonne);
    scanf("%d", &righe);



    // Allocazione della matrice bidimensionale
   	int** matrice = (int**) malloc(sizeof(int*) * colonne);

   	int iX;
   	for(iX = 0; iX < colonne; ++iX) 
   	{
   		matrice[iX] = (int*) malloc(sizeof(int) * righe);
   	}



   	// Menu di scelta (lettura/scrittura)
   	int scelta;
   	scanf("%d", &scelta);
   	while(scelta != -1)
   	{
   		if(scelta == 0)
   		{
   			// Scrivi un valore
   			int x, y, valore;
   			scanf("%d, %d = %d", &x, &y, &valore);  

   			matrice[x][y] = valore;  
   		}
   		else if(scelta == 1)
   		{
   			// Leggi un valore
   			int x, y;
   			scanf("%d, %d", &x, &y);

   			printf("Valore: %d", matrice[x][y]);
   		}
   		scanf("%d", &scelta);
   	} 



   	// Deallocazione della matrice bidimensionale
   	for(iX = 0; iX < colonne; ++iX) 
   	{
   		free(matrice[iX]);
   	}

   	free(matrice);
}
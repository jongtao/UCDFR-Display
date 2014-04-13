#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *file = fopen("converts.txt", "w");
	FILE *file_in = fopen("inputs.txt", "r");
	char out[128];
	char input[128];
	char letter[128];
	unsigned int number[9] = {0};
	int i,j,k;

	while(1)
	{
		printf("\nLetter: ");

		//scanf("%s", letter);
		fscanf(file_in, "%s", letter);
		printf("%s", letter);

		if(letter[0] == 'e' && letter[1] == 'n' && letter[2] == 'd')
			return 0;

		printf("        12345678\n");
		for(j=0; j<9; j++)
		{
			printf("Binary: ");
			//scanf("%s", input);
			fscanf(file_in, "%s", input);
			printf("%s", input);

			for(i=0; i<7; i++)
			{
				number[j] <<= 1;
				number[j] |= input[i] - '0';
			}
			number[j] <<=1;

		}
		printf("\nunsigned char %c[9] = {0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X};\n\n",
			letter[0], number[0], number[1], number[2], number[3], number[4], number[5], number[6], number[7], number[8]);
		fprintf(file, "unsigned char %c[9] = {0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X};\n\n",
			letter[0], number[0], number[1], number[2], number[3], number[4], number[5], number[6], number[7], number[8]);

		for(k=0; k<9; k++)
			number[k]=0;

	}
	fclose(file);
	fclose(file_in);
	

	return 0;
}

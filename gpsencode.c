/* Converts a comma delimited spreadsheet of words to binary.  */
/* Note: Make sure that there are no spaces after the commas in input
   files.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "bool.h"

void ConvertToBinary(char* number, FILE* fOutput);

bool byteSwap = false;

int main(int argc, char* argv[])
{
	FILE* fInput;
	FILE* fOutput;

	if (argc == 1)
	{
		puts(
"Converts a comma delimited spreadsheet of words to binary\n\
Ussage: gpsencode <input> <output> <endian>\n\
If <endian> is specified as 'convert', then byte swapping will be performed\n\
during the conversion.\n\
Note: Make sure that there are no spaces after the commas in input files.");
		return 0;
	}
	if (argc == 4 && strcmp(argv[3], "convert") == 0)
		byteSwap = true;

	/* The file is read as bytes for internal newline conversion.  */
	fInput = fopen(argv[1], "rb");
	if (fInput == NULL)
		return 1;
	fOutput = fopen(argv[2], "wb");
	if (fOutput == NULL)
		return 1;
	while (true)
	{
		char buffer[5];
		char c;
		short output;

		fread(buffer, 4, 1, fInput);
		if (feof(fInput) != 0)
			break;
		buffer[4] = '\0';
		ConvertToBinary(buffer, fOutput);
		c = getc(fInput);
		if (c == '\r')
		{
			c = getc(fInput);
			if (c != '\n') /* In case of Macintosh newlines */
				fseek(fInput, -1, SEEK_CUR);
		}
	}
	fclose(fInput);
	fclose(fOutput);

	return 0;
}

void ConvertToBinary(char* number, FILE* fOutput)
{
	unsigned src;
	short intermed;
	short dest;
	unsigned i;

	/* Convert string to lowercase.  */
	for (i = 0; i < 4; i++)
		number[i] = tolower(number[i]);

	/* Convert to integer.  */
	sscanf(number, "%x", &src);
	intermed = (short)src;

	if (byteSwap == true)
		swab((char*)&intermed, (char*)&dest, sizeof(short));
	else
		dest = intermed;

	fwrite(&dest, sizeof(short), 1, fOutput);
}

/* Converts a GPS record file from binary to a comma delimited
   spreadsheet of hex words.  */

#include <stdio.h>
#include <stdlib.h>

#include "bool.h"

struct msgHead_t
{
	short r1;
	short blockNum;
	short startId; /* Always 0x81ff */
	short msgId;
	short numWords;
	short flags;
	short checksum;
};

void ConvertData(char* data, unsigned size, FILE* fOutput);

int main(int argc, char* argv[])
{
	FILE* fInput;
	FILE* fOutput;

	if (argc == 1)
	{
		puts(
"Converts a GPS record file from binary to a comma delimited spreadsheet\n\
of hex words.\n\
Ussage: gpsdecode <input> <output>");
		return 0;
	}

	fInput = fopen(argv[1], "rb");
	if (fInput == NULL)
		return 1;
	fOutput = fopen(argv[2], "wt");
	if (fOutput == NULL)
		return 1;
	while (true)
	{
		struct msgHead_t header;
		char* buffer;
		unsigned buffSize;

		/* Read the header.  */
		fread(&header, sizeof(struct msgHead_t), 1, fInput);
		if (feof(fInput) != 0)
			break;

		/* Write header to output.  */
		ConvertData((char*)&header, sizeof(struct msgHead_t), fOutput);
		if (header.numWords == 0)
		{
			/* Overwrite last comma and place newline.  */
			fseek(fOutput, -1, SEEK_CUR);
			putc('\n', fOutput);
			/* Skip data checksum.  */
			continue;
		}

		/* Convert binary data and write to output.  */
		buffSize = sizeof(short) * (header.numWords + 1);
		buffer = (char*)malloc(buffSize);
		fread(buffer, sizeof(short), header.numWords + 1, fInput);
		ConvertData(buffer, buffSize, fOutput);
		free(buffer);

		/* Overwrite last comma and place newline.  */
		fseek(fOutput, -1, SEEK_CUR);
		fprintf(fOutput, "\n");
	}
	fclose(fInput);
	fclose(fOutput);

	return 0;
}

void ConvertData(char* data, unsigned size, FILE* fOutput)
{
	unsigned i;
	for (i = 0; i < size; i++)
	{
		int c1 = 0; c1 += (unsigned char)data[i++];
		int c2 = 0; c2 += (unsigned char)data[i];

		/* Use byte swapping for little endian.  */
		fprintf(fOutput, "%02x%02x,", c2, c1);
	}
}

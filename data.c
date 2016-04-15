#include <stdio.h>
#include <stdlib.h>

typedef struct entry {
	char vcod[7];
	char produtor[24];
	char cepa[15];
	int colheita;
	char pais[15];
} entry;

void write_data(entry *entries, int num_entries, FILE *f) {
	fwrite(entries, sizeof(entry), num_entries, f);
}

int main() {

	int num_entries = 2;
	
	entry e[2] = {
		{"V100", "Don Laurindo", "Merlot", 2011, "Brasil"},
		{"V150", "Chateau Lafitte", "Chardonnay", 2010, "Franca"}
	};

	// write
	FILE *f;
	f = fopen("Vinhos.dat", "wb+");

	// printf("%1ld\n", sizeof(e));

	write_data(e, num_entries, f);
	fclose(f);

	// read
	FILE* in;
	entry e_;

	in = fopen("Vinhos.dat", "rb+");
	fread(&e_, sizeof(e_), 1, in);

	printf("%s\n", e_.vcod);

	fclose(in);

	return 0;
}
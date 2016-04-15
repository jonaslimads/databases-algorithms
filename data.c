#include <stdio.h>
#include <stdlib.h>

typedef struct record {
	char vcod[7];
	char produtor[24];
	char cepa[15];
	int colheita;
	char pais[15];
} record;

void write_data(record *records, int num_records, FILE *f) {
	fwrite(records, sizeof(record), num_records, f);
}

// fseek moves the file pointer to offset position
void load_data(FILE *f, long offset) {
	record r;

	fseek(f, offset, SEEK_SET);
	fread(&r, sizeof(r), 1, f);

	printf("%s %s\n", r.vcod, r.cepa);
}

int main() {

	int num_records = 3;
	
	record r[3] = {
		{"V100", "Don Laurindo", "Merlot", 2011, "Brasil"},
		{"V150", "Chateau Lafitte", "Chardonnay", 2010, "Franca"},
		{"V175", "Chryseia", "Douro", 2014, "Portugal"}
	};

	// write
	FILE *f;
	f = fopen("Dados.dat", "wb+");

	// printf("%1ld\n", sizeof(r));

	write_data(r, num_records, f);
	fclose(f);

	// read
	FILE* in;
	record r_;

	in = fopen("Dados.dat", "rb+");

	load_data(f, 0*sizeof(record)); // first record
	load_data(f, 1*sizeof(record)); // second...
	load_data(f, 2*sizeof(record)); // 

	// printf("%s\n", r_.vcod);

	fclose(in);

	return 0;
}
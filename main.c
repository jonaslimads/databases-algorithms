#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bplus_tree.h"
#include "bplus_tree_io.h"


int main() {

	static const int num_records = 11;
	
	record r[num_records] = {
		{"V100", "Don Laurindo", "Merlot", 2017, "Brasil"},
		{"V150", "Chateau Lafitte", "Chardonnay", 2016, "Franca"},
		{"V180", "Chryseia", "Douro", 2014, "Portugal"},
		{"V190", "Chryseia", "Douro", 2012, "Portugal"},
		{"V200", "Chryseia", "Douro", 2018, "Portugal"},
		{"V200", "Chryseia", "Douro", 2020, "Portugal"},
		{"V200", "Chryseia", "Douro", 2022, "Portugal"},
		{"V200", "Chryseia", "Douro", 2019, "Portugal"},
		{"V200", "Chryseia", "Douro", 2030, "Portugal"},
		{"V200", "Chryseia", "Douro", 2021, "Portugal"},
		{"V200", "Chryseia", "Douro", 2040, "Portugal"},
	};

	// write
	FILE *f, *i;
	f = fopen("Dados.dat", "wb+");
	i = fopen("Indices.dat", "wb+");


	write_records_data(f, r, num_records);
	write_indices_data(i, r, num_records);

	// print all records from disk
	int j;
	record rec;
	for (j = 1; j <= num_records; j++) {
		rec = load_record_data(f, j); // index
		
		printf("rid=%d [%s, %s, %s, %d, %s]\n",
			j, rec.vcod, rec.produtor, rec.cepa,
			rec.colheita, rec.pais);
	}

	printf("\n\n\n");

	// print all indices from disk
	index_storage index;
	for (j = 1; j <= num_records; j++) {
		index = load_index_data(i, j); // index
		
		printf("rid=%d [%d %d, %d %d %d, %s]\n",
			j, index.key1, index.key2,
			index.rid1, index.rid2, index.rid3,
			index.tipo);
	}

	fclose(f);
	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bplus_tree.h"
#include "bplus_tree_io.h"


int main() {

	static const int num_records = 20;
	
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

	index_storage root = load_index_data(i, 1); // root
	
	printf("[%d %d, %d %d %d, %s]\n",
		root.key1, root.key2,
		root.rid1, root.rid2, root.rid3,
		root.tipo);

	fclose(f);
	return 0;
}
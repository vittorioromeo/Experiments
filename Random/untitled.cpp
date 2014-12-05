int main()
{
	int k;

	int* puntatoreAk = &k;

	(*puntatoreAk) = 10;

	int** puntatoreAPuntatoreAk 
			= &puntatoreAk;
}
// Copyright Omer Tarik Ilhan 314CAa 2021-2022
#include <stdio.h>
#include <stdlib.h>

#define MODULO 10007
// maresc marimea alocata vectorului de matrice
void extend_vect(int ****vmat, int **vlin, int **vcol, int *size_m)
{
// maresc cu 1 numarul de "slot-uri available" pentru matrice
// in cadrul vectorului de matrice (care e triplu pointer)
// si realoc tot in pointeri auxiliari
	*size_m += 1;
	int ***aux = realloc(*vmat, *size_m * sizeof(int **));
	int *aux1 = realloc(*vlin, *size_m * sizeof(int));
	int *aux2 = realloc(*vcol, *size_m * sizeof(int));

// daca totul a decurs bine, definitivez realocarea
// pe pointerii initiali
	if (!aux1 || !aux2 || !aux) {
		free(aux1);
		free(aux2);
		free(aux);
		printf("nu s-a putut realoca");
	} else {
		*vlin = aux1;
		*vcol = aux2;
		*vmat = aux;
	}
}

// aloc si citesc o matrice pe care o returnez
int **read_and_alloc_matrix(int m, int n)
{
// aloc si verific pointer pentru o matrice, apoi pentru fiecare linie
// returnez la final pointerul cu matricea alocata si pe urma citita
	int **matrix = malloc(m * sizeof(int *));
	if (!matrix) {
		free(matrix);
		printf("matricea nu s-a putut aloca");
		return NULL;
	}
	for (int i = 0; i < m; i++) {
		matrix[i] = malloc(n * sizeof(int));
		if (!matrix[i]) {
			free(matrix[i]);
			printf("o linie nu s-a putut aloca");
			return NULL;
		}
		for (int j = 0; j < n; j++)
			scanf("%d", &matrix[i][j]);
	}
	return matrix;
}

// aloc si initializez cu 0 o matrice pe care o returnez
int **alloc_and_init_matrix(int m, int n)
{
// similar cu "read_and_alloc_matrix()", dar in loc de a citi,
// dau valori de 0 elementelor
	int **matrix = malloc(m * sizeof(int *));
	if (!matrix) {
		free(matrix);
		printf("matricea nu s-a putut aloca");
		return NULL;
	}
	for (int i = 0; i < m; i++) {
		matrix[i] = malloc(n * sizeof(int));
		if (!matrix[i]) {
			free(matrix[i]);
			printf("o linie nu s-a putut aloca");
			return NULL;
		}
		for (int j = 0; j < n; j++)
			matrix[i][j] = 0;
	}
	return matrix;
}

// eliberez memoria la care se afla matricea data ca parametru
void free_matrix(int **matrix, int lin)
{
	for (int i = 0; i < lin; i++)
		free(matrix[i]);
	free(matrix);
}

// afisez o matrice
void print_matrix(int **matrix, int lin, int col)
{
	for (int i = 0; i < lin; i++) {
		for (int j = 0; j < col; j++)
			printf("%d ", matrix[i][j]);
		printf("\n");
	}
}

// inmultesc 2 matrice si returnez un dublu pointer care are adresa
// matricii rezultat
int **multiply_mat(int **mat1, int **mat2, int l_1, int c_1, int c_2)
{
	int **matrix_multiplied = alloc_and_init_matrix(l_1, c_2);
	for (int i = 0; i < l_1; i++) {
		for (int j = 0; j < c_2; j++)
			for (int k = 0; k < c_1; k++) {
				matrix_multiplied[i][j] += mat1[i][k] * mat2[k][j];
// trec prin "filtrul" de % 10^4 + 7
				matrix_multiplied[i][j] = matrix_multiplied[i][j] % MODULO;
			}

// trec prin "filtrul" de < 0
		for (int j = 0; j < c_2; j++) {
			if (matrix_multiplied[i][j] < 0)
				matrix_multiplied[i][j] += MODULO;
		}
	}
	return matrix_multiplied;
}

// calculez suma tuturor elementelor unei matrice
int sum_matrix(int **matrix, int lin, int col)
{
	int sum = 0;
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++)
			sum += matrix[i][j];

// trec prin "filtrul" de MODULO 10^4 + 7
	sum = sum % MODULO;
	if (sum < 0)
		sum = sum + MODULO;
	return sum;
}

// atunci cand citesc litera 'O', ordonez matricele crescator
// in functie de suma elementelor
void input_O(int ***vect_mat, int *vect_lin, int *vect_col, int matrix_counter)
{
	if (matrix_counter > 0) {
		for (int i = 0; i < matrix_counter; i++)
			for (int j = 0; j < matrix_counter - 1; j++) {
				int **mat_1 = vect_mat[j];
				int l_1 = vect_lin[j];
				int c_1 = vect_col[j];

				int **mat_2 = vect_mat[j + 1];
				int l_2 = vect_lin[j + 1];
				int c_2 = vect_col[j + 1];

				int sum_1 = sum_matrix(mat_1, l_1, c_1);
				int sum_2 = sum_matrix(mat_2, l_2, c_2);

				if (sum_1 > sum_2) {
					int **aux_mat = vect_mat[j];
					int aux_lin = vect_lin[j];
					int aux_col = vect_col[j];
					vect_mat[j] = vect_mat[j + 1];
					vect_lin[j] = vect_lin[j + 1];
					vect_col[j] = vect_col[j + 1];
					vect_mat[j + 1] = aux_mat;
					vect_lin[j + 1] = aux_lin;
					vect_col[j + 1] = aux_col;
				}
			}
	}
}

// atunci cand primesc litera L, citesc 2 dimensiuni,
// apoi o matrice cu acele dimensiuni
void input_L(int ***vect_mat, int matrix_counter, int *vect_col, int *vect_lin)
{
	int m, n;
	scanf("%d%d", &m, &n);

	vect_mat[matrix_counter] = read_and_alloc_matrix(m, n);
	vect_lin[matrix_counter] = m;
	vect_col[matrix_counter] = n;
}

// atunci cant primesc litera P,
// afisez matricea de la index-ul pe care il citesc
void input_P(int ***vect_mat, int *vect_col, int *vect_lin, int matrix_counter)
{
	int index;
	scanf("%d", &index);
	if (index < matrix_counter && index >= 0) {
		int **matrix = vect_mat[index];
		int lin = vect_lin[index];
		int col = vect_col[index];

		print_matrix(matrix, lin, col);
	} else {
		printf("No matrix with the given index\n");
	}
}

// atunci cand citesc litera Q, eliberez toata memoria alocata,
// si ies din bucla while(1) din main
void input_Q(int ***vect_mat, int *vect_col, int *vect_lin, int matrix_counter)
{
// eliberez intai toate matrice
	for (int i = 0; i < matrix_counter; i++)
		free_matrix(vect_mat[i], vect_lin[i]);

// apoi vectorul de matrice si ceilalti 2 vectori
	free(vect_mat);
	free(vect_col);
	free(vect_lin);
}

// atunci cand primesc litera D,
// afisez dimensiunile matricei care se afla la indicele citit
void input_D(int *vect_lin, int *vect_col, int matrix_counter)
{
	int index;
	scanf("%d", &index);

	if (index > matrix_counter - 1 && index >= 0)
		printf("No matrix with the given index\n");
	else
		printf("%d %d\n", vect_lin[index], vect_col[index]);
}

// atunci cand primesc litera F,
// eliberez memoria la care se afla matricea ceruta
// apoi shiftez toate matricele de dupa aceasta cu un index mai in spate
void input_F(int ***vect_mat, int *vect_lin, int *vect_col, int *ctr_ad)
{
	int matrix_counter = *ctr_ad;
	int index;
	scanf("%d", &index);

	if (index > matrix_counter - 1 || index < 0) {
		printf("No matrix with the given index\n");
	} else {
		free_matrix(vect_mat[index], vect_lin[index]);

		for (int k = index; k < matrix_counter - 1; k++) {
			vect_mat[k] = vect_mat[k + 1];
			vect_lin[k] = vect_lin[k + 1];
			vect_col[k] = vect_col[k + 1];
	}
	matrix_counter--;
	*ctr_ad = matrix_counter;
	}
}

// atunci cand citesc litera T,
// transpun matricea ceruta
void input_T(int ***vect_mat, int *vect_lin, int *vect_col, int matrix_counter)
{
	int index;
	scanf("%d", &index);

	if (index > matrix_counter - 1 || index < 0) {
		printf("No matrix with the given index\n");
	} else {
		int lin_vechi = vect_lin[index];
		int col_vechi = vect_col[index];
		int transpose_lin = vect_col[index];
		int transpose_col = vect_lin[index];

		int **matrix = vect_mat[index];
		int **tsp_matrix = alloc_and_init_matrix(transpose_lin, transpose_col);

// interschimb dimensiunea liniilor cu cea a coloanelor
		vect_lin[index] = transpose_lin;
		vect_col[index] = transpose_col;

// elementul de la pozitia [i][j] ajunge la pozitia [j][i]
		for (int i = 0; i < lin_vechi; i++)
			for (int j = 0; j < col_vechi; j++)
				tsp_matrix[j][i] = matrix[i][j];

// deoarece am stocat matricea transpusa la o adresa noua
// eliberez adresa matricei anterioare
		free_matrix(vect_mat[index], lin_vechi);
		vect_mat[index] = tsp_matrix;
	}
}

// atunci cand citesc 'C', redimensionez matricea in modul cerut in cerinta
void input_C(int ***vect_mat, int *vect_lin, int *vect_col, int matrix_counter)
{
	int index;
// citesc indexul matricei
	scanf("%d", &index);
// citesc numarul de linii si de coloane
// respectiv liniile si coloanele pe care intentionez
// sa le pastrez
	int num_lin, num_col;
	scanf("%d", &num_lin);
	int *lin = malloc(num_lin * sizeof(int));
	for (int i = 0; i < num_lin; i++)
		scanf("%d", &lin[i]);

	scanf("%d", &num_col);
	int *col = malloc(num_col * sizeof(int));
	for (int i = 0; i < num_col; i++)
		scanf("%d", &col[i]);

	if (index > matrix_counter - 1 || index < 0) {
		printf("No matrix with the given index\n");
	} else {
		// aloc o noua matrice, pe care o umplu cu valorile de la pozitiile
		// pe care vreau sa le pastrez
		int **matrix_redim = alloc_and_init_matrix(num_lin, num_col);
		int **matrix = vect_mat[index];

		for (int i = 0; i < num_lin; i++)
			for (int j = 0; j < num_col; j++)
				matrix_redim[i][j] = matrix[lin[i]][col[j]];
		// eliberez memoria unde se afla matricea veche
		free_matrix(vect_mat[index], vect_lin[index]);
		// inseram matricea noua in locul celei vechi
		vect_lin[index] = num_lin;
		vect_col[index] = num_col;
		vect_mat[index] = matrix_redim;
	}
	free(col);
	free(lin);
}

// atunci cand citesc litera 'M', inmultesc 2 matrice
void input_M(int ***vmat, int *vlin, int *vcol, int i_1, int i_2, int mat_cnt)
{
	int **m_1 = vmat[i_1];
	int **m_2 = vmat[i_2];
	int l_1 = vlin[i_1];
	int c_1 = vcol[i_1];
	int c_2 = vcol[i_2];
	// calculam matricea rezultat
	int **mult_mat = multiply_mat(m_1, m_2, l_1, c_1, c_2);
	// inseram matricea rezultat in vectorul de matrice
	vmat[mat_cnt] = mult_mat;
	vlin[mat_cnt] = vlin[i_1];
	vcol[mat_cnt] = vcol[i_2];
}

// Strassen
// adunam doua matrice
int **add_matrix(int **matrix_1, int **matrix_2, int size_of_mat)
{
	int **result = alloc_and_init_matrix(size_of_mat, size_of_mat);
	for (int i = 0; i < size_of_mat; i++)
		for (int j = 0; j < size_of_mat; j++) {
			result[i][j] = matrix_1[i][j] + matrix_2[i][j];
			result[i][j] = result[i][j] % MODULO;
			if (result[i][j] < 0)
				result[i][j] += MODULO;
		}
	return result;
}

// scadem doua matrice
int **subtract_matrix(int **matrix_1, int **matrix_2, int size_of_mat)
{
	int **result = alloc_and_init_matrix(size_of_mat, size_of_mat);
	for (int i = 0; i < size_of_mat; i++)
		for (int j = 0; j < size_of_mat; j++) {
			result[i][j] = matrix_1[i][j] - matrix_2[i][j];
			result[i][j] = result[i][j] % MODULO;
			if (result[i][j] < 0)
				result[i][j] += MODULO;
		}
	return result;
}

int is_power_of_two(int num)
{
	// verificam daca un numar este putere de 2
	// vom folosi asta pentru a verifica dimensiunea matricei pentru Strassen
	while (num != 1) {
		if (num % 2 == 1)
			return 0;
		num /= 2;
	}
	return 1;
}

int **submat(int **mat, int l_st, int l_en, int c_st, int c_en, int size)
{
	int **matrix_redim = alloc_and_init_matrix(size, size);
	// spargem matricea intr-o submatrice ("patrat")
	// facem asta dupa o linie de inceput si de final, respectiv coloana
	for (int i = l_st; i < l_en; i++)
		for (int j = c_st; j < c_en; j++)
			matrix_redim[i - l_st][j - c_st] = mat[i][j];

	return matrix_redim;
}

int **back_together(int **c1, int **c2, int **c3, int **c4, int size)
{
	int **matrix = alloc_and_init_matrix(size, size);
	// refacem matricea de dinainte de spargere
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			// verificam in ce "patrat" se afla elementul
			// si il copiem din C-ul respectiv
			if (i < size / 2 && j < size / 2)
				matrix[i][j] = c1[i][j];
			else if (i < size / 2 && j >= size / 2)
				matrix[i][j] = c2[i][j - size / 2];
			else if (i >= size / 2 && j < size / 2)
				matrix[i][j] = c3[i - size / 2][j];
			else if (i >= size / 2 && j >= size / 2)
				matrix[i][j] = c4[i - size / 2][j - size / 2];
	return matrix;
}

// algoritmul Strassen in sine
int **strassen(int **matrix_1, int **matrix_2, int size)
{
	if (size < 2)
		return multiply_mat(matrix_1, matrix_2, size, size, size);

	// facem submatricele
	int **mat1_11 = submat(matrix_1, 0, size / 2, 0, size / 2, size / 2);
	int **mat1_12 = submat(matrix_1, 0, size / 2, size / 2, size, size / 2);
	int **mat1_21 = submat(matrix_1, size / 2, size, 0, size / 2, size / 2);
	int **mat1_22 = submat(matrix_1, size / 2, size, size / 2, size, size / 2);
	int **mat2_11 = submat(matrix_2, 0, size / 2, 0, size / 2, size / 2);
	int **mat2_12 = submat(matrix_2, 0, size / 2, size / 2, size, size / 2);
	int **mat2_21 = submat(matrix_2, size / 2, size, 0, size / 2, size / 2);
	int **mat2_22 = submat(matrix_2, size / 2, size, size / 2, size, size / 2);

	// calculam niste termeni intermediari pt M-uri
	int **m1_1 = add_matrix(mat1_11, mat1_22, size / 2);
	int **m1_2 = add_matrix(mat2_11, mat2_22, size / 2);
	int **m2_1 = add_matrix(mat1_21, mat1_22, size / 2);
	int **m3_1 = subtract_matrix(mat2_12, mat2_22, size / 2);
	int **m4_1 = subtract_matrix(mat2_21, mat2_11, size / 2);
	int **m5_1 = add_matrix(mat1_11, mat1_12, size / 2);
	int **m6_1 = subtract_matrix(mat1_21, mat1_11, size / 2);
	int **m6_2 = add_matrix(mat2_11, mat2_12, size / 2);
	int **m7_1 = subtract_matrix(mat1_12, mat1_22, size / 2);
	int **m7_2 = add_matrix(mat2_21, mat2_22, size / 2);

	// calculam M-urile (recursiv, tot cu Strassen)
	int **m1 = strassen(m1_1, m1_2, size / 2);
	int **m2 = strassen(m2_1, mat2_11, size / 2);
	int **m3 = strassen(mat1_11, m3_1, size / 2);
	int **m4 = strassen(mat1_22, m4_1, size / 2);
	int **m5 = strassen(m5_1, mat2_22, size / 2);
	int **m6 = strassen(m6_1, m6_2, size / 2);
	int **m7 = strassen(m7_1, m7_2, size / 2);

	// calculam C-urile (si folosim niste matrice auxiliare)
	int **aux_1 = add_matrix(m1, m4, size / 2);
	int **aux_2 = subtract_matrix(aux_1, m5, size / 2);
	int **c1 = add_matrix(aux_2, m7, size / 2);
	int **c2 = add_matrix(m3, m5, size / 2);
	int **c3 = add_matrix(m2, m4, size / 2);
	int **aux_3 = subtract_matrix(m1, m2, size / 2);
	int **aux_4 = add_matrix(aux_3, m3, size / 2);
	int **c4 = add_matrix(aux_4, m6, size / 2);

	// reintregim cele 4 C-uri
	int **mat_multiplied = back_together(c1, c2, c3, c4, size);

	// eliberam tot spatiul alocat
	free_matrix(mat1_11, size / 2); free_matrix(mat1_12, size / 2);
	free_matrix(mat1_21, size / 2); free_matrix(mat1_22, size / 2);
	free_matrix(mat2_11, size / 2); free_matrix(mat2_12, size / 2);
	free_matrix(mat2_21, size / 2); free_matrix(mat2_22, size / 2);
	free_matrix(m1_1, size / 2); free_matrix(m1_2, size / 2);
	free_matrix(m2_1, size / 2); free_matrix(m3_1, size / 2);
	free_matrix(m4_1, size / 2); free_matrix(m5_1, size / 2);
	free_matrix(m6_1, size / 2); free_matrix(m6_2, size / 2);
	free_matrix(m7_1, size / 2); free_matrix(m7_2, size / 2);
	free_matrix(aux_1, size / 2); free_matrix(aux_2, size / 2);
	free_matrix(c1, size / 2); free_matrix(c2, size / 2);
	free_matrix(c3, size / 2); free_matrix(aux_3, size / 2);
	free_matrix(aux_4, size / 2); free_matrix(c4, size / 2);
	free_matrix(m1, size / 2); free_matrix(m2, size / 2);
	free_matrix(m3, size / 2); free_matrix(m4, size / 2);
	free_matrix(m5, size / 2); free_matrix(m6, size / 2);
	free_matrix(m7, size / 2);

	return mat_multiplied;
}

// atunci cand citesc litera 'S', inmultesc 2 matrice folosind alg Strassen
void input_S(int ****avect_mat, int **avlin, int **avcol, int *amat_ct, int *sm)
{
	// ne folosim de adresele pointerilor si variabilelor
	// pentru a putea sa facem realocarea vectorului de matrice
	int ***vect_mat = *avect_mat;
	int *vlin = *avlin;
	int *vcol = *avcol;
	int mat_ct = *amat_ct;
	int size_multiplier = *sm;
	int i_1, i_2;
	scanf("%d%d", &i_1, &i_2);
	// verificam ca indicii sa fie valizi
	// si daca matricele se pot inmulti folosind Strassen (2^n x 2^n)
	if (i_1 >= mat_ct || i_2 >= mat_ct || i_1 < 0 || i_2 < 0) {
		printf("No matrix with the given index");
	} else if (vlin[i_1] != vlin[i_2]) {
		printf("Cannot perform matrix multiplication");
	} else if (vcol[i_2] != vcol[i_1]) {
		printf("Cannot perform matrix multiplication");
	} else if (vlin[i_2] != vlin[i_1]) {
		printf("Cannot perform matrix multiplication");
	} else if (vcol[i_2] != vcol[i_1]) {
		printf("Cannot perform matrix multiplication");
	} else {
		int size = vlin[i_1];
		if (is_power_of_two(size) == 0) {
			printf("Cannot perform matrix multiplication");
		} else {
			// calculam matricea rezultat folosind Strassen
			int **matrix_1 = vect_mat[i_1];
			int **matrix_2 = vect_mat[i_2];
			int **matrix_multiplied = strassen(matrix_1, matrix_2, size);

			// marim vectorul de matrice si inseram la final matricea rezultat
			size_multiplier++;
			extend_vect(&vect_mat, &vlin, &vcol, &size_multiplier);
			vect_mat[mat_ct] = matrix_multiplied;
			vcol[mat_ct] = size;
			vlin[mat_ct] = size;
			mat_ct++;	
			*amat_ct = mat_ct;
		}
	}
	// "salvam" modificarile inapoi la adresele initiale
	// adica cele declarate in main()
	*sm = size_multiplier;
	*amat_ct = mat_ct;
	*avect_mat = vect_mat;
	*avlin = vlin;
	*avcol = vcol;
}

int main(void)
{
	int i1, i2;
	int matrix_counter = 0;
	int ***vect_mat;
	int size_m = 1;
	int *vect_lin, *vect_col;
// la inceput, aloc 1 spatiu in vectorul de matrice
	if (size_m == 1 && matrix_counter == 0) {
		vect_mat = malloc(size_m * sizeof(int **));
		vect_lin = malloc(size_m * sizeof(int));
		vect_col = malloc(size_m * sizeof(int));
	}
// bucla infinita care este oprita doar la citirea literei Q
	while (1) {
		char input;
		scanf(" %c", &input);
// daca vectorul de matrice nu mai are loc, il extind
		if (matrix_counter == size_m)
			extend_vect(&vect_mat, &vect_lin, &vect_col, &size_m);
// in functie de input, fac o operatie din cele date in enunt
		switch (input) {
// citire de matrice
		case 'L':
			input_L(vect_mat, matrix_counter, vect_col, vect_lin);
			matrix_counter++;
			break;
// afisare de matrice
		case 'P':
			input_P(vect_mat, vect_col, vect_lin, matrix_counter);
			break;
// afisare dimensiuni de matrice
		case 'D':
			input_D(vect_lin, vect_col, matrix_counter);
			break;
// inmultirea a 2 matrice
		case 'M':
				scanf("%d%d", &i1, &i2);
		// verific daca exista cele doua matrice macar
			if (i1 > matrix_counter || i2 > matrix_counter) {
				printf("No matrix with the given index\n");
		// verific daca pot fi inmultite matricele
			} else if (vect_col[i1] != vect_lin[i2]) {
				printf("Cannot perform matrix multiplication\n");
			} else {
				extend_vect(&vect_mat, &vect_lin, &vect_col, &size_m);
				input_M(vect_mat, vect_lin, vect_col, i1, i2, matrix_counter);
				matrix_counter++;
			}
			break;
// ordonarea crescator a matricelor in functie de suma elementelor
		case 'O':
			input_O(vect_mat, vect_lin, vect_col, matrix_counter);
			break;
// stergerea unei matrice (din memorie si din vectorul de matrice)
		case 'F':
			input_F(vect_mat, vect_lin, vect_col, &matrix_counter);
			break;
// transpunerea unei matrice
		case 'T':
			input_T(vect_mat, vect_lin, vect_col, matrix_counter);
			break;
// redimensionarea unei matrice
		case 'C':
			input_C(vect_mat, vect_lin, vect_col, matrix_counter);
			break;
// eliberarea intregii memorii si oprirea programului
		case 'Q':
			input_Q(vect_mat, vect_col, vect_lin, matrix_counter);
			break;
// inmultirea folosind algoritmul Strassen
		case 'S':
			input_S(&vect_mat, &vect_col, &vect_lin, &matrix_counter, &size_m);
			break;
		default:
			printf("Unrecognized command\n");
		}
		if (input == 'Q')
			break;
	}
	return 0;
}

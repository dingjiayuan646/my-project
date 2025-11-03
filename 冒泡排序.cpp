#include <stdio.h>

int main() {
	int i, j, t;
	int a[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 10 - 1 - i; j++) {
			if (a[j] > a[j + 1]) {
				t = a[j + 1];
				a[j + 1] = a[j];
				a[j] = t;
			}
		}
	}

	for (i = 0; i < 10; i++) {
		printf("%d ", a[i]);
	}


	return 0;
}
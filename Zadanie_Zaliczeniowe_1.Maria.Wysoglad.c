/*
"Polynomials"

This project is the polynomial calculator with two operations: addition and multiplication.
Maximum degree of polynomial that might be counted by this caltulator is 10.
Polynomials coefficients might be represented as integers (type int).
There is storage cell which contains previous counted polynomial.
At the beginning polynomial is equal 0.
author: Maria Wysogląd
*/

#include <stdio.h>
#define MAX_DEG 11
#define INT_MIN -2147483648
/* Maximum degree of our polynomial is 10, therefore we have also space
for monomial of degree 0.
All analised polynomials are represented in arrays such that each
monomial of degree i is on i-th position. */

void reset(int *arr) {
    for (int i = 0; i < MAX_DEG; i++) {
        arr[i] = 0;
    }
}

// Arithmetical functions:
void addition(int *arr1, int *arr2) {
    for (int i = 0; i < MAX_DEG; i++) {
        arr1[i] = arr1[i]+ arr2[i];
    }
}

void multiplication(int *arr1, int *arr2) {
    int arr3[MAX_DEG];
    int i3 = 0;
    int i2 = 0;

    for (int i = 0; i < MAX_DEG; i++) {
        arr3[i] = arr1[i];
        arr1[i] = 0;
    }
     while (i2 < MAX_DEG) {
        while ((i3 + i2) < MAX_DEG) {
            int s = arr3[i3] * arr2[i2];

            arr1[i2 + i3] = s + arr1[i2 + i3];
            i3 = i3 + 1;
        }
        i2 = i2 + 1;
        i3 = 0;
    }
}

int absolute_value (int n) {
    if (n < 0) {
        n = (-1) * n;
    }

    return n;
}

// Write_out functions:
void out_int_min(int degree, int firstmonomial) {
    if (firstmonomial == 1) {
        printf("-2147483648");
    }
    else printf(" - 2147483648");

    if (degree != 0 && degree != 1) {
        printf("x^%d", degree);
    }
    else if (degree == 1) {
        printf("x");
    }
}

// Monomials of degree 0 or 1 must be considered separately.
void out_0_degree(int current_coefficient) {
    printf("%d", current_coefficient);
}

void out_1_degree(int current_coefficient) {
    if (current_coefficient != 1) {
        printf("%d", current_coefficient);
    }

    printf("x");
}

void out_other_degree(int current_coefficient, int degree) {
    if (current_coefficient != 1) {
        printf("%d", current_coefficient);
    }

    printf("x^%d", degree);
}

void out_monomial(int current_coefficient, int degree) {
    if (degree == 0) {
        out_0_degree(current_coefficient);
    }
    else if (degree == 1) {
        out_1_degree(current_coefficient);
    }
    else {
        out_other_degree(current_coefficient, degree);
    }
}

// Only first monomial, if negative, is written without space between '-' and first digit.
void out_first_monomial(int current_coefficient, int degree) {
    if (current_coefficient < 0) {
        printf("-");
        current_coefficient = absolute_value(current_coefficient);
    }

    out_monomial(current_coefficient, degree);
}

void out_other_monomial(int current_coefficient, int degree) {
    if (current_coefficient < 0) {
        printf(" - ");
        current_coefficient = absolute_value(current_coefficient);
    }
    else printf(" + ");
    /* I change monomial's coefficient in its absolute value and write out ' - '
    separately, analogiously with ' + '. */

    out_monomial(current_coefficient, degree);
}

void write_out(int *polynomial) {
    int min_deg = 0;
    int first_monomial = 1;
    // Only first monomial's coefficient is written out without its absolute value.

    while ((polynomial[min_deg] == 0) && (min_deg < MAX_DEG)) {
        min_deg++;
    }
    if (min_deg == MAX_DEG) {
        printf("%d\n", 0);
        /* Only if all monomials' coefficients are equal 0, project is supposed to
        write out '0', otherwise all coefficients equal 0 are skipped. */
    }
    else {
        for (int i = (MAX_DEG-1); i>=min_deg; i--) {
        int current_coefficient = polynomial[i];
        // We write out numbers from the maximum degree (<=10) to the minimum degree (>=0).
            if (polynomial[i] != 0) {
            /* If polynomial's coefficient equals 0, nothing will be written out for this
             monomial, moreover if a degree of monomial equals 0 or 1, we do not write out
             a degree and these cases must be considered separately. */
                if (current_coefficient == INT_MIN) {
                    /* Absolute value of minimum int is greater than maximum int, therefore
                    it must be considered separately. */
                    out_int_min(i, first_monomial);
                    first_monomial = 0;
                }
                else {
                    if (first_monomial == 1) {
                        out_first_monomial(current_coefficient, i);
                        first_monomial = 0;
                    }
                    else {
                        out_other_monomial(current_coefficient, i);
                    }
                }
            }
        }

        printf("\n");
    }
}

//Write_in functions:
void in_monomial(int *newpolynomial, int *current_deg, int *positive, int *coefficient) {
    *current_deg = 1;

    if (*positive == 1) {
        *coefficient = *coefficient * (-1);
    }

    newpolynomial[*current_deg] = *coefficient;
    *coefficient = 0;
    *positive = 1;
}

void newcoefficient (int *coefficient, char *current_character) {
    int digit = *current_character - '0';

    *coefficient = 10 * (*coefficient) - digit;
// Negative value is treated as default.
}

void in_degree(int *newpolynomial, int *current_deg, int *positive, int *coefficient) {
    if (*positive == 1) {
        *coefficient = *coefficient * (-1);
    }

    if (*current_deg == 1) {
    /* Maximum degree equals 10 and degree 1 is not written out,
    therefore digit '1' after '^' explicitly defines monomial's
    degree as 10. */
        *current_deg = 10;
    }

    newpolynomial[*current_deg] = *coefficient;
    *coefficient = 0;
    *positive = 1;
}

// The last monomials of degree 0 or 1 must be considered separately.
void in_last(int *newpolynomial, int *coefficient, int *positive, char *previous_character) {
    if (*positive == 1) {
        *coefficient = *coefficient * (-1);
    }

    if (*previous_character == 'x') {
        newpolynomial[1] = *coefficient;
        *positive = 1;
    }
    else {
        newpolynomial[0] = *coefficient;
        *positive = 1;
    }
}

void write_in(int *newpolynomial, char *operation) {
    *operation = (char)getchar();

    if (*operation != '.') {
    /* '.' ends the writing in, therefore after this character nothing is supposed to be
    written in. */
        char current_character = *operation;
        char previous_character = *operation;
        int coefficient = 0;
        int current_deg;
        int positive = 1;
        /* In order to avoid problems with minimum int, all monomials' coefficients
        are negative and in the end, if there was not '-' before, one
        is changed into positive value. */

        while (current_character != '\n') {
        // New polynomial is writting in till the end of line.
            if (current_character != ' ') {
            // Spaces does not affect anything in the program and they must be skipped.
                if ((current_character == '0') && (previous_character == *operation)){
                /* If '0' is written in as a first character after first '+' or '*',
                polynomial equals 0. */
                    reset(newpolynomial);
                }
                else {
                    if((current_character == 'x') && (coefficient == 0)){
                    /* There might be no coefficient next to a monomial of degree 1, but the
                    array that represents new polynomial needs to be actualised. */
                        coefficient = (-1);
                    }

                    if ((previous_character == 'x') && (current_character != '^')) {
                    /* If 'x' is not followed by '^', it is a monomial of degree 1 and must
                    be considered separately. */
                        in_monomial(newpolynomial, &current_deg, &positive, &coefficient);
                    }

                    if (current_character == '-') {
                        positive = 0;
                    }

                    if ((previous_character != '^') && (current_character >= '0') && (current_character <= '9')) {
                    // Updates coefficient.
                        newcoefficient(&coefficient, &current_character);
                    }
                    else if (previous_character == '^') {
                    // A digit that is after '^' defines monomial's degree.
                        current_deg = current_character - '0';
                        in_degree(newpolynomial, &current_deg, &positive, &coefficient);
                    }
                }

                previous_character = current_character;
            }

            current_character = (char)getchar();
        }

        if ((current_character == '\n') && (coefficient != 0)) {
        // In the end we need to write coefficients next to monomials of degree 0 and 1.
            in_last(newpolynomial, &coefficient, &positive, &previous_character);
        }
    }
}

int main() {
    int polynomial[MAX_DEG] = {0};
    int newpolynomial[MAX_DEG] = {0};
    char operation = ' ';
    /* At the beginning both polynomials equal 0.
    <polynomial> represents the resulted polynomial, which is supposed to be written out.
    <newpolynomial> represents the written in polynomial, by which <polynomial>
    is supposed to be multiplicated or which is supposed to be added. */

    while (operation != '.') {
        write_in(newpolynomial, &operation);

        if (operation == '+') {
            addition(polynomial, newpolynomial);
            write_out(polynomial);
        }

        if (operation == '*') {
            multiplication(polynomial, newpolynomial);
            write_out(polynomial);
        }

        reset(newpolynomial);
        /* After each round, <newpolynomial> must be equal 0 to prevent
        bugs from previous <newpolynomial>. */
    }

    return 0;
}
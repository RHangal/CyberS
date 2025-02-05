#include <stdio.h>
#include <stdlib.h>

#define NC 26 // Number of characters in the alphabet
#define A 65  // ASCII value of 'A'

// Rotors
const char rotors[][NC + 1] = {
    "BDFHJLCPRTXVZNYEIWGAKMUSQO", // Fast
    "AJDKSIRUXBLHWTMCQGZNPYFVOE", // Medium
    "EKMFLGDQVZNTOWYHXUSPAIBRCJ", // Slow
    "IXUHFEZDAOMTKQJWNSRLCYPBVG"  // Reflect
};

// Array to hold the extended rotors (including inverted versions)
char extended_rotors[7][NC + 1];

// Apply a rotor to a letter
char rapply(char c, const char *r) {
    return r[c - A];
}

// Invert a rotor
void invert(const char *r, char *inv) {
    for (int i = 0; i < NC; i++) {
        inv[r[i] - A] = A + i;
    }
    inv[NC] = '\0'; // Null-terminate the inverted rotor
}

// Precompute extended rotors (original + inverted)
void initialize_extended_rotors() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < NC; j++) {
            extended_rotors[i][j] = rotors[i][j];
        }
        extended_rotors[i][NC] = '\0'; // Null-terminate
    }

    // Add inverted rotors in reverse order
    for (int i = 0; i < 3; i++) {
        invert(rotors[2 - i], extended_rotors[4 + i]);
    }
}

// Shift a letter forward `n` positions
char nshift(char c, int n) {
    return (c - A + n + NC) % NC + A;
}

// Compute rotor shifts based on letter position and rotor index
int shifts(int l, int n) {
    switch (n) {
        case 0: return l % NC;
        case 1: return (l / NC) % NC;
        case 2: return (l / (NC * NC)) % NC;
        case 4: return (l / (NC * NC)) % NC;
        case 5: return (l / NC) % NC;
        case 6: return l % NC;
        default: return 0;
    }
}

// Shift, apply rotor, and unshift a letter
char shiply(char c, int n, const char *r) {
    return nshift(rapply(nshift(c, n), r), -n);
}

// Encrypt a single letter with the Enigma machine
char letter(char c, int l) {
    for (int i = 0; i < 7; i++) {
        c = shiply(c, shifts(l, i), extended_rotors[i]);
    }
    return c;
}

// Encrypt a phrase
void enigma(const char *input, char *output, int length) {
    for (int i = 0; i < length; i++) {
        output[i] = letter(input[i], i + 1);
    }
    output[length] = '\0'; // Null-terminate the output string
}

// Main function
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <plaintext>\n", argv[0]);
        return 1;
    }

    // Initialize extended rotors
    initialize_extended_rotors();

    char *input = argv[1];
    int length = 0;

    // Calculate input length (no string.h)
    while (input[length] != '\0') {
        length++;
    }

    char *output = (char *)malloc(length + 1);
    if (!output) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    // Encrypt the input
    enigma(input, output, length);

    // Print the encrypted output
    printf("%s\n", output);

    free(output);
    return 0;
}


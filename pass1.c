#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void displayFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Cannot open %s\n", filename);
        return;
    }
    printf("\nContents of %s:\n", filename);
    char ch;
    while ((ch = fgetc(fp)) != EOF)
        putchar(ch);
    fclose(fp);
}

void passOne() {
    char label[10], opcode[10], operand[10], code[10], mnemonic[3];
    int locctr, start = 0, length = 0;
    FILE *fp1, *fp2, *fp3, *fp4, *fp5;

    fp1 = fopen("source2.txt", "r");
    fp2 = fopen("optab.txt", "r");
    fp3 = fopen("symtab.txt", "w");
    fp4 = fopen("intermediate.txt", "w");
    fp5 = fopen("length.txt", "w");

    if (!fp1 || !fp2 || !fp3 || !fp4 || !fp5) {
        printf("Error opening files\n");
        return;
    }

    fscanf(fp1, "%s %s %s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        start = (int)strtol(operand, NULL, 16);
        locctr = start;
        fprintf(fp4, "\t%s\t%s\t%s\n", label, opcode, operand);
        fscanf(fp1, "%s %s %s", label, opcode, operand);
    } else {
        locctr = 0;
    }

    while (strcmp(opcode, "END") != 0) {
        fprintf(fp4, "%X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

        if (strcmp(label, "**") != 0) {
            fprintf(fp3, "%s\t%X\n", label, locctr);
        }

        rewind(fp2);
        int found = 0;
        while (fscanf(fp2, "%s %s", code, mnemonic) != EOF) {
            if (strcmp(opcode, code) == 0) {
                locctr += 3;
                found = 1;
                break;
            }
        }

        if (!found) {
            if (strcmp(opcode, "WORD") == 0)
                locctr += 3;
            else if (strcmp(opcode, "RESW") == 0)
                locctr += 3 * atoi(operand);
            else if (strcmp(opcode, "BYTE") == 0)
                locctr += 1; // Simplified
            else if (strcmp(opcode, "RESB") == 0)
                locctr += atoi(operand);
        }

        fscanf(fp1, "%s %s %s", label, opcode, operand);
    }

    fprintf(fp4, "%X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

    length = locctr - start;
    fprintf(fp5, "%X\n", length);

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);

    printf("\nPass 1 complete. Program length = %X\n", length);
    
    displayFile("intermediate.txt");
    displayFile("optab.txt");
    displayFile("symtab.txt");
    displayFile("length.txt");

}

int main() {
    passOne();
    return 0;
}


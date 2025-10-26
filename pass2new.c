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

void passTwo() {
    char label[20], opcode[20], operand[20];
    char symbol[20], symaddr[20], code[20], mnemonic[5];
    char objcode[20];
    int startAddr = 0, locctr = 0, length = 0;

    FILE *fp1 = fopen("intermediate.txt", "r");  
    FILE *fp2 = fopen("optab.txt", "r");
    FILE *fp3 = fopen("symtab.txt", "r");
    FILE *fp4 = fopen("output.txt", "w");        
    FILE *fp5 = fopen("length.txt", "r");

    if (!fp1 || !fp2 || !fp3 || !fp4 || !fp5) {
        printf("Error opening files for Pass 2.\n");
        return;
    }

    fscanf(fp5, "%X", &length);
    fclose(fp5);

    // Read first line of intermediate
    fscanf(fp1, "%s %s %s", label, opcode, operand);
    if (strcmp(opcode, "START") == 0) {
        startAddr = (int)strtol(operand, NULL, 16);
        fprintf(fp4, "H^%-6s^%06X^%06X\n", label, startAddr, length);
        fscanf(fp1, "%X %s %s %s", &locctr, label, opcode, operand);
    } else {
        fprintf(fp4, "H^%-6s^%06X^%06X\n", "**", startAddr, length);
    }

    int textStart = locctr, textLength = 0;
    char textRecord[200] = "";

    while (strcmp(opcode, "END") != 0) {
        int found = 0;
        rewind(fp2);

        
        while (fscanf(fp2, "%s %s", code, mnemonic) != EOF) {
            if (strcmp(opcode, code) == 0) {
                found = 1;
                
                rewind(fp3);
                int symFound = 0;
                while (fscanf(fp3, "%s %s", symbol, symaddr) != EOF) {
                    if (strcmp(operand, symbol) == 0) {
                        sprintf(objcode, "%s%s", mnemonic, symaddr);
                        symFound = 1;
                        break;
                    }
                }
                if (!symFound) {
                    sprintf(objcode, "%s0000", mnemonic);
                }
                if (strlen(textRecord) > 0) strcat(textRecord, "^");
                strcat(textRecord, objcode);
                textLength += 3;
                break;
            }
        }

        if (!found) {
            if (strcmp(opcode, "WORD") == 0) {
                sprintf(objcode, "%06X", atoi(operand));
                if (strlen(textRecord) > 0) strcat(textRecord, "^");
                strcat(textRecord, objcode);
                textLength += 3;
            } else if (strcmp(opcode, "BYTE") == 0) {
                if (operand[0] == 'C') {
                    int L = strlen(operand);
                    char temp[20] = "";
                    for (int i = 2; i < L-1; i++) {
                        char buf[5];
                        sprintf(buf, "%02X", operand[i]);
                        strcat(temp, buf);
                    }
                    strcpy(objcode, temp);
                    if (strlen(textRecord) > 0) strcat(textRecord, "^");
                    strcat(textRecord, objcode);
                    textLength += (L-3);
                } else if (operand[0] == 'X') {
                    strncpy(objcode, operand+2, strlen(operand)-3);
                    objcode[strlen(operand)-3] = '\0';
                    if (strlen(textRecord) > 0) strcat(textRecord, "^");
                    strcat(textRecord, objcode);
                    textLength += (strlen(objcode)/2);
                }
            }
            else if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0) {
                if (strlen(textRecord) > 0) {
                    fprintf(fp4, "T^%06X^%02X^%s\n", textStart, textLength, textRecord);
                    textRecord[0] = '\0';
                    textLength = 0;
                }
            }
        }

        fscanf(fp1, "%X %s %s %s", &locctr, label, opcode, operand);
        if (textLength == 0) textStart = locctr;
    }

   
    if (strlen(textRecord) > 0) {
        fprintf(fp4, "T^%06X^%02X^%s\n", textStart, textLength, textRecord);
    }

    int execAddr = startAddr;
    if (strcmp(operand, "**") != 0) {
        rewind(fp3);
        while (fscanf(fp3, "%s %s", symbol, symaddr) != EOF) {
            if (strcmp(operand, symbol) == 0) {
                execAddr = (int)strtol(symaddr, NULL, 16);
                break;
            }
        }
    }
    fprintf(fp4, "E^%06X\n", execAddr);

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);

    printf("\nPass 2 complete. Object program generated in output.txt\n");
    displayFile("output.txt");
}

int main() {
    passTwo();
    return 0;
}

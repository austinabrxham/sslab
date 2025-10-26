#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *inter, *optab, *symtab, *object;
    char label[30], opcode[15], operand[15];
    char opt_opcode[15], opt_value[15];
    char sym_label[30];
    int sym_addr, locctr, start_addr = 0, program_length = 0;

    inter = fopen("intermediate.txt", "r");
    optab = fopen("optab.txt", "r");
    symtab = fopen("symtab.txt", "r");
    object = fopen("objectcode.txt", "w");

    if (!inter || !optab || !symtab || !object)
    {
        printf("Error opening files.\n");
        return 1;
    }

    fscanf(inter, "%x %s %s %s", &locctr, label, opcode, operand);

    if (strcmp(opcode, "START") == 0)
    {
        start_addr = (int)strtol(operand, NULL, 16);
        fprintf(object, "H^%-6s^%06X^%06X", label, start_addr, 0);
        fscanf(inter, "%x %s %s %s", &locctr, label, opcode, operand);
    }

    fprintf(object, "\nT^%06X^06^", locctr);

    while (strcmp(opcode, "END") != 0)
    {
        int found = 0;

        rewind(optab);
        while (fscanf(optab, "%s %s", opt_opcode, opt_value) == 2)
        {
            if (strcmp(opcode, opt_opcode) == 0)
            {
                rewind(symtab);
                int sym_found = 0;
                while (fscanf(symtab, "%s %x", sym_label, &sym_addr) == 2)
                {
                    if (strcmp(sym_label, operand) == 0)
                    {
                        fprintf(object, "%s%04X^", opt_value, sym_addr);
                        sym_found = 1;
                        break;
                    }
                }
                if (!sym_found)
                {
                    fprintf(object, "%s0000^", opt_value);
                }
                found = 1;
                break;
            }
        }

        if (!found)
        {
            if (strcmp(opcode, "WORD") == 0)
            {
                fprintf(object, "%06X^", atoi(operand));
            }
            else if (strcmp(opcode, "BYTE") == 0)
            {
                if (operand[0] == 'C')
                {
                    for (int i = 2; i < strlen(operand) - 1; i++)
                        fprintf(object, "%02X", operand[i]);
                    fprintf(object, "^");
                }
                else if (operand[0] == 'X')
                {
                    fprintf(object, "%02X^", (int)strtol(&operand[2], NULL, 16));
                }
            }
        }

        fscanf(inter, "%x %s %s %s", &locctr, label, opcode, operand);
    }

    program_length = locctr - start_addr;
    fseek(object, 16, SEEK_SET);
    fprintf(object, "%06X", program_length);
    fseek(object, 0, SEEK_END);

    fprintf(object, "\nE^%06X\n", start_addr);

    printf("PASS 2 completed! Object code written to objectcode.txt\n");

    fclose(inter);
    fclose(optab);
    fclose(symtab);
    fclose(object);

    return 0;
}

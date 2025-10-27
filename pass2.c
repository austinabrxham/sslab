#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void displayFile(const char *filename)
{
  FILE *fp = fopen(filename, "r");
  printf("%s\n", filename);

  char line_buffer[300];
  while (fgets(line_buffer, sizeof(line_buffer), fp) != NULL)
  {
    printf("%s", line_buffer);
  }
  printf("\n");

  fclose(fp);
}

void main()
{
  char label[20], opcode[20], operand[20], mnemonic[20], code[20], sym_label[20];
  int sym_address, locctr, start = 0, length = 0;

  FILE *f1 = fopen("intermediate.txt", "r");
  FILE *f2 = fopen("optab.txt", "r");
  FILE *f3 = fopen("symtab.txt", "r");
  FILE *f4 = fopen("object.txt", "w");

  fscanf(f1, "%x %s %s %s", &locctr, label, opcode, operand);

  if (strcmp(opcode, "START") == 0)
  {
    start = (int)strtol(operand, NULL, 16);
    fprintf(f4, "H^%-6s^%06X^%06X\n", label, start, 0);
    fscanf(f1, "%x %s %s %s", &locctr, label, opcode, operand);
  }

  fprintf(f4, "T^%06X^06^", locctr);

  while (strcmp(opcode, "END") != 0)
  {
    int found = 0;
    rewind(f2);
    while (fscanf(f2, "%s %s", mnemonic, code) != EOF)
    {
      if (strcmp(mnemonic, opcode) == 0)
      {
        int sym_found = 0;
        rewind(f3);
        while (fscanf(f3, "%s %x", sym_label, &sym_address) != EOF)
        {
          if (strcmp(sym_label, operand) == 0)
          {
            fprintf(f4, "%s%04X^", code, sym_address);
            sym_found = 1;
            break;
          }
        }
        if (!sym_found)
        {
          fprintf(f4, "%s0000", code);
        }
        found = 1;
        break;
      }
    }
    if (!found)
    {
      if (strcmp(opcode, "WORD") == 0)
      {
        fprintf(f4, "%06X^", atoi(operand));
      }
      else if (strcmp(opcode, "BYTE") == 0)
      {
        if (operand[0] == 'C')
        {
          for (int i = 2; i < strlen(operand) - 1; i++)
          {
            fprintf(f4, "%02X", operand[i]);
          }
          fprintf(f4, "^");
        }
        else if (operand[0] == 'X')
        {
          fprintf(f4, "%02X^", (int)strtol(&operand[2], NULL, 16));
        }
      }
    }

    fscanf(f1, "%x %s %s %s", &locctr, label, opcode, operand);
  }
  length = locctr - start;
  fprintf(f4, "\nE^%06X", start);

  fclose(f4);

  f4 = fopen("object.txt", "r+");

  fseek(f4, 16, SEEK_SET);
  fprintf(f4, "%06X", length);
  fclose(f4);

  printf("PASS 2 completed! Object code written to objectcode.txt\n");
  displayFile("object.txt");

  fclose(f1);
  fclose(f2);
  fclose(f3);
}

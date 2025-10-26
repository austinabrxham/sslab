#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void displayFile(const char *filename)
{
  FILE *fp = fopen(filename, "r");
  char ch;
  printf("\nContents of %s\n", filename);
  while ((ch = fgetc(fp)) != EOF)
  {
    putchar(ch);
  }
  fclose(fp);
}

void passOne()
{
  char label[10], opcode[10], operand[10], mnemonic[10], code[10];
  int locctr, start = 0, length = 0;

  FILE *fp1 = fopen("input.txt", "r");
  FILE *fp2 = fopen("optab.txt", "r");
  FILE *fp3 = fopen("symtab.txt", "w");
  FILE *fp4 = fopen("intermediate.txt", "w");
  FILE *fp5 = fopen("length.txt", "w");

  fscanf(fp1, "%s %s %s", label, opcode, operand);

  if (strcmp(opcode, "START") == 0)
  {

    start = (int)strtol(operand, NULL, 16);
    locctr = start;
    fprintf(fp4, "\t%s\t%s\t%s\n", label, opcode, operand);

    fscanf(fp1, "%s %s %s", label, opcode, operand);
  }
  else
  {
    locctr = 0;
  }

  while (strcmp(opcode, "END") != 0)
  {
    fprintf(fp4, "%X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

    if (strcmp(label, "**") != 0)
    {
      fprintf(fp3, "%s\t%X\n", label, locctr);
    }
    rewind(fp2);
    int found = 0;

    while ((fscanf(fp2, "%s %s", mnemonic, code)) != EOF)
    {
      if (strcmp(opcode, mnemonic) == 0)
      {
        locctr += 3;
        found = 1;
        break;
      }
    }
    if (!found)
    {
      if (strcmp(opcode, "WORD") == 0)
      {
        locctr += 3;
      }
      else if (strcmp(opcode, "RESW") == 0)
      {
        locctr += 3 * atoi(operand);
      }
      else if (strcmp(opcode, "BYTE") == 0)
      {
        if (operand[0] == 'C' || operand[0] == 'c')
        {
          // Character constant: C'EOF' = 3 bytes
          locctr += strlen(operand) - 3; // Remove C' and '
        }
        else if (operand[0] == 'X' || operand[0] == 'x')
        {
          // Hex constant: X'F1' = 1 byte, X'F1A2' = 2 bytes
          locctr += (strlen(operand) - 3) / 2; // Remove X' and ', divide by 2
        }
        else
        {
          locctr += 1; // Default case
        }
      }
      else if (strcmp(opcode, "RESB") == 0)
      {
        locctr += atoi(operand);
      }
    }

    fscanf(fp1, "%s %s %s", label, opcode, operand);
  }
  fprintf(fp4, "%X\t%s\t%s\t%s\n", locctr, label, opcode, operand);
  length = locctr - start;
  fprintf(fp5, "%X", length);

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

int main()
{
  passOne();
  return 0;
}

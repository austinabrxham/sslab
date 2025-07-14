#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_BLOCKS 100
#define MAX_FILES 20
#define NAME_LEN 20

typedef struct {
    char name[NAME_LEN];
    int start;
    int length;
} File;

bool disk[MAX_BLOCKS];       // false means free, true means allocated
File files[MAX_FILES];
int fileCount = 0;

void initializeDisk() {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        disk[i] = false;
    }
}

void displayDisk() {
    printf("Disk Status:\n");
    for (int i = 0; i < MAX_BLOCKS; i++) {
        printf("%d", disk[i] ? 1 : 0);
        if ((i + 1) % 10 == 0)
            printf("\n");
    }
}

void displayFiles() {
    printf("\nFile Allocation Table:\n");
    printf("Filename\tStart\tLength\n");
    for (int i = 0; i < fileCount; i++) {
        printf("%s\t\t%d\t%d\n", files[i].name, files[i].start, files[i].length);
    }
}

bool allocateFile(char filename[], int length) {
    if (fileCount >= MAX_FILES) {
        printf("Error: File table full.\n");
        return false;
    }

    for (int i = 0; i <= MAX_BLOCKS - length; i++) {
        bool canAllocate = true;
        for (int j = 0; j < length; j++) {
            if (disk[i + j]) {
                canAllocate = false;
                break;
            }
        }

        if (canAllocate) {
            for (int j = 0; j < length; j++) {
                disk[i + j] = true;
            }
            strcpy(files[fileCount].name, filename);
            files[fileCount].start = i;
            files[fileCount].length = length;
            fileCount++;
            printf("File '%s' allocated from block %d to %d.\n", filename, i, i + length - 1);
            return true;
        }
    }

    printf("Error: Not enough contiguous space for file '%s'.\n", filename);
    return false;
}

int main() {
    int choice;
    initializeDisk();

    while (1) {
        printf("\n--- Sequential File Allocation Menu ---\n");
        printf("1. Allocate File\n");
        printf("2. Display Disk Status\n");
        printf("3. Display File Table\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            char name[NAME_LEN];
            int size;
            printf("Enter filename: ");
            scanf("%s", name);
            printf("Enter number of blocks needed: ");
            scanf("%d", &size);
            allocateFile(name, size);
        } else if (choice == 2) {
            displayDisk();
        } else if (choice == 3) {
            displayFiles();
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

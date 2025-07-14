#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_BLOCKS 100
#define MAX_FILES 20
#define NAME_LEN 20

typedef struct {
    char name[NAME_LEN];
    int indexBlock;
    int dataBlocks[20]; // Each file can use up to 20 data blocks
    int blockCount;
} File;

bool disk[MAX_BLOCKS];       // false = free, true = allocated
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
        if ((i + 1) % 10 == 0) printf("\n");
    }
}

void displayFiles() {
    printf("\nFile Allocation Table (Indexed):\n");
    printf("Filename\tIndex Block\tData Blocks\n");
    for (int i = 0; i < fileCount; i++) {
        printf("%s\t\t%d\t\t", files[i].name, files[i].indexBlock);
        for (int j = 0; j < files[i].blockCount; j++) {
            printf("%d ", files[i].dataBlocks[j]);
        }
        printf("\n");
    }
}

bool allocateIndexedFile(char filename[], int blockCount, int indexBlock, int dataBlockList[]) {
    if (fileCount >= MAX_FILES) {
        printf("Error: File table is full.\n");
        return false;
    }

    if (disk[indexBlock]) {
        printf("Error: Index block %d is already allocated.\n", indexBlock);
        return false;
    }

    for (int i = 0; i < blockCount; i++) {
        if (dataBlockList[i] < 0 || dataBlockList[i] >= MAX_BLOCKS) {
            printf("Error: Block number %d is invalid.\n", dataBlockList[i]);
            return false;
        }
        if (disk[dataBlockList[i]]) {
            printf("Error: Data block %d is already allocated.\n", dataBlockList[i]);
            return false;
        }
    }

    // Allocation successful
    disk[indexBlock] = true;
    for (int i = 0; i < blockCount; i++) {
        disk[dataBlockList[i]] = true;
        files[fileCount].dataBlocks[i] = dataBlockList[i];
    }

    strcpy(files[fileCount].name, filename);
    files[fileCount].indexBlock = indexBlock;
    files[fileCount].blockCount = blockCount;
    fileCount++;

    printf("File '%s' allocated with index block %d.\n", filename, indexBlock);
    return true;
}

int main() {
    int choice;
    initializeDisk();

    while (1) {
        printf("\n--- Indexed File Allocation Menu ---\n");
        printf("1. Allocate File\n");
        printf("2. Display Disk Status\n");
        printf("3. Display File Table\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            char name[NAME_LEN];
            int blockCount, indexBlock, dataBlocks[20];

            printf("Enter filename: ");
            scanf("%s", name);
            printf("Enter number of blocks required: ");
            scanf("%d", &blockCount);

            if (blockCount <= 0 || blockCount > 20) {
                printf("Invalid block count. Max 20 allowed.\n");
                continue;
            }

            printf("Enter index block number (0 to %d): ", MAX_BLOCKS - 1);
            scanf("%d", &indexBlock);

            printf("Enter %d data block numbers (0 to %d):\n", blockCount, MAX_BLOCKS - 1);
            for (int i = 0; i < blockCount; i++) {
                scanf("%d", &dataBlocks[i]);
            }

            allocateIndexedFile(name, blockCount, indexBlock, dataBlocks);

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

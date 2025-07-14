#include <stdio.h>
#include <stdlib.h>

#define MAX_BLOCKS 100

// Structure to represent an index block
struct IndexBlock {
    int data_blocks[MAX_BLOCKS];
};

// Initialize index block with -1 (unallocated)
void initializeIndexBlock(struct IndexBlock* indexBlock) {
    for (int i = 0; i < MAX_BLOCKS; ++i) {
        indexBlock->data_blocks[i] = -1;
    }
}

// Allocate a data block to the index block
int allocateDataBlock(struct IndexBlock* indexBlock, int blockNumber) {
    for (int i = 0; i < MAX_BLOCKS; ++i) {
        if (indexBlock->data_blocks[i] == -1) {
            indexBlock->data_blocks[i] = blockNumber;
            return 1; // Success
        }
    }
    return 0; // Index block full
}

// Read data from a specific block number
void readData(struct IndexBlock* indexBlock, int fileBlockNumber) {
    int flag = 0;
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (indexBlock->data_blocks[i] == fileBlockNumber) {
            printf("Reading data from block %d\n", indexBlock->data_blocks[i]);
            flag = 1;
            break;
        }
    }

    if (!flag)
        printf("Block %d is not allocated.\n", fileBlockNumber);
}

int main() {
    struct IndexBlock indexBlock;
    initializeIndexBlock(&indexBlock);

    int choice;

    while (1) {
        printf("\n--- Indexed File Allocation Menu ---\n");
        printf("1. Allocate Data Block\n");
        printf("2. Read Data from Block\n");
        printf("3. Display Allocated Blocks\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            int blockNumber;
            printf("Enter block number to allocate (0 to %d): ", MAX_BLOCKS - 1);
            scanf("%d", &blockNumber);

            if (blockNumber < 0 || blockNumber >= MAX_BLOCKS) {
                printf("Invalid block number!\n");
                continue;
            }

            int success = allocateDataBlock(&indexBlock, blockNumber);
            if (success)
                printf("Block %d allocated successfully.\n", blockNumber);
            else
                printf("Index block is full. Cannot allocate more blocks.\n");

        } else if (choice == 2) {
            int fileBlock;
            printf("Enter block number to read: ");
            scanf("%d", &fileBlock);
            readData(&indexBlock, fileBlock);

        } else if (choice == 3) {
            printf("Allocated blocks: ");
            for (int i = 0; i < MAX_BLOCKS; i++) {
                if (indexBlock.data_blocks[i] != -1)
                    printf("%d ", indexBlock.data_blocks[i]);
            }
            printf("\n");

        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

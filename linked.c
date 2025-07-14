#include <stdio.h>
#include <stdlib.h>

// Define the maximum number of blocks
#define MAX_BLOCKS 100

// Structure to represent a block in linked allocation
struct Block {
    int block_number;
    struct Block* next;
};

// Function to allocate a new block
struct Block* allocateBlock(int blockNumber) {
    struct Block* newBlock = (struct Block*)malloc(sizeof(struct Block));
    newBlock->block_number = blockNumber;
    newBlock->next = NULL;
    return newBlock;
}

int main() {
    struct Block* head = NULL;
    struct Block* current = NULL;
    int numBlocks;

    printf("Enter the number of blocks to allocate for the file: ");
    scanf("%d", &numBlocks);

    if (numBlocks <= 0 || numBlocks > MAX_BLOCKS) {
        printf("Invalid number of blocks. Must be between 1 and %d.\n", MAX_BLOCKS);
        return 1;
    }

    printf("Enter the block numbers (0 to %d):\n", MAX_BLOCKS - 1);
    for (int i = 0; i < numBlocks; i++) {
        int blockNumber;
        scanf("%d", &blockNumber);

        if (blockNumber < 0 || blockNumber >= MAX_BLOCKS) {
            printf("Invalid block number: %d. Try again.\n", blockNumber);
            i--;
            continue;
        }

        struct Block* newBlock = allocateBlock(blockNumber);
        if (head == NULL) {
            head = newBlock;
            current = head;
        } else {
            current->next = newBlock;
            current = newBlock;
        }
    }

    // Display the allocated blocks
    printf("Allocated blocks: ");
    current = head;
    while (current != NULL) {
        printf("%d ", current->block_number);
        current = current->next;
    }
    printf("\n");

    // Clean up (free memory)
    current = head;
    while (current != NULL) {
        struct Block* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}

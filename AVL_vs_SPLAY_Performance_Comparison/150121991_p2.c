// Name: Niyazi Ozan Ateþ - StudentNumber: 150121991 - ProjectNumber: 2
#include <stdio.h>

// Here we struct the node for creating the AVL and SPLAY tree. It contains the value, height, occurrence it will hold and two pointers for their child nodes.
struct node {
	int value;
	int height;
	int occurrence;
	struct node *leftPtr;
	struct node *rightPtr;
};

// Here we typedef our node and its pointer so that writing the code would by simplified and reading the code would be easier.
typedef struct node node;
typedef node *nodePtr;

// These are our function prototypes.
void insertToSPLAY(int number, int *timeUnits, int *splaycontrol, int *currentDepth, nodePtr *currentPtr);
void insertToAVL(int number, int *timeUnits, nodePtr *currentPtr);
int maxHeight(int num1, int num2);
int findHeight(nodePtr findNodesHeight);
void rightRotation(nodePtr *startPtr);
void leftRotation(nodePtr *startPtr);
void doubleLeftRightRotation(nodePtr *startPtr);
void doubleRightLeftRotation(nodePtr *startPtr);
void printPreOrder(nodePtr currentNode);

// This is our main function where the main tasks will be performed.
int main(int argc, char* argv[]){
	
	if(argc==1) {
		printf("Error message!");
	}
    if(argc>=2) {
    	// This is our pointer to the input text.
		FILE *fptrInput = fopen(argv[1], "r");
		
		// Here we create our roots for the AVL and SPLAY trees.
		nodePtr rootAVLPtr = malloc(sizeof(node));
		rootAVLPtr = NULL;
		nodePtr rootSPLAYPtr = malloc(sizeof(node));
		rootSPLAYPtr = NULL;
	
		char c; // Is used to get the next char from the text file.
		int number = 0; // Creates the number from the text file.
		int timeUnitsAVL = 0; // Keeps track of the time units of the AVL tree.
		int timeUnitsSPLAY = 0; // Keeps track of the time units of the SPLAY tree.
		int splayControl = 0; // This tells us in the insertToSPLAY function if we need a rotation or not.
		int splayDepth = 0; // This keeps track of the seached/inserted node its depth while it makes its way to the root. Also tells us what kind of rotation should be performed.
	
		/* We accepted that the numbers in the text file are all positive numbers.
		   With the outer while loop we keep track if we are at the end of the file. It also gets the first digit of the new number.
		   The inner loop checks if we read a digit and if it is a digit, we perform the needed operations to create the number that the text file holds.
		   When we got our number, we check if we didn't have read the char value 10 or -1 which sometimes can happen.
		   If that is not the case, then we reset our splayControl and splayDepth variables. Also, we insert the new number to the AVL and SPLAY tree.
		*/
		while (!feof(fptrInput)) {
			(c = fgetc(fptrInput));
			number = (c-48);
			while (48 <= c && c <=57) {
				(c = fgetc(fptrInput));
				if (48 <= c && c <=57) {
					number = number * 10 + c - 48;
				}
			}
			if (c != 10 && c != -1) {
				splayControl = 0;
				splayDepth = 0;
				insertToAVL(number, &timeUnitsAVL, &rootAVLPtr);
				insertToSPLAY(number, &timeUnitsSPLAY, &splayControl, &splayDepth, &rootSPLAYPtr);
			}
		}
	
		// Here we print the needed information.
		printf("AVL tree:\n");
		printPreOrder(rootAVLPtr);
		printf("\nTotal cost: %d", timeUnitsAVL);
		printf("\n\nSPLAY tree:\n");
		printPreOrder(rootSPLAYPtr);
		printf("\nTotal cost: %d", timeUnitsSPLAY);
	
		// At last we close our text file pointer for the safety.
		fclose(fptrInput);
		return 0;
	}
}

/* This function creates our SPLAY tree. It will be used as a recursive function.
   It takes as the input args the number, timeUnits, splaycontrol, currentDepth and currentPtr.
   It has the basic Binary Search Tree operation. But while it is recursing backward it also rotates and hold the time unit information.
*/ 
void insertToSPLAY(int number, int *timeUnits, int *splaycontrol, int *currentDepth, nodePtr *currentPtr) {
	
	if (*currentPtr == NULL) { // If the currentPtr is NULL, this means that we are at a pointer of a leave and need to put the number in that spot.
		nodePtr tempPtr = malloc(sizeof(node));
		if (tempPtr != NULL) { // Checks if we have enough space.
			tempPtr->value = number;
			tempPtr->height = 0;
			tempPtr->occurrence = 1;
			tempPtr->leftPtr = NULL;
			tempPtr->rightPtr = NULL;
			*currentPtr = tempPtr;
			(*splaycontrol) = 0;
		}
		else { // Prints an error message in case that there is not enough space.
			printf("Not enough space available to create a Node!");
		}
		return;
	}
	else if (number < (*currentPtr)->value) { // If the number is smaller than the value in the current node, then we have to move to the left pointer of that node.
		(*currentDepth)++;
		(*timeUnits)++; // Each comparsation increases the time unit by 1.
		insertToSPLAY(number, timeUnits, splaycontrol, currentDepth, &(*currentPtr)->leftPtr);
	}
	else if (number > (*currentPtr)->value) { // If the number is bigger than the value in the current node, then we have to move to the right pointer of that node.
		(*currentDepth)++;
		(*timeUnits)++; // Each comparsation increases the time unit by 1.
		insertToSPLAY(number, timeUnits, splaycontrol, currentDepth, &(*currentPtr)->rightPtr);
	}
	else if (number == (*currentPtr)->value) { // If the number is equal to the value in the current node, then we have to increase the occurrence by 1.
		(*timeUnits)++; // Each comparsation increases the time unit by 1.
		(*currentPtr)->occurrence++;
		return;
	}

	if ((*currentDepth) > 1) { // This checks if our node with our number is at a higher depth level than 1.
		if ((*splaycontrol) == 1) { // This checks if the difference between our node and the current node is bigger than 1.
			if ((*currentPtr)->leftPtr != NULL) { // We have to check if the left pointer is not NULL otherwise we can't compare the number with the node values.
				if ((*currentPtr)->leftPtr->leftPtr != NULL && number == (*currentPtr)->leftPtr->leftPtr->value) { // Here we check if the number is a ZIG-ZIG situation with our number smaller than the current value.
					rightRotation(&(*currentPtr));
					rightRotation(&(*currentPtr));
					(*timeUnits) = (*timeUnits) + 2; // Because of the double rotation we add 2 time units.
				}
				else if ((*currentPtr)->leftPtr->rightPtr != NULL && number == (*currentPtr)->leftPtr->rightPtr->value) { // Here we check if the number is a ZIG-ZAG situation with our number smaller than the current value.
					doubleLeftRightRotation(&(*currentPtr));
					(*timeUnits) = (*timeUnits) + 2; // Because of the double rotation we add 2 time units.
				}
			}
			if ((*currentPtr)->rightPtr != NULL) { // We have to check if the right pointer is not NULL otherwise we can't compare the number with the node values.
				if ((*currentPtr)->rightPtr->rightPtr != NULL && number == (*currentPtr)->rightPtr->rightPtr->value) { // Here we check if the number is a ZIG-ZIG situation with our number bigger than the current value.
					leftRotation(&(*currentPtr));
					leftRotation(&(*currentPtr));
					(*timeUnits) = (*timeUnits) + 2; // Because of the double rotation we add 2 time units.
				}
				else if ((*currentPtr)->rightPtr->leftPtr != NULL && number == (*currentPtr)->rightPtr->leftPtr->value) { // Here we check if the number is a ZIG-ZAG situation with our number bigger than the current value.
					doubleRightLeftRotation(&(*currentPtr));
					(*timeUnits) = (*timeUnits) + 2; // Because of the double rotation we add 2 time units.
				}
			}
			(*currentDepth) = (*currentDepth) - 2; // We update the currentDepth variable.
			(*splaycontrol) = 0; // We update the splaycontrol variable because the space difference between our number and the node value will be 1.
		}
		else if ((*splaycontrol) == 0) { // If the difference between our node and the current node is not bigger than 1, then we update the control variable. This is because next time the difference will be 2.
			(*splaycontrol) = 1;
		}
	}
	else if ((*currentDepth) == 1) { // If the current depth is 1, then we have to perform a single rotation.
		if ((*currentPtr)->leftPtr != NULL) { // We have to check if the left pointer is not NULL otherwise we can't compare the number with the node values.
			if (number == (*currentPtr)->leftPtr->value) { // If the number is equal to the value of the left we use a single right rotation.
				rightRotation(&(*currentPtr));
				(*timeUnits)++; // Because of the single rotation we add 1 time unit.
			}
		}
		if ((*currentPtr)->rightPtr != NULL) { // We have to check if the right pointer is not NULL otherwise we can't compare the number with the node values.
			if (number == (*currentPtr)->rightPtr->value) { // If the number is equal to the value of the right we use a single left rotation.
				leftRotation(&(*currentPtr));
				(*timeUnits)++; // Because of the single rotation we add 1 time unit.
			}
		}
	}
	return;
}

/* This function creates our AVL tree. It will be used as a recursive function.
   It takes as the input args the number, timeUnits and currentPtr.
   It has the basic Binary Search Tree operation. But while it is recursing backward it also rotates and hold the time unit information.
*/ 
void insertToAVL(int number, int *timeUnits, nodePtr *currentPtr) {
	
	if (*currentPtr == NULL) { // If the currentPtr is NULL, this means that we are at a pointer of a leave and need to put the number in that spot.
		nodePtr tempPtr = malloc(sizeof(node));
		tempPtr->value = number;
		tempPtr->height = 0;
		tempPtr->occurrence = 1;
		tempPtr->leftPtr = NULL;
		tempPtr->rightPtr = NULL;
		*currentPtr = tempPtr;
		return;
	}
	else if (number < (*currentPtr)->value) { // If the number is smaller than the value in the current node, then we have to move to the left pointer of that node.
		(*timeUnits)++; // Each comparsation increases the time unit by 1.
		insertToAVL(number, timeUnits, &(*currentPtr)->leftPtr);
	}
	else if (number > (*currentPtr)->value) { // If the number is bigger than the value in the current node, then we have to move to the right pointer of that node.
		(*timeUnits)++; // Each comparsation increases the time unit by 1.
		insertToAVL(number, timeUnits, &(*currentPtr)->rightPtr);
	}
	else if (number == (*currentPtr)->value) { // If the number is equal to the value in the current node, then we have to increase the occurrence by 1.
		(*timeUnits)++; // Each comparsation increases the time unit by 1.
		(*currentPtr)->occurrence++;
		return;
	}
	
	// These variable will represent the currentPtr its height and the difference between the height of the left and right nodes.
	(*currentPtr)->height = 1 + maxHeight(findHeight((*currentPtr)->leftPtr), findHeight((*currentPtr)->rightPtr));
	int difference = findHeight((*currentPtr)->leftPtr) - findHeight((*currentPtr)->rightPtr);
	
	if (difference > 1 && number < (*currentPtr)->leftPtr->value) { // If the difference between the left and right child is bigger than 1 and the value of the left child is bigger than the number, then we do a single right rotation.
		(*timeUnits)++; // Because of the single rotation we add 1 time unit.
		rightRotation(&(*currentPtr));
		return;
	}
	else if (difference < -1 && number > (*currentPtr)->rightPtr->value) { // If the difference between the right and left child is bigger than 1 and the value of the right child is smaller than the number, then we do a single left rotation.
		(*timeUnits)++; // Because of the single rotation we add 1 time unit.
		leftRotation(&(*currentPtr));
		return;
	}
	else if (difference > 1 && number > (*currentPtr)->leftPtr->value) { // If the difference between the left and right child is bigger than 1 and the value of the left child is smaller than the number, then we do a double (L/R) rotation.
		(*timeUnits) = (*timeUnits) + 2; // Because of the double rotation we add 2 time units.
		doubleLeftRightRotation(&(*currentPtr));
		return;
	}
	else if (difference < -1 && number < (*currentPtr)->rightPtr->value) { // If the difference between the right and left child is bigger than 1 and the value of the right child is bigger than the number, then we do a double (R/L) rotation.
		(*timeUnits) = (*timeUnits) + 2; // Because of the double rotation we add 2 time units.
		doubleRightLeftRotation(&(*currentPtr));
		return;
	}
	return;
}

// This function finds the maximum height between two numbers.
int maxHeight(int num1, int num2) {
	if (num1 > num2) {
		return num1;
	}
	else {
		return num2;
	}
}

// This function returns the height of a node if it exists. Also, it helps us to avoid errors.
int findHeight(nodePtr findNodesHeight) {
	if (findNodesHeight == NULL) {
		return -1;
	}
	return findNodesHeight->height;
}

// This is our single right rotation function.
void rightRotation(nodePtr *startPtr) {
	nodePtr tempPtr = (*startPtr);
	(*startPtr) = (*startPtr)->leftPtr;
	tempPtr->leftPtr = (*startPtr)->rightPtr;
	(*startPtr)->rightPtr = tempPtr;
	// We will update the heights of the updated trees. By doing this we avoid problem in the recursive comparison.
	tempPtr->height = 1 + maxHeight(findHeight((tempPtr)->leftPtr), findHeight((tempPtr)->rightPtr));
	(*startPtr)->height = 1 + maxHeight(findHeight((*startPtr)->leftPtr), findHeight((*startPtr)->rightPtr));
	return;
}

// This is our single left rotation function.
void leftRotation(nodePtr *startPtr) {
	nodePtr tempPtr = (*startPtr);
	(*startPtr) = (*startPtr)->rightPtr;
	tempPtr->rightPtr = (*startPtr)->leftPtr;
	(*startPtr)->leftPtr = tempPtr;
	// We will update the heights of the updated trees. By doing this we avoid problem in the recursive comparison.
	tempPtr->height = 1 + maxHeight(findHeight((tempPtr)->leftPtr), findHeight((tempPtr)->rightPtr));
	(*startPtr)->height = 1 + maxHeight(findHeight((*startPtr)->leftPtr), findHeight((*startPtr)->rightPtr));
	return;
}

// This is our double (R/L) rotation function.
void doubleLeftRightRotation(nodePtr *startPtr) {
	nodePtr tempPtr = (*startPtr)->leftPtr->rightPtr;
	(*startPtr)->leftPtr->rightPtr = tempPtr->leftPtr;
	tempPtr->leftPtr = (*startPtr)->leftPtr;
	(*startPtr)->leftPtr = tempPtr->rightPtr;
	tempPtr->rightPtr = (*startPtr);
	(*startPtr) = tempPtr;
	// We will update the heights of the updated trees. By doing this we avoid problem in the recursive comparison.
	tempPtr->leftPtr->height = 1 + maxHeight(findHeight((tempPtr)->leftPtr->leftPtr), findHeight((tempPtr)->leftPtr->rightPtr));
	tempPtr->rightPtr->height = 1 + maxHeight(findHeight((tempPtr)->rightPtr->leftPtr), findHeight((tempPtr)->rightPtr->rightPtr));
	(*startPtr)->height = 1 + maxHeight(findHeight((*startPtr)->leftPtr), findHeight((*startPtr)->rightPtr));
	return;
}

// This is our double (L/R) rotation function.
void doubleRightLeftRotation(nodePtr *startPtr) {
	nodePtr tempPtr = (*startPtr)->rightPtr->leftPtr;
	(*startPtr)->rightPtr->leftPtr = tempPtr->rightPtr;
	tempPtr->rightPtr = (*startPtr)->rightPtr;
	(*startPtr)->rightPtr = tempPtr->leftPtr;
	tempPtr->leftPtr = (*startPtr);
	(*startPtr) = tempPtr;
	// We will update the heights of the updated trees. By doing this we avoid problem in the recursive comparison.
	tempPtr->leftPtr->height = 1 + maxHeight(findHeight((tempPtr)->leftPtr->leftPtr), findHeight((tempPtr)->leftPtr->rightPtr));
	tempPtr->rightPtr->height = 1 + maxHeight(findHeight((tempPtr)->rightPtr->leftPtr), findHeight((tempPtr)->rightPtr->rightPtr));
	(*startPtr)->height = 1 + maxHeight(findHeight((*startPtr)->leftPtr), findHeight((*startPtr)->rightPtr));
	return;
}

// This function prints our trees in a pre-order (NLR) form.
void printPreOrder(nodePtr currentNode) {
	if (currentNode != NULL) {
		printf("%d ", currentNode->value);
		printPreOrder(currentNode->leftPtr);
    	printPreOrder(currentNode->rightPtr);
  	}
  	return;
}

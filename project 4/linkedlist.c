#include "linkedlist.h"

// PRIVATE OPERATIONS
int		_copyElement(Element src, Element *dest) {
    // To set a new Element to a Node in the list.
	int i = 0, error = LIST_NO_ERROR;

	dest->items = (Item **) malloc (sizeof(Item *) * src.n_items);

	if (NULL != dest->items) {
	    for (i = 0; i < src.n_items; i++) {
		    dest->items[i] = src.items[i];
		}

		dest->n_items = src.n_items;
		dest->price = src.price;
	} else {
	    error = LIST_ERROR_MALLOC;
	}

	return error;
}

int 	_isAtEnd (LinkedList list) {
	// To check if the list is at the end (POV after the last element) we 
	//  need to check if there is any Node after the previous pointer.
	return NULL == list.previous->next;
}

void 	_goToHead (LinkedList * list) {
	// To move the POV to the first element in the list, we need to point
	//  whoever is before the first element. That is the phantom node.
	list->previous = list->head;
}

void 	_next (LinkedList * list) {
	// We cannot move to the next element if the POV is not valid.
	// The POV will not be valid when the previous pointer points to the last
	//  node in the list (there is noone after PREVIOUS).
	if (_isAtEnd (*list)) {
		list->error = LIST_ERROR_END;
	}
	else {
		// Move the POV to the next element.
		list->previous = list->previous->next;

		// If there are no errors, set error code to NO_ERROR.
		list->error = LIST_NO_ERROR;
	}
}

int 	_searchPosition(LinkedList * list, int position) {
	int pos = 0;
	// To place the POV to the specified position, we need to start by the 
	//  beginning. So, we first move the POV to the head of the list.
	_goToHead (list);

	// Then we move through the nodes in the list until we find the position
	//  we were searching or we reach the end, whatever happens first.
	while (pos < position && !_isAtEnd (*list)) {
		pos++;
		_next (list);
	}

	// Finally, we want to know if we reached the position or the end of the 
	//  list while traversing the nodes.
	return pos;
}

#include <stdio.h>
void	_debugList(LinkedList list) {
	int i = 0;
	Node* aux = NULL;

	printf("-- LIST STATUS --\n");
	printf("\tERROR:      %d\n", list.error);
	printf("\tCOUNT:      %d\n", list.count);
	printf("\t@head:      %p\n", list.head);
	printf("\t@previous:  %p\n", list.previous);

	if (list.head != NULL) {
		printf("\t@head.next: %p\n", list.head->next);
		printf("-- NODE STATUS --\n");

		aux = list.head->next;
		while (aux != NULL) {
			printf("\tNode %d\n", i);
			printf("\t\tElement: %d\n", aux->element);
			printf("\t\tNext: %p\n", aux->next);
			i++;
			aux = aux->next;
		}
	}
}

// PUBLIC OPERATIONS
/**************************************************************************** 
 *
 * @Objective: Creates an empty linked list.
 *			   If the list fails to create the phantom node, it will set
 *				the error code to LIST_ERROR_MALLOC.
 *
 *        +---+
 *   head | o-|---------
 *        +---+         |
 *   prev | o-|---------|--
 *        +---+         | |
 *                      v v          Point of View (After the last element)
 *                    +---+----+   
 *                    |   |NULL| 
 *                    +---+----+   
 *
 * @Parameters: ---
 * @Return: An empty linked list
 *
 ****************************************************************************/
LinkedList LINKEDLIST_create () {
	LinkedList list;
	
	// Request a Node. This node will be the auxiliary "Phantom" node.
	// The list's head now is the phantom node.
	list.head = (Node*) malloc(sizeof(Node));
	if (NULL != list.head) {
		// There is noone after the phantom node, so next is NULL.
		list.head->next = NULL;
		// We set the previous pointer to the phantom node. Now the point
		//  of view is after the last valid element in the list (there are 
		//  no valid elements as the list is empty).
		list.previous = list.head;
		list.count = 0;

		// Everything was fine, so we set the error code to NO_ERROR
		list.error = LIST_NO_ERROR;
	}
	else {
		// Could not get dynamic memory for the phantom node, so we set the
		//  error to malloc error code.
		list.error = LIST_ERROR_MALLOC;
	}

	return list;
}

/**************************************************************************** 
 *
 * @Objective: Creates a new element and initializes it with the provided
 *             data. Should be used to set the element to add to the list.
 * @Parameters: (in)     items    = a dinamic list of item references for
 *                                  the element's items.
 *				(in)     n_items  = the number of items of the element.
 *              (in)     price    = the price of the element.
 * @Return: A new element containing the specified items and price, which can
 *          be added to a list.
 *
 ****************************************************************************/
Element	LINKEDLIST_createElement (Item **items, int n_items, float price) {
    Element e;

	e.items = items;
	e.n_items = n_items;
	e.price = price;

	return e;
}

/**************************************************************************** 
 *
 * @Objective: Inserts the specified element in this list at the specified 
 *              position. Shifts any subsequent elements to the right. If the
 *              position is greater than the size of the list, the new element 
 *              will be added after the current last element.
 *             The operation returns the position where the element was 
 *              inserted in the list, being 0 the first position.
 *			   If the list fails to create the new node to store the element,
 *				it will set	the error code to LIST_ERROR_MALLOC and the 
 *              operation will return a -1.
 *
 *        +---+
 *   head | o-|---------
 *        +---+         |
 *   prev | o-|---------|-----------  Will point to 4.
 *        +---+         |           |
 *                      v           v          Point of View
 *                    +---+---+   +---+---+     +---+---+     +---+----+ 
 *                    |   | o-|-->| 1 | o-|--X->| 2 | o-|---->| 3 |NULL| 
 *                    +---+---+   +---+---+ |   +---+---+     +---+----+
 *										    |         ^
 *										    |         |
 *											|   +---+-|-+
 *										     -->| 4 | o | New Node
 *											    +---+---+
 *
 * @Parameters: (in/out) list     = the linked list where to add the new element
 *				(in)     element  = the element to add to the list.
 *              (in)     position = the position in the list where we want to
 *                                  add the element to.
 * @Return: The position where the new element was inserted. Should always be
 *          the same position as the user requested, unless the size of the
 *          list was smaller than the position or, when the list fails to create
 *          a new node.
 *
 ****************************************************************************/
int 	LINKEDLIST_add (LinkedList * list, Element element, int position) {
	// 1- Create a new node to store the new element.
	Node * new_node = (Node*) malloc (sizeof(Node));

	if (NULL != new_node) {
		// 2- Move the POV so we are placed on the position where we want to 
		//    add the new element or at the end of the list after all the 
		//    stored elements. Is not relevant to know which result happened 
		//	  first, so we can ignore the return value of this function. 
		position = _searchPosition(list, position);

		// 3- Set the element in the element field inside the new node.
		list->error = _copyElement(element, &new_node->element);

		if (LIST_NO_ERROR == list->error) {
		    // 4- Link the new node to the next node after the POV. For example, 
			//	  if we want to add a new element at position 3, el POV will be 
			//	  set on the 3rd node (position 2) and its next is pointing to the
			//	  4th node (position 3). We want the new node to become the 4th 
			//	  node shifting the current 4th node into becoming the 5th one.
			new_node->next = list->previous->next;

			// 5- Link the new node to the list. The new node will go before the
			//    point of view, so
			list->previous->next = new_node;

			// And increase the counter of elements.
			list->count++;
		}
	}
	else {
		// Could not get dynamic memory for the new node, so we set the
		//  error to malloc error code.
		list->error = LIST_ERROR_MALLOC;
	}

	return position;
}


/**************************************************************************** 
 *
 * @Objective: Removes the element placed at the position specified by the 
 *              user. Shifts any subsequent elements to the left.
 *			   This operation will fail if the position is greater than the 
 *              size of the list. That will also happen for an empty list.
 *			   In that situation, this operation will set the error code to
 *				LIST_ERROR_END.
 *
 *        +---+
 *   head | o-|---------                        aux (free aux!)
 *        +---+         |                        |
 *   prev | o-|---------|-----------             |
 *        +---+         |           |            |
 *                      v           v          	 v  POV         NEW POV
 *                    +---+---+   +---+---+     +---+---+     +---+----+ 
 *                    |   | o-|-->| 1 | o-|--X->| 2 | o-|---->| 3 |NULL| 
 *                    +---+---+   +---+---+ |   +---+---+     +---+----+
 *										    |                   ^
 *										    |                   |
 *										     -------------------
 *
 * @Parameters: (in/out) list     = the linked list where to remove the element.
 *              (in)     position = the position of the element to be removed.
 * @Return: ---
 *
 ****************************************************************************/
void 	LINKEDLIST_remove (LinkedList * list, int position) {
	Node * aux = NULL;
	
	// First we search if the position exists in the list (any position smaller
	// than zero will remove the first element in the list), and the list is 
	// not empty.
	if (LINKEDLIST_isEmpty(*list) || position >= list->count) {
		list->error = LIST_ERROR_END;
	}
	else {
		// Move the POV so we are placed on the element we want to remove. 
		_searchPosition(list, position);

		// We need to set an auxiliary pointer to point the element we want
		//  to remove (the POV), so we can free it's memory later on.
		aux = list->previous->next;

		// "Remove" the POV. The element after the PREVIOUS node will be the
		//  element after the POV.
		list->previous->next = list->previous->next->next;

		// Free the POV. Remove the element.
		free(aux->element.items);
		free(aux);

		// If there are no errors, set error code to NO_ERROR.
		list->error = LIST_NO_ERROR;
		// And decrease the counter of elements.
		list->count--;
	}
}


/**************************************************************************** 
 *
 * @Objective: Returns the element placed at the specified position in this 
 *              list. The first element in a list is always placed at position 
 *              zero (0).
 *			   This operation will fail if the position is greated than the 
 *              size of the list. That will also happen for an empty list.
 *			   In that situation, this operation will set the error code to
 *				LIST_ERROR_END.
 * 
 * @Parameters: (in/out) list     = the linked list where to get the element.
 *								    in/out because of the error code.
 *              (in)     position = the position of the element to get.
 * 
 * @Return: The element placed in the specified position.
 *
 ****************************************************************************/
Element LINKEDLIST_get (LinkedList * list, int position) {
	Element element;		
	
	// First we search if the position exists in the list (any position smaller
	// than zero will return the first element in the list), and the list is 
	// not empty.
	if (LINKEDLIST_isEmpty(*list) || position >= list->count) {
		list->error = LIST_ERROR_END;
	}
	else {
		// Move the POV so we are placed on the element we want to retrieve.
		_searchPosition(list, position);

		// The element to return is the element stored in the POV.
		element = list->previous->next->element;

		// If there are no errors, set error code to NO_ERROR.
		list->error = LIST_NO_ERROR;
	}

	return element;
}

/**************************************************************************** 
 *
 * @Objective: Returns true (!0) if this list contains no elements.
 * @Parameters: (in)     list = the linked list to check
 * @Return: true (!0) if this list contains no elements, false (0) otherwise
 *
 ****************************************************************************/
int 	LINKEDLIST_isEmpty (LinkedList list) {
	// The list will be empty if there are no nodes after the phantom node.
	return NULL == list.head->next;
	// We could also check the counter of elements.
}


/**************************************************************************** 
 *
 * @Objective: Returns the number of elements stored in the list.
 * 
 * @Parameters: (in)     list = the linked list to check.
 * 
 * @Return: the number of elements in the list.
 *
 ****************************************************************************/
int     LINKEDLIST_size (LinkedList list) {
	return list.count;
}


/**************************************************************************** 
 *
 * @Objective: Removes all the elements from the list and frees any dynamic
 *				memory block the list was using. The list must be created
 *				again before usage.
 * @Parameters: (in/out) list = the linked list to destroy.
 * @Return: ---
 *
 ****************************************************************************/
void	LINKEDLIST_destroy (LinkedList * list) {
	int i = 0;	
	while (!LINKEDLIST_isEmpty(*list)) {
		LINKEDLIST_remove(list, i);
		i++;
	}

	free(list->head);
	list->head = NULL;
	list->previous = NULL;
	list->count = 0;
}

/**************************************************************************** 
 *
 * @Objective: This function returns the error code provided by the last 
 *				operation run. The operations that update the error code are:
 *				Create, Add, Remove and Get.
 * @Parameters: (in)     list = the linked list to check.
 * @Return: an error code from the list of constants defined.
 *
 ****************************************************************************/
int		LINKEDLIST_getErrorCode (LinkedList list) {
	return list.error;
}

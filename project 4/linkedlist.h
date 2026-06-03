/**************************************************************************** 
 * 
 * @Objective: Linked list data structure implementation
 *             A linked list is a linear data structure, in which the 
 *             elements are not stored at contiguous memory locations.
 * @Author/s:  Eduard de Torres (eduard.detorres@salle.url.edu)
 *			   David Vernet (david.vernet@salle.url.edu)	
 *			   Eduard Fernandez (eduard.fernandez@salle.url.edu)
 *			   Claudia Lajara (claudia.lajara@salle.url.edu)
 *
 * @Creation date: 23/03/2022
 * @Last modification date: 15/04/2025
 * 
 ****************************************************************************/

// Define guard to prevent compilation problems if we add the module more
//  than once in the project.
#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

// Libraries
#include <stdlib.h>					// To use dynamic memory.

// Constants to manage the list's error codes.
#define LIST_NO_ERROR 0
#define LIST_ERROR_FULL 1			// Error, the list is full.
#define LIST_ERROR_EMPTY 2			// Error, the list is empty.
#define LIST_ERROR_MALLOC 3			// Error, a malloc failed.
#define LIST_ERROR_END 4			// Error, the position requested is after 
                                    //  the last element.


// Data types
typedef struct {
    int day;
	int month;
	int year;
} Date;

typedef struct {
    int id;
	char type;
	char *brand;
	char *model;
	float *sizes;
	int n_sizes;
	float price;
	Date last_rented;
} Item;

typedef struct {
    Item **items;
	int n_items;
	float price;
} Pack;

typedef Pack Element;

/*
 * Node is a recursive structure that will contain each one of the elements.
 * A node has two main fields, the element to store and a pointer to the next
 *  node in the Linear Data Structure.
 * The structure is recursively defined (a Node has a pointer to another node),
 *  so we need to define a new type (typedef) from a structure (struct _Node).
 */
typedef struct _Node {		
	Element element;
	struct _Node * next;
} Node;

/*
 * A linked list is a linear data structure, in which the elements are not 
 *  stored at contiguous memory locations. The elements in a linked list 
 *  are stored inside Nodes that are linked using pointers.
 *
 *  +---+----+     +---+----+     +----+----+ 
 *  | 1 |  o-|---> | 2 |  o-|---> | 3  |NULL| 
 *  +---+----+     +---+----+     +----+----+
 *
 * This implementation of the linked list will be using an auxiliary Node
 *  we call the "phantom node". This auxiliary node will help us with the
 *  different operations from the list. It solves the problem of the list
 *  being empty (empty == no nodes) and let us assume that we will always
 *  have one node in the list.
 *
 * Example of an empty list:
 *  
 *               Phantom node
 *       +---+   +---+----+
 *  head | o-|-->|   |NULL|
 *       +---+   +---+----+
 *
 * The linked list will have a "Point of View" (POV). This point of view is the
 *  element (Node) we are visiting at the moment from the list. Whenever 
 *  we decide to add, remove or get an element, we will work from the point 
 *  of view. This point of view is represented by the "previous" pointer in
 *  the LinkedList type. This previous pointer will always point to "the 
 *  element before the point of view". That is why is called previous. We need
 *  to point to the element before the point of view to be able to add new
 *  elements before the first element.
 *
 *        +---+
 *   head | o-|---------
 *        +---+         |
 *   prev | o-|---------|-----------
 *        +---+         |           |
 *                      v           v          Point of View
 *                    +---+---+   +---+---+     +---+---+     +---+----+ 
 *                    |   | o-|-->| 1 | o-|---> | 2 | o-|---> | 3 |NULL| 
 *                    +---+---+   +---+---+     +---+---+     +---+----+
 *
 */
typedef struct {
	int error;			// Error code to keep track of failing operations;
	Node * head;	 	// Head/First element or Phantom node;
	Node * previous; 	// Previous node before the point of view;
    int count;
} LinkedList;


// Procedures & Functions

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
 * @Return: An empty linked list.
 *
 ****************************************************************************/
LinkedList LINKEDLIST_create ();


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
Element	LINKEDLIST_createElement (Item **items, int n_items, float price);


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
int 	LINKEDLIST_add (LinkedList * list, Element element, int position);


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
void 	LINKEDLIST_remove (LinkedList * list, int position);


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
Element LINKEDLIST_get (LinkedList * list, int position);


/**************************************************************************** 
 *
 * @Objective: Returns true (!0) if this list contains no elements.
 * 
 * @Parameters: (in)     list = the linked list to check.
 * 
 * @Return: true (!0) if this list contains no elements, false (0) otherwise.
 *
 ****************************************************************************/
int 	LINKEDLIST_isEmpty (LinkedList list);

/**************************************************************************** 
 *
 * @Objective: Returns the number of elements stored in the list.
 * 
 * @Parameters: (in)     list = the linked list to check.
 * 
 * @Return: the number of elements in the list.
 *
 ****************************************************************************/
int     LINKEDLIST_size (LinkedList list);

/**************************************************************************** 
 *
 * @Objective: Removes all the elements from the list and frees any dynamic
 *				memory block the list was using. The list must be created
 *				again before usage.
 * @Parameters: (in/out) list = the linked list to destroy.
 * @Return: ---
 *
 ****************************************************************************/
void 	LINKEDLIST_destroy (LinkedList * list);


/**************************************************************************** 
 *
 * @Objective: This function returns the error code provided by the last 
 *				operation run. The operations that update the error code are:
 *				Create, Add, Remove and Get.
 * @Parameters: (in)     list = the linked list to check.
 * @Return: an error code from the list of constants defined.
 *
 ****************************************************************************/
int		LINKEDLIST_getErrorCode (LinkedList list);


#endif


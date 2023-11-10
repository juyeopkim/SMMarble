//
//  smm_database.c
//  Sookmyung Marble
//  Database platform code for storing marble board elements
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_database.h"

#define LIST_END            -1
#define MAX_LIST            LISTNO_OFFSET_GRADE+MAX_PLAYER

//node definition for linked list
typedef struct node{
    int index;      //index of the node
    void* obj;      //object data
    void* next;         //pointer to the next
    void* prev;         //pointer to the next
} node_t;


static node_t* list_database[MAX_LIST];
static node_t* listPtr[MAX_LIST];
static int list_cnt[MAX_LIST];


//Inner functions (cannot used at the outside of this file)
static node_t* genNode(void)
{
    //allocate memory for creating the node
    node_t* ndPtr = (node_t*)malloc(sizeof(node_t));
    if (ndPtr != NULL)
    {
        ndPtr->next = NULL;
        ndPtr->obj = NULL;
        ndPtr->prev = NULL;
    }
    
    return ndPtr;
}

static node_t* smmList(int list_nr, int index)
{
    node_t* ndPtr = list_database[list_nr];
    if (listPtr[list_nr] != NULL && listPtr[list_nr]->index <= index)
    {
        ndPtr = listPtr[list_nr];
    }
    
    //return for wrong input
    if (index <-1)
    {
        printf("[ERROR] smmList() : either list is null or index is wrong! (offset : %i)\n", index);
        return NULL;
    }
    if (index >= list_cnt[list_nr])
    {
        printf("[ERROR] smmList() : index is larger than length (len:%i, index:%i)\n", list_cnt[list_nr], index);
        return NULL;
    }
    
    //repeat travelling until the node is the end or offset becomes 0
    while (ndPtr != NULL)
    {
        if (ndPtr->index == index)
            break;
        ndPtr = ndPtr->next;
    }
    
    return ndPtr;
}

static int updateIndex(int list_nr)
{
    int index=0;
    node_t* ndPtr = list_database[list_nr];
    
    while ( ndPtr != NULL )//travel until it is the end node
    {
        ndPtr->index = index++;
        ndPtr = ndPtr->next; //travel once
    }
    
    return index;
}



//API function
/*
    description : adding a data object to the list end
    input parameters : obj - data object to add to the list
    return value : addition result (0 - succeeded, -1 - failed)
    
    operation : 1. make a new node
                2. find the last node in the list
                3. make the last node's next pointer to point the new node
                4. update the index
*/
int smmdb_addTail(int list_nr, void* obj)
{
    node_t* ndPtr;
    node_t* newNdPtr;
    
    //parameter checking
    if (obj == NULL)
    {
        printf("[ERROR] smmdb_addTail() : Failed to do addTail : input object indicates NULL!\n");
        return -1;
    }
    
    //generate new node
    newNdPtr = genNode();
    if (newNdPtr == NULL)
    {
        printf("[ERROR] smmdb_addTail() : Failed to do addTail : Failed to create a node\n");
        return -1; //indicate to the upper function that node addition is failed
    }
    newNdPtr->obj = obj;
    
    //add node to the list tail
    if (list_database[list_nr] == NULL)
    {
        list_database[list_nr] = newNdPtr;
        newNdPtr->index = 0;
    }
    else
    {
        ndPtr = smmList(list_nr, list_cnt[list_nr]-1);
        ndPtr->next = newNdPtr;
        newNdPtr->prev = ndPtr;
        newNdPtr->index = ndPtr->index+1;
    }
    
    listPtr[list_nr] = newNdPtr;
    
    list_cnt[list_nr]++;
    
    return 0;
}


/*
    description : delete data object from the list
    input parameters : index - index'th data to delete
    return value : deletion result (0 - succeeded, -1 - failed)
*/
int smmdb_deleteData(int list_nr, int index)
{
    node_t* ndPrevPtr;
    node_t* ndNextPtr;
    node_t* delNdPtr;
    
    //parameter checking
    if ( (delNdPtr = smmList(list_nr, index)) == NULL)
    {
        printf("[ERROR] smmdb_deleteData() : Failed to do deleteData : input index is invalid (%i)!\n", index);
        return -1;
    }
    
    ndPrevPtr = delNdPtr->prev;
    if (ndPrevPtr != NULL)
    {
        ndPrevPtr->next = delNdPtr->next;
    }
    ndNextPtr = delNdPtr->next;
    if (ndNextPtr != NULL)
    {
        ndNextPtr->prev = delNdPtr->prev;
    }
    
    free(delNdPtr->obj);
    free(delNdPtr);
    
    list_cnt[list_nr]--;
    
    if (list_cnt[list_nr] == 0)
    {
        list_database[list_nr] = NULL;
    }
    else
    {
        updateIndex(list_nr);
    }
    
    return 0;
}


//functions for list observation -----------------------------

/*
    description : return the number of data objects in the list
    return value : length
*/
int smmdb_len(int list_nr)
{
    return list_cnt[list_nr];
}


/*
    description : get the object data
    input parameters : index
    return value : object pointer
*/
void* smmdb_getData(int list_nr, int index)
{
    void* obj = NULL;
    node_t* ndPtr;
    
    //parameter checking
    if ((ndPtr = smmList(list_nr, index)) != NULL)
    {
        obj = (void*)ndPtr->obj;
        listPtr[list_nr] = ndPtr;
    }
    
    if (obj == NULL)
        printf("[ERROR] smmdb_getData() : there is no data of index %i\n", index);
    
    return obj;
}

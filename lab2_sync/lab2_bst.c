/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 
*	    Student name : 
*
*   lab2_bst.c :
*       - thread-safe bst code.
*       - coarse-grained, fine-grained lock code
*
*   Implement thread-safe bst for coarse-grained version and fine-grained version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>

#include "lab2_sync_types.h"

lab2_tree* init_tree = NULL;
lab2_node* current_parents_node = NULL;

void recu_lab2_node_print_inorder(lab2_node *root)
{
	if(root == NULL)
		return;
	recu_lab2_node_print_inorder(root->left);
	printf("%d\n",root->key);
	recu_lab2_node_print_inorder(root->right);
}
/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *  
 *  @param lab2_tree *tree  : bst to print in-order. 
 *  @return                 : status (success or fail)
 */
int lab2_node_print_inorder(lab2_tree *tree) {
    // You need to implement lab2_node_print_inorder function.
	recu_lab2_node_print_inorder(tree->root);
	return 0;
}

/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree *lab2_tree_create() {
    // You need to implement lab2_tree_create function.
	init_tree = (lab2_tree*)malloc(sizeof(lab2_tree));
	init_tree->root = NULL;
	return init_tree;
}

/*
 * TODO
 *  Implement function which creates struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param int key          : bst node's key to creates
 *  @return                 : bst node which you created in this function.
 */
lab2_node * lab2_node_create(int key) {
    // You need to implement lab2_node_create function.
	int rc = 0;
	init_tree->root = (lab2_node*)malloc(sizeof(lab2_node));
	init_tree->root->left = init_tree->root->right = NULL;
	init_tree->root->key = key;
	rc = pthread_mutex_init((&(init_tree->root->mutex)),NULL);
	assert(rc == 0);
	return init_tree->root;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST. 
 *  
 *  @param lab2_tree *tree      : bst which you need to insert new node.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                 : satus (success or fail)
 */
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert function.
	lab2_node *p = tree->root;
	while(p != NULL)
	{
		if(p->key > new_node->key)
			p = p->left;
		else
			p = p->right;
	}
	p = (lab2_node*)malloc(sizeof(lab2_node));
	p->left = p->right = NULL;
	p->key = new_node->key;
	return 0;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node){
      // You need to implement lab2_node_insert_fg function.
/*
	pthread_mutex_t lock;
	pthread_mutex_t lock2;
	int rc = pthread_mutex_init(&lock,NULL);
	int rc2 = pthread_mutex_init(&lock2,NULL);
	assert(rc==0);
	assert(rc2==0);
	lab2_node *p = tree->root;
	lab2_node *q = NULL; // parents
	if(current_parents_node == NULL)
		pthread_mutex_lock(&lock);
	while(p != NULL)
	{
		q = p;
		if(current_parents_node == p->left || current_parents_node == p->right)
			pthread_mutex_lock(&lock2);
		if(p->key > new_node->key)
			p = p->left;
		else
			p = p->right;
	}
	if(current_parents_node == NULL)
		pthread_mutex_unlock(&lock);
	current_parents_node = q;
	p = (lab2_node*)malloc(sizeof(lab2_node));
	p->left = p->right = NULL;
	p->key = new_node->key;
	pthread_mutex_unlock(&lock2);
	return 0;
*/
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert_cg function.
	pthread_mutex_t lock;
	int rc = pthread_mutex_init(&lock,NULL);
	assert(rc == 0);
	pthread_mutex_lock(&lock);
	lab2_node_insert(tree, new_node);
	pthread_mutex_unlock(&lock);
	return 0;
}
	// find min key node
lab2_node* findMinNode(lab2_node* root)
{
	lab2_node* temp = root;
	while(temp->left != NULL)
		temp = temp->left;
	return temp;
}
	// node remove recursion
lab2_node* recu_remove(lab2_node *root, int key)
{	
	lab2_node* tempNode = NULL;
	lab2_node* p = root;
	if(p == NULL)
		return NULL;
	if(p->key > key)
		p->left = recu_remove(p->left, key);
	else if(p->key < key)
		p->right = recu_remove(p->right, key);
	else
	{
		// 2 child
		if(p->right != NULL && p->left != NULL)
		{
			tempNode = findMinNode(p->right);
			p->key = tempNode->key;
			p->right = recu_remove(p->right,tempNode->key);
		}
		else
		{
			tempNode = (p->left == NULL) ? p->right : p->left;
			free(p);
			return tempNode;
		}
	}
	return p;
}

	// node remove_fg recursion
lab2_node* recu_remove_fg(lab2_node *parents ,lab2_node *root, int key)
{

	pthread_mutex_t lock;
	lab2_node* tempNode = NULL;
	lab2_node* p = root;
	lab2_node* q = parents;
	int rc = pthread_mutex_init((&lock), NULL);
	assert(rc==0);
	pthread_mutex_lock(&lock);
	if(p->key > key)
		p->left = recu_remove_fg(p, p->left, key);
	else if(p->key <key)
		p->right = recu_remove_fg(p, p->right, key);
	else
	{
		if(p->right != NULL && p->left != NULL)
		{
			tempNode = findMinNode(p->right);
			p->key = tempNode->key;
			p->right = recu_remove_fg(p, p->right,tempNode->key);
		}
		else
		{
			tempNode = (p->left == NULL) ? p->right : p->left;
			pthread_mutex_unlock(&lock);
			free(p);
			return tempNode;
		}
	}
	pthread_mutex_unlock(&lock);
	return p;

}
/* 
 * TODO
 *  Implement a function which remove nodes from the BST.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove function.
	recu_remove(tree->root, key);
	return 0;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove_fg function.
//	recu_remove_fg(NULL, tree->root, key);
	return 0;
}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove_cg function.
	pthread_mutex_t lock;
	int rc = pthread_mutex_init(&lock,NULL);
	assert(rc==0);
	pthread_mutex_lock(&lock);
	lab2_node_remove(tree, key);
	pthread_mutex_unlock(&lock);
}


/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete. 
 *  @return                 : status(success or fail)
 */
void lab2_tree_delete(lab2_tree *tree) {
    // You need to implement lab2_tree_delete function.
	free(tree);
}

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
void lab2_node_delete(lab2_node *node) {
    // You need to implement lab2_node_delete function.
	lab2_node_remove(init_tree, node->key);
}


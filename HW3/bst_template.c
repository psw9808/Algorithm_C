/* Binary search tree using doubly-linked lists
 * COMP319 Algorithms, Fall 2019
 * School of Electronics Engineering, Kyungpook National University
 * Instructor: Gil-Jin Jang
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MEASURE_TIME	// to measure time

/////////////////////////////////////////////////////////////
// DATA STRUCTURE:
// binary tree node definition using doubly linked lists
// key is a string of a fixed length
// KEYLENGTH	length of the key
// BULK_SIZE	key is hidden in the "BULK"
//	to retrieve key, we have to dig into the "BULK"
//	so accessing key takes average "BULK_SIZE"/2 searches
//	this is INTENTIONALLY to add extra overhead for search
//	your program should reduce the number of key accesses at your best
/////////////////////////////////////////////////////////////
#define KEYLENGTH	3
#define BULK_SIZE	4096
//#define BULK_SIZE	65536
#define STACK_SIZE 12000
struct BTNode {
  char bulk[BULK_SIZE];	// null character to be added
  struct BTNode *left, *right;	// binary tree: left and right children
};


/////////////////////////////////////////////////////////////
// GIVEN: functions for binary tree node
// name and parameters only
// implementations are moved after "main" function
/////////////////////////////////////////////////////////////

const char* getkey(struct BTNode *a);
// return value: char array of KEYLENGTH+1 (+1 for '\0' character)
//  key is hidden in "bulk", so use the following function to
//  read key string of length KEYLENGTH
//  it will make BULK_SIZE/2 searches on average
//  so try to use it as rarely as possible

int setkey(struct BTNode *a, const char kw[]);
// return value: 0 for failure (NULL a), 1 for success
//  the following function hides a string "kw" of KEYLENGTH
//  by randomly selecting the location to save key

int copykey(struct BTNode *dst, struct BTNode *src)
{ return setkey(dst,getkey(src)); }
// copies the key of one node to the other
// very simple, single line, so implementation is given here

int comparekey(struct BTNode *a, struct BTNode *b);
// return value: (by character comparison)
//  -1 if a's key < b's key
//  0 if a's key == b's key
//  +1 if a's key > b's key
//  may be needed for binary search tree search and build-up

struct BTNode *generate_btnode(const char kw[]);
// return value: pointer to a single BTNode (left/right are NULL)
//  generates a node for binary tree

void free_bt_recursive (struct BTNode *bt);
// frees a binary tree

struct BTNode *copy_bt_recursive (struct BTNode *bt);
// return value: pointer to the root of the copy of the given binary tree "bt"

struct BTNode *insert_left_bcnode(
    struct BTNode *parent, struct BTNode *newPtr );
//  adds a node to the left of a BTNode parent
//  it will be used to generate a left-half binary tree
//  (LHBT, all rights are NULL)
// pre-condition: left pointer to the new node should be NULL
// to store the left pointer to the parent node
// return value: parent if the given parent is not NULL; newPtr if parent NULL

struct BTNode *readkeys_textfile_LHBT( const char infile[], int *pN );
// File I/O: read key words from the given file
// and generate a binary tree which is left-half
// (all right children are NULL)

/////////////////////////////////////////////////////////////
// FILL 1: generate a binary search tree using insertion
/////////////////////////////////////////////////////////////
struct BTNode *insert_to_BST_leaf(struct BTNode *bst, struct BTNode *newPtr)
{
  if ( bst == NULL ) return newPtr;	// new bst as the input node
  else if ( newPtr == NULL ) return bst;	// nothing to add
  else {
    if ( comparekey(bst, newPtr) < 0 ) {
      /* FILL */
		  bst->right=insert_to_BST_leaf(bst->right,newPtr);
    }
    else {
      /* FILL */
		  bst->left=insert_to_BST_leaf(bst->left,newPtr);
    }
  }
  return bst;
}

struct BTNode *generate_BST_by_insertion(struct BTNode *lhbt)
{
  /* FILL */
  /* (hint: use insert_to_BST_leaf repeatedly) */
  struct BTNode *tmp;
  struct BTNode *bst=NULL;

  while(lhbt != NULL){
	tmp=lhbt->left;
	lhbt->left=NULL;
    bst=insert_to_BST_leaf(bst,lhbt);
	lhbt=tmp;
  }

  return bst;
}

/////////////////////////////////////////////////////////////
// FILL 2: PRINT
/////////////////////////////////////////////////////////////
int print_LHBT(FILE *fp, struct BTNode *lhbt)
  // prints left-half binary tree
  // ___-___-___
  // INPUT
  //   fp: file pointer for the output file, stdout for monitor output
  //   lhbt: left-half binary tree (right pointers are all null)
  // RETURNs number of NODES in the list
{
  int num_nodes;

  num_nodes=0;
  while ( lhbt ) {
    if ( lhbt->right != NULL ) {	// check if left-half
      fprintf(stderr,"Non-left-half binary tree for %s\n",__FUNCTION__);
      break;
    }
    else {
      fprintf(fp, "%s", getkey(lhbt));
      if ( lhbt->left != NULL ) fprintf(fp, "-");
    }
    num_nodes++;
    lhbt = lhbt->left;
  }

  fprintf(fp, "\n");	// change the line at the end

  return num_nodes;
}

int print_BST_sortedorder(FILE *fp, struct BTNode *bst, int level)
  // prints a binary search tree nodes by a single line
  // in a SORTED ORDER
  // (hint: inorder traversal)
  // INPUT
  //   fp: file pointer for the output file, stdout for monitor output
  //   bst: root node of the BST, should satisfy the property of
  //      binary search tree, left <= center < right
  //   level: level of the root node, starting from 0 (empty)
  //      if it is unnecessary, do not have to use it
  // RETURNs number of NODES in the list
{
  /* GJ: you may fill out the *FILL* lines, or completely rewrite */
  int count;	// to count the number of nodes

  count = 0;
  if ( bst != NULL ) {
    level++;	// root node of the current subtree exists

    /* FILL: print left subtree */
    if(bst->left != NULL) {
        count += print_BST_sortedorder(fp,bst->left,level);
    }

    // center node
    fprintf(fp, "%s ",getkey(bst));
    count++;

    /* FILL: print right subtree */
    if(bst->right != NULL) {
        count += print_BST_sortedorder(fp,bst->right,level);
    }
  }

  // change the line once - only at the bst node
  if ( level <= 1 ) fprintf(fp, "\n");

  return count;
}

int print_BST_right_center_left(FILE *fp, struct BTNode *bst, int level)
  // prints a binary search tree, rotated by 270 degrees
  // Note: key's length is fixed to KEYLENGTH, so there are
  // (KEYLENGTH+1)*level spaces. For examples,
  //         999
  //     777
  //         555
  // 333
  //     222
  //         111
  // INPUT
  //   (same as print_BST_sortedorder)
  // RETURNs HEIGHT-1 of the printed tree (2 in the above example)
  //   (hint: printing order is right -> center -> left
  //    carefully count the number of spaces)
{
  /* FILL */
	int height;
	int tmp=0;
  if ( bst != NULL ) {
    level++;	// root node of the current subtree exists
	height=level;

    /* FILL: print right subtree */
    if(bst->right != NULL) {
       tmp = print_BST_right_center_left(fp,bst->right,level);
	   height = height>tmp ? height : tmp;
    }

    // center node

	for(int i=0; i<level-1; i++){
		fprintf(fp,"    ");
	}
    fprintf(fp, "%s\n",getkey(bst));

    /* FILL: print left subtree */
    if(bst->left != NULL) {
       tmp = print_BST_right_center_left(fp,bst->left,level);
	   height = height>tmp ? height : tmp;
    }
  }

  return height;
}

int print_BST_1(FILE *fp, struct BTNode *bst, int level)
  // prints a binary search tree, rotated by 270 degrees, with less lines
  //  1) center and right are in the same line
  //  2) left subtree is below the center
  //  3) right is connected by '/' and left by '+'
  // Note: key's length is fixed to KEYLENGTH,
  // so left and right begins at (KEYLENGTH+1)*level+1
  // 333/777/999
  //        +555
  //    +222
  //        +111
  // (or)
  // 100/400/800/900
  //                +900
  //            +800
  //                +500/700
  //    +000
  // INPUT and OUPUT
  //   (same as print_BST_right_center_left)
  //   (hint: printing order is center -> right -> left)
{
  /* FILL */

  int height;
  int tmp=0;
  if ( bst != NULL ) {
    level++;	// root node of the current subtree exists
	height=level;

	// center node
    fprintf(fp, "%s",getkey(bst));

    /* FILL: print right subtree */
    if(bst->right != NULL) {
	fprintf(fp,"/");

	tmp = print_BST_1(fp,bst->right,level);
	height = height>tmp ? height : tmp;
    }

    /* FILL: print left subtree */
    if(bst->left != NULL) {

		fprintf(fp,"\n");
		for(int i=0; i<level-1; i++){
			fprintf(fp,"    ");
		}
		fprintf(fp,"   +");

		tmp = print_BST_1(fp,bst->left,level);

		height = height>tmp ? height : tmp;
    }
  }
	if(level==1) fprintf(fp,"\n");
  return height;
}

int lstack[STACK_SIZE];

int print_BST_2(FILE *fp, struct BTNode *bst, int level, int lstack[])
  // same as print_BST_1 except vertical line from center to left
  // 100/400/800/900
  //    |       |   +900
  //    |       +800
  //    |           +500/700
  //    +000
  // Hint: stack or some extra variables may help.
  //       static variable can be used as well
  //       You may add additional parameter to the function if necessary
{
  /* FILL */


  int height;
  int tmp=0;


  if ( bst != NULL ) {
    level++;	// root node of the current subtree exists
	height=level;

	if(bst->left==NULL){
		lstack[level-1] = 0;
	}
	else{
		lstack[level-1] = 1;
	}

	// center node
    fprintf(fp, "%s",getkey(bst));

    /* FILL: print right subtree */
    if(bst->right != NULL) {
	fprintf(fp,"/");

	tmp = print_BST_2(fp,bst->right,level,lstack);
	height = height>tmp ? height : tmp;
    }

    /* FILL: print left subtree */
    if(bst->left != NULL) {

		fprintf(fp,"\n");

		for(int i=0; i<level-1; i++){
			if(lstack[i]==0){
				fprintf(fp,"    ");
			}
			else{
				fprintf(fp,"   |");
			}
		}
		fprintf(fp,"   +");
		lstack[level-1] = 0;

		tmp = print_BST_2(fp,bst->left,level,lstack);

		height = height>tmp ? height : tmp;
    }
  }
	if(level==1) fprintf(fp,"\n");
  return height;
}


/////////////////////////////////////////////////////////////
// FILL 3: Conversion of an BST to a complete BST
/////////////////////////////////////////////////////////////

struct BTNode *CBTstack[STACK_SIZE];
int stack2[STACK_SIZE] = {0,};

struct BTNode *insert_to_BST_leaf2(struct BTNode *bst, struct BTNode *newPtr, int level)
{
  struct BTNode *start;
  start=bst;

  if ( bst == NULL ) return newPtr;	// new bst as the input node
  else if ( newPtr == NULL ) return bst;	// nothing to add
  else {
    for(int i=1; i<level; i++){
      if(stack2[i]==1){
        bst=bst->left;
      }
      else{
        bst=bst->right;
      }
    }
    if(stack2[level]==1){
      bst->left = newPtr;
    }
    else{
      bst->right =newPtr;
    }
  }
  return start;
}

int Make_stack(struct BTNode *bst,struct BTNode *CBTstack[], int count)
{

	if ( bst != NULL ) {
		if(bst->left != NULL){
			count = Make_stack(bst->left, CBTstack, count);
		}

		CBTstack[++count] = bst;

		if(bst->right != NULL){
			count = Make_stack(bst->right, CBTstack, count);
		}
	}
	return count;
}

struct BTNode *Make_CBST(struct BTNode *cbst, struct BTNode *CBTstack[], int numNodes, int level)
{
	int n=1, c=0, root, others;

	while(numNodes>=n-1){
		n *= 2;
		c++;
	}
	c--;
	n /= 2;
	n -= 1;
	others = numNodes-n;
	if((n+1)/2 > others)
		root= n/2 + others + 1;
	else
		root= n/2 + (n+1)/2 + 1;

		cbst=insert_to_BST_leaf2(cbst, CBTstack[root], level++);

	if(root != 1){
    stack2[level]=1;
    cbst = Make_CBST(cbst, CBTstack, root-1, level);
  }
	if(root != numNodes){
    stack2[level]=2;
    cbst = Make_CBST(cbst, CBTstack+root, numNodes-root, level);
  }
  stack2[level]=0;

	return cbst;
}

struct BTNode *BST_to_completeBST(struct BTNode *bst, int numNodes)
  // convert a BST to complete BST (minimum height, filling in left first)
  // INPUT
  //   bst: root node of the BST, should satisfy the property of
  //      binary search tree, left <= center < right
  //   numNodes: number of nodes in the bst
  //      if not necessary in your implementation, do not have to use it
  // RETURNs a COMPLETE BST
  // (hint: using extra memory (arrays or lists) may help,
  //  array's rule for parent-child, sorted list, etc.)
{
  /* FILL */
  struct BTNode *cbst = NULL;
  Make_stack(bst,CBTstack,0);

  for(int i=1; i<numNodes+1; i++){
	CBTstack[i]->right = NULL;
	CBTstack[i]->left = NULL;
  }
  cbst = Make_CBST(cbst, CBTstack, numNodes, 0);
  return cbst;
}


/////////////////////////////////////////////////////////////
// FILL 4: generate binary search tree from a left-half binary tree
// using quick sort
/////////////////////////////////////////////////////////////
struct BTNode *generate_BST_quicksort_basic(struct BTNode *lhbt)
  // gerate a BST using quick sort algorithm
  // the resultant tree should be identical to generate_BST_by_insertion
{
  /* FILL */
  struct BTNode *start, *tmp, *tr;
  start = lhbt;

  while(lhbt->left != NULL){

	  if(comparekey(start,lhbt->left)<0){
		  if(start->right == NULL){
			  tmp = lhbt->left;
			  lhbt->left = tmp->left;
			  tmp->left = NULL;

			  start->right = tmp;
			  tr = tmp;
		  }
		  else{
			  tmp = lhbt->left;
			  lhbt->left = tmp->left;
			  tmp->left = NULL;

			  tr = insert_left_bcnode(tr,tmp);
		  }
	  }
	  else{
		  lhbt = lhbt->left;
	  }

  }

  if(start->left != NULL)
	start->left = generate_BST_quicksort_basic(start->left);
  if(start->right != NULL)
	start->right = generate_BST_quicksort_basic(start->right);
  return start;
}

struct BTNode *generate_BST_quicksort_advanced(struct BTNode *lhbt)
  // challenge: try to reduce the height using quick sort algorithm
{
  /* FILL */
  return lhbt;
}

/////////////////////////////////////////////////////////////
// main function
/////////////////////////////////////////////////////////////
#define MAXLINE	1024
int main()
{
  int numWords;	// number of words
  //int wordLen;	// word length: number of characters per word
  struct BTNode *root, *bst1, *bst2;
  int numNodes, lev;	// level of the tree

  /* for file name, max length 1023 including path */
  char line[MAXLINE];
  //char infile[MAXLINE], outfile[MAXLINE];
  char infile[] = "i6.txt", outfile[] = "o.txt";
  FILE *fp;

#ifdef MEASURE_TIME
  clock_t start, end;
  double cpu_time_used;
#endif

  /* input file name given by keyboard */
  memset(line,0,sizeof(char)*MAXLINE);	// clear the buffer
  fprintf(stderr,"Input file name? ");
  /*fgets(line,MAXLINE,stdin);
  if ( strlen(line) == 0 || sscanf(line,"%s",infile) != 1 ) {
    fprintf(stderr,"cannot read input file name from '%s'\n",line);
    exit(0);
  }
*/
  /* output file name: enter for standard out */
  memset(line,0,sizeof(char)*MAXLINE);	// clear the buffer
  fprintf(stderr,"Output file name? ");
  /*fgets(line,MAXLINE,stdin);
  if ( strlen(line) == 0 || sscanf(line,"%s",outfile) != 1 ) {
    fprintf(stderr,"cannot read output file name from '%s'\n",line);
    fprintf(stderr,"output to stdout\n");
    fp = stdout;
    memset(outfile,0,sizeof(char)*MAXLINE);	// clear the buffer
  }
  else {
     open output file pointer
    fp = fopen(outfile,"w");
    if ( fp == NULL ) {
      fprintf(stderr, "cannot open file '%s' for write\n",outfile);
      fprintf(stderr,"output to stdout\n");
      fp = stdout;
    }
  }
*/
fp = fopen(outfile,"w");
  /* read text file of integers:
   * number_of_intergers integer1 integer2 ...
   * then convert it to a linked list */
  root = readkeys_textfile_LHBT(infile, &numWords);

#ifdef MEASURE_TIME
  start = clock();
#endif

  if ( root != NULL ) {

    // prints input
    fprintf(fp,"=====================================\n");
    numNodes = print_LHBT(fp, root);
    fprintf(fp,"total %d nodes\n",numNodes);

    // BST construction by simple insertion
    // keep root unchanged
    bst1 = generate_BST_by_insertion(copy_bt_recursive(root));

    fprintf(fp,"=====================================\n");
    numNodes = print_BST_sortedorder(fp,bst1,0);
    fprintf(fp,"total %d nodes (sorted)\n",numNodes);
    fprintf(fp,"=====================================\n");
    lev = print_BST_right_center_left(fp,bst1,0);
    fprintf(fp,"BST height %d\n",lev);
    fprintf(fp,"=====================================\n");
    lev = print_BST_1(fp,bst1,0);
    fprintf(fp,"BST height %d\n",lev);
    fprintf(fp,"=====================================\n");
    lev = print_BST_2(fp,bst1,0,lstack);
    fprintf(fp,"BST height %d\n",lev);
    fprintf(fp,"=====================================\n");
    bst1 = BST_to_completeBST(bst1,numNodes);
    lev = print_BST_2(fp,bst1,0,lstack);
    fprintf(fp,"Complete BST height %d\n",lev);
    fprintf(fp,"=====================================\n");

    // BST using quick sort, pivot as left, basic
    bst2 = generate_BST_quicksort_basic(copy_bt_recursive(root));
    lev = print_BST_2(fp,bst2,0,lstack);
    fprintf(fp,"BST by QUICKSORT, height %d\n",lev);
    fprintf(fp,"=====================================\n");

    // BST using quick sort, advanced, to reduce height
    bst2 = generate_BST_quicksort_advanced(copy_bt_recursive(root));
    lev = print_BST_2(fp,bst2,0,lstack);
    fprintf(fp,"BST by QUICKSORT (advanced), height %d\n",lev);
    fprintf(fp,"=====================================\n");

    free_bt_recursive(root);
    free_bt_recursive(bst1);
    free_bt_recursive(bst2);
  }

#ifdef MEASURE_TIME
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  fprintf(fp,"TIME %.5f seconds\n",cpu_time_used);
#endif

  if ( fp != NULL && fp != stdout ) fclose(fp);
  return 0;
}


/////////////////////////////////////////////////////////////
// implementation: functions for binary tree node
/////////////////////////////////////////////////////////////

const char* getkey(struct BTNode *a)
{
  int i;
  for (i=0; i<BULK_SIZE-KEYLENGTH; i++) {
    if ( a->bulk[i] != '\0' ) return a->bulk+i;
  }
  return NULL;	// not found
}

int setkey(struct BTNode *a, const char kw[])
{
  int pos;
  if ( a != NULL ) {
    // fill with 0
    memset(a->bulk,0,sizeof(char)*BULK_SIZE);

    // find position randomly to store KEYLENGTH+1 characters
    pos = rand()%(BULK_SIZE-KEYLENGTH);
    if ( kw != NULL ) memcpy(a->bulk+pos,kw,sizeof(char)*KEYLENGTH);
    a->bulk[pos+KEYLENGTH] = '\0';	// to make it a C string

    // success
    return 1;
  }
  else return 0;
}

struct BTNode *generate_btnode(const char kw[])
{
  struct BTNode *tmp;

  tmp = (struct BTNode*)malloc(sizeof(struct BTNode));
  setkey(tmp,kw);

  // initial left and right children for the generated leaf node
  tmp->left = tmp->right = NULL;

  return tmp;
}

void free_bt_recursive (struct BTNode *bt)
{
  if ( bt != NULL ) {
    free_bt_recursive(bt->left);
    free_bt_recursive(bt->right);
    free(bt);
  }
}

struct BTNode *copy_bt_recursive (struct BTNode *bt)
{
  struct BTNode *dup;

  if ( bt != NULL ) {
    dup = (struct BTNode*) malloc(sizeof(struct BTNode));
    copykey(dup,bt);
    dup->left = copy_bt_recursive(bt->left);
    dup->right = copy_bt_recursive(bt->right);
  }
  else dup = NULL;
  return dup;
}

struct BTNode *insert_left_bcnode(struct BTNode *parent, struct BTNode *newPtr)
{
  if ( parent == NULL ) return newPtr;	// no parent
  else if ( newPtr == NULL ) return parent;	// Nothing to add
  else if ( newPtr->left != NULL ) {
    fprintf(stderr,"cannot add a node with non-null left tree\n");
    return parent;
  }
  else {
    newPtr->left = parent->left;
    parent->left=newPtr;
    return newPtr;	// returning new node as a new parent
  }
}

// static: internal use only
static int _compare_n_char(const char a[], const char b[], int L)
{
  int i;
  for (i=0; i<L; i++) {
    if ( a[i] < b[i] ) return -1;
    else if ( a[i] > b[i] ) return 1;
    else continue;	// to next character
  }
  return 0;
}

int comparekey(struct BTNode *a, struct BTNode *b)
{
  return _compare_n_char(getkey(a),getkey(b), KEYLENGTH);
}

/////////////////////////////////////////////////////////////
// File I/O
/////////////////////////////////////////////////////////////
struct BTNode *readkeys_textfile_LHBT( const char infile[], int *pN )
  // read key words from the given file
  // and generate a binary tree which is skewed to the left
  // (all right children are NULL)
{
  struct BTNode *root, *cur, *tmp;
  char word[1024];
  FILE *fp;
  int i;

  // check for input file name
  if ( infile == NULL ) {
    fprintf(stderr, "NULL file name\n");
    return NULL;
  }

  // check for file existence
  fp = fopen(infile,"r");
  if ( !fp ) {
    fprintf(stderr, "cannot open file %s\n",infile);
    return NULL;
  }

  // check for number of keys
  if ( fscanf(fp, "%d", pN) != 1 || *pN <= 0 ) {
    fprintf(stderr, "File %s: ",infile);
    fprintf(stderr, "number of keys cannot be read or or wrong\n");
    fclose(fp);
    return NULL;
  }

  /*
  // check for number of characters per key
  if ( fscanf(fp, "%d", pL) != 1 || *pL <= 0 ) {
    fprintf(stderr, "File %s: ",infile);
    fprintf(stderr, "number of characters per key cannot be read or or wrong\n");
    fclose(fp);
    return NULL;
  }
  */

  // reading keys
  root = cur = tmp = NULL;
  for (i=0; i<(*pN); i++) {
    if ( fscanf(fp, "%s", word) != 1 ) {
      fprintf(stderr, "cannot read a word at %d/%d\n",i+1,(*pN));
      *pN = i;	// number of read keys so far
      break;
    }
    else {
      //check_and_correct_word(word, KEYLENGTH);

      // generate a new node
      tmp = generate_btnode(word);

      if ( root == NULL ) root = cur = tmp;
      else cur = insert_left_bcnode(cur,tmp);
    }
  }

  return root;
}

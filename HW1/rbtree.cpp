// C program for Red-Black Tree insertion
#include<stdio.h>
#include<stdlib.h>
 
//A Red-Black tree node structure
struct node
{
    int data;     // for data part
    char color;  // for color property
 
    //links for left, right children and parent
    struct node *left, *right, *parent;
};
 
 
// 左旋
void LeftRotate(struct node **root,struct node *x)
{
    //y 儲存x的右子節點的指標
    struct node *y = x->right;
 
    //將y左邊的子節點指標放到x的右子節點
    x->right = y->left;
	//更新x右邊的父節點指標
    if (x->right != NULL)
        x->right->parent = x;
	//更新y的父節點指標
    //update y's parent pointer
    y->parent = x->parent;
	// 如果x的父節點是空的，則使y作為樹的根
    // if x's parent is null make y as root of tree
    if (x->parent == NULL)
        (*root) = y;
	// 將x左邊子節點的父節點變成y
    // store y at the place of x
    else if (x == x->parent->left)
        x->parent->left = y;
    else    x->parent->right = y;
	//y左邊子節點換成x
    // make x as left child of y
    y->left = x;
	//更新x的父節點指標
    //update parent pointer of x
    x->parent = y;
}
 
 
// 右旋
void rightRotate(struct node **root,struct node *y)
{
    struct node *x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    x->parent =y->parent;
    if (x->parent == NULL)
        (*root) = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}
// 使用二元搜索樹插入法之後，修復RBtree，因為插入後，不一定會符合RBtree的平衡，所以必須做修復
// Utility function to fixup the Red-Black tree after standard BST insertion
void insertFixUp(struct node **root,struct node *z)
{
	// 讓z一直疊代，直到z不是根，然後使其父節點為紅色;這是為了讓其可以符合規則所使用
    // iterate until z is not the root and z's parent color is red
    while (z != *root && z->parent->color == 'R')
    {
        struct node *y;
		// 找其叔父節點並且放在y中，因為後半部的運作都需要依靠叔父節點做判斷
        // Find uncle and store uncle in y
        if (z->parent == z->parent->parent->left)
            y = z->parent->parent->right;
        else
            y = z->parent->parent->left;
		// 紅色節點
		// 如果叔父節點為紅，將叔父節點的父節點轉成黑色以及父節點變為黑色，再將父節點的父節點轉為紅，將z移到祖父節點
        // If uncle is RED, do following
        // (i)  Change color of parent and uncle as BLACK
        // (ii) Change color of grandparent as RED
        // (iii) Move z to grandparent
        if (y->color == 'R')
        {
            y->color = 'B';
            z->parent->color = 'B';
            z->parent->parent->color = 'R';
            z = z->parent->parent;
        }
		// 黑色節點
		// 當叔父節點是黑色，將會四種情況必須考慮
        // Uncle is BLACK, there are four cases (LL, LR, RL and RR)
        else
        {

            // Left-Left (LL) case, do following
			//將父節點與祖父節點互相交換
			//並對祖父節點進行右旋
            // (i)  Swap color of parent and grandparent
            // (ii) Right Rotate Grandparent
            if (z->parent == z->parent->parent->left &&
                z == z->parent->left)
            {
                char ch = z->parent->color ;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                rightRotate(root,z->parent->parent);
            }
 
            // Left-Right (LR) case, do following
			// 先更改當前節點與祖父節點的顏色，在對當前節點的父節點進行左旋，然後再對祖父節點進行右旋。
            // (i)  Swap color of current node  and grandparent
            // (ii) Left Rotate Parent
            // (iii) Right Rotate Grand Parent
            if (z->parent == z->parent->parent->left &&
                z == z->parent->right)
            {
                char ch = z->color ;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                LeftRotate(root,z->parent);
                rightRotate(root,z->parent->parent);
            }
 
            // Right-Right (RR) case, do following
			//更改父節點與祖父節點，在對祖父節點做左旋
            // (i)  Swap color of parent and grandparent
            // (ii) Left Rotate Grandparent
            if (z->parent == z->parent->parent->right &&
                z == z->parent->right)
            {
                char ch = z->parent->color ;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                LeftRotate(root,z->parent->parent);
            }
 
            // Right-Left (RL) case, do following
			//將當前節點與祖父節點的顏色做對調，並且對當前節點的父節點進行右旋，然後再對祖父節點進行左旋
            // (i)  Swap color of current node  and grandparent
            // (ii) Right Rotate Parent
            // (iii) Left Rotate Grand Parent
            if (z->parent == z->parent->parent->right &&
                z == z->parent->left)
            {
                char ch = z->color ;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                rightRotate(root,z->parent);
                LeftRotate(root,z->parent->parent);
            }
        }
    }
    (*root)->color = 'B'; //最後都要保證根節點為黑色//keep root always black
}
//插入新的節點進去RBtree
// Utility function to insert newly node in RedBlack tree
void insert(struct node **root, int data)
{
	//分配空間給新的節點
    // Allocate memory for new node
    struct node *z = (struct node*)malloc(sizeof(struct node));
    z->data = data;
    z->left = z->right = z->parent = NULL;
	//如果根節點為空節點，則讓新節點作為根節點，不然會導致其不符合紅黑樹
     //if root is null make z as root
    if (*root == NULL)
    {
        z->color = 'B';
        (*root) = z;
    }
    else
    {
        struct node *y = NULL;
        struct node *x = (*root);
		// 根據基本二員搜索樹插入法進行第一次插入節點
        // Follow standard BST insert steps to first insert the node
        while (x != NULL)
        {
            y = x;
            if (z->data < x->data)
                x = x->left;
            else
                x = x->right;
        }
        z->parent = y;
        if (z->data > y->data)
            y->right = z;
        else
            y->left = z;
        z->color = 'R';
		//呼叫插入修復，有可能插入後導致其不符合紅黑樹的性質
        // call insertFixUp to fix reb-black tree's property if it
        // is voilated due to insertion.
        insertFixUp(root,z);
    }
}
 //此方程式是將紅黑樹轉成inorder fashion，會有一個序列(其順序會變成左子樹節點=>根=>右子樹節點)
// A utility function to traverse Red-Black tree in inorder fashion
void inorder(struct node *root)
{
    if (root == NULL)
        return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}
 //主程式運作
/* Drier program to test above function*/
int main()
{
    struct node *root = NULL;
    insert(&root,20);
    insert(&root,28);
    insert(&root,72);
    insert(&root,25);
    insert(&root,59);
    insert(&root,39);
    insert(&root,60);
    insert(&root,48);
    printf("inorder Traversal Is : ");
    inorder(root);
 
    return 0;
}

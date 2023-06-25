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
 
 
// ����
void LeftRotate(struct node **root,struct node *x)
{
    //y �x�sx���k�l�`�I������
    struct node *y = x->right;
 
    //�Ny���䪺�l�`�I���Щ��x���k�l�`�I
    x->right = y->left;
	//��sx�k�䪺���`�I����
    if (x->right != NULL)
        x->right->parent = x;
	//��sy�����`�I����
    //update y's parent pointer
    y->parent = x->parent;
	// �p�Gx�����`�I�O�Ū��A�h��y�@���𪺮�
    // if x's parent is null make y as root of tree
    if (x->parent == NULL)
        (*root) = y;
	// �Nx����l�`�I�����`�I�ܦ�y
    // store y at the place of x
    else if (x == x->parent->left)
        x->parent->left = y;
    else    x->parent->right = y;
	//y����l�`�I����x
    // make x as left child of y
    y->left = x;
	//��sx�����`�I����
    //update parent pointer of x
    x->parent = y;
}
 
 
// �k��
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
// �ϥΤG���j���𴡤J�k����A�״_RBtree�A�]�����J��A���@�w�|�ŦXRBtree�����šA�ҥH�������״_
// Utility function to fixup the Red-Black tree after standard BST insertion
void insertFixUp(struct node **root,struct node *z)
{
	// ��z�@���|�N�A����z���O�ڡA�M��Ϩ���`�I������;�o�O���F����i�H�ŦX�W�h�Ҩϥ�
    // iterate until z is not the root and z's parent color is red
    while (z != *root && z->parent->color == 'R')
    {
        struct node *y;
		// �������`�I�åB��by���A�]����b�����B�@���ݭn�̾a�����`�I���P�_
        // Find uncle and store uncle in y
        if (z->parent == z->parent->parent->left)
            y = z->parent->parent->right;
        else
            y = z->parent->parent->left;
		// ����`�I
		// �p�G�����`�I�����A�N�����`�I�����`�I�ন�¦�H�Τ��`�I�ܬ��¦�A�A�N���`�I�����`�I�ର���A�Nz���쯪���`�I
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
		// �¦�`�I
		// ������`�I�O�¦�A�N�|�|�ر��p�����Ҽ{
        // Uncle is BLACK, there are four cases (LL, LR, RL and RR)
        else
        {

            // Left-Left (LL) case, do following
			//�N���`�I�P�����`�I���ۥ洫
			//�ùﯪ���`�I�i��k��
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
			// ������e�`�I�P�����`�I���C��A�b���e�`�I�����`�I�i�楪�ۡA�M��A�ﯪ���`�I�i��k�ۡC
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
			//�����`�I�P�����`�I�A�b�ﯪ���`�I������
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
			//�N��e�`�I�P�����`�I���C�ⰵ��աA�åB���e�`�I�����`�I�i��k�ۡA�M��A�ﯪ���`�I�i�楪��
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
    (*root)->color = 'B'; //�᳣̫�n�O�Үڸ`�I���¦�//keep root always black
}
//���J�s���`�I�i�hRBtree
// Utility function to insert newly node in RedBlack tree
void insert(struct node **root, int data)
{
	//���t�Ŷ����s���`�I
    // Allocate memory for new node
    struct node *z = (struct node*)malloc(sizeof(struct node));
    z->data = data;
    z->left = z->right = z->parent = NULL;
	//�p�G�ڸ`�I���Ÿ`�I�A�h���s�`�I�@���ڸ`�I�A���M�|�ɭP�䤣�ŦX���¾�
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
		// �ھڰ򥻤G���j���𴡤J�k�i��Ĥ@�����J�`�I
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
		//�I�s���J�״_�A���i�ഡ�J��ɭP�䤣�ŦX���¾𪺩ʽ�
        // call insertFixUp to fix reb-black tree's property if it
        // is voilated due to insertion.
        insertFixUp(root,z);
    }
}
 //����{���O�N���¾��নinorder fashion�A�|���@�ӧǦC(�䶶�Ƿ|�ܦ����l��`�I=>��=>�k�l��`�I)
// A utility function to traverse Red-Black tree in inorder fashion
void inorder(struct node *root)
{
    if (root == NULL)
        return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}
 //�D�{���B�@
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

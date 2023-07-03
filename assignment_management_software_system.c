#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0

// Node structure for student groups
struct StudentGroupNode {
    int group_id;
    char status[20];
    int marks;
    struct StudentGroupNode* left;
    struct StudentGroupNode* right;
     struct StudentGroupNode* next;
};

// Node structure for assignments
struct AssignmentNode {
    int assignment_id;
    char topic_name[50];
    char status[20];
    struct StudentGroupNode* student_groups;
    struct AssignmentNode* left;
    struct AssignmentNode* right;
    int height;
};


// AVL tree structure
struct AVLTree {
    struct AssignmentNode* root;
};

// Function to read student records from file and link them to the appropriate assignment record
void link_student_records(struct AVLTree* tree, char* student_file_name) {
    FILE* student_file = fopen(student_file_name, "r");
    if (student_file == NULL) {
        printf("Error opening student file.");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), student_file) != NULL) {
        int group_id;
        int assignment_id;
        char status[20];
        int marks;
        sscanf(line, "%d,%d,%[^,],%d", &group_id, &assignment_id, status, &marks);

        // Find the assignment node in the AVL tree with the matching assignment ID
        struct AssignmentNode* current_node = tree->root;
        while (current_node != NULL) {
            if (assignment_id < current_node->assignment_id) {
                current_node = current_node->left;
            } else if (assignment_id > current_node->assignment_id) {
                current_node = current_node->right;
            } else {
                // Found the matching assignment node, create and link the student group node
                struct StudentGroupNode* new_student_group = (struct StudentGroupNode*) malloc(sizeof(struct StudentGroupNode));
                new_student_group->group_id = group_id;
                strcpy(new_student_group->status, status);
                new_student_group->marks = marks;
                new_student_group->left = NULL;
                new_student_group->right = NULL;

                if (current_node->student_groups == NULL) {
                    current_node->student_groups = new_student_group;
                } else {
                    // Add the new student group node to the end of the linked list
                    struct StudentGroupNode* current_student_group = current_node->student_groups;
                    while (current_student_group->right != NULL) {
                        current_student_group = current_student_group->right;
                    }
                    current_student_group->right = new_student_group;
                    new_student_group->left = current_student_group;
                }
                break;
            }
        }
    }

    fclose(student_file);
}

// Function to calculate the height of a node in the AVL tree
int node_height(struct AssignmentNode* node) {
if (node == NULL) {
return 0;
} else {
int left_height = node_height(node->left);
int right_height = node_height(node->right);
    if (left_height > right_height) {
        return left_height + 1;
    } else {
        return right_height + 1;
    }
}
}
// Function to calculate the balance factor of a node in the AVL tree
int balance_factor(struct AssignmentNode* node) {
if (node == NULL) {
return 0;
} else {
return node_height(node->left) - node_height(node->right);
}
}

// Function to perform a left rotation on a node in the AVL tree
struct AssignmentNode* left_rotate(struct AssignmentNode* node) {
struct AssignmentNode* new_parent = node->right;
node->right = new_parent->left;
new_parent->left = node;
return new_parent;
}

// Function to perform a right rotation on a node in the AVL tree
struct AssignmentNode* right_rotate(struct AssignmentNode* node) {
struct AssignmentNode* new_parent = node->left;
node->left = new_parent->right;
new_parent->right = node;
return new_parent;
}
// Function to balance a node in the AVL tree
struct AssignmentNode* balance_node(struct AssignmentNode* node) {
int bf = balance_factor(node);
if (bf > 1) {
    if (balance_factor(node->left) < 0) {
        node->left = left_rotate(node->left);
    }
    node = right_rotate(node);
} else if (bf < -1) {
    if (balance_factor(node->right) > 0) {
        node->right = right_rotate(node->right);
    }
    node = left_rotate(node);
}

return node;
}

// Recursive function to insert a new assignment node into the AVL tree
struct AssignmentNode* insert_assignment_node(struct AssignmentNode* current_node, struct AssignmentNode* new_node) {
if (current_node == NULL) {
return new_node;
} else if (new_node->assignment_id < current_node->assignment_id) {
current_node->left = insert_assignment_node(current_node->left, new_node);
} else {
current_node->right = insert_assignment_node(current_node->right, new_node);
}
current_node = balance_node(current_node);
return current_node;
}

// Function to print the student records for a given assignment ID
void print_student_records(struct AVLTree* tree, int assignment_id) {
// Find the assignment node in the AVL tree with the matching assignment ID
struct AssignmentNode* current_node = tree->root;
while (current_node != NULL) {
if (assignment_id < current_node->assignment_id) {
current_node = current_node->left;
} else if (assignment_id > current_node->assignment_id) {
current_node = current_node->right;
} else {
// Found the matching assignment node, print the student group records
printf("Student records for Assignment ID %d:\n", assignment_id);
struct StudentGroupNode* current_student_group = current_node->student_groups;
while (current_student_group != NULL) {
printf("Group ID: %d\n", current_student_group->group_id);
printf("Status: %s\n", current_student_group->status);
printf("Marks: %d\n", current_student_group->marks);
printf("\n");
current_student_group = current_student_group->right;
}
return;
}
}
printf("No record found for Assignment ID %d.\n", assignment_id);
}

void print_student_records1(struct AVLTree* tree) {
// Find the assignment node in the AVL tree with the matching assignment ID
 int assignment_id;
struct AssignmentNode* current_node = tree->root;
while (current_node != NULL) {
if (assignment_id < current_node->assignment_id) {
current_node = current_node->left;
} else if (assignment_id > current_node->assignment_id) {
current_node = current_node->right;
} else {
// Found the matching assignment node, print the student group records
printf("Student records for Assignment ID %d:\n", assignment_id);
struct StudentGroupNode* current_student_group = current_node->student_groups;
while (current_student_group != NULL) {
printf("Group ID: %d\n", current_student_group->group_id);
printf("Status: %s\n", current_student_group->status);
printf("Marks: %d\n", current_student_group->marks);
printf("\n");
current_student_group = current_student_group->right;
}
return;
}
}
printf("No record found for Assignment ID %d.\n", assignment_id);
}

// Function to read assignment records from file and insert them into the AVL tree
void insert_assignment_records(struct AVLTree* tree, char* assignment_file_name) {
    FILE* assignment_file = fopen(assignment_file_name, "r");
    if (assignment_file == NULL) {
        printf("Error opening assignment file.");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), assignment_file) != NULL) {
        int assignment_id;
        char topic_name[50];
        char status[20];
        sscanf(line, "%d,%[^,],%[^,]", &assignment_id, topic_name, status);

        // Create the new assignment node
        struct AssignmentNode* new_assignment_node = (struct AssignmentNode*) malloc(sizeof(struct AssignmentNode));
        new_assignment_node->assignment_id = assignment_id;
        strcpy(new_assignment_node->topic_name, topic_name);
        strcpy(new_assignment_node->status, status);
        new_assignment_node->student_groups = NULL;
        new_assignment_node->left = NULL;
        new_assignment_node->right = NULL;
        // Insert the new node into the AVL tree
    if (tree->root == NULL) {
        tree->root = new_assignment_node;
    } else {
        tree->root = insert_assignment_node(tree->root, new_assignment_node);
    }
}

fclose(assignment_file);
}

void inorder_assignment_nodes(struct AssignmentNode* root) {
    if (root == NULL) {
        return;
    }
    inorder_assignment_nodes(root->left);
    printf("Assignment ID: %d, Topic Name: %s, Status: %s\n", root->assignment_id, root->topic_name, root->status);

    // Traverse the StudentGroupNodes for the current AssignmentNode
    struct StudentGroupNode* current_student = root->student_groups;
    while (current_student != NULL) {
        printf("\tGroup ID: %d, Status: %s, Marks: %d\n", current_student->group_id, current_student->status, current_student->marks);
        current_student = current_student->right;
    }
    inorder_assignment_nodes(root->right);
}

void inorder_student_group_nodes(struct StudentGroupNode* root) {
    if (root == NULL) {
        return;
    }
    inorder_student_group_nodes(root->left);
    printf("\tGroup ID: %d, Status: %s, Marks: %d\n", root->group_id, root->status, root->marks);
    inorder_student_group_nodes(root->right);
}

void inorder_assignment_tree(struct AssignmentNode* root) {
    if (root == NULL) {
        return;
    }
    inorder_assignment_tree(root->left);
    printf("Assignment ID: %d\nTopic Name: %s\nStatus: %s\n\n", root->assignment_id, root->topic_name, root->status);
    inorder_assignment_tree(root->right);
}

//for student
typedef struct {
    int id;
    char name[50];
    int group_partner;
    int group_id;
    char assignment_name[50];
    char deadline[20];
    char status[20];
    int offline_marks;
    int viva_marks;
    struct StudentGroupNode* group_root;
} Student;

typedef struct AVLNode {
    Student data;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

AVLNode *createNode(Student data) {
    AVLNode *newNode = (AVLNode *) malloc(sizeof(AVLNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

int getHeight(AVLNode *node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

int getBalanceFactor(AVLNode *node) {
    if (node == NULL) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

AVLNode *rightRotate(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + (getHeight(y->left) > getHeight(y->right) ? getHeight(y->left) : getHeight(y->right));
    x->height = 1 + (getHeight(x->left) > getHeight(x->right) ? getHeight(x->left) : getHeight(x->right));

    // Return new root
    return x;
}

AVLNode *leftRotate(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + (getHeight(x->left) > getHeight(x->right) ? getHeight(x->left) : getHeight(x->right));
    y->height = 1 + (getHeight(y->left) > getHeight(y->right) ? getHeight(y->left) : getHeight(y->right));

    // Return new root
    return y;
}

AVLNode *insert(AVLNode *root, Student data) {
    if (root == NULL) {
        return createNode(data);
    } else if (data.id < root->data.id) {
        root->left = insert(root->left, data);
    } else if (data.id > root->data.id) {
        root->right = insert(root->right, data);
    } else {
        return root;
    }

    root->height = 1 + (getHeight(root->left) > getHeight(root->right) ? getHeight(root->left) : getHeight(root->right));
    int balance = getBalanceFactor(root);

    // Left Left Case
    if (balance > 1 && data.id < root->left->data.id) {
        return rightRotate(root);
    }
    // Left Right Case
    if (balance > 1 && data.id > root->left->data.id) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    // Right Right Case
    if (balance < -1 && data.id > root->right->data.id) {
        return leftRotate(root);
    }
    // Right Left Case
    if (balance < -1 && data.id < root->right->data.id) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

void inorder(AVLNode *root) {
    if (root != NULL) {
        inorder(root->left);
        printf("ID: %d\nName: %s\nGroup Partner: %d\nGroup ID: %d\nAssignment Name: %s\nDeadline: %s\nStatus: %s\nOffline Marks: %d\nViva Marks: %d\n",
               root->data.id, root->data.name, root->data.group_partner, root->data.group_id,
               root->data.assignment_name, root->data.deadline, root->data.status, root->data.offline_marks, root->data.viva_marks);
        inorder(root->right);
    }
}

//for only assignment record
struct AssignmentNode* newNode(int assignment_id, char* topic_name, char* status) {
    struct AssignmentNode* node = (struct AssignmentNode*)malloc(sizeof(struct AssignmentNode));
    node->assignment_id = assignment_id;
    strcpy(node->topic_name, topic_name);
    strcpy(node->status, status);
    node->student_groups = NULL;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

int heightassig( struct AssignmentNode* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int getBalanceassig(struct AssignmentNode* node) {
    if (node == NULL) {
        return 0;
    }
    return heightassig(node->left) - heightassig(node->right);
}

struct AssignmentNode* rightRotateassig(struct AssignmentNode* y) {
    struct AssignmentNode* x = y->left;
    struct AssignmentNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(heightassig(y->left), heightassig(y->right)) + 1;
    x->height = max(heightassig(x->left), heightassig(x->right)) + 1;

    return x;
}

struct AssignmentNode* leftRotateassig(struct AssignmentNode* x) {
    struct AssignmentNode* y = x->right;
    struct AssignmentNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(heightassig(x->left), heightassig(x->right)) + 1;
    y->height = max(heightassig(y->left), heightassig(y->right)) + 1;

    return y;
}

struct AssignmentNode* insertassig(struct AssignmentNode* node, int assignment_id, char* topic_name, char* status) {
    if (node == NULL) {
        return newNode(assignment_id, topic_name, status);
    }
    if (assignment_id < node->assignment_id) {
        node->left = insertassig(node->left, assignment_id, topic_name, status);
    } else if (assignment_id > node->assignment_id) {
        node->right = insertassig(node->right, assignment_id, topic_name, status);
    } else {
        // Duplicate keys are not allowed
        return node;
    }

    node->height = 1 + max(heightassig(node->left), heightassig(node->right));

    int balance = getBalanceassig(node);

    if (balance > 1 && assignment_id < node->left->assignment_id) {
        return rightRotateassig(node);
    }

    if (balance < -1 && assignment_id > node->right->assignment_id) {
        return leftRotateassig(node);
    }

    if (balance > 1 && assignment_id > node->left->assignment_id) {
        node->left = leftRotateassig(node->left);
        return rightRotateassig(node);
    }

    if (balance < -1 && assignment_id < node->right->assignment_id) {
        node->right = rightRotateassig(node->right);
        return leftRotateassig(node);
    }

    return node;
}

void inorderassig(struct AssignmentNode* root) {
    if (root == NULL) {
        return;
    }
    inorderassig(root->left);
    printf("Assignment ID: %d\n", root->assignment_id);
    printf("Topic Name: %s\n", root->topic_name);
    printf("Status: %s\n", root->status);
    inorderassig(root->right);
}
//que 1
void insert_student(struct AVLNode** root, Student student, struct AVLTree* assignments) {
    // Step 1: Traverse the AVL tree to find the appropriate position to insert the new node.
    if (*root == NULL) {
        // Step 2: Create a new AVLNode and populate it with the student data.
        *root = (struct AVLNode*)malloc(sizeof(struct AVLNode));
        (*root)->data = student;
        (*root)->left = NULL;
        (*root)->right = NULL;
        (*root)->height = 1;
    }
    else if (student.id < (*root)->data.id) {
        insert_student(&(*root)->left, student, assignments);
    }
    else if (student.id > (*root)->data.id) {
        insert_student(&(*root)->right, student, assignments);
    }
    else {
        // Student already exists in the database, do not insert.
        return;
    }
    
    // Step 3: Update the height of the AVLNode.
    (*root)->height = 1 + max(getHeight((*root)->left), getHeight((*root)->right));

    // Check if the AVL tree is still balanced.
    int balance = getBalanceFactor(*root);

    // If the AVL tree is unbalanced, perform rotations to balance it.
    if (balance > 1 && student.id < (*root)->left->data.id) {
        *root = rightRotate(*root);
    }
    else if (balance < -1 && student.id > (*root)->right->data.id) {
        *root = leftRotate(*root);
    }
    else if (balance > 1 && student.id > (*root)->left->data.id) {
        (*root)->left = leftRotate((*root)->left);
        *root = rightRotate(*root);
    }
    else if (balance < -1 && student.id < (*root)->right->data.id) {
        (*root)->right = rightRotate((*root)->right);
        *root = leftRotate(*root);
    }

    // Step 4: Traverse the AVL tree to find the corresponding assignment-node.
    struct AssignmentNode* assignment_node = assignments->root;
    while (assignment_node != NULL) {
        int cmp = strcmp(student.assignment_name, assignment_node->topic_name);
        if (cmp < 0) {
            assignment_node = assignment_node->left;
        }
        else if (cmp > 0) {
            assignment_node = assignment_node->right;
        }
        else {
            // Step 5: Add the student-group-node to the assignment-node's student_groups linked list.
            struct StudentGroupNode* new_group_node = (struct StudentGroupNode*)malloc(sizeof(struct StudentGroupNode));
            new_group_node->group_id = student.group_id;
            strcpy(new_group_node->status, student.status);
            new_group_node->marks = student.offline_marks + student.viva_marks;
            new_group_node->left = NULL;
            new_group_node->right = NULL;

            if (assignment_node->student_groups == NULL) {
                assignment_node->student_groups = new_group_node;
            }
            else {
                struct StudentGroupNode* current = assignment_node->student_groups;
                while (current->right != NULL) {
                    current = current->right;
                }
                current->right = new_group_node;
            }

            // Step 6: Update the assignment-node's status if necessary.
            int total_marks = 0;
            int total_students = 0;
            struct StudentGroupNode* group_node = assignment_node->student_groups;
            while (group_node != NULL) {
                total_marks += group_node->marks;
                total_students++;
                if (strcmp(group_node->status, "Submitted") != 0) {
                    strcpy(assignment_node->status, "Incomplete");
                }
                group_node = group_node->right;
            }
            if (total_students > 0) {
                int avg_marks = total_marks / total_students;
                if (avg_marks >= 40) {
                    strcpy(assignment_node->status, "Complete");
                }
            }
            break;
        }
    }
}



//que 2
struct StudentGroupNode* insert_student_group_node(struct StudentGroupNode* root, struct StudentGroupNode* new_node) {
    // If the root is NULL, the new node becomes the root
    if (root == NULL) {
        return new_node;
    }
    
    // If the group id of the new node is less than the group id of the root node, insert it into the left subtree
    if (new_node->group_id < root->group_id) {
        root->left = insert_student_group_node(root->left, new_node);
    }
    // If the group id of the new node is greater than the group id of the root node, insert it into the right subtree
    else if (new_node->group_id > root->group_id) {
        root->right = insert_student_group_node(root->right, new_node);
    }
    
    // Return the root node
    return root;
}
struct StudentGroupNode* search_student_group_node(struct StudentGroupNode* current_node, int group_id) {
    if (current_node == NULL || current_node->group_id == group_id) {
        return current_node;
    } else if (group_id < current_node->group_id) {
        return search_student_group_node(current_node->left, group_id);
    } else {
        return search_student_group_node(current_node->right, group_id);
    }
}

struct AssignmentNode* insert_assignment_nodenew(struct AssignmentNode* current_node, struct AssignmentNode* new_node, Student* students, int num_students, int num_groups) {
    if (current_node == NULL) {
        // Allocate the assignment to the student groups
        struct StudentGroupNode* student_groups = NULL;
        for (int i = 0; i < num_groups; i++) {
            struct StudentGroupNode* new_group = (struct StudentGroupNode*) malloc(sizeof(struct StudentGroupNode));
            new_group->group_id = i + 1;
            strcpy(new_group->status, "Pending");
            new_group->marks = 0;
            new_group->left = NULL;
            new_group->right = NULL;
            student_groups = insert_student_group_node(student_groups, new_group);
        }
        new_node->student_groups = student_groups;
        
        // Insert the new node into the AVL tree
        return new_node;
    } else if (new_node->assignment_id < current_node->assignment_id) {
        current_node->left = insert_assignment_nodenew(current_node->left, new_node, students, num_students, num_groups);
    } else {
        current_node->right = insert_assignment_nodenew(current_node->right, new_node, students, num_students, num_groups);
    }
    
    // Update student records with the new assignment allocation
    for (int i = 0; i < num_students; i++) {
        if (students[i].assignment_name == new_node->topic_name && students[i].group_id != 0) {
            int group_id = students[i].group_id;
            struct StudentGroupNode* student_group = search_student_group_node(new_node->student_groups, group_id);
            if (student_group != NULL) {
                strcpy(students[i].status, student_group->status);
                students[i].offline_marks = student_group->marks;
                students[i].viva_marks = 0;
            }
        }
    }
    
    // Balance the AVL tree if necessary and return the root node
    current_node = balance_node(current_node);
    return current_node;
}


//que 3a
// Traverse the AVL tree and find the AssignmentNode with the matching assignment_id
// Then, traverse the StudentGroupNode linked list and find the StudentGroupNode with the matching group_id
// Finally, update the status field of the StudentGroupNode to "submitted"

void submit_assignment(struct AVLTree* tree, int assignment_id, int group_id) {
    struct AssignmentNode* current_assignment = tree->root;
    while (current_assignment != NULL && current_assignment->assignment_id != assignment_id) {
        if (assignment_id < current_assignment->assignment_id) {
            current_assignment = current_assignment->left;
        } else {
            current_assignment = current_assignment->right;
        }
    }

    if (current_assignment != NULL) {
        struct StudentGroupNode* current_group = current_assignment->student_groups;
        while (current_group != NULL && current_group->group_id != group_id) {
            current_group = current_group->right;
        }

        if (current_group != NULL) {
            strcpy(current_group->status, "submitted");
        }
    }
}

//que 3b
// Traverse the AVL tree and find the AssignmentNode with the matching assignment_id
// Then, traverse the StudentGroupNode linked list and find the StudentGroupNode with the matching group_id
// Update the marks field of the StudentGroupNode based on the offline_marks and viva_marks fields of the corresponding Student
// If all StudentGroupNodes have been evaluated for the assignment, update the status field of the AssignmentNode to "evaluated"

void evaluate_assignment(struct AVLTree* tree, int assignment_id,int group_id,int marks) {
    struct AssignmentNode* current_assignment = tree->root;
    while (current_assignment != NULL && current_assignment->assignment_id != assignment_id) {
        if (assignment_id < current_assignment->assignment_id) {
            current_assignment = current_assignment->left;
        } else {
            current_assignment = current_assignment->right;
        }
    }

    if (current_assignment != NULL) {
        struct StudentGroupNode* current_group = current_assignment->student_groups;
        while (current_group != NULL && current_group->group_id != group_id) {
            current_group = current_group->right;
        }

        if (current_group != NULL) {
            current_group->marks = marks;

            // Check if all StudentGroupNodes for this assignment have been evaluated
            bool all_evaluated = true;
            current_group = current_assignment->student_groups;
            while (current_group != NULL && all_evaluated) {
                if (strcmp(current_group->status, "submitted") == 0 && current_group->marks == 0) {
                    all_evaluated = false;
                }
                current_group = current_group->right;
            }

            // If all StudentGroupNodes have been evaluated, update the status of the AssignmentNode
            if (all_evaluated) {
                strcpy(current_assignment->status, "evaluated");
            }
        }
    }
}


//que4
void printDeclaredAssignments(struct AssignmentNode *root) {
    if (root == NULL) {
        return;
    }
    printDeclaredAssignments(root->left);

    if (strcmp(root->status, "declared") == 0 && root->student_groups == NULL) {
        printf("Assignment ID: %d\nTopic: %s\nStatus: %s\n", root->assignment_id, root->topic_name, root->status);
    }

    printDeclaredAssignments(root->right);
}

//que 5

void print_unsubmitted_and_overdue_groups(struct AssignmentNode* node) {
    if (node == NULL) {
        return;
    }

    // Traverse left subtree
    print_unsubmitted_and_overdue_groups(node->left);

    // Traverse student group nodes
    struct StudentGroupNode* group_node = node->student_groups;
    while (group_node != NULL) {
        if (strcmp(group_node->status, "submitted") != 0) {
            if (strcmp(node->status, "overdue") == 0) {
                printf("Group ID: %d\n", group_node->group_id);
                printf("Assignment Name: %s\n", node->topic_name);
                printf("Status: %s\n", node->status);
                printf("Status: %s\n", group_node->status);
                printf("Marks: %d\n", group_node->marks);
                printf("------------------------\n");
            } else {
                printf("-------------");
            }
        }
        group_node = group_node->right;
    }

    // Traverse right subtree
    print_unsubmitted_and_overdue_groups(node->right);
}
void print_unsubmitted_and_overdue_groupsinavl(struct AVLTree* tree) {
    print_unsubmitted_and_overdue_groups(tree->root);
    
}
//que6

void printPendingAssignmentsHelper(struct AssignmentNode* node) {
    if (node == NULL) {
        return;
    }
    
    // Traverse left subtree
    printPendingAssignmentsHelper(node->left);
    
    // Check if the assignment is pending
    if (strcmp(node->status, "Pending") == 0) {
        // Traverse the student groups
        struct StudentGroupNode* group = node->student_groups;
        while (group != NULL) {
            if (strcmp(group->status, "Submitted") == 0 && group->marks == -1) {
                printf("Group ID: %d, Assignment Name: %s, Status: %s, Marks: %d", group->group_id, node->topic_name, "Pending Evaluation", group->marks);
                if (strcmp(node->status, "VivaPending") == 0) {
                    printf("Viva Pending");
                    printf("Group ID: %d, Assignment Name: %s, Status: %s, Marks: %d", group->group_id, node->topic_name, "VivaPending", group->marks);
                }
                printf("\n");
            }
            group = group->right;
        }
    }
    
    // Traverse right subtree
    printPendingAssignmentsHelper(node->right);
}
void printPendingAssignments(struct AVLTree* tree) {
    printPendingAssignmentsHelper(tree->root);
}

//que 7
void printStudentGroupsForAssignment(struct AVLTree* tree,int assignment_id) {
    struct AssignmentNode* curr_assignment = tree->root;
    while (curr_assignment != NULL) {
        if (curr_assignment->assignment_id == assignment_id) {
            printf("Details of student-groups for assignment %d:\n", assignment_id);
            struct StudentGroupNode* curr_group = curr_assignment->student_groups;
            int num_groups = 0;
            while (curr_group != NULL) {
                num_groups++;
                curr_group = curr_group->right;
            }
            int group_ids[num_groups];
            int group_marks[num_groups];
            curr_group = curr_assignment->student_groups;
            for (int i = 0; i < num_groups; i++) {
                group_ids[i] = curr_group->group_id;
                group_marks[i] = curr_group->marks;
                curr_group = curr_group->right;
            }
            for (int i = 0; i < num_groups; i++) {
                for (int j = i+1; j < num_groups; j++) {
                    if (group_marks[i] < group_marks[j]) {
                        int temp_id = group_ids[i];
                        group_ids[i] = group_ids[j];
                        group_ids[j] = temp_id;
                        int temp_marks = group_marks[i];
                        group_marks[i] = group_marks[j];
                        group_marks[j] = temp_marks;
                    }
                }
            }
            for (int i = 0; i < num_groups; i++) {
                printf("Group ID: %d, Marks: %d\n", group_ids[i], group_marks[i]);
            }
            return;
        } else if (assignment_id < curr_assignment->assignment_id) {
            curr_assignment = curr_assignment->left;
        } else {
            curr_assignment = curr_assignment->right;
        }
    }
    printf("Assignment with ID %d not found.\n", assignment_id);
}

//que 8
void printTopScoringGroups(struct AssignmentNode* assignment) {
    if (assignment == NULL) {
        return;
    }
    printTopScoringGroups(assignment->left);
    struct StudentGroupNode* curr_group = assignment->student_groups;
    int top_marks = -1;
    while (curr_group != NULL) {
        if (curr_group->marks > top_marks) {
            top_marks = curr_group->marks;
        }
        curr_group = curr_group->right;
    }
    printf("Top scoring groups for assignment '%s' with marks %d:\n", assignment->topic_name, top_marks);
    curr_group = assignment->student_groups;
    while (curr_group != NULL) {
        if (curr_group->marks == top_marks) {
            printf("Group %d\n", curr_group->group_id);
        }
        curr_group = curr_group->right;
    }
    printTopScoringGroups(assignment->right);
}
void printTopScoringGroupsInAVL(struct AVLTree* tree) {
    printTopScoringGroups(tree->root);
}

//que 9
void search_assignment_between_ids(struct AssignmentNode* node, int assignment_id_1, int assignment_id_2) {
    if (node == NULL) {
        return;
    }

    if (node->assignment_id > assignment_id_2) {
        search_assignment_between_ids(node->left, assignment_id_1, assignment_id_2);
    } else if (node->assignment_id < assignment_id_1) {
        search_assignment_between_ids(node->right, assignment_id_1, assignment_id_2);
    } else {
        // node->assignment_id is between assignment_id_1 and assignment_id_2
        printf("Assignment ID: %d\n", node->assignment_id);
        printf("Topic Name: %s\n", node->topic_name);
        printf("Status: %s\n", node->status);

        struct StudentGroupNode* current_group = node->student_groups;
        while (current_group != NULL) {
            printf("Group ID: %d\n", current_group->group_id);
            printf("Status: %s\n", current_group->status);
            printf("Marks: %d\n", current_group->marks);
            current_group = current_group->right;
        }

        search_assignment_between_ids(node->left, assignment_id_1, assignment_id_2);
        search_assignment_between_ids(node->right, assignment_id_1, assignment_id_2);
    }
}
void search_assignment_between_idsinavl(struct AVLTree* tree,int assignment_id_1, int assignment_id_2) {
    search_assignment_between_ids(tree->root, assignment_id_1, assignment_id_2);
}

int main() {
    struct AVLTree tree = {NULL};
    struct AssignmentNode* treeroot;
    // Read assignment records from file
    FILE* assignment_file = fopen("assigyes.txt", "r");
    if (assignment_file == NULL) {
        printf("Error opening assignment file.");
        return 1;
    }
    char line[100];
    while (fgets(line, sizeof(line), assignment_file) != NULL) {
        int assignment_id;
        char topic_name[50];
        char status[20];
        sscanf(line, "%d,%[^,],%s", &assignment_id, topic_name, status);
        struct AssignmentNode* new_node = (struct AssignmentNode*) malloc(sizeof(struct AssignmentNode));
        new_node->assignment_id = assignment_id;
        strcpy(new_node->topic_name, topic_name);
        strcpy(new_node->status, status);
        new_node->student_groups = NULL;
        new_node->left = NULL;
        new_node->right = NULL;
        tree.root = insert_assignment_node(tree.root, new_node);
    }
    fclose(assignment_file);

    // Read student records from file and link them to the appropriate assignment record
    link_student_records(&tree, "studentyes.txt");

    // Print student records for a given assignment ID
    int assignment_id;
    printf("Enter an assignment ID: ");
    scanf("%d", &assignment_id);
    print_student_records(&tree, assignment_id);
    print_student_records1(&tree);
    inorder_assignment_nodes(tree.root);

    //for student
    AVLNode *root = NULL;
    FILE *fp;
    char filename[] = "student.txt";
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to open file\n");
        return 1;
    }
    char line1[256];
    while (fgets(line1, sizeof(line), fp)) {
        Student student;
        sscanf(line1, "%d,%[^,],%d,%d,%[^,],%[^,],%[^,],%d,%d\n",
            &student.id, student.name, &student.group_partner, &student.group_id,
            student.assignment_name, student.deadline, student.status, &student.offline_marks,
            &student.viva_marks);
        root = insert(root, student);
    }
    fclose(fp);
    printf("Inorder traversal of the student AVL tree:\n");
    inorder(root);

    
    //for assignmentrecord
    
    FILE *fp2 = fopen("assigyes.txt", "r");

    if (fp2 == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    struct AssignmentNode *root2 = NULL;

    char line2[100];
    while (fgets(line2, sizeof(line), fp) != NULL) {
        int id;
        char topic_name[50], status[20];

        sscanf(line2, "%d,%[^,],%s", &id, topic_name, status);
        root2 = insertassig(root2, id, topic_name, status);
    }

    fclose(fp);
    printf("Inorder traversal of the assigrecord avl tree:\n");
    
    inorderassig(root2);


     
     int option;
    int condition = 1;
    while (condition){
        printf("Enter Option::\n1)insert student and update assig 2)insert assig and update student 3)Change status of assignment and student database based on different events 4)assig that are declared but not evaluated 5)student groups who have not submitted the assignments even if the due-date is over.6)student groups for whom the assignment is yet-to-be-evaluated even though they have been submitted 7)student-groups, in the decreasing order of marks 8)student groups receiving top marks 9)Range search Search \n");
        scanf("%d", &option);
        if(option==0){
            printf("nothing to print\n");
        }
        else if(option==1){
            // Create a student record to insert
            printf("que 1\n");
            Student student;
    student.id = 1001;
    strcpy(student.name, "John Doe");
    student.group_partner = 1002;
    student.group_id = 1;
    strcpy(student.assignment_name, "Assignment 1");
    strcpy(student.deadline, "2023-05-01");
    strcpy(student.status, "Submitted");
    student.offline_marks = 80;
    student.viva_marks = 20;
    printf("que1\n");
    insert_student(&root, student, &tree);
    inorder(root);
    inorder_assignment_nodes(tree.root);
        }
        else if(option==2){
            // create assignment record to insert
    int num_students = 3;
    Student students[num_students];
    students[0] = (Student) {.name = "Alice", .group_id = 21, .assignment_name = "dspd2", .status = "Pending", .offline_marks = 0, .viva_marks = 0};
    students[1] = (Student) {.name = "Bob", .group_id = 22, .assignment_name = "dspd2", .status = "Pending", .offline_marks = 0, .viva_marks = 0};
    students[2] = (Student) {.name = "Charlie", .group_id = 23, .assignment_name = "Assignment 2", .status = "Pending", .offline_marks = 0, .viva_marks = 0};
    
    // Insert a new assignment node
    struct AssignmentNode* new_node = (struct AssignmentNode*) malloc(sizeof(struct AssignmentNode));
    new_node->assignment_id = 15;
    strcpy(new_node->topic_name, "dspd2");
    strcpy(new_node->status,"declared");
    new_node->left = NULL;
    new_node->right = NULL;
            printf("que2\n");
            insert_assignment_nodenew(tree.root, new_node, students, num_students, 2);
    inorder_assignment_nodes(tree.root);

        }
        else if(option==3){
//que 3a
printf("que 3a\n");
    int assig_id;
    int gro_id;
    printf("assig id9\n");
    scanf("%d" ,&assig_id);
    printf("group_id19\n");
    scanf("%d" ,&gro_id);
    printf("submitted assignments\n");
    submit_assignment(&tree,assig_id, gro_id);
    inorder_assignment_nodes(tree.root);

    //que 3b
printf("que 3b\n");
int assignment_id13;
printf("assignment_id13\n");
scanf("%d",&assignment_id13);
int group_id17;
printf("group_id17\n");
scanf("%d",&group_id17);
 int marks50;
 printf("enter marks50\n");
 scanf("%d",&marks50);
evaluate_assignment(&tree,assignment_id13,group_id17,marks50);
printf("evaluated assignments\n");
inorder_assignment_nodes(tree.root);
        }
        else if(option==4){
            printf(" que4:\n");
            printDeclaredAssignments(root2);
        }
        else if(option==5){
            printf(" que5:\n");
    print_unsubmitted_and_overdue_groupsinavl(&tree);
    }
    else if(option==6){
      printf("que6\n");
    printPendingAssignments(&tree);
    }
    else if(option==7){
      printf("que7\n");
    printStudentGroupsForAssignment(&tree ,assignment_id);
    }
    else if(option==8){
      printf("que 8\n");
     printTopScoringGroupsInAVL(&tree);
    }
    else if(option==9){
      printf("que 9\n");
     int assignment_id_1;
     int assignment_id_2;
     printf("Enter an assignment ID 1: ");
    scanf("%d", &assignment_id_1);
    printf("Enter an assignment ID 2: ");
    scanf("%d", &assignment_id_2);
     search_assignment_between_idsinavl(&tree, assignment_id_1,assignment_id_2);

    }
    }
     
    return 0;
}


---
Theme: Binary Trees and Traversals
Date created: 2026-01-31 12:00
tags: [ProgramLevel, DataStructures, Trees, Algorithms]
---

## 📚 Idea/Concept

A binary tree is a hierarchical data structure where each node has at most two children (left and right). Binary trees form the foundation for many important structures like BSTs, heaps, and expression trees. Understanding tree traversals is essential for processing hierarchical data.

### Binary Tree Structure
```c
struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
};

// Visual representation
         [50]           ← Root
        /    \
     [30]    [70]       ← Children
     /  \    /  \
   [20][40][60][80]     ← Leaves

// Terminology
- Root: Top node (50)
- Parent: Node with children (30 is parent of 20, 40)
- Leaf: Node with no children (20, 40, 60, 80)
- Height: Longest path from root to leaf
- Depth: Distance from root to node
- Subtree: Any node and all its descendants
```

### Tree Types

**Full Binary Tree**: Every node has 0 or 2 children
```
       [1]
      /   \
    [2]   [3]
   /   \
 [4]   [5]
```

**Complete Binary Tree**: All levels filled except possibly last, filled left to right
```
        [1]
       /   \
     [2]   [3]
    /  \   /
  [4] [5][6]
```

**Perfect Binary Tree**: All internal nodes have 2 children, all leaves at same level
```
        [1]
       /   \
     [2]   [3]
    /  \   /  \
  [4] [5][6] [7]
```

**Binary Search Tree (BST)**: Left child < parent < right child

### Tree Traversals

**DFS Traversals** (Depth-First):
```c
// In-order: Left → Root → Right (gives sorted order for BST)
void inorder(TreeNode* root) {
    if (root == NULL) return;
    inorder(root->left);      // L
    printf("%d ", root->data); // Root
    inorder(root->right);     // R
}
// Result for example tree: 20, 30, 40, 50, 60, 70, 80

// Pre-order: Root → Left → Right (useful for copying tree)
void preorder(TreeNode* root) {
    if (root == NULL) return;
    printf("%d ", root->data); // Root
    preorder(root->left);      // L
    preorder(root->right);     // R
}
// Result: 50, 30, 20, 40, 70, 60, 80

// Post-order: Left → Right → Root (useful for deletion)
void postorder(TreeNode* root) {
    if (root == NULL) return;
    postorder(root->left);     // L
    postorder(root->right);    // R
    printf("%d ", root->data); // Root
}
// Result: 20, 40, 30, 60, 80, 70, 50
```

**BFS Traversal** (Breadth-First / Level-Order):
```c
void levelOrder(TreeNode* root) {
    if (root == NULL) return;
    
    Queue q;
    enqueue(q, root);
    
    while (!isEmpty(q)) {
        TreeNode* node = dequeue(q);
        printf("%d ", node->data);
        
        if (node->left) enqueue(q, node->left);
        if (node->right) enqueue(q, node->right);
    }
}
// Result: 50, 30, 70, 20, 40, 60, 80
```

### Binary Search Tree (BST)

BST Property: For every node, all values in left subtree < node < all values in right subtree
```c
// Search: O(log n) average, O(n) worst
TreeNode* search(TreeNode* root, int key) {
    if (root == NULL || root->data == key)
        return root;
    
    if (key < root->data)
        return search(root->left, key);
    else
        return search(root->right, key);
}

// Insert: O(log n) average
TreeNode* insert(TreeNode* root, int key) {
    if (root == NULL)
        return createNode(key);
    
    if (key < root->data)
        root->left = insert(root->left, key);
    else if (key > root->data)
        root->right = insert(root->right, key);
    
    return root;
}

// Delete: O(log n) average
// Three cases:
// 1. Leaf node: Simply remove
// 2. One child: Replace with child
// 3. Two children: Replace with inorder successor/predecessor
```

### AVL Tree (Self-Balancing BST)

Maintains height balance: |height(left) - height(right)| ≤ 1
```c
struct AVLNode {
    int data;
    int height;
    AVLNode* left;
    AVLNode* right;
};

// Balance factor
int balanceFactor(AVLNode* node) {
    return height(node->left) - height(node->right);
}

// Rotations to restore balance
// Right rotation (for left-heavy)
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    updateHeight(y);
    updateHeight(x);
    
    return x;  // New root
}

//     y                x
//    / \              / \
//   x   T3    →     T1   y
//  / \                  / \
// T1  T2              T2  T3
```

### Common Tree Operations

| Operation | BST Average | BST Worst | AVL |
|-----------|-------------|-----------|-----|
| Search | O(log n) | O(n) | O(log n) |
| Insert | O(log n) | O(n) | O(log n) |
| Delete | O(log n) | O(n) | O(log n) |
| Min/Max | O(log n) | O(n) | O(log n) |

### Expression Trees

Represent mathematical expressions:
```
Expression: (3 + 4) * 5

        [*]
       /   \
     [+]   [5]
    /   \
  [3]   [4]

// In-order traversal: 3 + 4 * 5 (infix, needs parentheses)
// Pre-order: * + 3 4 5 (prefix notation)
// Post-order: 3 4 + 5 * (postfix notation - easy to evaluate)
```

## 📌 Key Points (Optional)
- Binary trees have at most two children per node
- In-order traversal of BST gives sorted sequence
- BST operations are O(log n) average, O(n) worst case
- AVL trees guarantee O(log n) through self-balancing
- Traversal choice depends on the problem (copy, delete, evaluate, etc.)

## 🖼️ Recommended Image
- Diagram showing all three traversal orders on the same tree

## 🔗 Connections
- [[171-Data Structures Overview]]
- [[173-Heap Data Structure]]
- [[167-Recursion Fundamentals]]
- [[174-Graph Representations and Traversals]]
- [[175-Sorting Algorithms Comparison]]
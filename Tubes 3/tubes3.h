

#ifndef TUBES3_H_INCLUDED
#define TUBES3_H_INCLUDED

#include <string>
using namespace std;

typedef string infotype;

struct Node {
    infotype info;
    Node* prev;
    Node* next;
};

typedef Node* adr;

struct TextEditor {
    adr head;
    adr tails;
    adr cursor;
};

struct StackNode {
    string operation; // "insert" atau "delete"
    infotype data;
    adr position;     // Posisi operasi
    StackNode* next;
};

struct Stack {
    StackNode* top;
};

// Deklarasi fungsi
void CreateNewList(TextEditor &editor);
adr CreateNewElement(infotype newText);
void insertAt(TextEditor &editor, adr &cursor, infotype newText, Stack &undoStack);
void deleteAt(TextEditor &editor, adr &cursor, Stack &undoStack, bool fromUndoRedo = false);
void display(const TextEditor &editor);

// Operasi Stack
void push(Stack &stack, string operation, infotype data, adr position);
void pop(Stack &stack, string &operation, infotype &data, adr &position);
bool isEmpty(const Stack &stack);
void undo(TextEditor &editor, Stack &undoStack, Stack &redoStack);
void redo(TextEditor &editor, Stack &redoStack, Stack &undoStack);

#endif // TUBES3_H_INCLUDED

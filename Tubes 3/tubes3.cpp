

#include "TUBES3.h"
#include <iostream>
using namespace std;

void CreateNewList(TextEditor &editor) {
    editor.head = nullptr;
    editor.tails = nullptr;
    editor.cursor = nullptr;
}

adr CreateNewElement(infotype newText) {
    adr newNode = new Node;
    newNode->info = newText;
    newNode->prev = nullptr;
    newNode->next = nullptr;
    return newNode;
}

void insertAt(TextEditor &editor, adr &cursor, infotype newText, Stack &undoStack) {
    adr newNode = CreateNewElement(newText);

    if (!editor.head) {
        // Jika list kosong
        editor.head = editor.tails = cursor = newNode;
    } else if (!cursor) {
        // Jika cursor null, tambahkan di akhir
        editor.tails->next = newNode;
        newNode->prev = editor.tails;
        editor.tails = newNode;
    } else {
        // Insert setelah cursor
        newNode->next = cursor->next;
        newNode->prev = cursor;

        if (cursor->next) {
            cursor->next->prev = newNode;
        } else {
            editor.tails = newNode; // Jika cursor di akhir
        }

        cursor->next = newNode;
    }

    cursor = newNode; // Pindahkan cursor ke elemen baru

    if (!undoStack.top || undoStack.top->operation != "redo") {
        push(undoStack, "insert", newText, newNode);
    }
}



void deleteAt(TextEditor &editor, adr &cursor, Stack &undoStack, bool fromUndoRedo) {
    if (!editor.head || !cursor) {
        cout << "Tidak ada yang dapat dihapus.\n";
        return;
    }

    adr temp = cursor;

    if (editor.head == editor.tails) {
        // Jika hanya satu elemen
        editor.head = editor.tails = nullptr;
        cursor = nullptr;
    } else if (cursor == editor.head) {
        // Jika elemen pertama
        editor.head = editor.head->next;
        if (editor.head) editor.head->prev = nullptr;
        cursor = editor.head;
    } else if (cursor == editor.tails) {
        // Jika elemen terakhir
        editor.tails = editor.tails->prev;
        if (editor.tails) editor.tails->next = nullptr;
        cursor = editor.tails;
    } else {
        // Jika elemen di tengah
        cursor->prev->next = cursor->next;
        cursor->next->prev = cursor->prev;
        cursor = cursor->next; // Pindahkan cursor ke elemen berikutnya
    }

    if (!fromUndoRedo) {
        push(undoStack, "delete", temp->info, temp);
    }

    delete temp;
}



void display(const TextEditor &editor) {
    adr current = editor.head;
    while (current) {
        cout << current->info;
        if (current == editor.cursor) cout << "|";
        cout << " ";
        current = current->next;
    }
    cout << endl;
}

void push(Stack &stack, string operation, infotype data, adr position) {
    StackNode* newNode = new StackNode;
    newNode->operation = operation;
    newNode->data = data;
    newNode->position = position;
    newNode->next = stack.top;
    stack.top = newNode;
}

void pop(Stack &stack, string &operation, infotype &data, adr &position) {
    if (isEmpty(stack)) {
        operation = "";
        data = "";
        position = nullptr;
        return;
    }

    StackNode* temp = stack.top;
    operation = temp->operation;
    data = temp->data;
    position = temp->position;
    stack.top = temp->next;
    delete temp;
}

bool isEmpty(const Stack &stack) {
    return stack.top == nullptr;
}

void undo(TextEditor &editor, Stack &undoStack, Stack &redoStack) {
    if (isEmpty(undoStack)) {
        cout << "Tidak ada operasi yang dapat di-undo.\n";
        return;
    }

    string operation;
    infotype data;
    adr position;

    // Ambil operasi terakhir dari undoStack
    pop(undoStack, operation, data, position);

    if (operation == "insert") {
        // Undo untuk operasi insert: hapus elemen yang ditambahkan
        editor.cursor = position; // Pindahkan cursor ke elemen yang akan dihapus
        push(redoStack, "insert", data, position);
        deleteAt(editor, editor.cursor, undoStack, true);
    } else if (operation == "delete") {
        // Undo untuk operasi delete: tambahkan elemen kembali
        adr tempCursor = position->prev ? position->prev : nullptr;
        insertAt(editor, tempCursor, data, undoStack);
        editor.cursor = position; // Pindahkan cursor kembali ke elemen yang di-insert
        push(redoStack, "delete", data, position);
    }

    cout << "Undo berhasil.\n";
}



void redo(TextEditor &editor, Stack &redoStack, Stack &undoStack) {
    if (isEmpty(redoStack)) {
        cout << "Tidak ada operasi yang dapat di-redo.\n";
        return;
    }

    string operation;
    infotype data;
    adr position;

    // Ambil operasi terakhir dari redoStack
    pop(redoStack, operation, data, position);

    if (operation == "insert") {
        // Lakukan kembali operasi insert
        adr tempCursor = position->prev ? position->prev : nullptr; // Posisi sebelum elemen diinsert
        insertAt(editor, tempCursor, data, undoStack);
        editor.cursor = position; // Pastikan cursor berada pada elemen yang baru diinsert
    } else if (operation == "delete") {
        // Lakukan kembali operasi delete
        editor.cursor = position;
        deleteAt(editor, editor.cursor, undoStack, true);
    }

    cout << "Redo berhasil.\n";
}

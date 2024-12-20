
#include "TUBES3.h"
#include <iostream>

using namespace std;

void displayMenu() {
    cout << "\n=== MENU BANTUAN ===\n";
    cout << ":show  - Tampilkan Hasil\n";
    cout << ":undo  - Batalkan tindakan terakhir\n";
    cout << ":redo  - Ulangi tindakan yang dibatalkan\n";
    cout << ":del   - Hapus teks pada posisi cursor\n";
    cout << ":up    - Pindahkan cursor ke atas\n";
    cout << ":down  - Pindahkan cursor ke bawah\n";
    cout << ":menu  - Tampilkan menu ini\n";
    cout << ":exit  - Keluar dari program\n";
}

int main() {
    TextEditor editor;
    Stack undoStack = {nullptr};
    Stack redoStack = {nullptr};

    CreateNewList(editor);
    string inputTeks;

    cout << "=== TEXT EDITOR EL-Gasing ===\n";
    cout << "Ketik ':menu' untuk melihat daftar perintah.\n\n";
    cout << "Ketik teks yang diinginkan.\n";

    while (true) {
        cout << "> ";
        getline(cin, inputTeks);

        if (inputTeks.empty()) continue;

        if (inputTeks[0] == ':') {
            if (inputTeks == ":undo") undo(editor, undoStack, redoStack);
            else if (inputTeks == ":redo") redo(editor, redoStack, undoStack);
//            else if (inputTeks == ":del") deleteAt(editor, editor.cursor, undoStack);
            else if (inputTeks == ":del") {
                if (editor.cursor) deleteAt(editor, editor.cursor, undoStack);
                else cout << "Tidak ada teks untuk dihapus.\n";
            }
            else if (inputTeks == ":up" && editor.cursor && editor.cursor->prev) editor.cursor = editor.cursor->prev;
            else if (inputTeks == ":down" && editor.cursor && editor.cursor->next) editor.cursor = editor.cursor->next;
            else if (inputTeks == ":menu") displayMenu();
            else if (inputTeks == ":show") display(editor);
//            else if (inputTeks == ":show") {
//                display(editor);
//                if (editor.cursor) {
//                    cout << "Cursor saat ini: " << editor.cursor->info << "\n";
//                } else {
//                    cout << "Cursor saat ini: (null)\n";
//                }
//            }
            else if (inputTeks == ":exit") break;
            else cout << "Perintah tidak dikenali.\n";
        } else {
            insertAt(editor, editor.cursor, inputTeks, undoStack);
        }
    }

    cout << "\n=== TEKS AKHIR ===\n";
    display(editor);
    cout << "Program selesai.\n";

    return 0;
}

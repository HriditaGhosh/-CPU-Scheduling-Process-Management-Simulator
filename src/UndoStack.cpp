#include "UndoStack.h"

// push: create a new node and place it above the current top (classic
// linked-list stack push).
void pushUndo(UndoStack &s, UndoAction action) {
    UndoNode *node = new UndoNode();
    node->action = action;
    node->next = s.top;
    s.top = node;
    s.count++;
}

// pop: remove the top node, hand back its action.
bool popUndo(UndoStack &s, UndoAction &outAction) {
    if (s.top == nullptr) return false;
    UndoNode *node = s.top;
    outAction = node->action;
    s.top = node->next;
    delete node;
    s.count--;
    return true;
}

bool isUndoEmpty(const UndoStack &s) {
    return s.top == nullptr;
}

int undoCount(const UndoStack &s) {
    return s.count;
}

void clearUndo(UndoStack &s) {
    while (s.top != nullptr) {
        UndoNode *node = s.top;
        s.top = s.top->next;
        delete node;
    }
    s.count = 0;
}

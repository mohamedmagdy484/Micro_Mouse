#include <stdio.h>
#include <stdlib.h>
#include "queues.h"

struct _queue {
    struct node* front;
    struct node* rear;
    int size;
};

// إنشاء الكيو
queue queue_create() {
    queue q = (queue)malloc(sizeof(struct _queue));
    if (q == NULL) {
        return NULL; // فشل في الحجز
    }
    q->front = q->rear = NULL;
    q->size = 0;
    return q;
}

// تدمير الكيو
void queue_destroy(queue q) {
    if (q == NULL) return;
    queue_clear(q);
    free(q);
}

// إضافة عنصر
void queue_push(queue q, item_type elem) {
    if (q == NULL) return;
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    if (new_node == NULL) return; // فشل في الحجز

    new_node->data = elem;
    new_node->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
    q->size++;
}

// إزالة عنصر (FIFO)
item_type queue_pop(queue q) {
    if (q == NULL || q->front == NULL) {
        item_type empty_item;
        return empty_item; // بيرجع عنصر فاضي (لازم تتأكد قبل ما تستدعي pop)
    }

    struct node* temp = q->front;
    item_type data = temp->data;

    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    q->size--;
    return data;
}

// إرجاع أول عنصر من غير إزالة
item_type queue_first(queue q) {
    if (q == NULL || q->front == NULL) {
        item_type empty_item;
        return empty_item;
    }
    return q->front->data;
}

// هل الكيو فاضي؟
int queue_is_empty(queue q) {
    return (q == NULL || q->size == 0);
}

// حجم الكيو
int queue_size(queue q) {
    if (q == NULL) return 0;
    return q->size;
}

// مسح الكيو بالكامل
void queue_clear(queue q) {
    if (q == NULL) return;
    while (!queue_is_empty(q)) {
        queue_pop(q);
    }
}

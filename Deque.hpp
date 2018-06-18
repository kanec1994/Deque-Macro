#ifndef DEQUE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Deque_DEFINE(t)                                                                                             \
    struct Deque_##t;                                                                                               \
    struct Deque_##t##_Iterator{                                                                                    \
        int index;                                                                                                  \
        Deque_##t *deque;                                                                                           \
        t &(*deref)(Deque_##t##_Iterator *);                                                                        \
        void (*inc)(Deque_##t##_Iterator *);                                                                        \
        void (*dec)(Deque_##t##_Iterator *);                                                                        \
    };                                                                                                              \
    struct Obj_holder_##t{                                                                                          \
        t data;                                                                                                     \
        bool (*compObj)(const t &, const t &);                                                                      \
    };                                                                                                              \
    struct Deque_##t{                                                                                               \
        t *arr;                                                                                                     \
        int start, finish, length, capacity;                                                                        \
        const char type_name[strlen(#t)+7] = "Deque_"#t;                                                            \
        int (*size)(Deque_##t *);                                                                                   \
        Deque_##t##_Iterator (*begin)(Deque_##t *);                                                                 \
        Deque_##t##_Iterator (*end)(Deque_##t *);                                                                   \
        t &(*at)(Deque_##t *, int);                                                                                 \
        t &(*front)(Deque_##t *);                                                                                   \
        t &(*back)(Deque_##t *);                                                                                    \
        bool (*empty)(Deque_##t *);                                                                                 \
        bool (*compare)(const t &, const t &);                                                                      \
        void (*push_front)(Deque_##t *, t);                                                                         \
        void (*push_back)(Deque_##t *, t);                                                                          \
        void (*pop_front)(Deque_##t *);                                                                             \
        void (*pop_back)(Deque_##t *);                                                                              \
        void (*sort)(Deque_##t *, Deque_##t##_Iterator, Deque_##t##_Iterator);                                      \
        void (*clear)(Deque_##t *);                                                                                 \
        void (*dtor)(Deque_##t *);                                                                                  \
    };                                                                                                              \
    int mask(Deque_##t *obj, int num){                                                                              \
        return num & (obj->capacity-1);                                                                             \
    }                                                                                                               \
    int Deque_##t##_size(Deque_##t *obj){                                                                           \
        return obj->length;                                                                                         \
    }                                                                                                               \
    int sort_compare_##t(const void *obj1, const void *obj2){                                                       \
        Obj_holder_##t new_obj1 = *(Obj_holder_##t *)obj1;                                                          \
        Obj_holder_##t new_obj2 = *(Obj_holder_##t *)obj2;                                                          \
        if(new_obj1.compObj(new_obj1.data, new_obj2.data)){                                                         \
            return -1;                                                                                              \
        }                                                                                                           \
        else if(new_obj1.compObj(new_obj2.data, new_obj1.data)){                                                    \
            return 1;                                                                                               \
        }                                                                                                           \
        else return 0;                                                                                              \
    }                                                                                                               \
    void Deque_##t##_Iterator_inc(Deque_##t##_Iterator *it){                                                        \
        it->index++;                                                                                                \
    }                                                                                                               \
    void Deque_##t##_Iterator_dec(Deque_##t##_Iterator *it){                                                        \
        it->index--;                                                                                                \
    }                                                                                                               \
    t &Deque_##t##_Iterator_deref(Deque_##t##_Iterator *it){                                                        \
        return it->deque->arr[mask(it->deque, it->index)];                                                          \
    }                                                                                                               \
    Deque_##t *expand_array(Deque_##t *obj){                                                                        \
        t *temp = (t *) malloc(2*obj->capacity*sizeof(t));                                                          \
        int i;                                                                                                      \
        for(i=0; i<obj->capacity; i++){                                                                             \
            temp[i] = obj->arr[mask(obj, obj->finish+i)];                                                           \
        }                                                                                                           \
        free(obj->arr);                                                                                             \
        obj->arr = temp;                                                                                            \
        obj->start = 0;                                                                                             \
        obj->finish = obj->length;                                                                                  \
        obj->length = obj->capacity;                                                                                \
        obj->capacity = 2*obj->capacity;                                                                            \
        return obj;                                                                                                 \
    }                                                                                                               \
    Deque_##t##_Iterator Deque_##t##_Iterator_begin(Deque_##t *obj){                                                \
        Deque_##t##_Iterator it;                                                                                    \
        it.index = obj->start;                                                                                      \
        it.deque = obj;                                                                                             \
        it.inc = &Deque_##t##_Iterator_inc;                                                                         \
        it.dec = &Deque_##t##_Iterator_dec;                                                                         \
        it.deref = &Deque_##t##_Iterator_deref;                                                                     \
        return it;                                                                                                  \
    }                                                                                                               \
    Deque_##t##_Iterator Deque_##t##_Iterator_end(Deque_##t *obj){                                                  \
        Deque_##t##_Iterator it;                                                                                    \
        it.index = obj->finish;                                                                                     \
        it.deque = obj;                                                                                             \
        it.inc = &Deque_##t##_Iterator_inc;                                                                         \
        it.dec = &Deque_##t##_Iterator_dec;                                                                         \
        it.deref = &Deque_##t##_Iterator_deref;                                                                     \
        return it;                                                                                                  \
    }                                                                                                               \
    t &Deque_##t##_at(Deque_##t *obj, int num){                                                                     \
        return obj->arr[mask(obj, obj->start+num)];                                                                 \
    }                                                                                                               \
    t &Deque_##t##_front(Deque_##t *obj){                                                                           \
        return obj->arr[mask(obj, obj->start)];                                                                     \
    }                                                                                                               \
    t &Deque_##t##_back(Deque_##t *obj){                                                                            \
        return obj->arr[mask(obj, (obj->finish-1))];                                                                \
    }                                                                                                               \
    bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator it1, Deque_##t##_Iterator it2){                            \
        if(it1.index == it2.index && it1.deque == it2.deque) return true;                                           \
        return false;                                                                                               \
    }                                                                                                               \
    bool Deque_##t##_empty(Deque_##t *obj){                                                                         \
        return (obj->length == 0);                                                                                  \
    }                                                                                                               \
    bool Deque_##t##_equal(Deque_##t obj1, Deque_##t obj2){                                                         \
        if(obj1.length == obj2.length){                                                                             \
            for(int i=0; i<obj1.length; i++){                                                                       \
                if(obj1.compare(obj1.at(&obj1, obj1.start+i), obj2.at(&obj2, obj2.start+i))){                       \
                    return false;                                                                                   \
                }                                                                                                   \
                else if(obj1.compare(obj1.at(&obj2, obj2.start+i), obj2.at(&obj1, obj1.start+i))){                  \
                    return false;                                                                                   \
                }                                                                                                   \
            }                                                                                                       \
            return true;                                                                                            \
        }                                                                                                           \
        return false;                                                                                               \
    }                                                                                                               \
    void Deque_##t##_push_front(Deque_##t *obj, t num){                                                             \
        if(obj->length == obj->capacity){                                                                           \
            obj = expand_array(obj);                                                                                \
            obj->push_front(obj, num);                                                                              \
        }                                                                                                           \
        else{                                                                                                       \
            obj->arr[mask(obj, --obj->start)] = num;                                                                \
            obj->length++;                                                                                          \
        }                                                                                                           \
    }                                                                                                               \
    void Deque_##t##_push_back(Deque_##t *obj, t num){                                                              \
        if(obj->length == obj->capacity){                                                                           \
            obj = expand_array(obj);                                                                                \
            obj->push_back(obj, num);                                                                               \
        }                                                                                                           \
        else{                                                                                                       \
            obj->arr[mask(obj, obj->finish++)] = num;                                                               \
            obj->length++;                                                                                          \
        }                                                                                                           \
    }                                                                                                               \
    void Deque_##t##_pop_front(Deque_##t *obj){                                                                     \
        assert(obj->length != 0);                                                                                   \
        obj->start++;                                                                                               \
        obj->length--;                                                                                              \
    }                                                                                                               \
    void Deque_##t##_pop_back(Deque_##t *obj){                                                                      \
        assert(obj->length != 0);                                                                                   \
        obj->finish--;                                                                                              \
        obj->length--;                                                                                              \
    }                                                                                                               \
    void Deque_##t##_sort(Deque_##t *obj, Deque_##t##_Iterator it1, Deque_##t##_Iterator it2){                      \
        Obj_holder_##t *temp = (Obj_holder_##t *) malloc((it2.index-it1.index)*sizeof(Obj_holder_##t));             \
        int i;                                                                                                      \
        for (i = 0; i < (it2.index-it1.index); i++) {                                                               \
            temp[i].data = obj->arr[mask(obj, it1.index + i)];                                                      \
            temp[i].compObj = obj->compare;                                                                         \
        }                                                                                                           \
        qsort(temp, (it2.index - it1.index), sizeof(Obj_holder_##t), sort_compare_##t);                             \
        for (i = 0; i < (it2.index-it1.index); i++) {                                                               \
            obj->arr[mask(obj, it1.index + i)] = temp[i].data;                                                      \
        }                                                                                                           \
        free(temp);                                                                                                 \
    }                                                                                                               \
    void Deque_##t##_clear(Deque_##t *obj){                                                                         \
        obj->start = 0;                                                                                             \
        obj->finish = 0;                                                                                            \
        obj->length = 0;                                                                                            \
        obj->capacity = 32;                                                                                         \
    }                                                                                                               \
    void Deque_##t##_dtor(Deque_##t *obj){                                                                          \
            free(obj->arr);                                                                                         \
    }                                                                                                               \
    void Deque_##t##_ctor(Deque_##t *obj, bool (*func)(const t &, const t &)){                                      \
        obj->arr = (t *) malloc(32*sizeof(t));                                                                      \
        obj->start = 0;                                                                                             \
        obj->finish = 0;                                                                                            \
        obj->length = 0;                                                                                            \
        obj->capacity = 32;                                                                                         \
        obj->at = &Deque_##t##_at;                                                                                  \
        obj->front = &Deque_##t##_front;                                                                            \
        obj->back = &Deque_##t##_back;                                                                              \
        obj->size = &Deque_##t##_size;                                                                              \
        obj->empty = &Deque_##t##_empty;                                                                            \
        obj->compare = func;                                                                                        \
        obj->begin = &Deque_##t##_Iterator_begin;                                                                   \
        obj->end = &Deque_##t##_Iterator_end;                                                                       \
        obj->push_front = &Deque_##t##_push_front;                                                                  \
        obj->push_back = &Deque_##t##_push_back;                                                                    \
        obj->pop_front = &Deque_##t##_pop_front;                                                                    \
        obj->pop_back = &Deque_##t##_pop_back;                                                                      \
        obj->sort = &Deque_##t##_sort;                                                                              \
        obj->clear = &Deque_##t##_clear;                                                                            \
        obj->dtor = &Deque_##t##_dtor;                                                                              \
    }                                    

#endif

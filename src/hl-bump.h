#ifndef __HL_BUMP_ALLOC_H_
#define __HL_BUMP_ALLOC_H_

#pragma once

#include <hl.h>

class BumpAllocatorInternal;

struct BumpCompactArrayInternal {
    uint8_t *base;     //64
    uint8_t *dynamics; //64
    int count;      //32
    int stride;     //32
    hl_type *type;  //64
    BumpAllocatorInternal *allocator; //64
};

class BumpAllocatorInternal {
   private:
    uint8_t *_buffer;
    hl_type *_type;
    int _allocated;
    int _size;

   public:
    BumpAllocatorInternal(int bytes) {
        _buffer = new uint8_t[bytes];
        _allocated = 0;
        _size = bytes;
    }

    ~BumpAllocatorInternal() {
        delete[] _buffer;
        _buffer = nullptr;
    }

    inline uint8_t *allocRaw(int size) {
        auto a = _allocated;
        _allocated += size;
        return &_buffer[a];
    }

    int totalBytes() {
        return _size;
    }
    int availableBytes() {
        return _size - _allocated;
    }
    int allocatedBytes() {
        return _allocated;
    }

    vdynamic *alloc(hl_type *at) {
        if (at->kind != HOBJ && at->kind != HSTRUCT)
            hl_error("Invalid bump type");

        _type = at;

        hl_runtime_obj *rt = at->obj->rt;
        if (rt == NULL || rt->methods == NULL) rt = hl_get_obj_proto(at);
        int osize = rt->size;
        if (osize & (HL_WSIZE - 1)) osize += HL_WSIZE - (osize & (HL_WSIZE - 1));

        vobj *out = nullptr;
        vdynamic *outDyn = nullptr;
        if (at->kind == HOBJ) {
            uint8_t *mem = allocRaw(osize);
            out = (vobj *)(mem);
            out->t = at;
            outDyn = (vdynamic *)out;
        } else {
            outDyn = (vdynamic *)(allocRaw(sizeof(vdynamic)));
            out = (vobj *)(allocRaw(osize));
            outDyn->t = at;
            outDyn->v.ptr = out;
        }

        for (auto i = 0; i < rt->nbindings; i++) {
            hl_runtime_binding *b = rt->bindings + i;
            // Need to fix the closure allocation
            *(void **)(((char *)out) + rt->fields_indexes[b->fid]) = b->closure ? hl_alloc_closure_ptr(b->closure, b->ptr, out) : b->ptr;
        }

        return outDyn;
    }

    void *allocArray(hl_type *at, int count) {
        return nullptr;
    }
    void *allocCompactArray(hl_type *at, int count) {
        if (at->kind != HOBJ && at->kind != HSTRUCT)
            hl_error("Invalid bump type");

        _type = at;

        hl_runtime_obj *rt = at->obj->rt;
        if (rt == NULL || rt->methods == NULL) rt = hl_get_obj_proto(at);
        int osize = rt->size;
        if (osize & (HL_WSIZE - 1)) osize += HL_WSIZE - (osize & (HL_WSIZE - 1));

        BumpCompactArrayInternal *arr = (BumpCompactArrayInternal *)allocRaw(sizeof(BumpCompactArrayInternal));
        arr->count = count;
        arr->allocator = this;
        arr->stride = osize;
        arr->base = allocRaw(osize * count);
        arr->dynamics = at->kind == HSTRUCT ? allocRaw(sizeof(vdynamic) * count) : nullptr;
        arr->type = at;

        for (auto k = 0; k < count; k++) {
            vobj *o = (vobj *)(arr->base + osize * k);
            if (at->kind == HOBJ)
                o->t = at;
            else {
                vdynamic *d = (vdynamic *)(arr->dynamics + sizeof(vdynamic) * k);
                d->t = at;
                d->v.ptr = o;
            }

            for (auto i = 0; i < rt->nbindings; i++) {
                hl_runtime_binding *b = rt->bindings + i;
                // Need to fix the closure allocation
                *(void **)(((char *)o) + rt->fields_indexes[b->fid]) = b->closure ? hl_alloc_closure_ptr(b->closure, b->ptr, o) : b->ptr;
            }
        }

        return arr;
    }

    /*
    HL_PRIM vclosure *hl_alloc_closure_ptr( hl_type *fullt, void *fvalue, void *v ) {
            hl_type *t = hl_get_closure_type(fullt);
            vclosure *c = (vclosure*)hl_gc_alloc(t, sizeof(vclosure) + sizeof(void*) * hl_closure_stack_capture);
            c->t = t;
            c->fun = fvalue;
            c->hasValue = 1;
    #	ifdef HL_64
            int stack = 0;
            if( hl_closure_stack_capture ) stack = hl_internal_capture_stack((void**)(c + 1), hl_closure_stack_capture);
            c->stackCount = stack;
    #	endif
            c->value = v;
            return c;
    }
    */
    /*
    HL_PRIM void *hl_alloc_carray( hl_type *at, int size ) {
            if( at->kind != HOBJ && at->kind != HSTRUCT )
                    hl_error("Invalid array type");
            if( size < 0 )
                    hl_error("Invalid array size");

            hl_runtime_obj *rt = at->obj->rt;
            if( rt == NULL || rt->methods == NULL ) rt = hl_get_obj_proto(at);
            int osize = rt->size;
            if( osize & (HL_WSIZE-1) ) osize += HL_WSIZE - (osize & (HL_WSIZE-1));
            hl_carray *arr;
            int header = sizeof(hl_carray);
            if( at->kind == HSTRUCT ) {
                    header += sizeof(vdynamic) * size;
                    arr = (hl_carray*)hl_gc_alloc_gen(at, header + size * osize, (rt->hasPtr ? MEM_KIND_RAW : MEM_KIND_NOPTR) | MEM_ZERO);
                    arr->osize = sizeof(vdynamic);
            } else {
                    arr = (hl_carray*)hl_gc_alloc_gen(at, header + size * osize, (rt->hasPtr ? MEM_KIND_DYNAMIC : MEM_KIND_NOPTR) | MEM_ZERO);
                    arr->osize = osize;
            }
            arr->size = size;
            arr->at = at;

            int i,k;
            for(k=0;k<size;k++) {
                    vobj *o = (vobj*)((char*)arr + header + osize * k);
                    if( at->kind == HOBJ )
                            o->t = at;
                    else {
                            vdynamic *d = (vdynamic*)((char*)(arr + 1) + k * sizeof(vdynamic));
                            d->t = at;
                            d->v.ptr = o;
                    }
                    for(i=0;i<rt->nbindings;i++) {
                            hl_runtime_binding *b = rt->bindings + i;
                            *(void**)(((char*)o) + rt->fields_indexes[b->fid]) = b->closure ? hl_alloc_closure_ptr(b->closure,b->ptr,o) : b->ptr;
                    }
            }
            return arr;
    }
    */
    /*
    //carray get
    if( pos < 0 || pos >= arr->size ) return NULL;
            return (vdynamic*)((char*)(arr + 1) + pos * arr->osize);
    */
    void clear() {
        _allocated = 0;
    }
    void dispose() {
        _allocated = 0;
        delete [] _buffer;
        _buffer = nullptr;
    }
};


inline vdynamic *BumpArrayInternal_getAsDynamic(void *ptr, int index) {

    return nullptr;
}

inline vdynamic *BumpCompactArrayInternal_getAsDynamic(void *ptr, int index) {
    BumpCompactArrayInternal *arr = (BumpCompactArrayInternal*)ptr;

    if (arr->dynamics) {
        return &((vdynamic *)arr->dynamics)[index];
    }

    return (vdynamic *)&(arr->base[arr->stride * index]);
}
#endif
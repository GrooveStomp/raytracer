#ifndef RENDERABLE_LIST_C
#define RENDERABLE_LIST_C

#include <string.h> /* memset */

typedef bool (*hit_fn)(void *Renderable, ray3 Ray, float Min, float Max, hit_record *Record);

typedef struct
{
        void **Renderables;
        hit_fn *HitFunctions;
        unsigned int Count;
        unsigned int Capacity;
        unsigned int NumBytes;
} renderable_list;

unsigned int
RenderableListAllocSize(unsigned int Count)
{
        unsigned int ListSize = sizeof(renderable_list);
        unsigned int ObjPtrSize = sizeof(void *) * Count;
        unsigned int FnPtrSize = sizeof(hit_fn) * Count;
        unsigned int Result = ListSize + ObjPtrSize + FnPtrSize;
        return(Result);
}

renderable_list * /* Memory must be RenderableListAllocSize large. */
RenderableListInit(void *Memory, unsigned int Count)
{
        renderable_list *Result = (renderable_list *)Memory;
        Result->Count = 0;
        Result->Capacity = Count;
        Result->NumBytes = RenderableListAllocSize(Count);
        Result->Renderables = (void **)((char *)Memory + sizeof(renderable_list));
        memset((char *)Result->Renderables, 0, sizeof(void *) * Count);
        Result->HitFunctions = (hit_fn *)((char *)Memory + sizeof(renderable_list) + (sizeof(void *) * Count));
        memset((char *)Result->HitFunctions, 0, sizeof(hit_fn) * Count);

        return(Result);
}

bool
RenderableListHit(renderable_list *Self, ray3 Ray, float Min, float Max, hit_record *Record)
{
        hit_record TempRecord;
        bool HitAnything = false;
        double ClosestSoFar = Max;

        for(int I = 0; I < Self->Count; I++)
        {
                void *Renderable = Self->Renderables[I];
                hit_fn HitFn = Self->HitFunctions[I];
                if(HitFn(Renderable, Ray, Min, ClosestSoFar, &TempRecord))
                {
                        HitAnything = true;
                        ClosestSoFar = TempRecord.T;
                        *Record = TempRecord;
                }
        }

        return(HitAnything);
}

bool
RenderableListAdd(renderable_list *Self, void *Renderable, hit_fn Function)
{
        if(Self->Count >= Self->Capacity) return(false);

        unsigned int Index = Self->Count;
        Self->Renderables[Index] = Renderable;
        Self->HitFunctions[Index] = Function;
        Self->Count++;

        return(true);
}

#endif /* RENDERABLE_LIST_C */

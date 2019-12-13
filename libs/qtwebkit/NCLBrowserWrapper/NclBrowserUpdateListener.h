#ifndef NclBrowserUpdateListener_h
#define NclBrowserUpdateListener_h

class NclBrowserUpdateListener {
    public:
        virtual void graphicsUpdated(int x, int y, int w, int h) = 0;
};

#endif

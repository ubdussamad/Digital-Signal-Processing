#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#define DEFAULT_CIRCULAR_BUFFER_WIDTH 10

template <typename T>
class circularBuffer  {

    protected:
        unsigned int bufferWidth;
        T*           bufferPointer; // This is the pointer to very first primitive type.
        T*           readPointer;
        T*           writePointer;
        bool         bufferFull;
    
    public:

        circularBuffer (unsigned int bufferWidth_ = DEFAULT_CIRCULAR_BUFFER_WIDTH);
        unsigned int getBufferWidth(void);
        void write          (const T value);
        T    read           (void);
        bool isEmpty        (void);
        void printBuffer   (void);

};

#endif // _CIRCULAR_BUFFER_H_



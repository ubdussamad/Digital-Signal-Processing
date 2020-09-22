#ifndef FIFO_H
#define FIFO_H

#include <QtCore/qvector.h>
//#include <iostream>
QT_BEGIN_NAMESPACE

//template<class T>
//class QStack : public QVector<T>
//{


template <class T>
class fifoVector: public QVector<T> {
    public:
        fifoVector (T defaultValue = 0 ,int bufferSize_ = 100);
        void append_( const T& x);
    private:
        int bufferSize;
};

template <class T>
fifoVector<T>::fifoVector (T defaultValue ,int bufferSize_) :
    QVector<T>(bufferSize_, defaultValue),
    bufferSize(bufferSize_) {

}

template <class T>
void fifoVector<T>::append_( const T& x) {
//    std::cout << "Append called!\n\n";
    if (this->size() == this->bufferSize) {
        this->erase( this->begin() ); // Remove the first element.
    }
    this->append(x);
}

QT_END_NAMESPACE
#endif // FIFO_H




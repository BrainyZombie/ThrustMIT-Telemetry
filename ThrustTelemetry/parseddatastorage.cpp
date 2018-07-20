#include "parseddatastorage.h"

ParsedDataStorage::ParsedDataStorage(QString file)
{
    fileLocation=file;

    X.reserve(20000);
    Y.reserve(20000);
    Z.reserve(20000);
    W.reserve(20000);
    T.reserve(20000);

}

bool ParsedDataStorage::write(QVector<double>* x,QVector<double>* y,QVector<double>* z,QVector<double>* t){
    if (mutex.tryLock()){
        X.append(*x);
        Y.append(*y);
        Z.append(*z);
        T.append(*t);

        x->clear();
        y->clear();
        z->clear();
        t->clear();

        mutex.unlock();
        return true;
    }
    else return false;
}

bool ParsedDataStorage::write(QVector<double>* w,QVector<double>* t){
    if (mutex.tryLock()){
        W.append(*w);
        T.append(*t);

        w->clear();
        t->clear();

        mutex.unlock();
        return true;
    }
    else return false;
}

bool ParsedDataStorage::read(QVector<double>* x,QVector<double>* y,QVector<double>* z,QVector<double>* t){
    if (mutex.tryLock()){
        x->append(X);
        y->append(Y);
        z->append(Z);
        t->append(T);

        X.clear();
        Y.clear();
        Z.clear();
        T.clear();

        mutex.unlock();
        return true;
    }
    else return false;
}

bool ParsedDataStorage::read(QVector<double>* w, QVector<double>* t){
    if (mutex.tryLock()){
        w->append(W);
        t->append(T);

        W.clear();
        T.clear();

        mutex.unlock();
        return true;
    }
    else return false;
}

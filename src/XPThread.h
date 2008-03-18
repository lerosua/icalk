/***************************************************************************
*   Copyright (C) 2005 by xihe   *
*   xihels@163.com                                                        *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
#ifndef X_THREAD_H
#define X_THREAD_H

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <stdexcept>
using std::runtime_error;

using namespace std;


class XPthreadMutex
{

        pthread_mutex_t mutex_;
        XPthreadMutex(const XPthreadMutex& rhs);
        XPthreadMutex& operator = (const XPthreadMutex& rhs);

public:
        XPthreadMutex(const pthread_mutexattr_t* attr = NULL) {
                ::pthread_mutex_init(&mutex_, attr);
        }

        ~XPthreadMutex() {
                while (EBUSY == ::pthread_mutex_destroy(&mutex_) )
                        unLock();

        }

        operator pthread_mutex_t () const {
                return mutex_;
        }

        operator pthread_mutex_t* () {
                return &mutex_;
        }

        int lock () {
                return ::pthread_mutex_lock(&mutex_);
        }

        int tryLock() {
                return ::pthread_mutex_trylock(&mutex_);
        }

        int unLock() {
                return ::pthread_mutex_unlock(&mutex_);
        }
};

class XMutexLock
{

public:
        XMutexLock(XPthreadMutex& mutex) : mutex_(mutex) {
                mutex_.lock();
        }

        ~XMutexLock() {
                try {
                        mutex_.unLock();
                } catch (...) {}

        }

        /*
        void unLock() {
            if (mutex_) {
         mutex_->unLock();
         mutex_ = NULL;
            }
        }
        void reset(XPthreadMutex* mutex) {
            unLock();
            mutex_ = mutex;
            if (mutex)
         mutex_->lock();
        }
        XPthreadMutex* release() {
            XPthreadMutex* tmp = mutex_;
            mutex_ = NULL;
            return tmp;
        }
        */

private:
        XMutexLock(const XMutexLock&);
        XMutexLock& operator = (const XMutexLock&);
        XPthreadMutex& mutex_;
};



class XPthreadCond
{
        XPthreadMutex mutex_;
        pthread_cond_t cond_;
        XPthreadCond(const XPthreadCond&);
        XPthreadCond& operator = (const XPthreadCond&);

public:
        XPthreadCond(const pthread_condattr_t* attr = NULL) {
                ::pthread_cond_init(&cond_, attr);
        }

        ~XPthreadCond() {
                while (EBUSY == ::pthread_cond_destroy(&cond_))
                        sleep(1);
        }

        operator pthread_cond_t () const {
                return cond_;
        }

        operator pthread_cond_t* () {
                return &cond_;
        }

        XPthreadMutex& getMutex() {
                return mutex_;
        }

        int wait(int second = 0) {
                if (mutex_.lock()) {
                        perror("pthread_mutex_lock");
                        mutex_.unLock();
                        pthread_exit(NULL);
                }

                int ret;

                if (!second) {
                        ret = ::pthread_cond_wait(&cond_, mutex_);
                } else {

                        struct timeval now;

                        struct timespec timeout;
                        gettimeofday(&now, NULL);
                        timeout.tv_sec = now.tv_sec + second;
                        timeout.tv_nsec = now.tv_usec * 1000;
                        ret = ::pthread_cond_timedwait(&cond_, mutex_, &timeout);
                }

                mutex_.unLock();

                return ret;

        }

        int wake() {
                return ::pthread_cond_signal(&cond_);
        }

        int wakeAll() {
                return ::pthread_cond_broadcast(&cond_);
        }
};

class XPthreadKey
{
        typedef void (*FUN)(void*);

public:
        XPthreadKey(FUN fun = NULL) {
                pthread_key_create(&key, fun);
        }

        ~XPthreadKey() {
                pthread_key_delete(key);
        }

        int setSpecific(void* data) {
                return pthread_setspecific(key, data);
        }

        void* getSpecific() {
                return pthread_getspecific(key);
        }

private:
        pthread_key_t key;
};


template <class T>

class XPThread
{
        T* worker_;
        typedef void* (T::*PMF)(void*) ;
        PMF pmf_;
        pthread_t id_;
        void * arg_;
        static void* run(void* me) {
                T* t = ((XPThread*)me)->worker_;
                PMF pmf = ((XPThread*)me)->pmf_;
                void* arg = ((XPThread*)me)->arg_;
                return (t->*pmf)(arg);
        }

public:
        XPThread(T* w, PMF pmf, void* arg = NULL) :
                        worker_(w), pmf_(pmf), arg_(arg), id_(0) {}

        ~XPThread() {
                if (id_) {
                        join();
                }
        }

        int start() {
                return pthread_create(&id_, NULL, XPThread<T>::run, this);
        }

        int join( void** value_ptr = NULL) {
                int ret = 0;

                if (id_) {
                        ret = ::pthread_join(id_, value_ptr) ;
                        id_ = 0;
                }

                return ret;
        }

        int cancel() {
                return ::pthread_cancel(id_);
        }

        int detach() {
                return ::pthread_detach(id_);
        }


        pthread_t getId() const {
                return id_;
        }
};

#endif //X_THREAD_H


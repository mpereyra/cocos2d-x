//
//  cocos_ptr.h
//  griffin
//
//  Created by Jeaye on 11/27/13.
//  Copyright 2013 TinyCo. All rights reserved.
//
// BPC: Added to cocos project on 9/26/16 by Brooke Costa
//

#pragma once

#include "platform/CCPlatformMacros.h"

NS_CC_BEGIN

  template <typename T>
  class cocos_ptr {
    public:
      using value_t = T;

#pragma mark - Constructors
      cocos_ptr()
        : m_ptr(nullptr)
      { }
      cocos_ptr(cocos_ptr<T> const &ptr)
        : m_ptr(ptr.m_ptr)
      { retain(); }
      cocos_ptr(cocos_ptr<T> &&ptr)
        : m_ptr(ptr.m_ptr)
      {
        retain();
        ptr.release();
      }

      template <typename C>
      explicit cocos_ptr(C const &ptr)
        : m_ptr(static_cast<T*>(ptr))
      { retain(); }
      cocos_ptr(T * const ptr)
        : m_ptr(ptr)
      { retain(); }
      cocos_ptr(std::nullptr_t)
        : m_ptr(nullptr)
      { }
      
#pragma mark - Factories
      template <typename... Args>
      static cocos_ptr<T> create(Args&&... args)
      {
        auto * const ptr(new T(std::forward<Args>(args)...));
        ptr->autorelease();
        return cocos_ptr<T>(ptr);
      }

#pragma mark - Destructor
      ~cocos_ptr()
      { release(); }

#pragma mark - Accessors
      T* get() const
      { return m_ptr; }

      operator bool() const
      { return m_ptr; }
      operator T*() const
      { return m_ptr; }
      T* operator ->() const {
        validate();
        return m_ptr;
      }

#pragma mark - Comparators
      template <typename C>
      bool operator ==(C const &c) const
      { return m_ptr == c; }
      bool operator ==(cocos_ptr<T> const &ptr) const 
      { return m_ptr == ptr.m_ptr; }

      template <typename C>
      bool operator !=(C const &c) const
      { return m_ptr != c; }
      bool operator !=(cocos_ptr<T> const &ptr) const 
      { return m_ptr != ptr.m_ptr; }

      template <typename C>
      bool operator <(C const &c) const
      { return m_ptr < c; }
      bool operator <(cocos_ptr<T> const &ptr) const
      { return m_ptr < ptr.m_ptr; }

#pragma mark - Mutators
      /* Allows for assignment to children types. */
      template <typename C>
      void reset(C const &ptr) {
        *this = static_cast<T*>(ptr);
      }
      void reset(T * const ptr)
      { *this = ptr; }
      void reset(std::nullptr_t)
      { *this = nullptr; }
      void reset(cocos_ptr<T> const &ptr)
      { *this = ptr; }
      void reset(cocos_ptr<T> &&ptr)
      { *this = std::move(ptr); }

      /* Allows for assignment to children types. */
      template <typename C>
      cocos_ptr<T>& operator =(C const &ptr) {
        return (*this = static_cast<T*>(ptr));
      }
      cocos_ptr<T>& operator =(T * const ptr) {
        if(m_ptr != ptr) {
          release();
          m_ptr = ptr;
          retain();
        }

        return *this;
      }
      cocos_ptr<T>& operator =(std::nullptr_t) {
        release();
        return *this;
      }
      cocos_ptr<T>& operator =(cocos_ptr<T> const &ptr) {
        if(m_ptr != ptr.m_ptr) {
          release();
          m_ptr = ptr.m_ptr;
          retain();
        }

        return *this;
      }
      cocos_ptr<T>& operator =(cocos_ptr<T> &&ptr) {
        if(m_ptr != ptr.m_ptr) {
          release();
          m_ptr = ptr.m_ptr;
          retain();
          ptr.release();
        }

        return *this;
      }

    private:
      void retain() {
        if(m_ptr)
        { m_ptr->retain(); }
      }
      void release() {
        if(m_ptr)
        { m_ptr->release(); }
        m_ptr = nullptr;
      }

      void validate() const
      { CCAssert(m_ptr, "Invalid cocos_ptr"); }

      T *m_ptr{ nullptr };
  };

  template <typename T>
  std::ostream& operator <<(std::ostream &stream, cocos_ptr<T> const &ptr)
  { return stream << ptr.m_ptr; }
NS_CC_END

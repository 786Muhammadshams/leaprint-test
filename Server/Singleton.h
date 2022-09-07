///////////////////////////////////////////////////////////////////////////////
/// \file Singleton.h
///
/// \ver 1.0
/// \date  2.06.2012, HW, Created
/// \brief Creates Singleton's
///////////////////////////////////////////////////////////////////////////////
#pragma once


///////////////////////////////////////////////////////////////////////////////
// Singleton Template
///////////////////////////////////////////////////////////////////////////////
template <typename T>
class Singleton
{
public:
   static T* Instance ()
   {
      if (!_instance)
         _instance = new T ();

      return _instance;
   }

   virtual ~Singleton ()
   {
      _instance = 0;
   }

   static void Destroy()
   {
      delete _instance;
      _instance = 0;
   }

private:
   static T* _instance;

protected:
   Singleton () { };
};

template <typename T> T* Singleton <T>::_instance = 0;
/**
  \file 

  \brief EmbeddedLite file
  \author V. Ridtchenko

  \notes

  Copyright: (C) 2010 by Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#ifndef SINGLETON_H
#define SINGLETON_H

/**
 * A template class that implements the Singleton pattern.
 *
 * \date 08-23-2006
 * \author eran
 */
template <class T> class Singleton
{
	static T* ms_instance;
public:
	/**
	 * Static method to access the only pointer of this instance.
	 * \return a pointer to the only instance of this 
	 */
	static T* Get();

	/**
	 * Release resources.
	 */
	static void Free();

protected:
	/**
	 * Default constructor.
	 */
	Singleton();

	/**
	 * Destructor.
	 */
	virtual ~Singleton();
};
template <class T> T* Singleton<T>::ms_instance = 0;

template <class T> Singleton<T>::Singleton()
{
}

template <class T> Singleton<T>::~Singleton()
{
}

template <class T> T* Singleton<T>::Get()
{
	if(!ms_instance)
		ms_instance = new T();
	return ms_instance;
}

template <class T> void Singleton<T>::Free()
{
	if( ms_instance )
	{
		delete ms_instance;
		ms_instance = 0;
	}
}

#endif // SINGLETON_H

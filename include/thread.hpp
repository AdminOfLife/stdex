#ifndef _STDEX_THREAD_H
#define _STDEX_THREAD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./mutex"
#include "./condition_variable"
#include "./chrono"

// POSIX includes
#include <pthread.h>

// std includes
#include <ostream>
#include <memory>

#ifdef _STDEX_HAS_CPP11_SUPPORT

#define DELETED_FUNCTION =delete
#define NOEXCEPT_FUNCTION throw() 

#else

#define DELETED_FUNCTION 
#define NOEXCEPT_FUNCTION

#endif

namespace stdex
{
	namespace detail
	{
		template<class _T0 = detail::void_type, class _T1 = detail::void_type, class _T2 = detail::void_type, class _T3 = detail::void_type, class _T4 = detail::void_type, class _T5 = detail::void_type, class _T6 = detail::void_type, class _T7 = detail::void_type, class _T8 = detail::void_type, class _T9 = detail::void_type, class _T10 = detail::void_type, class _T11 = detail::void_type, class _T12 = detail::void_type, class _T13 = detail::void_type, class _T14 = detail::void_type, class _T15 = detail::void_type, class _T16 = detail::void_type, class _T17 = detail::void_type, class _T18 = detail::void_type, class _T19 = detail::void_type, class _T20 = detail::void_type, class _T21 = detail::void_type, class _T22 = detail::void_type, class _T23 = detail::void_type, class _T24 = detail::void_type>
		struct _thread_args_helper;

		template<class _Arg1>
		struct _thread_args_helper<_Arg1>
		{
			struct _arguments
			{
				_Arg1 arg1;

				_arguments(_Arg1 arg1_) :
					arg1(arg1_)
				{ }

				template<class _FuncT>
				void push(_FuncT fp)
				{
					return fp(arg1);
				}
			};

			typedef _arguments arguments_type;
		};

		template<class _Arg1, class _Arg2>
		struct _thread_args_helper<_Arg1, _Arg2>
		{
			struct _arguments
			{
				_Arg1 arg1;
				_Arg2 arg2;

				_arguments(_Arg1 arg1_, _Arg2 arg2_) :
					arg1(arg1_), arg2(arg2_)
				{ }

				template<class _FuncT>
				void push(_FuncT fp)
				{
					fp(arg1, arg2);
				}
			};

			typedef _arguments arguments_type;
		};

		template<class _Arg1, class _Arg2, class _Arg3>
		struct _thread_args_helper<_Arg1, _Arg2, _Arg3>
		{
			struct _arguments
			{
				_Arg1 arg1;
				_Arg2 arg2;
				_Arg3 arg3;

				_arguments(_Arg1 arg1_, _Arg2 arg2_, _Arg3 arg3_) :
					arg1(arg1_), arg2(arg2_), arg3(arg3_)
				{ }

				template<class _FuncT>
				void push(_FuncT fp)
				{
					fp(arg1, arg2, arg3);
				}
			};

			typedef _arguments arguments_type;
		};

		template<class _Arg1, class _Arg2, class _Arg3, class _Arg4>
		struct _thread_args_helper<_Arg1, _Arg2, _Arg3, _Arg4>
		{
			struct _arguments
			{
				_Arg1 arg1;
				_Arg2 arg2;
				_Arg3 arg3;
				_Arg4 arg4;

				_arguments(_Arg1 arg1_, _Arg2 arg2_, _Arg3 arg3_, _Arg4 arg4_) :
					arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_)
				{ }

				template<class _FuncT>
				void push(_FuncT fp)
				{
					fp(arg1, arg2, arg3, arg4);
				}
			};

			typedef _arguments arguments_type;
		};

		template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5>
		struct _thread_args_helper<_Arg1, _Arg2, _Arg3, _Arg4, _Arg5>
		{
			struct _arguments
			{
				_Arg1 arg1;
				_Arg2 arg2;
				_Arg3 arg3;
				_Arg4 arg4;
				_Arg5 arg5;

				_arguments(_Arg1 arg1_, _Arg2 arg2_, _Arg3 arg3_, _Arg3 arg4_, _Arg5 arg5_) :
					arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_), arg5(arg5_)
				{ }

				template<class _FuncT>
				void push(_FuncT fp)
				{
					fp(arg1, arg2, arg3, arg4, arg5);
				}
			};

			typedef _arguments arguments_type;
		};

		template<class _FuncT, class _ArgT>
		struct _thread_function_proxy
		{
			typedef _ArgT arguments_type;
			typedef _FuncT function_type;

			arguments_type args;
			function_type fp;

			_thread_function_proxy(function_type fp_, arguments_type args_) :
				fp(fp_),
				args(args_)
			{ }

			~_thread_function_proxy()
			{ }

			void call()
			{
				args.push(fp);
			}

			static void gproxy(void *data)
			{
				_thread_function_proxy *pf = reinterpret_cast<_thread_function_proxy*>(data);

				pf->call();

				delete pf;
			}
		};
	}

	//! Thread class.
	class thread {

		void init(void(*aFunction)(void *), void * aArg);

	public:
		typedef pthread_t native_handle_type;

		class id;

		//! Default constructor.
		//! Construct a @c thread object without an associated thread of execution
		//! (i.e. non-joinable).
		thread() NOEXCEPT_FUNCTION: 
			_not_a_thread(true)
		{
		}

		template<class _FuncT, class _Arg0T>
		thread(_FuncT fx, _Arg0T t0) :
			_not_a_thread(true)
		{
			typedef typename detail::_thread_args_helper<_Arg0T>::arguments_type args_t;
			typedef _FuncT func_t;
			init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0)));
		}

		template<class _FuncT, class _Arg0T, class _Arg1T>
		thread(_FuncT fx, _Arg0T t0, _Arg1T t1) :
			_not_a_thread(true)
		{
			typedef typename detail::_thread_args_helper<_Arg0T, _Arg1T>::arguments_type args_t;
			typedef _FuncT func_t;
			init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1)));
		}

		template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T>
		thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2) :
			_not_a_thread(true)
		{
			typedef typename detail::_thread_args_helper<_Arg0T, _Arg1T, _Arg2T>::arguments_type args_t;
			typedef _FuncT func_t;
			init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2)));
		}

		//! Destructor.
		//! @note If the thread is joinable upon destruction, @c std::terminate()
		//! will be called, which terminates the process. It is always wise to do
		//! @c join() before deleting a thread object.
		~thread();

		//! Wait for the thread to finish (join execution flows).
		//! After calling @c join(), the thread object is no longer associated with
		//! a thread of execution (i.e. it is not joinable, and you may not join
		//! with it nor detach from it).
		void join();

		//! Check if the thread is joinable.
		//! A thread object is joinable if it has an associated thread of execution.
		bool joinable() const NOEXCEPT_FUNCTION;

		//! Detach from the thread.
		//! After calling @c detach(), the thread object is no longer assicated with
		//! a thread of execution (i.e. it is not joinable). The thread continues
		//! execution without the calling thread blocking, and when the thread
		//! ends execution, any owned resources are released.
		void detach();

		//! Return the thread ID of a thread object.
		id get_id() const NOEXCEPT_FUNCTION;

		//! Get the native handle for this thread.
		//! @note Under Windows, this is a @c HANDLE, and under POSIX systems, this
		//! is a @c pthread_t.
		inline native_handle_type native_handle()
		{
			return _thread_handle;
		}

		//! Determine the number of threads which can possibly execute concurrently.
		//! This function is useful for determining the optimal number of threads to
		//! use for a task.
		//! @return The number of hardware thread contexts in the system.
		//! @note If this value is not defined, the function returns zero (0).
		static unsigned hardware_concurrency();

		void swap(thread &other) NOEXCEPT_FUNCTION;

	private:
		native_handle_type _thread_handle;   //!< Thread handle.
		mutable mutex _data_mutex;     //!< Serializer for access to the thread private data.
		bool _not_a_thread;			  //!< True if this object is not a thread of execution.

		struct thread_start_info;

		thread_start_info *_thread_info;

		//thread(thread&) DELETED_FUNCTION;
		thread(const thread&) DELETED_FUNCTION;
		thread& operator=(const thread&) DELETED_FUNCTION;

		// This is the internal thread wrapper function.
		static void* wrapper_function(void *aArg);
	};


	//! Thread ID.
	//! The thread ID is a unique identifier for each thread.
	//! @see thread::get_id()
	class thread::id {
	public:
		//! Default constructor.
		//! The default constructed ID is that of thread without a thread of
		//! execution.
		id() NOEXCEPT_FUNCTION :
			_id(0) 
		{}

		explicit id(unsigned long int aId) :
			_id(aId) 
		{}

		id(const id &aId) : 
			_id(aId._id) 
		{}

		inline id& operator=(const id &aId)
		{
			_id = aId._id;
			return *this;
		}

		inline friend bool operator==(const id &aId1, const id &aId2) NOEXCEPT_FUNCTION
		{
			return (aId1._id == aId2._id);
		}

		inline friend bool operator!=(const id &aId1, const id &aId2) NOEXCEPT_FUNCTION
		{
			return (aId1._id != aId2._id);
		}

		inline friend bool operator<=(const id &aId1, const id &aId2) NOEXCEPT_FUNCTION
		{
			return (aId1._id <= aId2._id);
		}

		inline friend bool operator<(const id &aId1, const id &aId2) NOEXCEPT_FUNCTION
		{
			return (aId1._id < aId2._id);
		}

		inline friend bool operator>=(const id &aId1, const id &aId2) NOEXCEPT_FUNCTION
		{
			return (aId1._id >= aId2._id);
		}

		inline friend bool operator>(const id &aId1, const id &aId2) NOEXCEPT_FUNCTION
		{
			return (aId1._id > aId2._id);
		}

		template<class _CharT, class _Traits>
		inline friend ::std::basic_ostream<_CharT, _Traits>& operator<<(::std::basic_ostream<_CharT, _Traits> &out, thread::id id)
		{
			if (id == thread::id())
				return out << "thread::id of a non-executing thread";
			else
				return out << id._id;
		}

	private:
		unsigned long int _id;
	};

	inline void swap(thread &rhs, thread &lhs) NOEXCEPT_FUNCTION
	{
		rhs.swap(lhs);
	}

	namespace detail
	{
		void sleep_for_impl(const struct timespec *reltime);
	}

	//! The namespace @c this_thread provides methods for dealing with the
	//! calling thread.
	namespace this_thread 
	{
		//! Return the thread ID of the calling thread.
		thread::id get_id() NOEXCEPT_FUNCTION;

		//! Yield execution to another thread.
		//! Offers the operating system the opportunity to schedule another thread
		//! that is ready to run on the current processor.
		inline static void yield() NOEXCEPT_FUNCTION
		{
			sched_yield();
		}


		//! Blocks the calling thread for a period of time.
		//! @param[in] aTime Minimum time to put the thread to sleep.
		//! Example usage:
		//! @code
		//! // Sleep for 100 milliseconds
		//! this_thread::sleep_for(chrono::milliseconds(100));
		//! @endcode
		//! @note Supported duration types are: nanoseconds, microseconds,
		//! milliseconds, seconds, minutes and hours.

		template <class _Rep, class _Period>
		inline void sleep_for(const chrono::duration<_Rep, _Period> &d)
		{
			chrono::seconds s = chrono::duration_cast<chrono::seconds>(d);
			chrono::nanoseconds ns = chrono::duration_cast<chrono::nanoseconds>(d - s);
			
			timespec ts;
			ts.tv_sec = static_cast<stdex::time_t>(s.count());
			ts.tv_nsec = static_cast<long>(ns.count());

			detail::sleep_for_impl(&ts);
		}

		template <class _Clock, class _Duration>
		inline void sleep_until(const chrono::time_point<_Clock, _Duration> &t)
		{
			chrono::time_point<_Clock> now = _Clock::now();

			if (_Clock::is_steady)
			{
				if (now < t)
					sleep_for(t - now);
				return;
			}

			while (now < t)
			{
				sleep_for(t - now);
				now = _Clock::now();
			}

		}
	}
} // namespace stdex

#endif // _STDEX_THREAD_H
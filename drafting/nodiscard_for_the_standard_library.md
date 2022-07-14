# nodiscard attribute for the standard library

* `unique_ptr::release()` -- might leak if dropped on the floor
   * may be whole class `unique_ptr`, but not sure
   * may be `make_unique` but not sure, can not leak
   
### Thread support library:
  
* `thread::thread` templated version only risks terminate, if ignored.   
* `lock_guard` whole class [thread.lock.guard]

* `mutex::try_lock` because no other means to assess locked state after trying, risks UB if unlocked not often enough!
* `recursive_mutex::try_lock` because no other means to assess locked state after trying, risks UB if unlocked not often enough!
* `timed_mutex::try_lock_*` because no other means to asses locked state after trying, risks UB if locking again!
* `recursive_timed_mutex::try_lock*` because no other means to assess locked state after trying, risks UB if unlocked not often enough!

* `shared_mutex::try_lock*` because no other means to assess locked state after trying, risks UB if locking again!

* `shared_timed_mutex::try_lock_*` because no other means to asses locked state after trying, risks UB if locking again!

* `scoped_lock` whole class [thread.lock.scoped] -- ignore pathological case of empty pack.

* `unique_lock` constructors taking mutex (except for defer lock ?)

* `shared_lock` some constructors: blocking one, try (and adopt?) ones.
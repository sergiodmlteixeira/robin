#include "robin/semaphore.h"
Semaphore::Semaphore(std::string name) : name_(name) { }
// opens semaphore
void Semaphore::open()
{
  if (isOpen())
  {
    ROS_ERROR("Semaphore '%s' is already open.", name_.c_str());
    throw 2;
  }
  errno = 0;
  semaphore_ptr_ = sem_open(name_.c_str(), O_CREAT, 00700, 1);  // open or create; 00700=700 like chmod
  // semaphore_ptr_ = sem_open(name_.c_str(), 0);  // open, don't create
  if (semaphore_ptr_ == SEM_FAILED)
  {
    ROS_ERROR("Failed to open semaphore '%s'. errno %d: %s", name_.c_str(), errno, strerror(errno));
    throw 1;
  }
}
// checks if semaphore is open
bool Semaphore::isOpen()
{
  return !(semaphore_ptr_ == NULL || semaphore_ptr_ == SEM_FAILED);
}
// waits semaphore
void Semaphore::wait()
{
  if (!isOpen())
  {
    ROS_ERROR("Semaphore '%s' is not open.", name_.c_str());
    throw 2;
  }
  errno = 0;
  if (sem_wait(semaphore_ptr_) == -1)
  {
    ROS_ERROR("Failed to wait semaphore '%s'. errno %d: %s", name_.c_str(), errno, strerror(errno));
    throw 1;
  }
}
// posts semaphore
void Semaphore::post()
{
  if (!isOpen())
  {
    ROS_ERROR("Semaphore '%s' is not open.", name_.c_str());
    throw 2;
  }
  errno = 0;
  if (sem_post(semaphore_ptr_) == -1)
  {
    ROS_ERROR("Failed to post semaphore '%s'. errno %d: %s", name_.c_str(), errno, strerror(errno));
    throw 1;
  }
}
// closes and unlinks semaphore
void Semaphore::close()
{
  if (!isOpen())
  {
    ROS_ERROR("Semaphore '%s' is not open.", name_.c_str());
    throw 2;
  }
  // close
  errno = 0;
  if (sem_close(semaphore_ptr_) == -1)
  {
    ROS_ERROR("Failed to close semaphore '%s'. errno %d: %s", name_.c_str(), errno, strerror(errno));
    // throw 1;
  }
  // unlink
  errno = 0;
  if (sem_unlink(name_.c_str()) == -1)
  {
    ROS_ERROR("Failed to unlink semaphore '%s'. errno %d: %s", name_.c_str(), errno, strerror(errno));
    throw 1;
  }
  semaphore_ptr_ = NULL;  // NEEDED?
}
// closes and unlinks semaphore if open
Semaphore::~Semaphore()
{
  if (isOpen())
  {
    close();
  }
}

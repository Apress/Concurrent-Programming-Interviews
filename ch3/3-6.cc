class RWLock {
public:
  void LockRead();
  void UnlockRead();
  void LockWrite();
  void UnlockWrite();

private:
  // Implementation details here
};

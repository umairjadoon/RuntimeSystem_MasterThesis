#ifndef _CORENUMBER_H
#define _CORENUMBER_H
#include <map>

class coreNumber {
	static coreNumber* instance;
	coreNumber() {
		core_count = 0;
	}

public:
	static coreNumber* getInstance() {
		if (instance == nullptr)
			instance = new coreNumber();
		return instance;
	}

	std::map<pthread_t, int> thread_ids;

	void set_values(int cn) {
		core_count = cn;
	}

	int getCoreCount();
	void setCoreID(pthread_t pid, int id);
	int getCoreID();

private:
	int core_count;
};
#endif /*_CORENUMBER_H*/

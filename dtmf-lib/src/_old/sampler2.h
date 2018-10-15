#include <thread>
#include <vector>
#include <iostream>

class sampler2
{
public:

	// Public Members
	static enum class state {
		unitialized,
		idle,
		running,
	};

	// Public Methods [Interface]
	static void	init(void(*callback)());
	static void	run();
	static void	end();
	static void	flush();

private:

	// Private Members
	static int					rate;
	static int					interval;
	static state				status;
	static std::vector<int>		buffer;
	static std::thread			performer;

	static void(*callback)();

	// Private Methods
	static void perform();

	// Unavilable Constructor
	sampler2() = delete;
};
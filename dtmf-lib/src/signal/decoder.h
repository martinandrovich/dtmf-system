//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////
namespace decoder
{
	
	// Public Members
	enum class state {
		unitialized,
		idle,
		running,
	};

	// Public Methods
	void run();
	void run(void(*callback)());
	void end();

}
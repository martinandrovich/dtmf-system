//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////
namespace sampler
{
	// Public Members

	enum class state {
		unitialized,
		idle,
		running,
	};
	
	// Public Methods
	void	init(void(*callback)());
	void	run();
	void	end();
	void	flush();

	sampler::state	getState();
	
}
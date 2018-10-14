namespace decoder
{
	void init();
	void run();
	void end();

	void decode();
		
	namespace {
		int status;
		int buffer[200];
		int queue[5];

		void thread();
	}

}